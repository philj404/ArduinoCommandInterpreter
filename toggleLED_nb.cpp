////////////////////////////////////////////////////////////////////////////////
// non blocking LED toggle
//
#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////
void toggleLED_nb(void)
{
  static unsigned long lastToggle = millis();
  static const unsigned long interval = 2000;  // millisec
  auto now = millis();
  if (now - lastToggle > interval)
  {
    lastToggle = now;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle
  }
}
