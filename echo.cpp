
////////////////////////////////////////////////////////////////////////////////
// echo
// simple version of Unix style 'echo'
//
// example:  "echo Hello world"
//  prints:  "Hello world\n" to serial monitor.
//
////////////////////////////////////////////////////////////////////////////////
//
#include "Arduino.h"
#include "CommandParser.hpp"
#include "Streaming.h"

////////////////////////////////////////////////////////////////////////////////
int echo(int argc, char **argv)
{
  auto lastArg = argc - 1;
  for ( int i = 1; i < argc; i++) {

    Serial << argv[i];

    if (i < lastArg)
      Serial << F(" ");
  }
  Serial << endl;

  return EXIT_SUCCESS;
}
