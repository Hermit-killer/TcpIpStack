#include "tnet_core.h"
#include <string.h>
#define min(a,b) ((a)<(b)? (a) : (b))
#define swap_order16(v) ((((v)&0xff)<<8) | (((v)>>8) & 0xff))
#define tipaddr_is_equal_buf(addr,buf)  ((memcmp((addr)->array,(buf),TNET_IPV4_ADDR_SIZE)) == 0)

static const tipaddr_t netif_ipaddr = TNET_CFG_NETIF_IP;
static const uint8_t ehter_broadcast[] = {0xff,0xff,0xff,0xff,0xff,0xff};

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
    return tarp_make_request(&netif_ipaddr);
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
            remove_header(packet,sizeof(tnet_packet_t));
            tarp_in(packet);
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

int tarp_make_request(const tipaddr_t * ipaddr){
    tnet_packet_t * packet = tnet_alloc_for_xfer(sizeof(tarp_packet_t));
    tarp_packet_t * arp_packet = (tarp_packet_t *) packet->data;
    arp_packet->hw_type = TARP_HW_ETHER;
    arp_packet->pro_type = swap_order16(TNET_PROTOCAL_IP);
    arp_packet->hw_len = TNET_MAC_ADDR_SIZE;
    arp_packet->pro_len = TNET_IPV4_ADDR_SIZE;
    arp_packet->opcode = TARP_REQUEST;
    memcpy(arp_packet->src_mac,netif_mac,TNET_MAC_ADDR_SIZE);
    memcpy(arp_packet->src_ip,netif_ipaddr.array,TNET_IPV4_ADDR_SIZE);
    memset(arp_packet->dest_mac,0,TNET_MAC_ADDR_SIZE);
    memcpy(arp_packet->dest_ip,ipaddr->array,TNET_IPV4_ADDR_SIZE);
    return ethernet_out_to(TNET_PROTOCAL_IP,ehter_broadcast,packet);
}

void tarp_in(tnet_packet_t * packet){
    if(packet->size>=sizeof(tarp_packet_t)){
        tarp_packet_t* arp_packet = (tarp_packet_t*) packet->data;
        uint16_t opcode = swap_order16(arp_packet->opcode);

        if((swap_order16(arp_packet->hw_type)!=TARP_HW_ETHER) || 
            (arp_packet->hw_len!=TNET_MAC_ADDR_SIZE)    ||
            (swap_order16(arp_packet->pro_type!=TNET_PROTOCAL_IP))  ||
            (arp_packet->pro_len!=TNET_IPV4_ADDR_SIZE)  ||
            ((arp_packet->opcode!=TARP_REQUEST)  && (arp_packet->opcode!=TARP_REPLY))
        ){
          return;  
        }

        if(!tipaddr_is_equal_buf(&netif_ipaddr,arp_packet->dest_ip)){
            return;
        }

        switch(opcode){
            case TARP_REQUEST:
                tarp_make_response(arp_packet);
                updata_arp_entry(arp_packet->src_ip,arp_packet->src_mac);
                break;
            
            case TARP_REPLY:
                updata_arp_entry(arp_packet->src_ip,arp_packet->src_mac);
                break;
        }
    }
}

tnet_err_t tarp_make_response(tarp_packet_t*arp_packet){
    tnet_packet_t * packet = tnet_alloc_for_xfer(sizeof(tarp_packet_t));
    tarp_packet_t * response_packet = (tarp_packet_t *) packet->data;

    response_packet->hw_type = TARP_HW_ETHER;
    response_packet->pro_type = swap_order16(TNET_PROTOCAL_IP);
    response_packet->hw_len = TNET_MAC_ADDR_SIZE;
    response_packet->pro_len = TNET_IPV4_ADDR_SIZE;
    response_packet->opcode = TARP_REPLY;
    memcpy(response_packet->src_mac,netif_mac,TNET_MAC_ADDR_SIZE);
    memcpy(response_packet->src_ip,netif_ipaddr.array,TNET_IPV4_ADDR_SIZE);
    memcpy(response_packet->dest_mac, arp_packet->src_mac,TNET_MAC_ADDR_SIZE);
    memcpy(response_packet->dest_ip,arp_packet->src_ip,TNET_IPV4_ADDR_SIZE);
    return ethernet_out_to(TNET_PROTOCAL_IP,arp_packet->src_mac,packet);
}

static void updata_arp_entry(uint8_t * src_ip,uint8_t * mac_addr){
    memcpy(arp_entry.ipaddr.array,src_ip,TNET_IPV4_ADDR_SIZE);
    memcpy(arp_entry.macaddr,mac_addr,TNET_MAC_ADDR_SIZE);
    arp_entry.state = TARP_ENTRY_OK;
}