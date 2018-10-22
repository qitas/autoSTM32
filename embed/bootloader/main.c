/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (c) SatoshiLabs
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <sys/types.h>
#include "display.h"
#include "common.h"
#include "image.h"
#include "flash.h"
#include "mini_printf.h"
#include "rng.h"
#include "secbool.h"
#include "touch.h"
#include "usb.h"
#include "version.h"
#include "vendorkeys.h"
#include "bootui.h"
#include "messages.h"
// #include "mpu.h"

const uint8_t BOOTLOADER_KEY_M = 2;
const uint8_t BOOTLOADER_KEY_N = 3;


#define USB_IFACE_NUM   0

static void usb_init_all(void) {

    static const usb_dev_info_t dev_info = {
        .device_class    = 0x00,
        .device_subclass = 0x00,
        .device_protocol = 0x00,
        .vendor_id       = 0x1209,
        .product_id      = 0x53C0,
        .release_num     = 0x0200,
        .manufacturer    = "OZ",
        .product         = "Wallet",
        .serial_number   = "000000000000000000000001",
        .interface       = "TREZOR Interface",
        .usb21_enabled   = sectrue,
    };

    static uint8_t rx_buffer[USB_PACKET_SIZE];

    static const usb_webusb_info_t webusb_info = {
        .iface_num        = USB_IFACE_NUM,
        .ep_in            = USB_EP_DIR_IN | 0x01,
        .ep_out           = USB_EP_DIR_OUT | 0x01,
        .subclass         = 0,
        .protocol         = 0,
        .max_packet_len   = sizeof(rx_buffer),
        .rx_buffer        = rx_buffer,
        .polling_interval = 1,
    };

    usb_init(&dev_info);

    ensure(usb_webusb_add(&webusb_info), NULL);

    usb_start();
}

static secbool bootloader_usb_loop(const vendor_header * const vhdr, const image_header * const hdr)
{
    usb_init_all();

    uint8_t buf[USB_PACKET_SIZE];

    for (;;) {
        int r = usb_webusb_read_blocking(USB_IFACE_NUM, buf, USB_PACKET_SIZE, USB_TIMEOUT);
        if (r != USB_PACKET_SIZE) {
            continue;
        }
        uint16_t msg_id;
        uint32_t msg_size;
        if (sectrue != msg_parse_header(buf, &msg_id, &msg_size)) {
            // invalid header -> discard
            continue;
        }
        switch (msg_id) {
            case 0: // Initialize
                process_msg_Initialize(USB_IFACE_NUM, msg_size, buf, vhdr, hdr);
                break;
            case 1: // Ping
                process_msg_Ping(USB_IFACE_NUM, msg_size, buf);
                break;
            case 5: // WipeDevice
                ui_fadeout();
                ui_screen_wipe_confirm();
                ui_fadein();
                int response = ui_user_input(INPUT_CONFIRM | INPUT_CANCEL);
                if (INPUT_CANCEL == response) {
                    ui_fadeout();
                    ui_screen_info(secfalse, vhdr, hdr);
                    ui_fadein();
                    send_user_abort(USB_IFACE_NUM, "Wipe cancelled");
                    break;
                }
                ui_fadeout();
                ui_screen_wipe();
                ui_fadein();
                r = process_msg_WipeDevice(USB_IFACE_NUM, msg_size, buf);
                if (r < 0) { // error
                    ui_fadeout();
                    ui_screen_fail();
                    ui_fadein();
                    usb_stop();
                    usb_deinit();
                    return secfalse; // shutdown
                } else { // success
                    ui_fadeout();
                    ui_screen_done(0, sectrue);
                    ui_fadein();
                    usb_stop();
                    usb_deinit();
                    return secfalse; // shutdown
                }
                break;
            case 6: // FirmwareErase
                process_msg_FirmwareErase(USB_IFACE_NUM, msg_size, buf);
                break;
            case 7: // FirmwareUpload
                r = process_msg_FirmwareUpload(USB_IFACE_NUM, msg_size, buf);
                if (r < 0 && r != -4) { // error, but not user abort (-4)
                    ui_fadeout();
                    ui_screen_fail();
                    ui_fadein();
                    usb_stop();
                    usb_deinit();
                    return secfalse; // shutdown
                } else
                if (r == 0) { // last chunk received
                    ui_screen_install_progress_upload(1000);
                    ui_fadeout();
                    ui_screen_done(4, sectrue);
                    ui_fadein();
                    ui_screen_done(3, secfalse);
                    hal_delay(1000);
                    ui_screen_done(2, secfalse);
                    hal_delay(1000);
                    ui_screen_done(1, secfalse);
                    hal_delay(1000);
                    usb_stop();
                    usb_deinit();
                    ui_fadeout();
                    return sectrue; // jump to firmware
                }
                break;
            case 55: // GetFeatures
                process_msg_GetFeatures(USB_IFACE_NUM, msg_size, buf, vhdr, hdr);
                break;
            default:
                process_msg_unknown(USB_IFACE_NUM, msg_size, buf);
                break;
        }
    }
}

secbool load_vendor_header_keys(const uint8_t * const data, vendor_header * const vhdr)
{
    return load_vendor_header(data, BOOTLOADER_KEY_M, BOOTLOADER_KEY_N, BOOTLOADER_KEYS, vhdr);
}

#define OTP_BLOCK_VENDOR_KEYS_LOCK 2

static secbool check_vendor_keys_lock(const vendor_header * const vhdr) {
    uint8_t lock[FLASH_OTP_BLOCK_SIZE];
    ensure(flash_otp_read(OTP_BLOCK_VENDOR_KEYS_LOCK, 0, lock, FLASH_OTP_BLOCK_SIZE), NULL);
    if (0 == memcmp(lock, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", FLASH_OTP_BLOCK_SIZE)) {
        return sectrue;
    }
    uint8_t hash[32];
    vendor_keys_hash(vhdr, hash);
    return sectrue * (0 == memcmp(lock, hash, 32));
}

// protection against bootloader downgrade

#if PRODUCTION

#define OTP_BLOCK_BOOTLOADER_VERSION 1

static void check_bootloader_version(void)
{
    uint8_t bits[FLASH_OTP_BLOCK_SIZE];
    for (int i = 0; i < FLASH_OTP_BLOCK_SIZE * 8; i++) {
        if (i < VERSION_MONOTONIC) {
             bits[i / 8] &= ~(1 << (7 - (i % 8)));
        } else {
             bits[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
    ensure(flash_otp_write(OTP_BLOCK_BOOTLOADER_VERSION, 0, bits, FLASH_OTP_BLOCK_SIZE), NULL);

    uint8_t bits2[FLASH_OTP_BLOCK_SIZE];
    ensure(flash_otp_read(OTP_BLOCK_BOOTLOADER_VERSION, 0, bits2, FLASH_OTP_BLOCK_SIZE), NULL);

    ensure(sectrue * (0 == memcmp(bits, bits2, FLASH_OTP_BLOCK_SIZE)), "Bootloader downgraded");
}

#endif

int main(void)
{
main_start:
#if PRODUCTION
    check_bootloader_version();
#endif

    touch_init();

    // delay to detect touch
    uint32_t touched = 0;
    for (int i = 0; i < 100; i++) {
        touched = touch_read();
        if (touched) {
            break;
        }
        hal_delay(1);
    }

    vendor_header vhdr;
    image_header hdr;
    secbool firmware_present;

    // detect whether the devices contains a valid firmware

    firmware_present = load_vendor_header_keys((const uint8_t *)FIRMWARE_START, &vhdr);
    if (sectrue == firmware_present) 
    {
        firmware_present = check_vendor_keys_lock(&vhdr);
        //display_printf("get firmware_present :%d",(int)firmware_present);
    }
    //hal_delay(1000);
    if (sectrue == firmware_present) 
    {
        firmware_present = load_image_header((const uint8_t *)(FIRMWARE_START + vhdr.hdrlen), FIRMWARE_IMAGE_MAGIC, FIRMWARE_IMAGE_MAXSIZE, vhdr.vsig_m, vhdr.vsig_n, vhdr.vpub, &hdr);
		//display_printf("get firmware_present :%d",(int)firmware_present);
    }
    //hal_delay(1000);
    if (sectrue == firmware_present) 
    {
        firmware_present = check_image_contents(&hdr, IMAGE_HEADER_SIZE + vhdr.hdrlen, firmware_sectors, FIRMWARE_SECTORS_COUNT);
		//display_printf("get firmware_present :%d",(int)firmware_present);
    }
    //firmware_present = secfalse;	//just for test the boot ui.
    // start the bootloader if no or broken firmware found ...
    if (firmware_present != sectrue) {
        // show intro animation

        // no ui_fadeout(); - we already start from black screen
        ui_screen_first();
        ui_fadein();

        hal_delay(1000);

        ui_fadeout();
        ui_screen_second();
        ui_fadein();

        hal_delay(1000);

        ui_fadeout();
        ui_screen_third();
        ui_fadein();

        // and start the usb loop
        if (bootloader_usb_loop(NULL, NULL) != sectrue) {
            return 1;
        }
    } else
    // ... or if user touched the screen on start
    if (touched) {
        // show firmware info with connect buttons

        // no ui_fadeout(); - we already start from black screen
        ui_screen_info(sectrue, &vhdr, &hdr);
        ui_fadein();

        for (;;) {
            int response = ui_user_input(INPUT_CONFIRM | INPUT_CANCEL | INPUT_INFO);
            ui_fadeout();

            // if cancel was pressed -> restart
            if (INPUT_CANCEL == response) {
                goto main_start;
            }

            // if confirm was pressed -> jump out
            if (INPUT_CONFIRM == response) {
                // show firmware info without connect buttons
                ui_screen_info(secfalse, &vhdr, &hdr);
                ui_fadein();
                break;
            }

            // if info icon was pressed -> show fingerprint
            if (INPUT_INFO == response) {
                // show fingerprint
                ui_screen_info_fingerprint(&hdr);
                ui_fadein();
                while (INPUT_LONG_CONFIRM != ui_user_input(INPUT_LONG_CONFIRM)) { }
                ui_fadeout();
                ui_screen_info(sectrue, &vhdr, &hdr);
                ui_fadein();
            }
        }

        // and start the usb loop
        if (bootloader_usb_loop(&vhdr, &hdr) != sectrue) {
            return 1;
        }
    }

#if 1
    ensure(
        load_vendor_header_keys((const uint8_t *)FIRMWARE_START, &vhdr),
        "invalid vendor header");

    ensure(
        check_vendor_keys_lock(&vhdr),
        "unauthorized vendor keys");

    ensure(
        load_image_header((const uint8_t *)(FIRMWARE_START + vhdr.hdrlen), FIRMWARE_IMAGE_MAGIC, FIRMWARE_IMAGE_MAXSIZE, vhdr.vsig_m, vhdr.vsig_n, vhdr.vpub, &hdr),
        "invalid firmware header");

    ensure(
        check_image_contents(&hdr, IMAGE_HEADER_SIZE + vhdr.hdrlen, firmware_sectors, FIRMWARE_SECTORS_COUNT),
        "invalid firmware hash");
#endif

    // if all VTRUST flags are unset = ultimate trust => skip the procedure
#if 1
    if ((vhdr.vtrust & VTRUST_ALL) != VTRUST_ALL) {

        // ui_fadeout();  // no fadeout - we start from black screen
        ui_screen_boot(&vhdr, &hdr);
        ui_fadein();

        int delay = (vhdr.vtrust & VTRUST_WAIT) ^ VTRUST_WAIT;
        if (delay > 1) {
            while (delay > 0) {
                ui_screen_boot_wait(delay);
                hal_delay(1000);
                delay--;
            }
        } else if (delay == 1) {
            hal_delay(1000);
        }

        if ((vhdr.vtrust & VTRUST_CLICK) == 0) {
            ui_screen_boot_click();
            touch_click();
        }

        ui_fadeout();
    }
#endif
    // mpu_config();
    // jump_to_unprivileged(FIRMWARE_START + vhdr.hdrlen + IMAGE_HEADER_SIZE);

    jump_to(FIRMWARE_START + vhdr.hdrlen + IMAGE_HEADER_SIZE);

    return 0;
}
