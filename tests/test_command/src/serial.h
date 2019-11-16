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

#ifndef SERIAL_H__
#define SERIAL_H__

#include <stdint.h>
#include <stdbool.h>

void serial_open(const char* portname);
void serial_close(void);
int kgetc(void);
void kputc(uint8_t c);
void ksendc(uint8_t c);
void ksendw(const uint8_t *buffer, uint8_t size);
uint8_t kreceivec(void);
void kreceivew(uint8_t *buffer, uint8_t size);

#endif  // SERIAL_H__