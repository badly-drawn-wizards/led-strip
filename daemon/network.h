#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>

#include <net/ethernet.h>
#include <net/if.h>

#include <netpacket/packet.h>

#ifndef _NETWORK_H_
#define _NETWORK_H_

void send_raw_frame(int socket_fd, int if_index, unsigned char *src_addr, unsigned char *dst_addr, void* data, uint16_t data_len);

int get_interface_index(int socket_fd, char* if_name, size_t if_name_len);

void get_mac_address(int socket_fd, unsigned char* out);

#endif
