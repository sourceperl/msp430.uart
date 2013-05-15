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

// prototypes
void red_led_blink(void); 
void green_led_blink(void);

// some global vars
job job1;
job job2;

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
  // start interrupt
  __enable_interrupt();

  // hello msg
  printf("\n\rsystem start\n\r");

  // program loop
  while(1) {
    job_update(&job1);
    job_update(&job2);
    /*
    // go to lowpower mode 1 with interrupt enable
    __bis_SR_register(LPM1_bits | GIE);
    // wake up !
    printf("wake up time: %lu\n\r", millis());
    uart_wait_tx();
    char c;
    while((c = uart_getc()) != EOF) {
      if (c == '1')
        P1OUT ^= BIT0;
      if (c == '2')
        P1OUT ^= BIT6;
      printf("%c", c);
    }
    uart_wait_tx();
    printf("\n\r");
    */
  }
}

void red_led_blink(void) {
  P1OUT ^= BIT0;
}

void green_led_blink(void) {
  P1OUT ^= BIT6;
  printf("wake up time: %lu\n\r", millis());
}
