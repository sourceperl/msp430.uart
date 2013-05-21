#ifndef _NMEA_H
#define _NMEA_H

typedef struct 
{
  long latitude;
  long longitude;
  unsigned char utc_h;
  unsigned char utc_m;
  unsigned char utc_s;
  unsigned char day;
  unsigned char month;
  unsigned char year;
} gprmc;

void nmea_parse(char c);
void nmea_set_gprmc_cb(void (*_nmea_rx_gprmc)(gprmc *));

#endif
