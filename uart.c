#include <msp430g2553.h>
#include <legacymsp430.h>
#include <stdio.h>

#include "uart.h"
#include "fifo.h"

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
  // Enable USCI_A0 Rx interrupt
  IE2      |= UCA0RXIE; 
}

unsigned char uart_getc()
{
  // buzy wait until RX buffer empty
  while (!(IFG2 & UCA0RXIFG));
  return UCA0RXBUF;
}

void uart_putc(unsigned char c)
{
  // add char in fifo
  fifo_putchar(&tx, c);
  IE2      |= UCA0TXIE;
  // buzy wait until Tx buffer empty
  //while (!(IFG2 & UCA0TXIFG));
  //UCA0TXBUF = c;
}

void uart_puts(const char *str)
{
  while(*str) uart_putc(*str++);
}

// interrupt service routine : Rx UART/I2C
interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  P1OUT ^= BIT0;
  // USCI A0 UART interrupt
  if (UC0IFG & UCA0RXIFG) {
    char c;
    if (UCA0RXBUF == '\r') {
      // parse code
      printf("cmd ");
      while ((c = fifo_getchar(&rx)) != EOF) 
        printf("%c", c);
      printf("\n\r");
    } else {
      // store char in FIFO
      fifo_putchar(&rx, UCA0RXBUF);
    }
  }
  // USCI BO I2C interrupt (not in use here)
}

// interrupt service routine : Tx UART/I2C
interrupt(USCIAB0TX_VECTOR) USCI0TX_ISR(void)
{  
  P1OUT ^= BIT6;
  __disable_interrupt();
  if (IFG2 & UCA0TXIFG) {
    UCA0TXBUF = tx.data[a];
    if (a < 31) {
      a++;
    } else {
      IE2 &= ~UCA0TXIE;
      a = 0;
    }
  __enable_interrupt();
  }
  // USCI BO I2C interrupt (not in use here) */
}
