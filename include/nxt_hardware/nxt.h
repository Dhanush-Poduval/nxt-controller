#define NXT_H
#ifndef NXT_H 
#include <stdio.h>
#include <stdlib.h>

enum motors{
  int A ,
  int B ,
  int C
};

struct bytes_message{
  uint8_t arr[8];
};

uint8_t connection(uint8_t arr[8]);
uint8_t check_firmware(uint8_t arr[8]);

