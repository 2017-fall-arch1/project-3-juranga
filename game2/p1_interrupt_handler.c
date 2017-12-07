#include <msp430.h>
#include "switches.h"

void
__interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & SW1) {
    P2IFG &= ~SW1;
    wdt_c_handler();
  }
  else if (P2IFG & SW2) {
    P2IFG &= ~SW2;
    wdt_c_handler();
  }
  else if (P2IFG & SW3) {
    P2IFG &= ~SW3;
    wdt_c_handler();
  }
  else if (P2IFG & SW4) {
    P2IFG &= ~SW4;
    wdt_c_handler();
  }
}

