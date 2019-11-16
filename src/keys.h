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

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
  
extern const keycore_keys_pk_t keycore_dev_pub_key;

extern const keycore_keys_sk_t keycore_dev_prv_key;

extern keycore_keys_pk_t keycore_sec_pub_key;

extern keycore_keys_sk_t keycore_sec_prv_key;

extern keycore_keys_hashcode_t keycore_sec_hashcode;

extern keycore_keys_signature_t keycore_sec_signature;
  
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // KEYS_H__