/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/06/2013
 */

#ifndef _COMMONREGS_H
#define _COMMONREGS_H
#include "swstatus.h"
#include "eeprom.h"
/**
 * Macros for the definition of common register indexes
 */
#define DEFINE_COMMON_REGINDEX_START()  \
enum CUSTOM_REGINDEX                    \
{                                       \
  REGI_PRODUCTCODE = 0,                 \
  REGI_HWVERSION,                       \
  REGI_FWVERSION,                       \
  REGI_SYSSTATE,                        \
  REGI_FREQCHANNEL,                     \
  REGI_SECUOPTION,                      \
  REGI_PASSWORD,                        \
  REGI_SECUNONCE,                       \
  REGI_NETWORKID,                       \
  REGI_DEVADDRESS,                      \
  REGI_TXINTERVAL,

#define DEFINE_COMMON_REGINDEX_END()    };

#define DEFINE_REGINDEX_START()   DEFINE_COMMON_REGINDEX_START()
#define DEFINE_REGINDEX_END()     DEFINE_COMMON_REGINDEX_END()

/**
 * Macro for the definition of registers common to all SWAP devices
 */
#define DEFINE_COMMON_REGISTERS()                                                                                            \
/* Product code */                                                                                                           \
static unsigned char dtProductCode[8] = {SWAP_MANUFACT_ID >> 24, SWAP_MANUFACT_ID >> 16 , SWAP_MANUFACT_ID >> 8, SWAP_MANUFACT_ID,    \
                       SWAP_PRODUCT_ID >> 24, SWAP_PRODUCT_ID >> 16 , SWAP_PRODUCT_ID >> 8, SWAP_PRODUCT_ID};                \
REGISTER regProductCode(dtProductCode, sizeof(dtProductCode), NULL, NULL);                                                   \
/* Hardware version */                                                                                                       \
static unsigned char dtHwVersion[4] = {HARDWARE_VERSION >> 24, HARDWARE_VERSION >> 16 , HARDWARE_VERSION >> 8, HARDWARE_VERSION};     \
REGISTER regHwVersion(dtHwVersion, sizeof(dtHwVersion), NULL, NULL);                                                         \
/* Firmware version */                                                                                                       \
static unsigned char dtFwVersion[4] = {FIRMWARE_VERSION >> 24, FIRMWARE_VERSION >> 16 , FIRMWARE_VERSION >> 8, FIRMWARE_VERSION};     \
REGISTER regFwVersion(dtFwVersion, sizeof(dtFwVersion), NULL, NULL);                                                         \
/* System state */                                                                                                           \
REGISTER regSysState(&panstamp.systemState, sizeof(panstamp.systemState), NULL, &setSysState);                               \
/* Frequency channel */                                                                                                      \
REGISTER regFreqChannel(&panstamp.radio.channel, sizeof(panstamp.radio.channel), NULL, &setFreqChannel, SWDTYPE_OTHER, EEPROM_FREQ_CHANNEL); \
/* Security option */                                                                                                        \
REGISTER regSecuOption(&panstamp.security, sizeof(panstamp.security), NULL, NULL);                                           \
/* Security password (not implemented yet) */                                                                                \
static unsigned char dtPassword[1];                                                                                                   \
REGISTER regPassword(dtPassword, sizeof(dtPassword), NULL, NULL);                                          \
/* Security nonce */                                                                                                         \
REGISTER regSecuNonce(&panstamp.nonce, sizeof(panstamp.nonce), NULL, NULL);                                                  \
/* Network Id */                                                                                                             \
REGISTER regNetworkId(panstamp.swapNetworkId, sizeof(panstamp.swapNetworkId), NULL, &setNetworkId);                        \
/* Device address */                                                                                                         \
REGISTER regDevAddress((unsigned char*)panstamp.swapAddress, sizeof(panstamp.swapAddress), NULL, &setDevAddress, SWDTYPE_INTEGER, EEPROM_DEVICE_ADDR); \
/* Periodic Tx interval */                                                                                                   \
REGISTER regTxInterval((unsigned char*)panstamp.txInterval, sizeof(panstamp.txInterval), NULL, &setTxInterval, SWDTYPE_INTEGER);

/**
 * Macros for the declaration of global table of registers
 */
#define DECLARE_REGISTERS_START()    \
REGISTER *regTable[] = {             \
        &regProductCode,             \
        &regHwVersion,               \
        &regFwVersion,               \
        &regSysState,                \
        &regFreqChannel,             \
        &regSecuOption,              \
        &regPassword,                \
        &regSecuNonce,               \
        &regNetworkId,               \
        &regDevAddress,              \
        &regTxInterval,

#define DECLARE_REGISTERS_END()      \
};                                   \
/* Size of regTable */               \
unsigned char regTableSize = sizeof(regTable)/sizeof(*regTable);

/**
 * Macro for the declaration of getter/setter functions related to all common registers
 */
#define DECLARE_COMMON_CALLBACKS()                          \
const void setSysState(unsigned char id, unsigned char *state);               \
const void setFreqChannel(unsigned char id, unsigned char *channel);          \
const void setDevAddress(unsigned char id, unsigned char *addr);              \
const void setNetworkId(unsigned char rId, unsigned char *nId);               \
const void setTxInterval(unsigned char id, unsigned char *interval);

/**
 * Macro for the definition of getter/setter functions related to all common registers
 */
#define DEFINE_COMMON_CALLBACKS()                           \
/**                                                         \
 * setSysState                                              \
 *                                                          \
 * Set system state                                         \
 *                                                          \
 * 'id'     Register ID                                     \
 * 'state'  New system state                                \
 */                                                         \
const void setSysState(unsigned char id, unsigned char *state)                \
{                                                           \
  switch(state[0])                                          \
  {                                                         \
    case SYSTATE_RESTART:                                   \
      /* Send status message before restarting the mote */  \
      panstamp.reset();                                     \
      break;                                                \
    default:                                                \
      panstamp.systemState = state[0];                      \
      break;                                                \
  }                                                         \
}                                                           \
                                                            \
/**                                                         \
 * setFreqChannel                                           \
 *                                                          \
 * Set frequency channel                                    \
 *                                                          \
 * @param id Register ID                                    \
 * @param channel New channel                               \
 */                                                         \
const void setFreqChannel(unsigned char id, unsigned char *channel)           \
{                                                           \
  if (channel[0] != regFreqChannel.value[0])                \
  {                                                         \
    /* Send status message before entering the new          \
    frequency channel */                                    \
    SWSTATUS packet = SWSTATUS(regFreqChannel.id, channel, regFreqChannel.length); \
    packet.send();                                          \
    /* Update register value */                             \
    panstamp.radio.setChannel(channel[0]);                  \
    /* Restart device */                                    \
    panstamp.reset();                                       \
  }                                                         \
}                                                           \
                                                            \
                                                            \
/**                                                         \
 * setDevAddress                                            \
 *                                                          \
 * Set device address                                       \
 *                                                          \
 * @param id Register ID                                    \
 * @param addr New device address                           \
 */                                                         \
const void setDevAddress(unsigned char id, unsigned char *addr)  \
{                                                           \
  /* Send status before setting the new address */          \
  SWSTATUS packet = SWSTATUS(regDevAddress.id, addr, regDevAddress.length); \
  packet.send();                                            \
  /* Update register value */                               \ 
  /* Set new SWAP address. BE to LE conversion */           \
  regDevAddress.setValueFromBeBuffer(addr);                 \
  /* Set new radio (1 byte) address */                      \
  panstamp.radio.setDevAddress(addr[0]);                    \
}                                                           \
                                                            \
/**                                                         \
 * setNetworkId                                             \
 *                                                          \
 * Set network id                                           \
 *                                                          \
 * @param rId Register ID                                   \
 * @paramn Id New network id                                \
 */                                                         \
const void setNetworkId(unsigned char rId, unsigned char *nId) \
{                                                           \
  if ((nId[0] != regNetworkId.value[0]) ||                  \
      (nId[1] != regNetworkId.value[1]))                    \
  {                                                         \
    /* Send status before taking the new network ID */      \
    SWSTATUS packet = SWSTATUS(regNetworkId.id, nId, regNetworkId.length); \
    packet.send();                                          \
    /* Update register value */                             \
    panstamp.radio.setSyncWord(nId);                        \
  }                                                         \
}                                                           \
/**                                                         \
 * setTxInterval                                            \
 *                                                          \
 * Set periodic Tx interval                                 \
 *                                                          \
 * @param id   Register ID                                  \
 * @param interval New interval (in seconds)                \
 */                                                         \
const void setTxInterval(unsigned char id, unsigned char *interval) \
{                                                           \
  /* Set new Tx interval. BE to LE conversion */            \
  regTxInterval.setValueFromBeBuffer(interval);             \
}
#endif
