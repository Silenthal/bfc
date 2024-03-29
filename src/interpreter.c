#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "memory.h"

// Vars
#define ERRSIZE 256
static int instPointer = 0;
static char* prog = NULL;
static int progSz = 0;
static bool error = false;
static char lastError[ERRSIZE];

// Machine Ops
char fetch();
void reset();
int readPC();
bool incPC();
bool decPC();

// Instructions
uint8_t readMem();
void readIO();
void writeIO();
void incMem();
void decMem();
void incMemPointer();
void decMemPointer();
void openLoop();
void closeLoop();

uint8_t readMem()
{
    return bf_mem_read();
}

void readIO()
{
    bf_mem_write(getchar());
}

void writeIO()
{
    putchar(bf_mem_read());
}

void incMem()
{
    bf_mem_inc();
}

void decMem()
{
    bf_mem_dec();
}

void incMemPointer()
{
    bf_mem_right();
}

void decMemPointer()
{
    bf_mem_left();
}

void openLoop()
{
    if (readMem() == 0)
    {
        int matcher = 1,
            curInstPtr = readPC();
        do
        {
            if (!incPC())
            {
                snprintf(lastError, ERRSIZE, "Error: unmatched open bracket at pos %d\n", curInstPtr);
                error = true;
                return;
            }
            if (fetch() == '[')
            {
                ++matcher;
            }
            else if (fetch() == ']')
            {
                --matcher;
            }
        }
        while (matcher > 0);
    }
}

void closeLoop()
{
    if (readMem() != 0)
    {
        int matcher = 1,
            curInstPtr = readPC();
        do
        {
            if (!decPC())
            {
                snprintf(lastError, ERRSIZE, "Error: unmatched closed bracket at pos %d\n", curInstPtr);
                error = true;
                return;
            }
            if (fetch() == ']')
            {
                ++matcher;
            }
            else if (fetch() == '[')
            {
                --matcher;
            }
        }
        while (matcher > 0 && fetch() != '\0');
    }
}



char fetch()
{
    if (instPointer >= progSz)
    {
        return '\0';
    }
    return prog[instPointer];
}

void reset()
{
    bf_mem_clear();
    instPointer = 0;
}

int readPC()
{
    return instPointer;
}

bool incPC()
{
    return instPointer++ < progSz;
}

bool decPC()
{
    return instPointer-- >= 0;
}

bool matchBraces()
{
    int stack = 0;
    char pCur;
    while ((pCur = fetch()) != '\0')
    {
        if (pCur == '[')
        {
            ++stack;
        }
        else if (pCur == ']')
        {
            --stack;
        }
        if (stack < 0)
        {
            reset();
            return false;
        }
        incPC();
    }
    reset();
    return stack == 0;
}

bool bf_setup(const char* program)
{
    progSz = strlen(program);
    prog = malloc(progSz);
    memcpy(prog, program, progSz);
    if (!matchBraces())
    {
        snprintf(lastError, ERRSIZE, "Error: program doesn't have matching braces.");
        error = true;
        return false;
    }
    reset();
    return true;
}

bool bf_setup_with_file(const char* filename)
{
    FILE* inFile = fopen(filename, "r");
    if (!inFile)
    {
        snprintf(lastError, ERRSIZE, "Error: File \"%s\" doesn't exist or cannot be opened.\n", filename);
        error = true;
        return false;
    }
    fseek(inFile, 0, SEEK_END);
    progSz = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);
    prog = malloc(progSz);
    char cur;
    long ind = 0L;
    while ((cur = fgetc(inFile)) != EOF)
    {
        prog[ind++] = cur;
    }
    fclose(inFile);
    if (!matchBraces())
    {
        snprintf(lastError, ERRSIZE, "Error: program doesn't have complete matching braces.\n");
        error = true;
        return false;
    }
    reset();
    return true;
}

const char* bf_get_last_error()
{
    return lastError;
}

void bf_free_resources()
{
    reset();
    free(prog);
}

bool bf_loop()
{
    while (true)
    {
        if (error)
        {
            return false;
        }
        switch(fetch())
        {
            case '.':
            {
                writeIO();
                break;
            }
            case ',':
            {
                readIO();
                break;
            }
            case '-':
            {
                decMem();
                break;
            }
            case '+':
            {
                incMem();
                break;
            }
            case '<':
            {
                decMemPointer();
                break;
            }
            case '>':
            {
                incMemPointer();
                break;
            }
            case '[':
            {
                openLoop();
                break;
            }
            case ']':
            {
                closeLoop();
                break;
            }
            case '\0':
            {
                return true;
            }
            default:
            {
                break;
            }
        }
        incPC();
    }
    return true;
}

#undef MEMSIZE
