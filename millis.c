#include <msp430.h>
#include <legacymsp430.h>
#include "millis.h"

// some vars
unsigned long _millis = 0;

void wdt_init(void)
{
  // set watchdog timer interval to 32ms, enable interrupt
  WDTCTL  = WDT_MDLY_32;
  IE1    |= WDTIE;
}

/*
 * Safe millis access
 */
unsigned long millis(void) 
{
  unsigned long safe_millis;
  __disable_interrupt();
  safe_millis = _millis;
  __enable_interrupt();
  return safe_millis;
}

/*
 * Watchdog (in timer mode) interrupt routine
 * use to emulate Arduino millis()
 */
interrupt(WDT_VECTOR) wd_timer_isr(void) {
  _millis += 32;
  // wake up processor for job handling
  __bic_SR_register_on_exit(LPM0_bits);
}
