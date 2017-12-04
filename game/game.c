#include <msp430.h>
#include "libTimer.h"
#include "clocksTimer.h"
#include <lcdutils.h>
#include <lcddraw.h>
#include "abCircle.h"
#include "states.h"
#include "switches.h"
#include "buzzer.h"

int main() {
  configureClocks();
  lcd_init();
  buzzer_init();
  switch_init();
  states_init();
  //lcd_init();
  or_sr(0x8);

  while(1) {
    switch_interrupt_handler();
  }
}
