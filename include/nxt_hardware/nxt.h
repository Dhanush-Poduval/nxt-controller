#ifndef NXT_H 
#define NXT_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

typedef struct {
  libusb_context *ctx;
  libusb_device_handle *handle;
}nxt_device_usb_t;

struct send_message_usb{
  uint8_t con_send[64];
};
struct recieve_message_usb {
  uint8_t con_check[64];
};
// enum motors{
//   A ,
//   B ,
//   C
// };
uint8_t establish_connection(nxt_device_usb_t *nxt);
int firmware_protocol_calc(struct recieve_message_usb *rec_msg);
uint8_t send_commands(nxt_device_usb_t *nxt, struct send_message_usb *send_msg);
uint8_t recieve(nxt_device_usb_t *nxt, struct recieve_message_usb *rec_msg);
uint8_t check_firmware(nxt_device_usb_t *nxt , struct send_message_usb *msg , struct recieve_message_usb *rec_msg);
int read_channel_values(char port , nxt_device_usb_t *nxt);
void run_motor(nxt_device_usb_t *nxt , char port , int power);
void break_motor(nxt_device_usb_t *nxt,char port);
#ifdef __cplusplus 
}
#endif
#endif

