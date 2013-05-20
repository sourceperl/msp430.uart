#ifndef _NMEA_H
#define _NMEA_H

typedef struct 
{
  volatile long latitude;
  volatile long longitude;
  volatile unsigned char utc_h;
  volatile unsigned char utc_m;
  volatile unsigned char utc_s;
  volatile unsigned char day;
  volatile unsigned char month;
  volatile unsigned char year;
} gprmc;

void nmea_parse(char c);
void nmea_set_gprmc_cb(void (*_nmea_rx_gprmc)(gprmc *));

#endif
