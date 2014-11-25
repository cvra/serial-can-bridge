# Serial to CAN Bridge

Send and receive CAN frames over a serial connection.

## Commands
Commands are encoded with MessagePack.

### PC -> MCU

1. Command: uint
2. followed by an argument

Commands | Argument              | Explanation
-------- | --------------------- | ------------------------------
0x00     | MessagePack CAN frame | send CAN frame
0x01     | CAN ID filter list    | selectively receive CAN frames

### MCU -> PC

The MCU just forwards every CAN frame it receives encoded as described below.

### Message Pack CAN Frame

CAN frames are encoded as following:

Order | Type | Explanation
----- | ---- | -----------------------------------
1.    | bool | extended frame flag
2.    | bool | remote transmission request flag
3.    | uint | CAN ID (11bit or 28bit)
4.    | bin  | CAN frame data, length: 1 - 8 bytes

### Message Pack CAN ID Filter List

todo
