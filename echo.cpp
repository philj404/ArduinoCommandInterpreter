#include "Arduino.h"
#include "CommandParser.hpp"
////////////////////////////////////////////////////////////////////////////////
// echo
// simple version of Unix style 'echo'
//
////////////////////////////////////////////////////////////////////////////////
//
int echo(int argc, char **argv)
{
  auto lastArg = argc - 1;
  for ( int i = 1; i < argc; i++) {

    Serial.print(argv[i]);

    if (i < lastArg)
      Serial.print(" ");
  }
  Serial.println();

  return EXIT_SUCCESS;
}
