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
