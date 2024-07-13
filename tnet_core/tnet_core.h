#ifndef TNET_CORE_H
#define TNET_CORE_H
#include <stdint.h>
#define TNET_IPV4_ADDR_SIZE 4
#define TNET_CFG_PACKET_MAX_SIZE    1516
#define TNET_MAC_ADDR_SIZE  8

#define TARP_ENTRY_FREE 0

#pragma pack(1)
typedef struct _tether_hdr_t{
    uint8_t dest[TNET_MAC_ADDR_SIZE];
    uint8_t src[TNET_MAC_ADDR_SIZE];
    uint16_t protocal;
}tether_hdr_t;
#pragma pack(0)

typedef enum _tnet_err_t{
    TNET_ERR_OK = 0,
    TNET_ERR_IO = -1,
}tnet_err_t;

typedef enum _tnet_protocal_t{
    TNET_PROTOCAL_ARP = 0x0806,
    TNET_PROTOCAL_IP = 0x0800,
}tnet_protocal_t;

typedef struct _tnet_packet_t{
    uint16_t size;
    uint8_t * data;
    uint8_t payload[TNET_CFG_PACKET_MAX_SIZE];
}tnet_packet_t;

typedef union _tipaddr_t{
    uint8_t array[TNET_IPV4_ADDR_SIZE];
    uint32_t addr;
}tipaddr_t;

typedef struct _tarp_entry_t{
    tipaddr_t ipaddr;
    uint8_t macaddr[TNET_MAC_ADDR_SIZE];
    uint8_t state;
    uint16_t tmo;
    uint8_t retry_cnt;
}tarp_entry_t;

void tarp_init(void);
void tnet_init(void);
void tnet_poll(void);
tnet_packet_t * tnet_alloc_for_xfer(uint16_t data_size);
tnet_packet_t * tnet_alloc_for_recv(uint16_t data_size);
tnet_err_t tnet_driver_open(uint8_t * mac_addr);
tnet_err_t tnet_driver_xfer(tnet_packet_t * packet);
tnet_err_t tnet_driver_read(tnet_packet_t ** packet);
#endif