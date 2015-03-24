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

A CAN frame is encoded as a MessagePack array with following entries:

Order | Type | Explanation
----- | ---- | -----------------------------------
1.    | bool | IDE extended frame id flag
2.    | bool | RTR remote transmission request flag
3.    | uint | CAN ID (11bit or 28bit)
4.    | uint | DLC data length code, values: 0 - 8
5.    | bin  | CAN frame data, length: 0 - 8 bytes

### Message Pack CAN ID Filter List

todo
