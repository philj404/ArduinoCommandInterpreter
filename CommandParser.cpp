#include "Arduino.h"
#include "CommandParser.hpp"
#include "Streaming.h"

CommandParser * shell = NULL;

//
CommandParser::Command * CommandParser::firstCommand = NULL;

////////////////////////////////////////////////////////////////////////////////
// associate a named command with the function to call.
class CommandParser::Command {
  public:
    Command(const char * n, CommandFunction f): name(n), myFunc(f) {};

    int execute(int argc, char **argv)
    {
      return myFunc(argc, argv);
    };

    const char * name;
    CommandFunction myFunc;
    Command * next;
};

////////////////////////////////////////////////////////////////////////////////
CommandParser::CommandParser()
{
  resetBuffer();

  // simple help.
  add("help", CommandParser::printHelp);
};

//////////////////////////////////////////////////////////////////////////////
void CommandParser::add(const char * name, CommandFunction f)
{
  auto * newCmd = new Command(name, f);

  // insert in list alphabetically
  // from stackoverflow...

  Command* temp2 = firstCommand;
  Command** temp3 = &firstCommand;
  while (temp2 != NULL && (strncasecmp(newCmd->name, temp2->name, BUFSIZE) > 0) )
  {
    temp3 = &temp2->next;
    temp2 = temp2->next;
  }
  *temp3 = newCmd;
  newCmd->next = temp2;
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::executeIfInput(void)
{
  bool bufferReady = prepInput();

  if (bufferReady)
    return execute();
  else
    return EXIT_SUCCESS; // not ready; try again later
}

//////////////////////////////////////////////////////////////////////////////
// Arduino serial monitor appears to 'cook' lines before sending them
// to output, so some of this is overkill.
//
// But for serial terminals, backspace would be useful.
//
bool CommandParser::prepInput(void)
{
  bool bufferReady = false; // assume not ready

  int c = Serial.read();
  switch (c)
  {
    case -1: // No character present; don't do anything.
      break;

    // Line editing characters
    case 127: // DEL delete key
    case '\b':  // CTRL(H) backspace
      // Destructive backspace: remove last character
      if (inptr > 0) {
        Serial.print("\010 \010");  // "\b \b" -- remove char in raw UI
        linebuffer[--inptr] = 0;
      }
      break;

    case 0x12: //CTRL('R')
      //Ctrl-R retypes the line
      Serial.print("\r\n");
      Serial.print(linebuffer);
      break;

    case 0x15: //CTRL('U')
      //Ctrl-U deletes the entire line and starts over.
      Serial.println("XXX");
      resetBuffer();
      break;

    case '\r':  //CTRL('M') carriage return (or "Enter" key)
      // raw input only sends "return" for the keypress
      // line is complete
      Serial.println();     // Echo newline too.
      bufferReady = true;
      break;

    case '\n':  //CTRL('J') linefeed
      // ignore newline as 'raw' terminals may not send it.
      // Serial Monitor sends a "\r\n" pair by default
      break;

    default:
      // Otherwise, echo the character and append it to the buffer
      linebuffer[inptr++] = c;
      Serial.write(c);
      break;
  }

  return bufferReady;
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(const char commandString[])
{
  // overwrites anything in linebuffer; hope you don't need it!
  strncpy(linebuffer, commandString, BUFSIZE);
  execute();
}

////////////////////////////////////////////////////////////////////////////////
//void printBuffer(const char * buf, int size)
//{
//  Serial << "Array:" << endl;
//
//  for (int i = 0; i < size; i++)
//  {
//    auto c = buf[i];
//    Serial << i << ": 0x" << _HEX( c) << " ";
//    if (isGraph(c) || isSpace(c))
//    {
//      Serial << " '" << c << "' ";
//    }
//    Serial << endl;
//  }
//}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(void)
{
  //printBuffer(linebuffer, BUFSIZE);

  char * argv[MAXARGS] = {0};
  linebuffer[BUFSIZE - 1] = '\0'; // play it safe
  int argc = 0;

  char * rest = NULL;
  const char * whitespace = " \t\r\n";
  char * commandName = strtok_r(linebuffer, whitespace, &rest);

  if (!commandName)
  {
    // empty line; no arguments found.
    //return report("could not parse any arguments", -1);
    Serial << F("OK") << endl;
    resetBuffer();
    return EXIT_SUCCESS;
  }
  argv[argc++] = commandName;

  for ( ; argc < MAXARGS; )
  {
    char * anArg = strtok_r(0, whitespace, &rest);
    if (anArg) {
      argv[argc++] = anArg;
    } else {
      // no more arguments
      //printBuffer(linebuffer, BUFSIZE);
      return execute(argc, argv);
    }
  }

  return report("Too many arguments to parse", -1);
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(int argc, char **argv)
{
  //  Serial << argc << " arguments. Command is: ";
  //  for (int i = 0; i < argc; i++)
  //  {
  //    Serial << "\"" << argv[i] << "\" ";
  //  }
  //  Serial << endl;

  for ( Command * aCmd = firstCommand; aCmd != NULL; aCmd = aCmd->next) {
    if (strncasecmp(argv[0], aCmd->name, BUFSIZE) == 0) {
      auto retval = aCmd->execute(argc, argv);
      resetBuffer();
      return retval;
    }
  }
  Serial << F("\"") << argv[0] << F("\": ");

  return report("command not found", -1);
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::report(const char * message, int errorCode)
{
  if (errorCode != EXIT_SUCCESS)
  {
    Serial.print(errorCode);
    if (message[0] != '\0') {
      Serial.print(": ");
      Serial.println(message);
    }
  }
  resetBuffer();
  return errorCode;
}
//////////////////////////////////////////////////////////////////////////////
void CommandParser::resetBuffer(void)
{
  memset(linebuffer, 0, sizeof(linebuffer));
  inptr = 0;
}

//////////////////////////////////////////////////////////////////////////////
// CommandParser::printHelp() is a static method.
// printHelp() can access the linked list of commands.
//
int CommandParser::printHelp(int argc, char **argv)
{
  Serial << F("Commands available are:") << endl;
  auto aCmd = firstCommand;  // first in list of commands.
  while (aCmd)
  {
    Serial << F("  ") << aCmd->name << endl;
    aCmd = aCmd->next;
  }
}
