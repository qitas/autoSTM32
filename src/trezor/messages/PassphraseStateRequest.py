# Automatically generated by pb2py
import protobuf as p


class PassphraseStateRequest(p.MessageType):
    FIELDS = {
        1: ('state', p.BytesType, 0),
    }
    MESSAGE_WIRE_TYPE = 77

    def __init__(
        self,
        state: bytes = None,
        **kwargs,
    ):
        self.state = state
        p.MessageType.__init__(self, **kwargs)
