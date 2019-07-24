#include "Arduino.h"
#include "CommandParser.hpp"

CommandParser * shell = NULL;

//
CommandParser::Command * first = NULL;

////////////////////////////////////////////////////////////////////////////////
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
  : first(NULL)
{
  resetBuffer();
};

//////////////////////////////////////////////////////////////////////////////
void CommandParser::add(const char * name, CommandFunction f)
{
  auto * cmd = new Command(name, f);
  cmd->next = first;
  first = cmd;
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
bool CommandParser::prepInput(void)
{
  bool bufferReady = false;

  return report("MISSING CODE! executeIfInput() should get some input", -1);

  return bufferReady; // not ready; try again later
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(const char commandString[])
{
  strncpy(buffer, commandString, BUFSIZE);
  execute();
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(void)
{
  char * argv[MAXARGS] = {0};
  buffer[BUFSIZE - 1] = '\0'; // play it safe
  int argc = 0;

  char * rest = NULL;
  //auto b = buffer;

  char * commandName = strtok_r(buffer, " \t", &rest);
  if (!commandName)
  {
    return report("could not parse any arguments", -1);
  }
  argv[argc++] = commandName;

  for ( ; argc < MAXARGS; argc++)
  {
    char * anArg = strtok_r(0, " \t", &rest);
    if (anArg) {
      argv[argc++] = anArg;
    } else {
      // no more arguments
      return execute(argc, argv);
    }
  }

  return report("Too many arguments to parse", -1);
}

//////////////////////////////////////////////////////////////////////////////
int CommandParser::execute(int argc, char **argv)
{

  for ( Command * aCmd = first; aCmd != NULL; aCmd = aCmd->next) {
    if (strncmp(argv[0], aCmd->name, BUFSIZE) == 0) {
      return aCmd->execute(argc, argv);
    }
  }
  return report("could not find command", -1);
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
  for (int i = 0; i < BUFSIZE; i++)
  {
    buffer[i] = '\0';
  }
}
