#ifndef CAN_INTERFACE_MOCK_H
#define CAN_INTERFACE_MOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

bool can_interface_mock_status(uint32_t *id, uint8_t *data, uint8_t *len);

#ifdef __cplusplus
}
#endif

#endif // CAN_INTERFACE_MOCK_H
