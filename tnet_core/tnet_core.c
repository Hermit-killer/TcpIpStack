#include "tnet_core.h"
#include <string.h>
#define min(a,b) ((a)<(b)? (a) : (b))
#define swap_order16(v) ((((v)&0xff)<<8) | (((v)>>8) & 0xff))
static uint8_t netif_mac[TNET_MAC_ADDR_SIZE];

static tnet_packet_t tx_packet,rx_packet;

static tarp_entry_t arp_entry;

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

static tnet_err_t ethernet_init(void){
    tnet_err_t err = tnet_driver_open(netif_mac);
    if(err<0) return err;
    return TNET_ERR_OK;
}

static tnet_err_t ethernet_out_to(tnet_protocal_t protocal, 
                                const uint8_t* mac_addr,
                                tnet_packet_t* packet){
    tether_hdr_t * ether_hdr;
    add_header(packet,sizeof(tether_hdr_t));
    ether_hdr = (tether_hdr_t *) packet->data;
    memcpy(ether_hdr->dest,mac_addr,TNET_MAC_ADDR_SIZE);
    memcpy(ether_hdr->src,netif_mac,TNET_MAC_ADDR_SIZE);
    ether_hdr->protocal = protocal;
    return tnet_driver_xfer(packet);
}

static void ethernet_in(tnet_packet_t * packet){
    tether_hdr_t * ether_hdr; 
    uint16_t protocal;
    if(packet->size<=sizeof(tether_hdr_t)){
        return;
    }
    ether_hdr = (tether_hdr_t*)packet->data;
    protocal = swap_order16(ether_hdr->protocal);
    switch(protocal){
        case TNET_PROTOCAL_ARP:
            break;
        case TNET_PROTOCAL_IP:
            break;
    }
}

static void ethernet_poll(void){
    tnet_packet_t * packet;
    if(tnet_driver_read(&packet)==TNET_ERR_OK){
        ethernet_in(packet);
    }
}

void tnet_init(void){
    ethernet_init();
}

void tnet_poll(void){
    ethernet_poll();
}

void tarp_init(void){
    arp_entry.state = TARP_ENTRY_FREE;
}