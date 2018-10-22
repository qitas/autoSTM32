# Automatically generated by pb2py
import protobuf as p


class CipherKeyValue(p.MessageType):
    FIELDS = {
        1: ('address_n', p.UVarintType, p.FLAG_REPEATED),
        2: ('key', p.UnicodeType, 0),
        3: ('value', p.BytesType, 0),
        4: ('encrypt', p.BoolType, 0),
        5: ('ask_on_encrypt', p.BoolType, 0),
        6: ('ask_on_decrypt', p.BoolType, 0),
        7: ('iv', p.BytesType, 0),
    }
    MESSAGE_WIRE_TYPE = 23

    def __init__(
        self,
        address_n: list = None,
        key: str = None,
        value: bytes = None,
        encrypt: bool = None,
        ask_on_encrypt: bool = None,
        ask_on_decrypt: bool = None,
        iv: bytes = None,
        **kwargs,
    ):
        self.address_n = [] if address_n is None else address_n
        self.key = key
        self.value = value
        self.encrypt = encrypt
        self.ask_on_encrypt = ask_on_encrypt
        self.ask_on_decrypt = ask_on_decrypt
        self.iv = iv
        p.MessageType.__init__(self, **kwargs)
