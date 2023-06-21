#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "./freader.h"

char* readText(const char* FILEPATH)
{
    FILE* fp = fopen(FILEPATH, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Failed to open file '%s'\n", FILEPATH);
        exit(-1);
        return NULL;
    }
    else
    {
        long fSize;
        char c;
        char* buff;

        // Check file size
        fseek(fp, 0, SEEK_END);
        fSize = ftell(fp);
        rewind(fp);

        // Allocate memory for file contents
        buff = malloc(fSize);

        // Get file contents
        long i;
        for(i = 0; i < fSize; ++i)
        {
            c = fgetc(fp);
            if(c == EOF){ break; }
            *(buff + i) = c;
        }

        // Close the file
        fclose(fp);
        fp = NULL;

        return buff;
    }
}
