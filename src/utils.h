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

#ifndef UTILS_H__
#define UTILS_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C {
#endif  // __cplusplus

#define GET32(a, b, c, d)  ( ((a) << 24) | ((b) << 16) | ((c) << 8) | ((d)) )

/**
  * @brief This function sends a character to STDOUT.
  *
  * @param c[in]    An input character.
  *
  * @return None.
  */
void ksendc(uint8_t c);

/**
  * @brief This function receives a character from STDIN.
  *
  * @param None.
  *
  * @return The received character.
  */
uint8_t kreceivec(void);

/**
  * @brief This function sends a string (or byte array) to STDOUT.
  *
  * @param buffer[in]    An input string or (byte array).
  * @param buffer[in]    The size of buffer.
  *
  * @return None.
  */
void ksendw(const uint8_t *buffer, uint8_t size);

/**
  * @brief This function sends a string (or byte array) to STDOUT.
  *
  * @param buffer[in,out]    The buffer to store received datas.
  * @param buffer[in]        Number of bytes to shall be read.
  *
  * @return None.
  */
void kreceivew(uint8_t *buffer, uint8_t size);

#ifdef __cplusplsu
}
#endif  // __cplusplus

#endif  // UTILS_H__