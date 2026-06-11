#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
struct send_message {
  uint8_t con_send[64];
};

typedef struct {
  int socket_fd;
}nxt_device_t;

int establish_connection(nxt_device_t *nxt)
{
    nxt->socket_fd = open("/dev/rfcomm0", O_RDWR);

    if (nxt->socket_fd < 0) {
        perror("open");
        return 1;
    }

    printf("Connected to /dev/rfcomm0\n");
    return 0;
}

void send_message(nxt_device_t *nxt , struct send_message *send_msg){
  nxt->socket_fd=open("/dev/rfcomm0",O_RDWR);
  send_msg->con_send[0]=0x01;
  send_msg->con_send[1]=0x88;
  int length=sizeof(send_msg->con_send);
  uint8_t packet[66];
  packet[0]=length & 0xFF;
  packet[1]=(length >>8) & 0xFF;
  memcpy(&packet[2],send_msg->con_send,2);
  int written = write(nxt->socket_fd,packet,2+2);
  printf("Written %d bytes \n",written);
};

int main(){
  nxt_device_t nxt; 
  struct send_message send_msg={0};
  establish_connection(&nxt);
  send_message(&nxt,&send_msg);
  close(nxt.socket_fd);
  return 0;
};
