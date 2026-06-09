#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "nxt_hardware/nxt.h"

struct send_message{
  uint8_t con_send[2];
}send_msg={
  .con_send={0x01,0x88},
};

struct recieve_message {
  uint8_t con_check[10];
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
uint8_t establish_connection(libusb_context **ctx, libusb_device_handle **dev){
  libusb_init(ctx);
  *dev=libusb_open_device_with_vid_pid(
    *ctx,
    0x0694,
    0x0002
  );
  if(*dev==NULL){
    printf("Connection could not be established with the nxt brick \n");
    return 1;
  };
  printf("Connection established with the nxt brick \n");
  return 0;
};

uint8_t send(libusb_device_handle *handle ,struct send_message *send_msg){
  int r=0;
  // libusb_context *ctx=NULL;
  // libusb_device_handle *handle=NULL;
  int length=sizeof((send_msg)->con_send);
  int transferred_bytes=0;
  // uint8_t a=establish_connection(&ctx,&handle);
  r=libusb_bulk_transfer((handle),0x01,(send_msg)->con_send,length,&transferred_bytes,5000);
    if(r==0){
      printf("Transferred bytes complete : %d \n",transferred_bytes);
      return 0;
    }else {
      printf("Failed in transferring bytes \n");
      return 1;
  };
};

uint8_t recieve(libusb_device_handle *handle ,  struct recieve_message *recieve_msg){
  // libusb_context *ctx=NULL;
  int r=0;
  // libusb_device_handle *handle=NULL;
  int length=sizeof(recieve_msg->con_check);
  int total_length=0;
  // uint8_t a=establish_connection(&ctx,&handle);
  r=libusb_bulk_transfer(handle,0x82,recieve_msg->con_check,length,&total_length,5000);
  if(r==0){
      printf("Successfully recieved bytes : %d \n",total_length);
      return 0;
  }else {
      printf("Failed to recieve bytes : %d\n",r);
      return 1;
  };
  
}
uint8_t check_firmware(struct send_message *send_msg,struct recieve_message *recieve_msg){
  uint8_t a;
  libusb_device_handle *handle=NULL;
  libusb_context *ctx=NULL;
  a=establish_connection(&ctx , &handle);
  if(a==0){
    a=send(handle ,send_msg);
    recieve(handle , recieve_msg);
    if(recieve_msg->con_check[2]==0){
      printf("Successfully connected to the nxt and communication established \n");
      // for(int i=0;i<7;i++){
      //   printf("%02X \t ",recieve_msg->con_check[i]);
      // };
      firmware_protocol_calc(recieve_msg);
      libusb_close(handle);
      libusb_exit(ctx);
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
  uint8_t a;
  struct recieve_message rec_msg;
  check_firmware(&send_msg,&rec_msg);
}
