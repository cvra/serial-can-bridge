import unittest
import can_bridge.commands
import msgpack


def unpackb(data):
    unpacker = msgpack.Unpacker()
    unpacker.feed(data)
    return tuple(unpacker)


class IdFilterTestCase(unittest.TestCase):
    def test_encode_id_has_correct_command_number(self):
        data = can_bridge.commands.encode_id_filter_set()
        command = unpackb(data)[0]
        self.assertEqual(1, command)

    def test_default_values(self):
        """
        Checks that by default we don't apply any filter.
        """
        data = can_bridge.commands.encode_id_filter_set()
        _, args = unpackb(data)
        no_filter_args = [False, False, 0, False, False, 0]
        self.assertEqual(args, no_filter_args)

    def test_extended_flag(self):
        """
        Checks that applying the extended flag filter works.
        """
        data = can_bridge.commands.encode_id_filter_set(extended_frame=True)
        _, args = unpackb(data)
        expected_args = [True, False, 0, True, False, 0]
        self.assertEqual(args, expected_args)

    def test_rtr_flag(self):
        """
        Checks that applying the remote transmission request flag filter works.
        """
        data = can_bridge.commands.encode_id_filter_set(rtr_frame=True)
        _, args = unpackb(data)
        expected_args = [False, True, 0, False, True, 0]
        self.assertEqual(args, expected_args)

    def test_id_mask(self):
        """
        Checks that we can apply the ID filter mask.
        """
        data = can_bridge.commands.encode_id_filter_set(id=0x80, id_mask=0xff)
        _, args = unpackb(data)
        expected_args = [False, False, 0x80, False, False, 0xff]
        self.assertEqual(args, expected_args)

    def test_single_id(self):
        """
        When giving an ID and no mask we should filter to allow only that
        single ID.
        """
        ALL_BITS_MASK = (1 << 29) - 1
        data = can_bridge.commands.encode_id_filter_set(id=0x80)
        _, args = unpackb(data)
        expected_args = [False, False, 0x80, False, False, ALL_BITS_MASK]
        self.assertEqual(args, expected_args)

    def test_single_id_zero(self):
        """
        ID0 is a valid CAN frame ID. We should handle it too.
        """
        ALL_BITS_MASK = (1 << 29) - 1
        data = can_bridge.commands.encode_id_filter_set(id=0)
        _, args = unpackb(data)
        expected_args = [False, False, 0, False, False, ALL_BITS_MASK]
        self.assertEqual(args, expected_args)

