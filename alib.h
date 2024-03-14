#ifndef ALIB_H
#define ALIB_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

// nob.h
#define DEFER(value) do { result = (value); goto defer; } while(0)
#define ALIB_ASSERT assert

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    
} LogLevel;

typedef struct {
    char *data;
    size_t count;
    size_t capacity;
} StringBuffer;

typedef struct {
    size_t count;
    const char *data;
} String_View;

void logMessage(LogLevel level, const char *format, ...);
bool readEntireFile(const char *path, StringBuffer *sb);

#endif //ALIB_H

#ifdef ALIB_IMPLEMENTATION
void logMessage(LogLevel level, const char *format, ...)
{

    time_t currentTime;
    time(&currentTime);
    char *timeString = ctime(&currentTime);
    timeString[strlen(timeString) - 1] = '\0';
    const char *levelString;
    switch(level)
    {
        case DEBUG:
            levelString = "DEBUG";
            break;
        case INFO:
            levelString = "INFO";
            break;
        case WARN:
            levelString = "WARN";
            break;
        case ERROR:
            levelString = "ERROR";
            break;
        default:
            break;
    }

    fprintf(stderr, "[%s] [%s] ", timeString, levelString);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr,"\n");
    va_end(args);
}

bool readEntireFile(const char *path, StringBuffer *sb)
{
   bool result = true; 
   // Open file
   FILE *fp = fopen(path, "rb");
   if (fp == NULL)
       DEFER(false);
   
   // File size
   if (fseek(fp, 0, SEEK_END) < 0)
       DEFER(false);
   long m = ftell(fp);
   if (m < 0)
       DEFER(false);
   if (fseek(fp, 0, SEEK_SET) < 0)
       DEFER(false);

   // make sure the capacity of the array can handle the current size plus the new
   size_t new_count = sb->count + m;
   if (new_count >= sb->capacity) {
        sb->data = (char *)realloc(sb->data, new_count);
        ALIB_ASSERT(sb->data != NULL);
        sb->capacity = new_count;
   }

   // Read everything to data
   fread(sb->data + sb->count, m, 1, fp);
   if (ferror(fp))
       DEFER(false);

   sb->count = new_count; 

defer:
   if (!result) logMessage(ERROR, "Couldn't read file %s: %s", path, strerror(errno));
   if(fp) fclose(fp);
   return result;
}

#endif
