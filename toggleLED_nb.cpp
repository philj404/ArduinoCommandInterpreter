////////////////////////////////////////////////////////////////////////////////
// non blocking LED toggle
//
#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////
void toggleLED_nb(void)
{
  // this should work for ~50 days (until millis() wraps)
  static unsigned long nextToggle = 0;
  const unsigned long interval = 1000;  // millisec
  if (millis() > nextToggle)
  {
    // toggle
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    nextToggle = millis() + interval; // next time to toggle
  }
}
