#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>

#include "uart.h"
#include "fifo.h"

// prototype : callback handler for receive
void (*uart_rx_isr_ptr)(unsigned char c);

// uart tx/rx FIFO
fifo tx;
fifo rx;

// define putchar for link stdio with the UART
int putchar(int c)
{
  uart_putc(c);
  return 1;
}

// init hardware uart
void uart_init(void)
{
  // init FIFO
  fifo_init(&tx);
  fifo_init(&rx);
  // init ISR ptr
  uart_set_rx_isr_ptr(0L);
  // Rx = P1.1 and Tx = P1.2
  P1SEL     = BIT1 | BIT2;                       
  P1SEL2    = BIT1 | BIT2;
  // select SMCLK                 
  UCA0CTL1 |= UCSSEL_2;
  // baudrate 9600 (SMCLK = 1 MHz)
  UCA0BR0   = 104;
  UCA0BR1   = 0;
  // Modulation UCBRSx = 1
  UCA0MCTL  = UCBRS0;
  // Initialize USCI state machine
  UCA0CTL1 &= ~UCSWRST;
  // Enable USCI_A0 RX interrupt
  IE2      |= UCA0RXIE;
}

void uart_set_rx_isr_ptr(void (*isr_ptr)(unsigned char c)) 
{
  uart_rx_isr_ptr = isr_ptr;	
}

unsigned char uart_getc()
{
  // buzy wait until RX buffer empty
  while (!(IFG2 & UCA0RXIFG));
  return UCA0RXBUF;
}

void uart_putc(unsigned char c)
{
  // buzy wait until Tx buffer empty
  while (!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = c;
}

void uart_puts(const char *str)
{
  while(*str) uart_putc(*str++);
}

// interrupt service routine : Rx char call handler if set
interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  if(uart_rx_isr_ptr != 0L) {
    (uart_rx_isr_ptr)(UCA0RXBUF);
  }
}
