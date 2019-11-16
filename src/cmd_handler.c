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
#include "utils.h"
#include "package.h"
#include "keycore.h"
#include "cmd_handler.h"

/*
 * The Get command.
 *
 * The Get command allows the user to get the version of the Cerberus Core
 * and the supported commands.
 */

static const get_return_pkt_t get_return_pkt = 
{
  .N = 9,                                 /* Number of bytes to be followed - 1. */
  .version = CERBERUS_KEYCORE_VERSION,    /* Keycore version. */
  .supported_commands = 
  {
    0x00,       /* Get command. */
    0x01,       /* Get Version command. */
    0x02,       /* Read Protection Status command. */
    0x03,       /* Get ID command. */
    0x11,       /* Read Device Public Key command. */
    0x12,       /* Read Hash Code command. */
    0x13,       /* Read Security Monitor Public Key command. */
    0x21,       /* Generate command. */
    0x31,       /* Sign command. */
  },
};

static void handle_get(void) 
{
  ksendc(ACK);                            
  ksendw((uint8_t *) &get_return_pkt,     
          sizeof(get_return_pkt));       
  ksendc(ACK);                            
}

/*
 * Get version command.
 *
 * The Get version command is used to get the Keycore version.
 */

static const get_version_return_pkt_t get_version_return_pkt = 
{
  .version = CERBERUS_KEYCORE_VERSION,
};

static void handle_get_version(void)
{
  ksendc(ACK);                               
  ksendw((uint8_t *) &get_version_return_pkt, 
          sizeof(get_version_return_pkt));    
  ksendc(ACK);                               
}

/*
 * Read Protection Status command.
 *
 * The Read Protection Status command is reserved for the future.
 */

static void handle_read_protection_status(void)
{

}

/*
 * The Get ID command.
 *
 * The Get ID command is used to get the version of the chip ID (identification).
 */

static const get_id_return_pkt_t get_id_return_pkt = 
{
  .N = 1,                         /* Number of bytes to be followed - 1. */
  .PID = CERBERUS_KEYCORE_PID,    /* Chip ID (identification). */
};

static void handle_get_id(void) 
{
  ksendc(ACK);                            
  ksendw((uint8_t *) &get_id_return_pkt,  
          sizeof(get_id_return_pkt));    
  ksendc(ACK);                           
}

/*
 * Read Device Public Key.
 *
 * The Read Device Public Key command is used to read the device public key 
 * from the Keycore.
 */

extern const keycore_keys_pk_t keycore_dev_pub_key;

static void handle_read_device_pk(void)
{
  ksendc(ACK);                               
  ksendc(sizeof(keycore_keys_pk_t) - 1);     
  ksendw((uint8_t *) &keycore_dev_pub_key, 
          sizeof(keycore_dev_pub_key));
  ksendc(ACK);                                
}

/*
 * Read Hash Code command.
 *
 * The Read Hash Code command is used to read the application hash code (see Secure Boot
 * documentation for more detail).
 *
 * note: Read Hash Code command is valid, only if the Generate command was executed.
 */

extern keycore_keys_hashcode_t keycore_sec_hashcode;

static void handle_read_hashcode(void)
{
  if (!keycore_keys_hashcode_isvalid()) { /* The Hashcode is not valid */
    /* Sends a NACK byte and end of command. */
    ksendc(NACK);        
    return;
  } 
  else {
    /* Else, send an ACK byte.
     * The command shall be executed. */
    ksendc(ACK);
  } 

  ksendc(sizeof(keycore_sec_hashcode) - 1);  /* Send the length of hash code bytes. */
  ksendw((uint8_t *) &keycore_sec_hashcode,  /* Send Hash code */ 
          sizeof(keycore_sec_hashcode));
  
  /* Send an ACK bytes 
   * and end of Read Hash Code command. */
  ksendc(ACK);                                    
}

/*
 * Read Security Monitor Public Key.
 *
 * The Read Security Monitor Public Key command is used for reading the Security 
 * Monitor Public Key (see Secure Boot documentation for more detail).
 *
 * note: Read Security Monitor Public Key (or Read SM Public Key) command is valid, only if 
 *       the Generate command was executed.
 */

extern const keycore_keys_pk_t keycore_sec_pub_key;

static void handle_read_sec_pk(void)
{
    if (!keycore_keys_sec_key_isvalid()) { 
      /* The Security Monitor Public Key is not valid. */
      /* Send a NACK byte and end of Read Security Monitor Public Key. */
      ksendc(NACK);
      return;
    } 
    else {
      /* Else, send an ACK byte. 
       * The command shall be executed.*/
      ksendc(ACK);
    }

    ksendc(sizeof(keycore_sec_pub_key) - 1);   /* Send the length of security monitor PK. */
    ksendw((uint8_t *) &keycore_sec_pub_key,   /* Send Security Monitor PK. */
            sizeof(keycore_sec_pub_key));
    
    /* Send an ACK byte and 
     * end of Read SM Public Key. */
    ksendc(ACK);                                    
}

/*
 * Generate command.
 *
 * The Generate command is used to generate the Hash Code (see in Read Hash Code command),
 * SM Public Key (see in Read SM Public Key command),...
 *
 * note: In Generate proccess, the command requires a application payload.
 */

/* This buffer is used for Generate command and Sign command. */
static uint8_t buffer[256];     

static void handle_generate(void)
{
  /* Send an ACK byte and 
   * start of generate process. */
  ksendc(ACK);
  
  /* Receives the length of payload bytes (4 bytes)
   * and a checksum (1 byte). */
  kreceivew(buffer, 4 + 1 /* 4 bytes length and 1 byte checksum */);

  /* Checksum received data. */
  uint8_t selfcheck = 0x00;
  for (int i = 0; i < 4; i++) 
    selfcheck ^= buffer[i];
  
  if (selfcheck != buffer[4]) {
    /* Send an NACK byte 
     * and end of Generate command.*/
    ksendc(NACK);
    return;
  }
  else {
    /* Else, Send a ACK byte.
     * The Application Payload shall be received. */
    ksendc(ACK);
  }
    
  uint32_t payload_len = GET32(buffer[0], buffer[1],  \
              buffer[2], buffer[3]);

  /* Number of data received bytes 
   * (not included: segment N, type, checksum). */
  uint32_t received_bytes_counter = 0;  
  
  keycore_keys_payload_init();
  
  while (received_bytes_counter < payload_len) {
    uint8_t segment_len = kreceivec();    /* Receives the length of segment. */
    kreceivew(buffer, segment_len + 1);   /* Receives segment bytes. */
    
    /* Checksum received data. */
    selfcheck = segment_len;
    for (int i = 0; i < segment_len; i++) {
      selfcheck ^= buffer[i];
    }
    
    if (selfcheck != buffer[segment_len]) { /* Checksum is not correct. */
      /* Send an NACK byte 
       * and end of Generate command. */
      ksendc(NACK);
      return;
    }
       
    keycore_data_segment_t *seg = (keycore_data_segment_t *) buffer;
    keycore_keys_payload_append(seg->data, segment_len - 1);
    received_bytes_counter += segment_len - 1;

    if (seg->type == KEYCORE_SEG_START || 
      seg->type == KEYCORE_SEG_NEXT) {
      // Nothing.
    }
    else if (seg->type == KEYCORE_SEG_LAST) {
      ksendc(ACK);
      break;
    }
    else {
      ksendc(NACK);
      return;
    }
    
    ksendc(ACK);
  }
    
  if (received_bytes_counter != payload_len) { 
    /* The size of payload is not correct. */
    ksendc(NACK);
    return;
  }
  
  keycore_keys_payload_final();
  
  /* Send an ACK and
   * end of Generate command. */
  ksendc(ACK);
}

/*
 * The Sign command is used to create a signature for messgae (see Secure Boot 
 * documentation for more detail). The Security Monitor Public and Private Key 
 * are used for generating a signature in this command.
 */

static void handle_sign(void)
{
  if (!keycore_keys_sec_key_isvalid()) { 
    /* The Security Monitor Public and Private Key is not valid */
    ksendc(NACK);
    return;
  }
  else {
    /* Else, Send an ACK byte. */
    /* The command shall be received. */
    ksendc(ACK);
  }

  /* Waits for receiving the size of message (4 byte) and
   * checksum (1 byte).*/
  kreceivew(buffer, 4 + 1);

  uint8_t selfcheck = 0x00;
  for (int i = 0; i < 4; i++) {
    selfcheck ^= buffer[i];
  }

  if (selfcheck != buffer[4]) { /* Checksum is not correct.*/ 
    ksendc(NACK);
    return;
  }
  else {
    /* Else, send an ACK byte. 
     * The message shall be received. */
    ksendc(ACK);
  }

  uint32_t msg_len = GET32(buffer[0], buffer[1], buffer[2], buffer[3]);

  /* Receives payload */
  uint32_t received_bytes_counter = 0;

  keycore_signature_msg_init();

  while (received_bytes_counter < msg_len) {
    uint8_t segment_len = kreceivec();      /* Receives the length of segment. */
    kreceivew(buffer, segment_len + 1);     /* Receives segment bytes. */

    /* Checksum received data. */
    selfcheck = segment_len;
    for (int i = 0; i < segment_len; i++) {
        selfcheck ^= buffer[i];
    }

    if (selfcheck != buffer[segment_len]) { /* Checksum is not correct. */
      /* Send an NACK byte 
       * and end of Generate command. */
      ksendc(NACK);
      return;
    }

    keycore_data_segment_t *seg = (keycore_data_segment_t *) buffer;
    keycore_signature_msg_append(seg->data, segment_len - 1);
    received_bytes_counter += segment_len - 1;

    if (seg->type == KEYCORE_SEG_START || 
      seg->type == KEYCORE_SEG_NEXT) {
      // Nothing.
    }
    else if (seg->type == KEYCORE_SEG_LAST) {
      ksendc(ACK);
      break;
    }
    else {
      ksendc(NACK);
      return;
    }

    ksendc(ACK);	
  }

  if (received_bytes_counter != msg_len) { 
    /* The size of payload is not correct. */
    ksendc(NACK);
    return;
  }
  else {
    ksendc(ACK);
  }
  
  keycore_keys_signature_t signature;
  keycore_signature_msg_final(&signature);

  ksendc(sizeof(signature) - 1);
  ksendw((uint8_t *) &signature, sizeof(signature));

  /* Send an ACK and
   * end of Generate command. */
  ksendc(ACK);
}

/*******************************************************************************
 * API
 ******************************************************************************/

void keycore_cmd_handler(void)
{
    uint8_t code;
    uint8_t checksum;
    
    while (1)
    {
        // Waiting for receiving a command code (1 byte)
        // and checksum (1 byte).
        kreceivew(buffer, 2);   
        code = buffer[0];
        checksum = ~code;
        
        if (checksum != buffer[1]) {  // Checksum is not correct.
            ksendc(NACK);
            continue;
        }
        
        switch (code) {
        case KEYCORE_CMD_GET:
            handle_get();
            break;
        case KEYCORE_CMD_GET_VERSION:
            handle_get_version();
            break;
        case KEYCORE_CMD_READ_PRO_STATUS:
            handle_read_protection_status();
            break;
        case KEYCORE_CMD_GET_ID:
            handle_get_id();
            break;
        case KEYCORE_CMD_READ_DEVICE_PK:
            handle_read_device_pk();
            break;
        case KEYCORE_CMD_READ_HASHCODE:
            handle_read_hashcode();
            break;
        case KEYCORE_CMD_READ_SEC_PK:
            handle_read_sec_pk();
            break;
        case KEYCORE_CMD_GENERATE:
            handle_generate();
            break;
        case KEYCORE_CMD_SIGN:
            handle_sign();
            break;
        default:
            ksendc(NACK);
        }
    }
}















