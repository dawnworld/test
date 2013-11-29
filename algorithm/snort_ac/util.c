#include <stdio.h>
#include <stdarg.h>
#include "util.h"

#define STD_BUF 1024

void LogMessage(const char *format, ...)
{
    char buf[STD_BUF + 1];

    va_list ap;

    va_start(ap, format);

    vfprintf(stderr, format, ap);

    va_end(ap);

}

//int vsnprintf(char * str, int count, char* fmt, va_list args)
//{
//    str[0] = 0;
//
//    return strlen((const char *) str);
//}
void FatalError(const char* format, ...)
{
    char buf[STD_BUF + 1];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, STD_BUF, format, ap);
    va_end(ap);

}
