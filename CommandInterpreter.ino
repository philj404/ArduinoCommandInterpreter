#include "CommandParser.hpp"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  shell = new CommandParser();

  shell->add("echo", echo);

}

////////////////////////////////////////////////////////////////////////////////
// non blocking
void toggleLED_nb()
{
  static unsigned long nextToggle = 0;
  unsigned long interval = 1000;  // millisec
  if(millis() > nextToggle)
  {
    // toggle
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    nextToggle = millis()+ interval;
  }
}



////////////////////////////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:

  //shell->execute("echo Hello World");
  shell->executeIfInput();

  // show loop() is still running
  toggleLED_nb();
}
