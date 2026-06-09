#define NXT_H
#ifndef NXT_H 
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
struct send_message{
  uint8_t message[2];
};
enum motors{
  int A ,
  int B ,
  int C
};
uint8_t establish_connection(libusb_context **ctx,libusb_device_handle **handle);
uint8_t send(struct send_message *msg);

#endif

