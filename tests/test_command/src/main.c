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
#include "test.h"

static const char* portname = NULL;

void system_init(void)
{
    serial_open(portname);
    test_init();
}

void system_deinit(void)
{
    serial_close();
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Syntax: %s PORT\n", argv[0]);
        exit(1);
    }
    else {
        portname = argv[1];
    }

    system_init();
    test_get_command();
    test_get_version_command();
    test_get_id_command();
    test_read_dev_pub_key();
    test_generate();
    test_read_sm_pub_key();
    test_hashcode_command();
    test_sign_command();
    system_deinit();

    return 0;
}