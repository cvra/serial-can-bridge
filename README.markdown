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
3.    | uint | CAN ID (11bit or 29bit)
4.    | uint | DLC data length code, values: 0 - 8
5.    | bin  | CAN frame data, length: 0 - 8 bytes

### Message Pack CAN ID Filter List

The filter list is a MessagePack array of at least one filter entry.
One filter entry is composed of an ID with a corresponding mask and is encoded as a MessagePack array as following:

Order | Type | Explanation
----- | ---- | -----------------------------------
1.    | bool | IDE extended frame id flag
2.    | bool | RTR remote transmission request flag
3.    | uint | CAN ID (11bit or 29bit)
4.    | bool | mask: IDE
5.    | bool | mask: RTR
6.    | uint | mask: ID

A mask bit set to 1 (or true) means the corresponding field has to match the given ID, 0 (or false) means the field doesn't matter.
For a CAN frame to pass the filter it has to match at least one filter entry.
