#ifndef _NMEA_H
#define _NMEA_H

typedef struct 
{
  unsigned char status; // 1: GPS sync, GPS not in sync
  long lat;             // latitude in degree x10E+6 (+ N, -S)
  long lon;             // longitude in degree x10E+6 (+E, -W)
  unsigned char utc_h;  // hour 0..23
  unsigned char utc_m;  // minute 0..59
  unsigned char utc_s;  // second: 0..59
  unsigned char date_d; // day: 1..31
  unsigned char date_m; // month: 1..12
  unsigned char date_y; // year: 0..99
} gprmc;

void nmea_parse(char c);
void nmea_set_gprmc_cb(void (*_nmea_rx_gprmc)(gprmc *));
int nmea_atoi_n(char *instr, unsigned char nstart, unsigned char nstop);

#endif
