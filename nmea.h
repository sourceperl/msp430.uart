#ifndef _NMEA_H
#define _NMEA_H

typedef struct 
{
  unsigned char status;
  long latitude;
  long longitude;
  unsigned char utc_h;
  unsigned char utc_m;
  unsigned char utc_s;
  unsigned char date_d;
  unsigned char date_m;
  unsigned char date_y;
} gprmc;

void nmea_parse(char c);
void nmea_set_gprmc_cb(void (*_nmea_rx_gprmc)(gprmc *));
int nmea_atoi_n(char *instr, unsigned char nstart, unsigned char nstop);

#endif
