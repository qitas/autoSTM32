# Automatically generated by pb2py
import protobuf as p


class SignMessage(p.MessageType):
    FIELDS = {
        1: ('address_n', p.UVarintType, p.FLAG_REPEATED),
        2: ('message', p.BytesType, 0),  # required
        3: ('coin_name', p.UnicodeType, 0),  # default='Bitcoin'
        4: ('script_type', p.UVarintType, 0),  # default=0
    }
    MESSAGE_WIRE_TYPE = 38

    def __init__(
        self,
        address_n: list = None,
        message: bytes = None,
        coin_name: str = None,
        script_type: int = None,
        **kwargs,
    ):
        self.address_n = [] if address_n is None else address_n
        self.message = message
        self.coin_name = coin_name
        self.script_type = script_type
        p.MessageType.__init__(self, **kwargs)
