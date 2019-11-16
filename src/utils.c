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

#include <stdint.h>
#include <stdbool.h>
#include "serial.h"

void ksendc(uint8_t c)
{
    kputc(c);
}

uint8_t kreceivec(void)
{
    return (uint8_t) kgetc();
}

void ksendw(const uint8_t *buffer, uint8_t size)
{
    for (int i = 0; i < size; i++) {
        ksendc(buffer[i]);
    }
}

void kreceivew(uint8_t *buffer, uint8_t size)
{
    for (int i = 0; i < size; i++) {
        buffer[i] = (uint8_t) kreceivec();
    }
}
