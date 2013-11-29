#ifndef __UTIL_H__
#define __UTIL_H__

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define UINT8_MAX 0xff
#define UINT16_MAX 0xffff
#define UINT32_MAX 0xffffffff


void LogMessage(const char *, ...);
void FatalError(const char* format, ...);

#endif
