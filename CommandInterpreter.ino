// basic example testing of the command parser.

#include "CommandParser.hpp"

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

// non blocking LED toggle
extern void toggleLED_nb(void);

////////////////////////////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:

  //shell->execute("echo Hello World");
  shell->executeIfInput();

  // show loop() is still running
  toggleLED_nb();
}
