#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "nxt_hardware/nxt.h"

struct send_message{
  uint8_t con_send[10];
};

struct recieve_message {
  uint8_t con_check[64];
};

typedef struct {
  libusb_device_handle *handle;
  libusb_context *ctx;
}nxt_device_t;

struct motor_commands {
  uint8_t byte_0;
  uint8_t byte_1;
  uint8_t port;
  uint8_t power;
  uint8_t mode ;
  uint8_t reg_mode;
  uint8_t turn_ration;
  uint8_t run_state;
  uint8_t tach0_limit;
};

struct check_motor {
  uint8_t byte0;
  uint8_t byte1;
  uint8_t port;
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

uint8_t establish_connection(nxt_device_t *nxt){
  nxt->ctx=NULL;
  nxt->handle=NULL;
  libusb_init(&nxt->ctx);
  nxt->handle=libusb_open_device_with_vid_pid(
    nxt->ctx,
    0x0694,
    0x0002
  );
  if(nxt->handle==NULL){
    printf("Connection could not be established with the nxt brick \n");
    return 1;
  };
  printf("Connection established with the nxt brick \n");
  return 0;
};

uint8_t send(nxt_device_t *nxt,struct send_message *send_msg){
  int r=0;
  // libusb_context *ctx=NULL;
  // libusb_device_handle *handle=NULL;
  int length=sizeof((send_msg)->con_send);
  int transferred_bytes=0;
  // uint8_t a=establish_connection(&ctx,&handle);
  r=libusb_bulk_transfer(nxt->handle,0x01,(send_msg)->con_send,length,&transferred_bytes,5000);
    if(r==0){
      printf("Transferred bytes complete : %d \n",transferred_bytes);
      return 0;
    }else {
      printf("Failed in transferring bytes \n");
      return 1;
  };
};

uint8_t recieve(nxt_device_t *nxt,  struct recieve_message *recieve_msg){
  // libusb_context *ctx=NULL;
  int r=0;
  // libusb_device_handle *handle=NULL;
  int length=sizeof(recieve_msg->con_check);
  int total_length=0;
  // uint8_t a=establish_connection(&ctx,&handle);
  r=libusb_bulk_transfer(nxt->handle,0x82,recieve_msg->con_check,length,&total_length,5000);
  if(r==0){
      printf("Successfully recieved bytes : %d \n",total_length);
      return 0;
  }else {
      printf("Failed to recieve bytes : %d\n",r);
      return 1;
  };
  
};

uint8_t check_firmware(nxt_device_t *nxt , struct send_message *send_msg,struct recieve_message *recieve_msg){
  uint8_t a;
  send_msg->con_send[0]=0x01;
  send_msg->con_send[1]=0x88;
  nxt->ctx=NULL;
  nxt->handle=NULL;
  a=establish_connection(nxt);
  if(a==0){
    send(nxt ,send_msg);
    recieve(nxt, recieve_msg);
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

int read_channel_values(char port,nxt_device_t *nxt){
  uint8_t port_value;
  if(port=='A'){
    port_value=0x00;
  }else if(port=='B'){
    port_value=0x01;
  }else {
    port_value=0x02;
  };
  struct send_message send_msg;
  struct recieve_message rec_msg;
  send_msg.con_send[0]=0x00;
  send_msg.con_send[1]=0x06;
  send_msg.con_send[2]=port_value;
  send(nxt,&send_msg);
  recieve(nxt,&rec_msg);
  if(rec_msg.con_check[2]==0){
    printf("Recieved motor status \n");
    for(int i=0;i<24;i++){
      printf("%02X ",rec_msg.con_check[i]);
    };

  };
  return 0;
};


int main(){
  uint8_t a;
  struct send_message send_msg;
  struct recieve_message rec_msg;
  nxt_device_t nxt;
  check_firmware(&nxt,&send_msg,&rec_msg);
  read_channel_values('B',&nxt);
  libusb_close(nxt.handle);
  libusb_exit(nxt.ctx);
  return 0;
}
