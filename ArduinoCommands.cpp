// ArduinoCommands.cpp
// support for controlling Arduino pins through a serial command line interface
//
//
////////////////////////////////////////////////////////////////////////////////
#include "Arduino.h"
#include "CommandParser.hpp"
#include "Streaming.h"
// see Arduino.h and pins_arduino.h for useful declarations.

////////////////////////////////////////////////////////////////////////////////
int failMsg(const char * command, int failId, const char * message = 0)
{
  Serial.print(command);
  if (message)
  {
    Serial.print(": ");
    Serial.println(message);
  }
  Serial.println();
  return failId;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// int <--> symbolic translations
//
struct lookupVals
{
  char * name;
  int val;
};

////////////////////////////////////////////////////////////////////////////////
int lookup(const char * aName, const lookupVals entries[])
{
  int i = 0;
  for (; entries[i].name; i++)
  {
    if (strncasecmp(aName, entries[i].name, 20) == 0) {
      //return entries[i].val;
      break;
    }
  }
  return entries[i].val;
}

////////////////////////////////////////////////////////////////////////////////
const char * reverseLookup(int aVal, const lookupVals entries[])
{
  int i = 0;
  for (; entries[i].name; i++)
  {
    if (aVal == entries[i].val) {
      break;
    }
  }
  return entries[i].val;
}

////////////////////////////////////////////////////////////////////////////////
const lookupVals PROGMEM modes[] = {
  //{"input", INPUT},
  {"output", OUTPUT},
  {"pullup", INPUT_PULLUP},
  {NULL, INPUT} // default
};

int setPinMode(int argc, char **argv)
{
  if (argc == 3)
  {
    auto pin = atoi(argv[1]);
    auto mode = lookup(argv[2], modes);

    pinMode(pin, mode);
    return EXIT_SUCCESS;
  }

  return failMsg(argv[0], -1, "bad arg count");
}

////////////////////////////////////////////////////////////////////////////////
int analogRead(int argc, char **argv)
{
  if (argc == 2)
  {
    auto pin = atoi(argv[1]);
    if (pin < 0 || pin > NUM_ANALOG_INPUTS)
    {
      return failMsg(argv[0], -1, "bad analog pin");
    }
    auto val = analogRead(pin);
    Serial.println(val);
    return EXIT_SUCCESS;
  }

  return failMsg(argv[0], -1, "bad arg count");
}
////////////////////////////////////////////////////////////////////////////////
int analogWrite(int argc, char **argv)
{
  if (argc == 3)
  {
    auto pin = atoi(argv[1]);
    if (digitalPinHasPWM(pin))
    {
      return failMsg(argv[0], -1, "bad analog output pin");
    }
    auto val = atoi(argv[2]);
    auto cval = constrain(val, 0, 255);
    if (val != cval)
    {
      return failMsg(argv[0], -1, "analog output out of range");
    }

    analogWrite(pin, val);
    return EXIT_SUCCESS;
  }

  return failMsg(argv[0], -1, "bad arg count");
}

////////////////////////////////////////////////////////////////////////////////
const lookupVals PROGMEM digLevels[] = {
  {"high", HIGH},
  {NULL, LOW} // default
};
////////////////////////////////////////////////////////////////////////////////
int digitalWrite(int argc, char **argv)
{
  if (argc == 3)
  {
    auto pin = atoi(argv[1]);
    if (pin < 0 || pin > NUM_DIGITAL_PINS)
    {
      return failMsg(argv[0], -1, "bad digital pin");
    }
    auto level = lookup(argv[2], digLevels);

    digitalWrite(pin, level);
    return EXIT_SUCCESS;
  }

  return failMsg(argv[0], -1, "bad arg count");
}

////////////////////////////////////////////////////////////////////////////////
int digitalRead(int argc, char **argv)
{
  if (argc == 2)
  {
    auto pin = atoi(argv[1]);
    if (pin < 0 || pin > NUM_DIGITAL_PINS)
    {
      return failMsg(argv[0], -1, "bad digital pin");
    }
    auto val = digitalRead(pin);
    Serial << val << F(" ");
    auto valName = reverseLookup(val, digLevels);
    Serial << valName << endl;

    return EXIT_SUCCESS;
  }

  return failMsg(argv[0], -1, "bad arg count");
}

////////////////////////////////////////////////////////////////////////////////
int addArduinoCommands(CommandParser * shell)
{
  shell->add("setpinmode", setPinMode);
  shell->add("digitalwrite", digitalWrite);
  shell->add("digitalread", digitalRead);
  shell->add("analogread", analogRead);
  shell->add("analogwrite", analogWrite);
  return EXIT_SUCCESS;
}
