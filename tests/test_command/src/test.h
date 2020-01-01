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

#ifndef TEST_H__
#define TEST_H__

#include <stdint.h>
#include <stdbool.h>

bool test_init(void);
bool test_get_command(void);
bool test_get_version_command(void);
bool test_get_id_command(void);
bool test_read_dev_pub_key(void);
bool test_hashcode_command(void);
bool test_read_signa(void);
bool test_generate(void);
bool test_read_sm_pub_key(void);
bool test_sm_sign_command(void);

#endif  // TEST_H__