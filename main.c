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

// some prototypes
unsigned long millis(void);

// some vars
unsigned long _millis = 0;

// ISR called when a char is receive
void uart_rx_isr(unsigned char c) 
{
  // echo millis current value
  printf("millis()=%lu\n\r", millis());
  // toggle red led
  P1OUT ^= BIT0;
}
 
/*
 * main routines
 */
int main(void)
{
  // set watchdog timer interval to 32ms, enable interrupt
  WDTCTL  = WDT_MDLY_32;
  IE1    |= WDTIE;
  // init internal digitally controlled oscillator
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  // IO setup (launchpad : bit 0 -> red led, bit 6 -> green led)
  P1DIR   = BIT0 | BIT6;
  P1OUT   = BIT0 | BIT6;
  // UART init
  uart_init();

  // register ISR called when data was received
  uart_set_rx_isr_ptr(uart_rx_isr);

  // enable global interrupt flag
  __bis_SR_register(GIE);

  printf("start\n\r");

  // go to lowpower mode 0 with interrupt enable
  __bis_SR_register(LPM0_bits | GIE);

  return 0;
}

/*
 * Watchdog (in timer mode) interrupt routine
 * use to emulate Arduino millis()
 */
interrupt(WDT_VECTOR) wd_timer_isr(void) {
  _millis += 32;
}

/*
 * Safe millis access
 *
 */
unsigned long millis(void) {
  unsigned long safe_millis;
  __disable_interrupt();
  safe_millis = _millis;
  __enable_interrupt();
  return safe_millis;
}
