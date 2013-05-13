/*
  sample code for MSP430 UART
  transmit serial data at 9600,N,8,1

  code for msp430-gcc
  MSP430G2553 at 1 MHz
*/

#include <msp430.h>
#include "uart.h"

// ISR called when a char is receive
void uart_rx_isr(unsigned char c) 
{
  // echo char
  uart_putc(c);
  // toggle red led
  P1OUT ^= BIT0;
}
 
// main
int main(void)
{
  // stop watchdog
  WDTCTL  = WDTPW | WDTHOLD;
  // init internal digitally controlled oscillator
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  // IO setup (launchpad : bit 0 -> green led, bit 6 -> green led)
  P1DIR   = BIT0 | BIT6;
  P1OUT   = BIT0 | BIT6;
  // UART init
  uart_init();

  // register ISR called when data was received
  uart_set_rx_isr_ptr(uart_rx_isr);

  __bis_SR_register(GIE);

  uart_puts((char *)"Test\n\r");

  unsigned char c = uart_getc();

  uart_putc(c);
  uart_puts((char *)"\n\rOK\n\r");

  // go to lowpower mode 0 with interrupt enable
  __bis_SR_register(LPM0_bits | GIE);

  return 0;
}
