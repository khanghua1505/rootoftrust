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
#include <string.h>

#include "sha3.h"
#include "ed25519.h"
#include "keycore.h"
#include "keys.h"

#define KEYCORE_STATUS_INIT             (0x0000)
#define KEYCORE_STATUS_SEC_KEYS_FLAG    (0x01 << 1)
#define KEYCORE_STATUS_HASHCODE_FLAG    (0x01 << 2)
#define KEYCORE_STATUS_SIGNATURE_FLAG   (0x01 << 3)

static keycore_status_t m_keycore_status = KEYCORE_STATUS_INIT;
static sha3_ctx_t       SHA3_ctx;

bool keycore_keys_hashcode_isvalid(void)
{
  return (m_keycore_status & KEYCORE_STATUS_HASHCODE_FLAG);
}

bool keycore_keys_sec_key_isvalid(void)
{
  return (m_keycore_status & KEYCORE_STATUS_SEC_KEYS_FLAG);
}

void keycore_keys_payload_init(void)
{
  sha3_init(&SHA3_ctx, 64);
}

void keycore_keys_payload_append(const uint8_t *buffer, 
             uint8_t size)
{
  sha3_update(&SHA3_ctx, buffer, size);
}

void keycore_keys_payload_final(void)
{
  uint8_t scratchpad[128];
  
  sha3_final(&keycore_sec_hashcode, &SHA3_ctx);
  m_keycore_status |= KEYCORE_STATUS_HASHCODE_FLAG;
  
  sha3_init(&SHA3_ctx, 64);
  sha3_update(&SHA3_ctx, &keycore_dev_prv_key, sizeof(keycore_dev_prv_key));
  sha3_update(&SHA3_ctx, &keycore_sec_hashcode, sizeof(keycore_sec_hashcode));
  sha3_final(scratchpad, &SHA3_ctx);
  
  ed25519_create_keypair(keycore_sec_pub_key.value, 
                         keycore_sec_prv_key.value, scratchpad);
  m_keycore_status |= KEYCORE_STATUS_SEC_KEYS_FLAG;
  
  memcpy(scratchpad, &keycore_sec_hashcode, 64);
  memcpy(scratchpad + 64, &keycore_sec_pub_key, 32);
  
  ed25519_sign(keycore_sec_signature.value, scratchpad, 96, 
               keycore_dev_pub_key.value, keycore_dev_prv_key.value);
  m_keycore_status |= KEYCORE_STATUS_SIGNATURE_FLAG;
  
  memset(scratchpad, 0, 128);
}

void keycore_signature_msg_init(void)
{
  sha3_init(&SHA3_ctx, 64);
}

void keycore_signature_msg_append(const uint8_t *buffer, 
             uint8_t size)
{
  sha3_update(&SHA3_ctx, buffer, size);
}

void keycore_signature_msg_final(keycore_keys_signature_t *signature)
{
  uint8_t scratchpad[128];
  keycore_keys_hashcode_t tmp;
  
  sha3_final(tmp.value, &SHA3_ctx);
  memcpy(scratchpad, tmp.value, 64);
  memcpy(scratchpad + 64, keycore_sec_pub_key.value, 32);
  
  ed25519_sign(signature->value, scratchpad, 96, 
               keycore_dev_pub_key.value, keycore_dev_prv_key.value);
}







