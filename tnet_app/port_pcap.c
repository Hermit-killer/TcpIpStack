#include "pcap_device.h"
#include "tnet_core.h"
#include "stdlib.h"
#include "string.h"

static pcap_t * pcap;
const char * ip_addr = "192.168.111.129";
const char my_mac_addr[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};

tnet_err_t tnet_driver_open(uint8_t * mac_addr){
    memcpy(mac_addr, my_mac_addr,sizeof(my_mac_addr));
    pcap = pcap_device_open(ip_addr,my_mac_addr,1);
    if(pcap==(pcap_t*)0){
        exit(-1);
    }
    return TNET_ERR_OK;
}

tnet_err_t tnet_driver_xfer(tnet_packet_t * packet){
    return pcap_device_xfer(pcap,packet->data,packet->size);
}

tnet_err_t tnet_driver_read(tnet_packet_t ** packet){
    uint16_t size;
    tnet_packet_t * rx_packet = tnet_alloc_for_recv(TNET_CFG_PACKET_MAX_SIZE);
    size = pcap_device_read(pcap,rx_packet->data,TNET_CFG_PACKET_MAX_SIZE);
    if(size){
        rx_packet->size = size;
        *packet = rx_packet;
        return TNET_ERR_OK;
    }
    return TNET_ERR_IO;
}