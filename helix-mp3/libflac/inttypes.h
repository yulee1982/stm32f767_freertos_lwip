/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id: inttypes.h,v 1.1 2009/02/07 22:03:27 edwards Exp $
 *
 * Copyright (C) 2005 by Dave Chapman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef __INTTYPES_H__
#define __INTTYPES_H__

#define WPSEDITOR  //add by yulee at 2025-11-06

#ifndef WPSEDITOR

#include <limits.h>

/* 8 bit */

#ifndef INT8_MIN
#define INT8_MIN    SCHAR_MIN
#endif
#ifndef INT8_MAX
#define INT8_MAX    SCHAR_MAX
#endif
#ifndef UINT8_MAX
#define UINT8_MAX   UCHAR_MAX
#endif
//#define int8_t      signed char
//#define uint8_t     unsigned char

/* 16 bit */
#if USHRT_MAX == 0xffff
#ifndef INT16_MIN
#define INT16_MIN   SHRT_MIN
#endif
#ifndef INT16_MAX
#define INT16_MAX   SHRT_MAX
#endif
#ifndef UINT16_MAX
#define UINT16_MAX  USHRT_MAX
#endif
//#define int16_t     short
//#define uint16_t    unsigned short

#endif

/* 32 bit */
#if ULONG_MAX == 0xfffffffful
#ifndef INT32_MIN
#define INT32_MIN   LONG_MIN
#endif
#ifndef INT32_MAX
#define INT32_MAX   LONG_MAX
#endif
#ifndef UINT32_MAX
#define UINT32_MAX  ULONG_MAX
#endif
//#define int32_t     long
//#define uint32_t    unsigned long

#ifndef INTPTR_MIN
#define INTPTR_MIN  LONG_MIN
#endif
#ifndef INTPTR_MAX
#define INTPTR_MAX  LONG_MAX
#endif
#ifndef UINTPTR_MAX
#define UINTPTR_MAX ULONG_MAX
#endif
//#define intptr_t    long
//#define uintptr_t   unsigned long

#elif UINT_MAX == 0xffffffffu

#define INT32_MIN   INT_MIN
#define INT32_MAX   INT_MAX
#define UINT32_MAX  UINT_MAX
#define int32_t     int
#define uint32_t    unsigned int

#endif

/* 64 bit */
#ifndef LLONG_MIN
#define LLONG_MIN   ((long long)9223372036854775808ull)
#endif

#ifndef LLONG_MAX
#define LLONG_MAX   9223372036854775807ll
#endif

#ifndef ULLONG_MAX
#define ULLONG_MAX  18446744073709551615ull
#endif

#if ULONG_MAX == 0xffffffffffffffffull

#define INT64_MIN   LONG_MIN
#define INT64_MAX   LONG_MAX
#define UINT64_MAX  ULONG_MAX
#define int64_t     long
#define uint64_t    unsigned long

#define INTPTR_MIN  LONG_MIN
#define INTPTR_MAX  LONG_MAX
#define UINTPTR_MAX ULONG_MAX
#define intptr_t    long
#define uintptr_t   unsigned long

#else

#ifndef INT64_MIN
#define INT64_MIN   LLONG_MIN
#endif
#ifndef INT64_MAX
#define INT64_MAX   LLONG_MAX
#endif
#ifndef UINT64_MAX
#define UINT64_MAX  ULLONG_MAX
#endif
//#define int64_t     long long
//#define uint64_t    unsigned long long

#endif
#else
#include <stdint.h>
#endif /* !WPSEDITOR*/

#endif /* __INTTYPES_H__ */
