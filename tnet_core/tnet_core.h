#ifndef TNET_CORE_H
#include <stdint.h>
#define TNET_CORE_H
#define TNET_CFG_PACKET_MAX_SIZE    1516

typedef enum _tnet_err_t{
    TNET_ERR_OK = 0,
    TNET_ERR_IO = -1,
}tnet_err_t;

typedef struct _tnet_packet_t{
    uint16_t size;
    uint8_t * data;
    uint8_t payload[TNET_CFG_PACKET_MAX_SIZE];
}tnet_packet_t;

void tnet_init(void);
void tnet_poll(void);
tnet_packet_t * tnet_alloc_for_xfer(uint16_t data_size);
tnet_packet_t * tnet_alloc_for_recv(uint16_t data_size);
tnet_err_t tnet_driver_open(uint8_t * mac_addr);
tnet_err_t tnet_driver_xfer(tnet_packet_t * packet);
tnet_err_t tnet_driver_read(tnet_packet_t ** packet);
#endif