#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>

#include "uart.h"
#include "fifo.h"

// Rx = P1.1 and Tx = P1.2
#define RXD BIT1
#define TXD BIT2

// prototype : callback handler for receive
void (*uart_rx_isr_ptr)(unsigned char c);

// uart tx/rx FIFO
fifo tx = {.raddr = 0, .waddr = 0};
fifo rx = {.raddr = 0, .waddr = 0};

// define putchar for link stdio with the UART
int putchar(int c)
{
  uart_putc(c);
  return 1;
}

// init hardware uart
void uart_init(void)
{
  uart_set_rx_isr_ptr(0L);
  // set some register
  P1SEL     = RXD | TXD;                       
  P1SEL2    = RXD | TXD;
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
  /*** 
    debug 
  ***/
  int i;
  uart_puts((char *)"start UART driver\n\r");
  // add data to FIFO for test
  for (i = 0; i < 32; i++)
    fifo_putchar(&rx, 'T');
  fifo_getchar(&rx);
  // print FIFO size
  int _rx_size = fifo_size(&rx);
  int _tx_size = fifo_size(&tx);
  printf ("_rx_size %d\n\r", _rx_size);
  printf ("_tx_size %d\n\r", _tx_size);
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
