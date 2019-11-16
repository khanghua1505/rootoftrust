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

#ifndef KEYS_H__
#define KEYS_H__

#include <stdint.h>

#define KEYCORE_DEV_PUB_KEY_SIZE        32
#define KEYCORE_SM_HASHCODE_SIZE        64
#define KEYCORE_SM_PUB_KEY_SIZE         32

#ifndef __packed
#define __packed        __attribute__((packed))
#endif  // __packed

typedef __packed struct
{
    uint8_t value[32];
}
keycore_keys_pk_t;

typedef __packed struct
{
    uint8_t value[64];
}
keycore_keys_sk_t;

typedef __packed struct
{
    uint8_t value[64];
}
keycore_keys_hashcode_t;

typedef __packed struct
{
    uint8_t value[64];
}
keycore_keys_signature_t;

#define KEYCORE_SEG_START       0x00
#define KEYCORE_SEG_NEXT        0x01
#define KEYCORE_SEG_LAST        0x03

extern const keycore_keys_pk_t keycore_dev_pub_key;
extern const keycore_keys_sk_t keycore_dev_prv_key;
extern keycore_keys_pk_t keycore_sec_pub_key;
extern keycore_keys_sk_t keycore_sec_prv_key;
extern keycore_keys_hashcode_t keycore_sec_hashcode;
extern keycore_keys_signature_t keycore_sec_signature;

#endif  // KEYS_H__