# Automatically generated by pb2py
import protobuf as p


class EthereumSignMessage(p.MessageType):
    FIELDS = {
        1: ('address_n', p.UVarintType, p.FLAG_REPEATED),
        2: ('message', p.BytesType, 0),  # required
    }
    MESSAGE_WIRE_TYPE = 64

    def __init__(
        self,
        address_n: list = None,
        message: bytes = None,
        **kwargs,
    ):
        self.address_n = [] if address_n is None else address_n
        self.message = message
        p.MessageType.__init__(self, **kwargs)
