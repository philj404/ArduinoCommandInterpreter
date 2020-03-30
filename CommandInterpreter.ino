// basic example testing of the command parser.
#include "Streaming.h"

#include "CommandParser.hpp"

////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  //Serial.begin(9600); older default bit rate
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Forgot what sketch was loaded to this board?
  //
  // Hint1: use the F() macro to keep const strings in FLASH and save RAM
  // Hint2: "Compiler " "catenates consecutive "
  //         "strings together"
  //         (can improve readability for very long strings)
  //
  Serial <<
         F(
           "Running " __FILE__ ", Built " __DATE__
#ifdef SHOW_BUILDTIME
           ", " __TIME__
#endif
         )
         << endl;

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
  //toggleLED_nb();
}
