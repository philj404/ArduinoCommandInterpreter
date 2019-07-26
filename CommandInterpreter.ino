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

  //example
  shell->add("echo", echo);

  addArduinoCommands(shell);

  Serial.println("Ready.");
}

////////////////////////////////////////////////////////////////////////////////
// non blocking LED toggle
//
void toggleLED_nb()
{
  // this should work for ~50 days (until millis() wraps)
  static unsigned long nextToggle = 0;
  unsigned long interval = 1000;  // millisec
  if (millis() > nextToggle)
  {
    // toggle
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    nextToggle = millis() + interval; // next time to toggle
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
