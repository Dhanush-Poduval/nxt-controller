#ifndef BLUETOOTH_NXT_H 
#define BLUETOOTH_NXT_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
typedef struct {
  int socket_fd; 
}nxt_device_t;

struct send_message{
  uint8_t con_send[64];
  int length;
};
struct recieve_message {
  uint8_t con_check[64];
};

int establish_connection_bluetooth(nxt_device_t *nxt);
int firmware_protocol_calc_bluetooth(struct recieve_message *rec_msg);
int send_message_bluetooth(nxt_device_t *nxt, struct send_message *send_msg);
int recieve_message_bluetooth(nxt_device_t *nxt, struct recieve_message *rec_msg);
uint8_t check_firmware_bluetooth(nxt_device_t *nxt , struct send_message *msg , struct recieve_message *rec_msg);
void run_motor_bluetooth(nxt_device_t *nxt , char port , int power);
void break_motor_bluetooth(nxt_device_t *nxt,char port);
#ifdef __cplusplus 
}
#endif
#endif

