import msgpack
import can_bridge.frame


class CANBridgeCommand:
    SendFrame = 0
    SetIDFilter = 1


def encode_frame_write(frame):
    """
    Encodes the send command with the given CAN frame.
    """
    packer = msgpack.Packer(use_bin_type=True)
    command = packer.pack(CANBridgeCommand.SendFrame)
    command = command + can_bridge.frame.encode(frame)
    return command


def encode_id_filter_set(extended_frame=None, rtr_frame=None, id=0,
                         id_mask=None):
    """
    Encodes a command to setup the CAN frame filter.

    A None value for extended_frame and rtr_frame means that we don't care
    about those values. For the id, a '1' bit in id_mask means that the filter
    is enabled on that bit.

    A None value for id_mask means that only this specific ID will be accepted.
    Otherwise the bits marked as 1 in id_mask must be correct in ID.
    """

    if id_mask is None:
        if id == 0:
            id_mask = 0
        else:  # If we give a specific ID, all bits must match
            id_mask = (1 << 29) - 1

    args = [False, False, id, False, False, id_mask]

    if extended_frame is not None:
        args[0], args[3] = extended_frame, True

    if rtr_frame is not None:
        args[1], args[4] = rtr_frame, True

    args = msgpack.packb(args)

    command = msgpack.packb(CANBridgeCommand.SetIDFilter)
    return command + args
