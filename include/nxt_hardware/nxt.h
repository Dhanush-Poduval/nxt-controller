#define NXT_H
#ifndef NXT_H 
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
struct send_message{
  uint8_t message[2];
};
struct recieve_message {
  uint8_t message[10];
};
enum motors{
  int A ,
  int B ,
  int C
};
uint8_t establish_connection(libusb_context **ctx,libusb_device_handle **handle);
uint8_t send(libusb_device_handle *handle, struct send_message *msg);
uint8_t recieve(libusb_device_handle *handle , struct recieve_message *rec_msg);
uint8_t check_firmware(struct send_message *msg , struct recieve_message *rec_msg);

#endif

