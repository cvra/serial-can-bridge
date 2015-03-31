import unittest
import can_bridge.commands
import msgpack

from can_bridge.frame import Frame


class WriteCommandTestCase(unittest.TestCase):
    def test_simple_case(self):
        """
        Simply try encoding a write command.
        """
        data = can_bridge.commands.encode_frame_write(Frame())
        a = msgpack.Unpacker()
        a.feed(data)
        self.assertEqual(next(a), 0)
