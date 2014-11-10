#ifndef SERIAL_CONN_H
#define SERIAL_CONN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void serial_interface_write(void *arg, const void *data, size_t len);
size_t serial_interface_read(void *arg, void *buf, size_t max);

#ifdef __cplusplus
}
#endif

#endif // SERIAL_CONN_H
