#include <network.h>

int try_or_die(int result) {
  if(result == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  return result;
}

int PROTO = 0xBEEF;

network_context *network_context_new(char *if_name, char *dst_addr) {
  network_context* ctxt = malloc(sizeof(network_context));
  int socket_fd = try_or_die(socket(AF_PACKET, SOCK_RAW, htons(PROTO)));

  int if_index = get_interface_index(socket_fd, if_name);
  get_mac_address(socket_fd, ctxt->src_addr);
  memcpy(ctxt->dst_addr, dst_addr, ETHER_ADDR_LEN);

  ctxt->socket_fd = socket_fd;
  ctxt->if_index = if_index;

  return ctxt;
}

void network_context_free(network_context *ctxt) {
  try_or_die(close(ctxt->socket_fd));
  free(ctxt);
}

void send_raw_frame(network_context *ctxt, void *data, uint16_t data_len) {
  if(data_len > 1500) {
    printf("Payload exceeds maximum size\n");
    exit(1);
  }
  struct sockaddr_ll socket_address = {
    .sll_family = AF_PACKET,
    .sll_ifindex = ctxt->if_index,
    .sll_halen = ETHER_ADDR_LEN,
    .sll_protocol = htons(PROTO)
  };
  memcpy(socket_address.sll_addr, ctxt->dst_addr, ETHER_ADDR_LEN);

  struct ethhdr header;
  memcpy(header.h_source, ctxt->src_addr, ETHER_ADDR_LEN);
  memcpy(header.h_dest, ctxt->dst_addr, ETHER_ADDR_LEN);
  header.h_proto = htons(data_len);

  uint8_t buf[2000];
  size_t buf_len = sizeof(header) + data_len;
  memcpy(buf, &header, sizeof(header));
  memcpy(buf+sizeof(header), data, data_len);

  try_or_die(sendto(ctxt->socket_fd, buf, buf_len, 0, (struct sockaddr *)&socket_address, sizeof(socket_address)));
}

int get_interface_index(int socket_fd, char* if_name) {
  struct ifreq ifr;
  size_t if_name_len = strlen(if_name);
  if(if_name_len < sizeof(ifr.ifr_name)) {
    memcpy(ifr.ifr_name, if_name, if_name_len);
  } else {
    printf("Interface name '%s' too long\n", if_name);
  }
  try_or_die(ioctl(socket_fd, SIOCGIFINDEX, &ifr));
  return ifr.ifr_ifindex;
}

void get_mac_address(int socket_fd, char* out) {
  struct ifreq ifr;
  try_or_die(ioctl(socket_fd, SIOCGIFHWADDR, &ifr));
  memcpy(out, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
}
