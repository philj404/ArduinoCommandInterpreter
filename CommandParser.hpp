
////////////////////////////////////////////////////////////////////////////////
class CommandParser {
  public:
    CommandParser(void);
    //CommandParser( Stream * inputStream);

    // functions must have a signature like: "int hello(int argc, char ** argv)"
    typedef int (*CommandFunction)(int, char ** );
    //
    void add(const char * name, CommandFunction f);

    // check for a complete command and run it if available
    // non blocking
    int executeIfInput(void);
    int execute( const char aCommandString[]);
    bool prepInput(void);
    
    class Command;

    void resetBuffer(void);
  private:
    int execute(void);
    int execute(int argc, char** argv);

    int report(const char * message, int errorCode);
    static const char BUFSIZE = 88;
    static const char MAXARGS = 10;
    char buffer[BUFSIZE];

    Command * first;
};

////////////////////////////////////////////////////////////////////////////////
extern CommandParser *shell;

//simple example command
extern int echo(int argc, char **argv);