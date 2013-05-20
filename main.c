/*
  sample code for MSP430 UART
  transmit serial data at 9600,N,8,1

  code for msp430-gcc
  MSP430G2553 at 1 MHz
*/

#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>
#include "uart.h"
#include "fifo.h"
#include "millis.h"
#include "job.h"
#include "nmea.h"

// prototypes
void red_led_blink(void); 
void green_led_blink(void);
void nmea_rx_gprmc(gprmc *_gprmc);
void start_adc(void);
void adc_measure(unsigned int chan, unsigned int ref);

// some global vars
job job1;
job job2;
job job3;

unsigned int  adc_value = 0;
unsigned char adc_ok    = 0;
char c;

/*
 * main routines
 */
int main(void)
{
  // init watchdog for millis()
  wdt_init();
  // init internal digitally controlled oscillator DCO
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  // IO setup (launchpad : bit 0 -> red led, bit 6 -> green led)
  P1DIR  |= BIT0 | BIT6;
  P1OUT  &= ~(BIT0 | BIT6);
  // UART init
  uart_init();
  // init job
  job_init(&job1,   100, red_led_blink);
  job_init(&job2, 10000, green_led_blink);
  job_init(&job3,  2000, start_adc);
  // init nmea callback
  //nmea_set_gprmc_cb(nmea_rx_gprmc);
  // start interrupt
  __enable_interrupt();

  // hello msg
  printf("\n\rsystem start\n\r");

  // program loop
  while(1) {
    // go to lowpower mode 0
    //__bis_SR_register(LPM0_bits);
    // wake up from lowpower
    // job schedule
    job_update(&job1);
    job_update(&job2);
    //job_update(&job3);
    // manage adc result
    if (adc_ok == 1) {
      adc_ok = 0;
      int temp_c = ((0.00146*adc_value)-0.986)/0.00355;
      printf("(%lu) ADC=%d\n\r", millis(), temp_c);
    }
    // nmea parsing
    while((c = uart_getc()) != EOF) {
      nmea_parse(c);
    }
  }
}

void red_led_blink(void) 
{
  P1OUT ^= BIT0;
}

void green_led_blink(void) 
{
  P1OUT ^= BIT6;
}

void nmea_rx_gprmc(gprmc *_gprmc)
{
  printf("gprmc is call\n\r");
}

void start_adc(void) {
  adc_measure(INCH_10, 0);
}

void adc_measure(unsigned int chan, unsigned int ref)
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0  = SREF_1 | ADC10SHT_3 | REFON | ADC10ON | ref | ADC10IE;
  
  ADC10CTL1 = ADC10SSEL_3 | chan;
  __delay_cycles(128);
  ADC10CTL0 |= ENC + ADC10SC;
}

/*
 * ADC interrupt routine
 * call when convertion is finish
 */
interrupt(ADC10_VECTOR) adc_isr(void) {
  adc_value = ADC10MEM;
  adc_ok    = 1;
}
