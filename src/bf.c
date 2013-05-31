#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#define EXIT_OK 0
#define EXIT_ERROR 1

void printHelp(const char* progName);
void exitProg(int exitCode);

void main(int argc, char* argv[])
{
    if (argc == 2)
    {
#ifdef WIN32
        if (!strcmp(argv[1], "/?"))
#else
        if (!(strcmp(argv[1], "-h") && strcmp(argv[1], "--help")))
#endif    
        {
            printHelp(argv[0]);
        }
        if (!(bf_setup(argv[1]) && bf_loop()));
        {
            fprintf(stderr, "%s", bf_get_last_error());
            exitProg(EXIT_ERROR);
        }
    }
    else if (argc == 3)
    {
#ifdef WIN32
        if (!strcmp(argv[1], "/f"))
#else
        if (!strcmp(argv[1], "-f"))
#endif
        {
            if (!(bf_setup_with_file(argv[2]) && bf_loop()))
            {
                fprintf(stderr, "%s", bf_get_last_error());
                exitProg(EXIT_ERROR);
            }
        }
    }
    else
    {
        printHelp(argv[0]);
    }
    exitProg(EXIT_OK);
}

void printHelp(const char* progName)
{
    fprintf(stdout, "Usage: %s \"brainfck string\"\n", progName);
#ifdef WIN32
    fprintf(stdout, "       %s /f \"filename\"", progName);
#else
    fprintf(stdout, "       %s -f \"filename\"", progName);
#endif
}

void exitProg(int exitCode)
{
    bf_free_resources();
    exit(exitCode);
}