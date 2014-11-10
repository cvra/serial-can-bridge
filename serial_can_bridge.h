#ifndef SERIAL_CAN_BRIDGE_H
#define SERIAL_CAN_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

void serial_rx_main(void *arg);
void serial_tx_main(void *arg);

#ifdef __cplusplus
}
#endif

#endif // SERIAL_CAN_BRIDGE_H
