#include "tnet_core.h"
#define min(a,b) ((a)<(b)? (a) : (b))

static tnet_packet_t tx_packet,rx_packet;

tnet_packet_t * tnet_alloc_for_xfer(uint16_t data_size){
    tx_packet.data = tx_packet.payload + TNET_CFG_PACKET_MAX_SIZE - data_size; 
    tx_packet.size = data_size;
    return &tx_packet;
}

tnet_packet_t * tnet_alloc_for_recv(uint16_t data_size){
    rx_packet.data = rx_packet.payload;
    rx_packet.size = data_size;
    return &rx_packet;
}

static void add_header(tnet_packet_t * packet, uint16_t header_size){
    packet->data -= header_size;
    packet->size += header_size;
}

static void remove_header(tnet_packet_t * packet, uint16_t header_size){
    packet->data += header_size;
    packet->size -= header_size;
}

static void truncate_header(tnet_packet_t * packet, uint16_t size){
    packet->size = min(packet->size,size);    

}
