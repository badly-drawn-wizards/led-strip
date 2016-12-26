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

typedef struct {
  int socket_fd;
  int if_index;
  char src_addr[ETHER_ADDR_LEN];
  char dst_addr[ETHER_ADDR_LEN];
} network_context;

network_context *network_context_new(char *if_name, char *dst_addr);

void network_context_free(network_context *ctxt);

void send_raw_frame(network_context *ctxt, void *data, uint16_t data_len);

int get_interface_index(int socket_fd, char *if_name);

void get_mac_address(int socket_fd, char *out);

#endif
