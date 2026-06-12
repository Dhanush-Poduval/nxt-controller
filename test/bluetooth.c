#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
struct send_message {
  uint8_t con_send[64];
};
struct recieve_message {
  uint8_t con_check[64];
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
  // send_msg->con_send[0]=0x01;
  // send_msg->con_send[1]=0x88;
  int length=2;
  uint8_t packet[66];
  packet[0]=length & 0xFF;
  packet[1]=(length >>8) & 0xFF;
  printf("Sending: ");

  memcpy(&packet[2],send_msg->con_send,2);
  int written = write(nxt->socket_fd,packet,2+2);
  for(int i=0;i<4;i++)
    printf("%02X ", packet[i]);
  printf("\n");
  printf("Written %d bytes \n",written);
};
int firmware_protocol_calc( struct recieve_message *rec_msg){
  uint8_t protocol_minor = rec_msg->con_check[3];
  uint8_t protocol_major = rec_msg->con_check[4];
  uint8_t firmware_minor = rec_msg->con_check[5];
  uint8_t firmware_major = rec_msg->con_check[6];
  printf("Protocol version : %u.%u\n",protocol_major,protocol_minor);
  printf("Firmware version : %u.%u\n",firmware_major,firmware_minor);
  return 0;
};
int recieve_message(nxt_device_t *nxt , struct recieve_message *rec_msg){
  printf("Waiting for response from nxt \n");
  uint8_t len_buf[2];
  int r = read(nxt->socket_fd,len_buf,2);
  if(r!=2){
    printf("Failed to read response message length \n");
    return 1;
  }
  int payload_len=len_buf[0] | (len_buf[1]<<8);
  printf("Payload length = %d\n",payload_len);
  uint8_t response[256];
  r=read(nxt->socket_fd,response,payload_len);
  memcpy(rec_msg->con_check,response,payload_len);
  printf("Response : \n");
  for(int i=0;i<payload_len;i++){
    printf("%02X \t",response[i]);
  };
  return 0;

};
uint8_t check_firmware(nxt_device_t *nxt , struct send_message *send_msg,struct recieve_message *recieve_msg){
  uint8_t a;
  // send_msg->con_send[0]=0x02;
  // send_msg->con_send[1]=0x00;
  send_msg->con_send[0]=0x01;
  send_msg->con_send[1]=0x88;
  a=establish_connection(nxt);
  if(a==0){
    send_message(nxt ,send_msg);
    recieve_message(nxt, recieve_msg);
    if(recieve_msg->con_check[2]==0){
      printf("Successfully connected to the nxt and communication established \n");
      // for(int i=0;i<7;i++){
      //   printf("%02X \t ",recieve_msg->con_check[i]);
      // };
      firmware_protocol_calc(recieve_msg);
      // libusb_close(nxt->handle);
      // libusb_exit(nxt->ctx);
      return 0;
    }else {
      printf("Not connected to nxt successfully \n");
      return 1;
    }
  }else {
    return 1;
  }
 };
int main(){
  nxt_device_t nxt; 
  struct send_message send_msg={0};
  struct recieve_message rec_msg={0};
  check_firmware(&nxt,&send_msg,&rec_msg);
  close(nxt.socket_fd);
  return 0;
};
