#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "nxt_hardware/nxt.h"

struct send_message{
  uint8_t con_send[2];
}send_msg={
  .con_send={0x01,0x88},
};

struct recieve_message {
  uint8_t con_check[3];
}recieve_msg={
  .con_check={0x02,0x88,0x00},
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

uint8_t send(struct send_message *send_msg){
  int r=0;
  libusb_context *ctx=NULL;
  libusb_device_handle *handle=NULL;
  int length=sizeof(send_msg->con_send);
  int transferred_bytes=0;
  uint8_t a=establish_connection(&ctx,&handle);
  if(a==0){
    r=libusb_bulk_transfer(handle,0x01,send_msg->con_send,length,&transferred_bytes,5000);
    if(r==0){
      printf("Transferred bytes complete : %d \n",transferred_bytes);
      return 0;
    }else {
      printf("Failed in transferring bytes \n");
      return 1;
    }
  }
  else{
    return 1;
  }
};

int main(){
  uint8_t a;
  send(&send_msg);
}
