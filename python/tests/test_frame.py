from can_bridge.frame import Frame
import can_bridge.frame
import unittest
import msgpack


def encode_decode_frame(frame):
    """
    Returns a list containing the decoded fields from the encoded frame.
    """
    a = msgpack.Unpacker()
    a.feed(can_bridge.frame.encode(frame))

    return next(a)


class CanFrameEncodingTestCase(unittest.TestCase):
    """
    Testcase grouping all the functions relating to encoding CAN frames.
    """

    def test_frame_constructor(self):
        """
        Tests if we can instantiate a CAN Frame with default values.
        """
        frame = Frame(id=42)
        self.assertEqual(frame.id, 42)
        self.assertEqual(frame.data, bytes())
        self.assertFalse(frame.extended)
        self.assertFalse(frame.transmission_request)

    def test_can_provide_datalen(self):
        """
        Tests if we can set a data length.
        """
        frame = Frame(id=42, data_length=8)
        self.assertEqual(frame.data_length, 8)

    def test_datalen_auto(self):
        """
        Checks that the data length is correctly populated if we provide data.
        """
        frame = Frame(data=bytes(range(3)))
        self.assertEqual(3, frame.data_length)

    def test_frame_encode_extended(self):
        """
        Tests if encoding of the extended flag works.
        """
        frame = Frame(extended=True)
        msg = encode_decode_frame(frame)
        self.assertEqual(True, msg[0])

    def test_frame_encode_rtr(self):
        """
        Tests if encoding of the remote transmition request flag (RTR) works.
        """
        frame = Frame(transmission_request=True)
        msg = encode_decode_frame(frame)
        self.assertEqual(True, msg[1])

    def test_frame_encode_id(self):
        """
        Checks if encoding of Frame ID works correctly.
        """
        frame = Frame(id=42)
        msg = encode_decode_frame(frame)
        self.assertEqual(42, msg[2])

    def test_frame_encode_data_length(self):
        """
        Checks if we can encode the data length.
        """
        frame = Frame(data_length=3)
        msg = encode_decode_frame(frame)
        self.assertEqual(3, msg[3])


    def test_frame_encode_data(self):
        """
        Checks if encoding of data works properly.
        """
        frame = Frame(data=bytes([1, 2, 3]))
        msg = encode_decode_frame(frame)
        self.assertEqual(bytes([1, 2, 3]), msg[4])

    def test_frame_data_is_binary(self):
        """
        Checks if the encoded data is binary, not string.
        """

        frame = Frame(data=bytes([1, 2, 3]))
        data = can_bridge.frame.encode(frame)
        marker = 0xc4  # binary marker
        self.assertEqual(data[-5], marker)
        self.assertEqual(data[-4], len(frame.data))

    def test_frames_are_equal(self):
        a = Frame(data=bytes([1, 2, 3]), id=42)
        b = Frame(data=bytes([1, 2, 3]), id=42)
        self.assertFalse(a is b)
        self.assertTrue(a == b)

    def test_frames_not_equal_if_different_id(self):
        a = Frame(id=42)
        b = Frame(id=43)
        self.assertFalse(a == b)

    def test_frames_not_equal_if_different_data(self):
        a = Frame(data=bytes([10, 20]))
        b = Frame(data=bytes([1, 2]))
        self.assertFalse(a == b)





class CanFrameDecodingTestCase(unittest.TestCase):
    """
    Checks that decoding CAN frames works well.

    This depends on CAN encoding tests working as well.
    """

    def test_extended_frame_flag(self):
        """
        Checks that the extended flag can be correctly decoded.
        """
        frame = can_bridge.frame.decode(
            can_bridge.frame.encode(Frame(extended=True)))
        self.assertTrue(frame.extended)

        frame = can_bridge.frame.decode(
            can_bridge.frame.encode(Frame(extended=False)))
        self.assertFalse(frame.extended)

    def test_rtr_flag(self):
        """
        Checks that the Remote Transmission Request can be correctly
        decoded.
        """
        frame = can_bridge.frame.decode(
            can_bridge.frame.encode(Frame(transmission_request=True)))
        self.assertTrue(frame.transmission_request)

        frame = can_bridge.frame.decode(
            can_bridge.frame.encode(Frame(transmission_request=False)))
        self.assertFalse(frame.transmission_request)

    def test_frameid(self):
        """
        Checks that the frame ID is correctly decoded.
        """
        frame = can_bridge.frame.decode(can_bridge.frame.encode(Frame(id=42)))
        self.assertEqual(frame.id, 42)

    def test_data(self):
        """
        Checks that the data field is correctly decoded.
        """
        data = 'hello'.encode('ascii')
        frame = can_bridge.frame.decode(
            can_bridge.frame.encode(Frame(data=data)))
        self.assertEqual(data, frame.data)
