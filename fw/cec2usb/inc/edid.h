#pragma once
#include "common.h"
#include "si2c.h"

#define EDID_Init SI2C_Init

#define EDID_ADDR_INVALID 0x0000

uint8_t   EDID_Read(const uint8_t a, const uint8_t l, uint8_t *b);
uint16_t  EDID_ReadPhysicalAddress(void);