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
#include <string.h>
#include "serial.h"
#include "keys.h"
#include "plaintext.h"
#include "ed25519/ed25519.h"
#include "sha3/sha3.h"

#define ACK                 0x79
#define NACK                0x1F

#define COMMAND_GET                 0x00
#define COMMAND_GET_VERSION         0x01
#define COMMAND_READ_PROT_STATUS    0x02
#define COMMAND_GET_ID              0x03
#define COMMAND_READ_DEV_PUB_KEY    0x11
#define COMMAND_READ_SM_HASHCODE    0x12
#define COMMAND_READ_SM_PUB_KEY     0x13
#define COMMAND_READ_SM_SIGNATURE   0x14
#define COMMAND_GENERATE            0x21
#define COMMAND_SM_SIGN             0x31

static unsigned char m_buffer[256];

void test_init(void)
{
    uint8_t scratchpad[128];
    sha3_ctx_t SHA3_ctx;

    sha3_init(&SHA3_ctx, 64);
    sha3_update(&SHA3_ctx, plaintext1, strlen(plaintext1));
    sha3_final(&keycore_sec_hashcode, &SHA3_ctx);

    sha3_init(&SHA3_ctx, 64);
    sha3_update(&SHA3_ctx, &keycore_dev_prv_key, sizeof(keycore_dev_prv_key));
    sha3_update(&SHA3_ctx, &keycore_sec_hashcode, sizeof(keycore_sec_hashcode));
    sha3_final(scratchpad, &SHA3_ctx);

    ed25519_create_keypair(keycore_sec_pub_key.value,
                           keycore_sec_prv_key.value, scratchpad);
    memcpy(scratchpad, &keycore_sec_hashcode, 64);
    memcpy(scratchpad + 64, &keycore_sec_pub_key, 32);

    ed25519_sign(keycore_sec_signature.value, scratchpad, 96,
                 keycore_dev_pub_key.value, keycore_dev_prv_key.value);

    memset(scratchpad, 0, 128);
}

bool test_get_command(void)
{
    ksendc(COMMAND_GET);
    ksendc(~COMMAND_GET);

    if (ACK != kreceivec()) {
        printf("Test Get Command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    printf("Test Get command: \n");
    printf(" - Version: %2x\n", m_buffer[0]);
    for (int i = 1; i < N + 1; i++) {
        switch (m_buffer[i]) {
            case COMMAND_GET:
                printf(" - Get command\n");
                break;
            case COMMAND_GET_VERSION:
                printf(" - Get Version command\n");
                break;
            case COMMAND_READ_PROT_STATUS:
                printf(" - Read Protection Status command\n");
                break;
            case COMMAND_GET_ID:
                printf(" - Get ID command\n");
                break;
            case COMMAND_READ_DEV_PUB_KEY:
                printf(" - Read Device Public Key command\n");
                break;
            case COMMAND_READ_SM_HASHCODE:
                printf(" - Read Hash Code command\n");
                break;
            case COMMAND_READ_SM_PUB_KEY:
                printf(" - Read Security Monitor Hash Code command\n");
                break;
            case COMMAND_GENERATE:
                printf(" - Generate command\n");
                break;
            case COMMAND_SM_SIGN:
                printf(" - Sign command\n");
                break;
            default:
                printf(" - Unknown command 0x%2x\n", m_buffer[i]);
        }
    }

    uint8_t c = kreceivec();
    if (c != ACK) {
        printf("Test get command failed.\n");
    }

    return true;
}

bool test_get_version_command(void)
{
    ksendc(COMMAND_GET_VERSION);
    ksendc(~COMMAND_GET_VERSION);

    if (ACK != kreceivec()) {
        printf("Test Get Version command failed\n");
        return false;
    }

    uint8_t version = kreceivec();

    if (ACK != kreceivec()) {
        printf("Test Get Version command failed\n");
        return false;
    }

    printf("Test Get Version command: ");
    printf(" - Version: %x\n", version);

    return true;
}

bool test_get_id_command(void)
{
    ksendc(COMMAND_GET_ID);
    ksendc(~COMMAND_GET_ID);

    if (ACK != kreceivec()) {
        printf("Test Get ID command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    if (ACK != kreceivec()) {
        printf("Test Get ID command failed\n");
        return false;
    }

    printf("Test Get ID command:\n");
    printf(" - PID: ");
    for (int i = 0; i < N+1; i++) {
        printf("%x", m_buffer[i]);
    }
    printf("\n");

    return true;
}

bool test_read_dev_pub_key(void)
{
    ksendc(COMMAND_READ_DEV_PUB_KEY);
    ksendc(~COMMAND_READ_DEV_PUB_KEY);

    if (ACK != kreceivec()) {
        printf("Test Get ID command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    if (ACK != kreceivec()) {
        printf("Test Get ID command failed\n");
        return false;
    }

    printf("Test Read Device Public Key command:\n");
    printf(" - Value read: ");
    for (int i = 0; i < N+1; i++) {
        printf("0x%2x ", m_buffer[i]);
    }
    printf("\n");
    printf(" - Compare to Device Public Key... ");
    for (int i = 0; i < KEYCORE_DEV_PUB_KEY_SIZE; i++) {
        if (m_buffer[i] != keycore_dev_pub_key.value[i]) {
            printf("failed\n");
            return false;
        }
    }
    printf("OK\n");

    return true;
}

bool test_hashcode_command(void)
{
    ksendc(COMMAND_READ_SM_HASHCODE);
    ksendc(~COMMAND_READ_SM_HASHCODE);

    if (ACK != kreceivec()) {
        printf("Test Read Hash Code command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    if (ACK != kreceivec()) {
        printf("Test Read Hash Code command failed\n");
        return false;
    }

    printf("Test Read Hash Code command result:\n");
    printf(" - Value read: ");
    for (int i = 0; i < N+1; i++) {
        printf("0x%2x ", m_buffer[i]);
    }
    printf("\n");
    printf(" - Compare to SM Hash Code... ");
    for (int i = 0; i < KEYCORE_SM_HASHCODE_SIZE; i++) {
        if (m_buffer[i] != keycore_sec_hashcode.value[i]) {
            printf("failed\n");
            return false;
        }
    }
    printf("OK\n");

    return true;
}

bool test_read_signa(void)
{
    ksendc(COMMAND_READ_SM_SIGNATURE);
    ksendc(~COMMAND_READ_SM_SIGNATURE);

    if (ACK != kreceivec()) {
        printf("Test Read SM Signature command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    printf("Test Read SM Signature command result:\n");
    printf(" - Value read: ");
    for (int i = 0; i < N+1; i++) {
        printf("0x%2x ", m_buffer[i]);
    }
    printf("\n");
    printf(" - Compare to SM Signature... ");
    for (int i = 0; i < KEYCORE_SM_SIGNATURE_SIZE; i++) {
        if (m_buffer[i] != keycore_sec_signature.value[i]) {
            printf("failed\n");
            return false;
        }
    }
    printf("OK\n");

    return true;
}

bool test_generate(void)
{
    ksendc(COMMAND_GENERATE);
    ksendc(~COMMAND_GENERATE);

    if (ACK != kreceivec()) {
        printf("Test Generate command failed\n");
        return false;
    }

    uint32_t plaintext_len = strlen(plaintext1);
    uint8_t plaintext_len_bytes[] =
    {
        (plaintext_len >> 24) & 0xFF,
        (plaintext_len >> 16) & 0xFF,
        (plaintext_len >>  8) & 0xFF,
        (plaintext_len >>  0) & 0xFF
    };
    uint8_t checksum = plaintext_len_bytes[0] ^
                       plaintext_len_bytes[1] ^
                       plaintext_len_bytes[2] ^
                       plaintext_len_bytes[3];
    ksendw(plaintext_len_bytes, 4);
    ksendc(checksum);

    if (ACK != kreceivec()) {
        printf("Test Generate command failed\n");
        return false;
    }

    for (int i = 0; i < plaintext_len; i += 25 /* Seg len */) {
        checksum = 0x00;
        if (i + 25 < plaintext_len) {
            ksendc(25 + 1);
            checksum ^= (25 + 1);
            ksendc(KEYCORE_SEG_NEXT);
            checksum ^= (KEYCORE_SEG_NEXT);
            for (int k = 0; k < 25; k++) {
                ksendc(plaintext1[k + i]);
                checksum ^= plaintext1[k + i];
            }
            ksendc(checksum);
        }
        else {
            ksendc(plaintext_len - i + 1);
            checksum ^= (plaintext_len - i + 1);
            ksendc(KEYCORE_SEG_LAST);
            checksum ^= (KEYCORE_SEG_LAST);
            for (int k = 0; k < plaintext_len - i; k++) {
                ksendc(plaintext1[k + i]);
                checksum ^= plaintext1[k + i];
            }
            ksendc(checksum);
        }

        if (ACK != kreceivec()) {
            printf("Test Generate command failed\n");
            return false;
        }
    }

    if (ACK != kreceivec()) {
        printf("Test Generate command failed\n");
        return false;
    }

    printf("Test Generate command... OK\n");

    return true;
}

bool test_read_sm_pub_key(void)
{
    ksendc(COMMAND_READ_SM_PUB_KEY);
    ksendc(~COMMAND_READ_SM_PUB_KEY);

    if (ACK != kreceivec()) {
        printf("Test Read SM Public Key command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    if (ACK != kreceivec()) {
        printf("Test Read SM Public Key command failed\n");
        return false;
    }

    printf("Test Read SM Public Key command:\n");
    printf(" - Value read: ");
    for (int i = 0; i < N+1; i++) {
        printf("0x%2x ", m_buffer[i]);
    }
    printf("\n");
    printf(" - Compare to SM Public Key... ");
    for (int i = 0; i < KEYCORE_SM_PUB_KEY_SIZE; i++) {
        if (m_buffer[i] != keycore_sec_pub_key.value[i]) {
            printf("failed\n");
            return false;
        }
    }
    printf("OK\n");

    return true;
}

bool test_sm_sign_command(void)
{
    ksendc(COMMAND_SM_SIGN);
    ksendc(~COMMAND_SM_SIGN);

    if (ACK != kreceivec()) {
        printf("Test Sign command failed\n");
        return false;
    }

    uint32_t plaintext_len = strlen(plaintext2);
    uint8_t plaintext_len_bytes[] =
    {
        (plaintext_len >> 24) & 0xFF,
        (plaintext_len >> 16) & 0xFF,
        (plaintext_len >>  8) & 0xFF,
        (plaintext_len >>  0) & 0xFF
    };
    uint8_t checksum = plaintext_len_bytes[0] ^
                       plaintext_len_bytes[1] ^
                       plaintext_len_bytes[2] ^
                       plaintext_len_bytes[3];
    ksendw(plaintext_len_bytes, 4);
    ksendc(checksum);

    if (ACK != kreceivec()) {
        printf("Test Sign command failed\n");
        return false;
    }

    for (int i = 0; i < plaintext_len; i += 25 /* Seg len */) {
        checksum = 0x00;
        if (i + 25 < plaintext_len) {
            ksendc(25 + 1);
            checksum ^= (25 + 1);
            ksendc(KEYCORE_SEG_NEXT);
            checksum ^= (KEYCORE_SEG_NEXT);
            for (int k = 0; k < 25; k++) {
                ksendc(plaintext2[k + i]);
                checksum ^= plaintext2[k + i];
            }
            ksendc(checksum);
        }
        else {
            ksendc(plaintext_len - i + 1);
            checksum ^= (plaintext_len - i + 1);
            ksendc(KEYCORE_SEG_LAST);
            checksum ^= (KEYCORE_SEG_LAST);
            for (int k = 0; k < plaintext_len - i; k++) {
                ksendc(plaintext2[k + i]);
                checksum ^= plaintext2[k + i];
            }
            ksendc(checksum);
        }

        if (ACK != kreceivec()) {
            printf("Test Sign command failed\n");
            return false;
        }
    }

    if (ACK != kreceivec()) {
        printf("Test Sign command failed\n");
        return false;
    }

    uint8_t N = kreceivec();
    kreceivew(m_buffer, N + 1);

    if (ACK != kreceivec()) {
        printf("Test Sign command failed\n");
        return false;
    }

    printf("Test Sign command result:\n");
    printf(" - Value read: ");
    for (int i = 0; i < N+1; i++) {
        printf("0x%2x ", m_buffer[i]);
    }
    printf("\n");
    printf(" - Verify message... ");

    sha3_ctx_t SHA3_ctx;
    keycore_keys_hashcode_t tmp;
    uint8_t scratchpad[128];

    sha3_init(&SHA3_ctx, 64);
    sha3_update(&SHA3_ctx, plaintext2, plaintext_len);
    sha3_final(tmp.value, &SHA3_ctx);

    memcpy(scratchpad, tmp.value, 64);
    memcpy(scratchpad + 64, keycore_sec_pub_key.value, 32);

    if (ed25519_verify(m_buffer, scratchpad, 96,
            keycore_dev_pub_key.value)) {
        printf("OK\n");
    }
    else {
        printf("Failed\n");
        return false;
    }

    return true;
}





