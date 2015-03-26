#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include "can_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

void can_interface_send(struct can_frame *frame);

#ifdef __cplusplus
}
#endif

#endif // CAN_INTERFACE_H
