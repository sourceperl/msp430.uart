#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nmea.h"

#define NMEA_BUF_SIZE 12

#define NMEA_TYPE_NONE  0
#define NMEA_TYPE_GPRMC 1
#define NMEA_TYPE_GPGSV 2

unsigned char nmea_xor;
unsigned char nmea_xor_char;
unsigned char nmea_xor_ok;
unsigned char nmea_type;
char nmea_buf[NMEA_BUF_SIZE];
unsigned char nmea_field;
unsigned char nmea_f_pos;
unsigned char i;
gprmc _gprmc;
void (*nmea_rx_gprmc)(gprmc *);

void nmea_parse(char c) {
  switch(c) {
    case '$':
      nmea_type = NMEA_TYPE_NONE;
      nmea_field      = 0;
      nmea_f_pos      = 0;
      nmea_xor        = 0;
      nmea_xor_char   = 1;
      return;
    case '\r':
      // nmea xor checksum test
      nmea_buf[2] = '\0';
      // checksum ok : call callback function for each frame type
      if (strtol(nmea_buf, NULL, 16) == nmea_xor) {
        switch(nmea_type) {
          case NMEA_TYPE_GPRMC:
            // call gprmc callback
            if(nmea_rx_gprmc != 0L)
              (nmea_rx_gprmc)(&_gprmc);
          break;
        }
      }
      return;
    case '*':
      nmea_xor_char = 0;
    case ',':
      // new field, check it
      if (nmea_field == 0) {
        if (strncmp(nmea_buf, "GPRMC", nmea_f_pos) == 0)
          nmea_type = NMEA_TYPE_GPRMC;
      } else {  
        // decode GPRMC frame
        if (nmea_type == NMEA_TYPE_GPRMC) {
          switch(nmea_field) {
            // UTC Time
            case 1:
              _gprmc.utc_h = 12;
              for (i=0; i < nmea_f_pos; i++) 
                printf("%c", nmea_buf[i]);
              printf("\n\r");
            break;
            // status (A=valid, V=no valid)
            case 2:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // latitude
            case 3:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // N=north, S=south
            case 4:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // longitude
            case 5:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // E=east, W=west
            case 6:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // Speed over ground (in knots)
            case 7:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // Course over ground
            case 8:
              //printf("%c\n\r", nmea_buf[0]);
            break;
            // Date
            case 9:
              for (i=0; i < nmea_f_pos; i++) 
                printf("%c", nmea_buf[i]);
              printf("\n\r");
            break;
          }
        } 
      }
      nmea_field++;
      nmea_f_pos = 0;
      // compute xor checksum
      if (nmea_xor_char)
        nmea_xor ^= c;
      return;
    default:  
      // add char to buffer, check overflow
      if (nmea_f_pos < sizeof(nmea_buf) - 1)
        nmea_buf[nmea_f_pos++] = c;
      // compute xor checksum
      if (nmea_xor_char)
        nmea_xor ^= c;
  }
}

void nmea_set_gprmc_cb(void (*_nmea_rx_gprmc)(gprmc *))
{
  nmea_rx_gprmc = _nmea_rx_gprmc;
}
