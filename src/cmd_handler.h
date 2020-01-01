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

#ifndef CMD_HANDLER_H__
#define CMD_HANDLER_H__

#define KEYCORE_CMD_GET                 (0x00)
#define KEYCORE_CMD_GET_VERSION         (0x01)
#define KEYCORE_CMD_READ_PRO_STATUS     (0x02)
#define KEYCORE_CMD_GET_ID              (0x03)
#define KEYCORE_CMD_READ_DEVICE_PK      (0x11)
#define KEYCORE_CMD_READ_SEC_HASHCODE   (0x12)
#define KEYCORE_CMD_READ_SEC_PK         (0x13)
#define KEYCORE_CMD_READ_SEC_SIGNATURE  (0x14)
#define KEYCORE_CMD_GENERATE            (0x21)
#define KEYCORE_CMD_SEC_SIGN            (0x31)
#define KEYCORE_CMD_DEV_SIGN            (0x32)

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
  
void keycore_cmd_handler(void);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CMD_HANDLER_H__
