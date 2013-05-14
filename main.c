/*
  sample code for MSP430 UART
  transmit serial data at 9600,N,8,1

  code for msp430-gcc
  MSP430G2553 at 1 MHz
*/

#include <msp430g2553.h>
#include <legacymsp430.h>
#include <stdio.h>
#include "uart.h"
#include "fifo.h"
#include "millis.h"

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
  // start interrupt
  __enable_interrupt();

  // hello msg
  printf("\n\rsystem start\n\r");

  // program loop
  while(1) {
    // go to lowpower mode 1 with interrupt enable
    __bis_SR_register(LPM1_bits | GIE);
    // wake up !
    printf("wake up time: %lu\n\r", millis());
    char c;
    while((c = uart_getc()) != EOF) {
      if (c == '1')
        P1OUT ^= BIT0;
      if (c == '2')
        P1OUT ^= BIT6;
      printf("%c", c);
    }
  }
}
