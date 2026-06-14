#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "nxt_hardware/nxt_bluetooth.h"


int establish_connection_bluetooth(nxt_device_t *nxt)
{
    nxt->socket_fd = open("/dev/rfcomm0", O_RDWR | O_NOCTTY);

    if (nxt->socket_fd < 0) {
        perror("open");
        return 1;
    }

 
    struct termios options;
    if (tcgetattr(nxt->socket_fd, &options) < 0) {
        perror("tcgetattr");
        close(nxt->socket_fd);
        return 1;
    }

    cfmakeraw(&options); 


    options.c_cc[VMIN]  = 1; 
    options.c_cc[VTIME] = 10; 

    if (tcsetattr(nxt->socket_fd, TCSANOW, &options) < 0) {
        perror("tcsetattr");
        close(nxt->socket_fd);
        return 1;
    }

  
    tcflush(nxt->socket_fd, TCIOFLUSH);

    printf("Connected and configured raw mode for /dev/rfcomm0\n");
    return 0;
}

int send_message_bluetooth(nxt_device_t *nxt, struct send_message *send_msg){
  int length = send_msg->length;
  uint8_t packet[66];
  packet[0] = length & 0xFF;
  packet[1] = (length >> 8) & 0xFF;
  
  printf("Sending: ");
  memcpy(&packet[2], send_msg->con_send, length);
  
  int written = write(nxt->socket_fd, packet, length + 2);
  for(int i = 0; i < length + 2; i++)
    printf("%02X ", packet[i]);
  printf("\n");
  printf("Written %d bytes \n", written);
  return 0;
}

int firmware_protocol_calc_bluetooth(struct recieve_message *rec_msg){
  uint8_t protocol_minor = rec_msg->con_check[3];
  uint8_t protocol_major = rec_msg->con_check[4];
  uint8_t firmware_minor = rec_msg->con_check[5];
  uint8_t firmware_major = rec_msg->con_check[6];
  printf("Protocol version : %u.%u\n", protocol_major, protocol_minor);
  printf("Firmware version : %u.%u\n", firmware_major, firmware_minor);
  return 0;
}

int recieve_message_bluetooth(nxt_device_t *nxt, struct recieve_message *rec_msg){
  printf("Waiting for response from nxt \n");
  uint8_t len_buf[2];
  

  int r = read(nxt->socket_fd, &len_buf[0], 1);
  if (r == 1) r += read(nxt->socket_fd, &len_buf[1], 1);

  if(r != 2){
    printf("Failed to read response message length (read %d bytes)\n", r);
    return 1;
  }
  
  int payload_len = len_buf[0] | (len_buf[1] << 8);
  printf("Payload length = %d\n", payload_len);
  
  if(payload_len <= 0 || payload_len > 64){
    printf("Invalid payload length : %d\n", payload_len);
    tcflush(nxt->socket_fd, TCIFLUSH);
    return 1;
  }

  uint8_t response[64];
  int total = 0;
  while(total < payload_len){
    int r = read(nxt->socket_fd, response + total, payload_len - total);
    if (r <= 0) {
        printf("Read error during payload\n");
        return 1;
    }
    total += r;
  }
  
  memcpy(rec_msg->con_check, response, payload_len);
  return 0;
}

uint8_t check_firmware_bluetooth(nxt_device_t *nxt, struct send_message *send_msg, struct recieve_message *recieve_msg){
  uint8_t a;
  send_msg->con_send[0] = 0x01; 
  send_msg->con_send[1] = 0x88;
  send_msg->length = 2;
  
  a = establish_connection_bluetooth(nxt);
  if(a == 0){
    send_message_bluetooth(nxt, send_msg);
    if(recieve_message_bluetooth(nxt, recieve_msg) == 0) {
      if(recieve_msg->con_check[2] == 0){
        printf("Successfully connected to the nxt and communication established \n");
        firmware_protocol_calc_bluetooth(recieve_msg);
        return 0;
      } else {
        printf("NXT returned error code: %02X\n", recieve_msg->con_check[2]);
        return 1;
      }
    }
  }
  return 1;
}

void run_motor_bluetooth(nxt_device_t *nxt, char port, int power ){
  struct send_message send_msg = {0};
  struct recieve_message rec_msg = {0};
  uint8_t port_value;
  
  if(port == 'A') port_value = 0x00;
  else if(port == 'B') port_value = 0x01;
  else port_value = 0x02;
  
  send_msg.con_send[0] = 0x00; 
  send_msg.con_send[1] = 0x04;
  send_msg.con_send[2] = port_value;
  send_msg.con_send[3] = (int8_t)power;
  send_msg.con_send[4] = 0x03;
  send_msg.con_send[5] = 0x01;
  send_msg.con_send[6] = 0;
  send_msg.con_send[7] = 0x20;
  send_msg.con_send[8] = 0x00;
  send_msg.con_send[9] = 0x00;
  send_msg.con_send[10] = 0x00;
  send_msg.con_send[11] = 0x00;
  send_msg.length = 12;
  
  send_message_bluetooth(nxt, &send_msg);
  // recieve_message_bluetooth(nxt, &rec_msg);
}

void break_motor_bluetooth(nxt_device_t *nxt, char port ){
  struct send_message send_msg = {0};
  struct recieve_message rec_msg = {0};
  uint8_t port_value;
  
  if(port == 'A') port_value = 0x00;
  else if(port == 'B') port_value = 0x01;
  else port_value = 0x02;
  
  send_msg.con_send[0] = 0x00;
  send_msg.con_send[1] = 0x04;
  send_msg.con_send[2] = port_value;
  send_msg.con_send[3] = 0;
  send_msg.con_send[4] = 0x00;
  send_msg.con_send[5] = 0;
  send_msg.con_send[6] = 0;
  send_msg.con_send[7] = 0x00;
  send_msg.con_send[8] = 0;
  send_msg.con_send[9] = 0x00;
  send_msg.con_send[10] = 0x00;
  send_msg.con_send[11] = 0x00;
  send_msg.length = 12;
  
  send_message_bluetooth(nxt, &send_msg);
  // recieve_message_bluetooth(nxt, &rec_msg);
}

// int main(){
//   nxt_device_t nxt; 
//
//   if (establish_connection(&nxt) != 0) {
//      return 1;
//   }
//
//   printf("Executing break_motor...\n");
//   run_motor(&nxt, 'A',50);
//   run_motor(&nxt,'B',50);
//   printf("Sleeping for 5 seconds...\n");
//   sleep(5);
//   break_motor(&nxt,'B');
//   sleep(5);
//   printf("Executing run_motor...\n");
//   break_motor(&nxt, 'A');
//
//   close(nxt.socket_fd);
//   return 0;
// }
