/*
 * linux_types.h
 * Copyright (C) 2014 mac <mac@Dawnworld>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef __LINUX_TYPES_H__
#define __LINUX_TYPES_H__

#pragma pack(push, 1)

#ifdef __linux__
# include "linux/types.h"
#else
typedef unsigned long       __u64;
typedef unsigned int        __u32;
typedef unsigned short      __u16;
typedef unsigned char       __u8;

typedef signed   int        __s32;
#endif

#pragma pack(pop)
#endif /* !__LINUX_TYPES_H__ */

