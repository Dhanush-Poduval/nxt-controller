#include <stdio.h>
#include<libusb-1.0/libusb.h>

int main(){
  libusb_context *ctx= NULL;
  libusb_device_handle *handle=NULL;
  libusb_init(&ctx);
  handle=libusb_open_device_with_vid_pid(
      ctx,
      0x0694,
      0x0002
  );
  if(handle ==NULL){
    printf("Failed to connect to device \n");
    return 1;
  };
  printf("Connected to device\n");
  libusb_close(handle);
  libusb_exit(ctx);
  return 0;
}
