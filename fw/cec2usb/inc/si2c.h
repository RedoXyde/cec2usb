#pragma once
#include "common.h"

#define SI2C_Dir    DDRC
#define SI2C_Port   PORTC
#define SI2C_Pin    PINC

#define SI2C_SCL    PC7
#define SI2C_SDA    PC6

// Ack Generation Bit Values
#define SI2C_SAck  1  // Read - Return Ack to Slave
#define SI2C_SNAck 0  // Read and NAck - Return NAck, 
                      // Tell slave to stop sending. 
                      // Used for last byte read

void SI2C_Init(void);
void SI2C_End(void);

uint8_t SI2C_Start(void);
void SI2C_Restart(void);
void SI2C_Stop(void);

uint8_t SI2C_Write(uint8_t c);
uint8_t SI2C_Read(uint8_t *c, uint8_t e);