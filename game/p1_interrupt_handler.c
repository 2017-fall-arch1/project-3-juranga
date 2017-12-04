#include <msp430.h>
#include "switches.h"

void
__interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & SW1) {
    P2IFG &= ~SW1;
    switch_interrupt_handler();
  }
  else if (P2IFG & SW2) {
    P2IFG &= ~SW2;
    switch_interrupt_handler();
  }
  else if (P2IFG & SW3) {
    P2IFG &= ~SW3;
    switch_interrupt_handler();
  }
  else if (P2IFG & SW4) {
    P2IFG &= ~SW4;
    switch_interrupt_handler();
  }
}
