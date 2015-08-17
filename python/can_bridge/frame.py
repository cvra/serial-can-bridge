import msgpack


class Frame:
    """
    A single CAN frame.
    """

    def __init__(self, id=0, data=None, extended=False,
                 transmission_request=False,
                 data_length=0):

        if data is None:
            data = bytes()

        if len(data) > 8:
            raise ValueError

        self.id = id

        self.data = data
        self.data_length = data_length

        if len(self.data) > 0:
            self.data_length = len(self.data)

        self.transmission_request = transmission_request
        self.extended = extended

    def __eq__(self, other):
        return self.id == other.id and self.data == other.data


def encode(frame):
    """
    Encodes the given frame to raw messagepack bytes.
    """
    packer = msgpack.Packer(use_bin_type=True)
    data = [frame.extended, frame.transmission_request, frame.id, frame.data_length, frame.data]
    return packer.pack(data)


def decode(data):
    """
    Decodes the given messagepack bytes to a Frame object.
    """
    result = Frame()
    data = msgpack.unpackb(data)

    result.extended = data[0]
    result.transmission_request = data[1]
    result.id = data[2]
    result.data_length = data[4]
    result.data = data[4]

    return result
