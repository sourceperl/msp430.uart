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
 * Init FIFO struct.
 *
 */
void fifo_init(fifo *_fifo)
{
  _fifo->size  = 0;
  _fifo->raddr = 0;
  _fifo->waddr = 0;
}

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
  if (_fifo->size == 0)
    // empty fifo: return EOF
    c = EOF;
  else {
    // read current value
    c = _fifo->data[_fifo->raddr];
    _fifo->size--;
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
  // stop interrupt
  __bic_SR_register(GIE);
  // check fifo level
  if (_fifo->size >= FIFO_BUFFER_SIZE) {
    // start interrupt
    __bis_SR_register(GIE);
    return EOF;
  }
  // write current value
  _fifo->data[_fifo->waddr] = c;
  _fifo->size++;
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
 * Safe access to size of the FIFO.
 *
 * @return	number of byte in FIFO
 */
unsigned char fifo_size(fifo *_fifo)
{
  unsigned char size;
  // stop interrupt
  __bic_SR_register(GIE);
  size = _fifo->size;
  // start interrupt
  __bis_SR_register(GIE);
  return size;
}
