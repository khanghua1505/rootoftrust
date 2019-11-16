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

#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "libserialport.h"

#define INTERNAL_SERIAL_BAUDRATE        115200
#define INTERNAL_SERIAL_WORDLENGTH      8
#define INTERNAL_SERIAL_PARITY          SP_PARITY_NONE
#define INTERNAL_SERIAL_STOPBITS        1

static struct sp_port *_port;

void serial_open(const char* portname)
{
    enum sp_return status;

    status = sp_get_port_by_name(portname, &_port);
    if (status != SP_OK) {
        printf("Error: Open %s is failed\n", portname);
        exit(status);
    }

    status = sp_open(_port, SP_MODE_READ_WRITE);
    if (status != SP_OK) {
        perror("Error: Open port failed");
        exit(status);
    }
    else {
        printf("Info: Open port %s successfully\n", portname);
    }

    status = sp_set_baudrate(_port, INTERNAL_SERIAL_BAUDRATE);
    if (status != SP_OK) {
        perror("Error: Set baudrate failed");
        exit(status);
    }

    status = sp_set_bits(_port, INTERNAL_SERIAL_WORDLENGTH);
    if (status != SP_OK) {
        perror("Error: Set word length failed");
        exit(status);
    }

    status = sp_set_parity(_port, INTERNAL_SERIAL_PARITY);
    if (status != SP_OK) {
        perror("Error: Set parity failed");
        exit(status);
    }

    status = sp_set_stopbits(_port, INTERNAL_SERIAL_STOPBITS);
    if (status != SP_OK) {
        perror("Error: Set stopbits failed");
        exit(status);
    }

    status = sp_set_flowcontrol(_port, SP_FLOWCONTROL_NONE );
    if (status != SP_OK) {
        perror("Error: Set flow control failed");
        exit(status);
    }
}

void serial_close(void)
{
    enum sp_return status;

    status = sp_close(_port);
    if (status != SP_OK) {
        perror("Error: Close port failed");
        exit(status);
    }
}

int kgetc(void)
{
    unsigned char c;

    sp_blocking_read(_port, &c, 1, 1000);

    return c;
}

void kputc(uint8_t c)
{
    sp_blocking_write(_port, &c, 1, 1000);
}

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