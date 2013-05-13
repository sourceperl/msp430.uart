/* 
Simple fifo struct access functions


first parameter of evry function is a pointer to the fifo struct (see fifo.h).

example :
fifo_getchar(&thefifostruct);


fifo design: 
read ptr  data array  write ptr
          |  max   |
          |  ...   |
          |   2    |
          |   1    |
raddr  -> |   0    | <- waddr

Code under GPLv2.
Share, it's happiness !
*/

#include <msp430.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include "fifo.h"

/**
 * Read one character from FIFO.
 *
 * @return	current character or EOF if FIFO empty
 */
int fifo_getchar(fifo *_fifo)
{
  unsigned char c;
  // stop interrupt
  __bic_SR_register(GIE);
  // check fifo level
  if (_fifo->raddr == _fifo->waddr)
    // empty fifo: return EOF
    c = EOF;
  else {
    // read current value
    c = _fifo->data[_fifo->raddr];
    // set read pointer
    if (_fifo->raddr < (FIFO_BUFFER_SIZE - 1)) {
      _fifo->raddr++;
    } else {
      _fifo->raddr = 0;
    }
  }
  // start interrupt
  __bis_SR_register(GIE);
  return c;
}

/**
 * Write one character to FIFO.
 *
 * @return	1 if write ok or EOF if FIFO full
 */
int fifo_putchar(fifo *_fifo, int c)
{
  // check fifo level
  if (fifo_size(_fifo) >= FIFO_BUFFER_SIZE) 
    // fifo is full: return EOF
    return EOF;
  // stop interrupt
  __bic_SR_register(GIE);
  // write current value
  _fifo->data[_fifo->waddr] = c;
  // set write pointer
  if (_fifo->waddr < (FIFO_BUFFER_SIZE - 1)) 
    _fifo->waddr++;
  else  
    _fifo->waddr = 0;
  // start interrupt
  __bis_SR_register(GIE);
  return 0;
}

/**
 * Read size of the FIFO.
 *
 * @return	number of byte in FIFO
 */
unsigned char fifo_size(fifo *_fifo)
{
  char size;
  // stop interrupt
  __bic_SR_register(GIE);
  size = (_fifo->waddr - _fifo->raddr);
  // start interrupt
  __bis_SR_register(GIE);
  // check rollover
  if (size < 0)
    size += FIFO_BUFFER_SIZE;
  return size;
}
