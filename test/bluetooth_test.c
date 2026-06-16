#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

struct send_message {
  uint8_t con_send[64];
  int length;
};

struct recieve_message {
  uint8_t con_check[64];
};

typedef struct {
  int socket_fd;
} nxt_device_t;

int establish_connection(nxt_device_t *nxt)
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

int main(){
  nxt_device_t nxt; 
  
  if (establish_connection(&nxt) != 0) {
     return 1;
  }


  
  close(nxt.socket_fd);
  return 0;
}
