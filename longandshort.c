#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum{
    shortFile_Upper,
    longFile_Lower
};

void setLineToUpper(char* input)
{
    int i = 0;
    while(input[i] != '\0' && i < 256)
    {
        input[i] = toupper(input[i]);
        i++;
    }
}

void setLineToLower(char* input)
{
    int i = 0;
    while(input[i] != '\0' && i < 256)
    {
        input[i] = tolower(input[i]);
        i++;
    }
}

void trim(char* input)
{
    int input_length = strlen(input);

    // I used a FOR with a break instead of a WHILE
    // as I just wanted that guaranteed breakout condition
    // while iterating easily.
    for(int i = 0; i < input_length; i++)
    {
        // I habitually had to try to capture '\r'
        if( input[i] == '\n' || input[i] == '\r' )
        {
            input[i] = '\0';
            break;
        }
    }
}

void writeFile(char* input, int mode, FILE* file, int *count)
{
    if(mode == shortFile_Upper)
    {
        setLineToUpper(input);
    }
    else
    {
        setLineToLower(input);
    }
    if((*count) == 0)
    {
        fprintf(file, "%s", input);
    }
    else
    {
        fprintf(file, "\n%s", input);
    }

    (*count)++;
}


int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Input missing from invocation. Please make sure you format your call as \"./a.out <Target File> <Short File> <Long File>\"\n");
        return 0;
    }

    FILE* readFromFile = fopen(argv[1], "r");
    if(!readFromFile)
    {
        printf("Target file \"%s\" is unable to be opened for read.\n", argv[1]);
        return 0;
    }

    FILE* writeShortFile = fopen(argv[2], "w");
    if(!writeShortFile)
    {
        printf("Output file \"%s\" is unable to be opened for writing.\n", argv[2]);
        return 0;
    }

    FILE* writeLongFile = fopen(argv[3], "w");
    if(!writeLongFile)
    {
        printf("Output file \"%s\" is unable to be opened for writing.\n", argv[3]);
        return 0;
    }

    char line[50];
    char *state;
    int count_short = 0, count_long = 0;

    while(!feof(readFromFile))
    {
        // Get a line from the file, make sure we don't double-read the last line
        if(fgets(line, 50, readFromFile))
        {
            // Clean off the newlines
            trim(line);

            if(strlen(line) < 20)
            {
                writeFile(line, shortFile_Upper, writeShortFile, &count_short);
            }
            else
            {
                writeFile(line, longFile_Lower, writeLongFile, &count_long);
            }
        }
    }
    fclose(readFromFile);
    fclose(writeShortFile);
    fclose(writeLongFile);

    printf("%d lines written to %s\n", count_short, argv[2]);
    printf("%d lines written to %s\n", count_long, argv[3]);
    
    return 0;
}