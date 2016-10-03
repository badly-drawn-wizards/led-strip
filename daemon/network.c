#include <network.h>

char IF_NAME[] = "wlp6s0";
unsigned char ADDR[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

int try_or_die(int result) {
  if(result == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  return result;
}

int PROTO = 0xBEEF;
int main() {
  uint8_t data[] = "PAYLOAD YO";
  int socket_fd = try_or_die(socket(AF_PACKET, SOCK_RAW, htons(PROTO)));

  int if_index = get_interface_index(socket_fd, IF_NAME, sizeof(IF_NAME));
  unsigned char if_addr[ETHER_ADDR_LEN];
  get_mac_address(socket_fd, if_addr);

  send_raw_frame(socket_fd, if_index, if_addr, ADDR, data, sizeof(data));

  try_or_die(close(socket_fd));
}

void send_raw_frame(int socket_fd, int if_index, unsigned char *src_addr, unsigned char *dst_addr, void* data, uint16_t data_len) {
  printf("%d data len\n", data_len);
  if(data_len > 1500) {
    printf("Payload exceeds maximum size\n");
    exit(1);
  }
  struct sockaddr_ll socket_address = {
    .sll_family = AF_PACKET,
    .sll_ifindex = if_index,
    .sll_halen = ETHER_ADDR_LEN,
    .sll_protocol = htons(PROTO)
  };
  memcpy(socket_address.sll_addr, dst_addr, ETHER_ADDR_LEN);

  struct ethhdr header;
  memcpy(header.h_dest, dst_addr, ETHER_ADDR_LEN);
  memcpy(header.h_source, src_addr, ETHER_ADDR_LEN);
  header.h_proto = htons(data_len);

  uint8_t buf[2000];
  uint8_t buf_len = sizeof(header) + data_len;
  memcpy(buf, &header, sizeof(header));
  memcpy(buf+sizeof(header), data, data_len);

  try_or_die(sendto(socket_fd, buf, buf_len, 0, (struct sockaddr *)&socket_address, sizeof(socket_address)));
}

int get_interface_index(int socket_fd, char* if_name, size_t if_name_len) {
  struct ifreq ifr;
  if(if_name_len < sizeof(ifr.ifr_name)) {
    memcpy(ifr.ifr_name, if_name, if_name_len);
  } else {
    printf("Interface name '%s' too long\n", if_name);
  }
  try_or_die(ioctl(socket_fd, SIOCGIFINDEX, &ifr));
  return ifr.ifr_ifindex;
}

void get_mac_address(int socket_fd, unsigned char* out) {
  struct ifreq ifr;
  try_or_die(ioctl(socket_fd, SIOCGIFHWADDR, &ifr));
  memcpy(out, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
}
