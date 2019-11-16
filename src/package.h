/* Copyright (c) 2019 - 2020, Khang Hua, email: khanghua1505@gmail.com
 * All right reserved.
 *
 * This file is written and modified by Khang Hua.
 *
 * This model is free software; you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License; either version 2.1 of the License, or (at your option)
 * any later version. See the GNU Lesser General Public License for more details,
 *
 * This model is distributed in the hope that it will be useful.
 */

#ifndef PACKAGE_H__
#define PACKAGE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
    
#define ACK     0x79
#define NACK    0x1F
  
typedef __packed struct   
{
    uint8_t N;                          /* Number of bytes to be followed - 1. */
    uint8_t version;                    /* Keycore version. */
    uint8_t supported_commands[9];      /* The supported command. */
}    
get_return_pkt_t;

typedef __packed struct
{
    uint8_t version;    /* Keycore version. */
}
get_version_return_pkt_t;

typedef __packed struct
{
    uint8_t N;          /* Number of bytes to be followed - 1. */
    uint8_t PID[2];     /* Chip ID (identification). */            
}
get_id_return_pkt_t;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PACKAGE_H__