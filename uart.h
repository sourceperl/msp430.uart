#ifndef __UART_H
#define __UART_H

/*
 * initialize soft UART
 */
void uart_init(void);

/*
 * read one character from UART blocking.
 *
 * @return	character received 
 */
unsigned char uart_getc();

/*
 * write one chracter to the UART blocking.
 *
 * @param[in]	*c	the character to write
 */
void uart_putc(unsigned char c);

/*
 * write string to the UART blocking.
 *
 * @param[in]	*str	the 0 terminated string to write
 */
void uart_puts(const char *str);

#endif
