/*
MIT License

Copyright (c) 2019 RedoX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "cec.h"
#include "common.h"

#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TEENSY 0

#if TEENSY
#define outHigh() PORTD &= ~_BV(CEC_OUT)
#define outLow()  PORTD |= _BV(CEC_OUT)
#else
#define outHigh() PORTD |= _BV(CEC_OUT)
#define outLow()  PORTD &= ~_BV(CEC_OUT)
#endif
#define cecAssertLow() outHigh()
#define cecRelease()   outLow()

#define inRead()  ((PIND & _BV(CEC_IN)) != 0)

#define EDGE_INT_MASK    _BV(CEC_IN)

// p84, for INT2
#define EDGE_INT_FALL  0x20
#define EDGE_INT_RISE  0x30
#define EDGE_INT_TOGG  0x01

#define edgeIntClear()    EIFR  |=  EDGE_INT_MASK
#define edgeIntEnable()   EIMSK |=  EDGE_INT_MASK
#define edgeIntDisable()  EIMSK &= ~EDGE_INT_MASK
// Disable Int, Change edge, clear flag, enable int
#define edgeIntAny()      do { edgeIntDisable(); EICRA &= ~0x30; EICRA |= 0x10; edgeIntClear(); edgeIntEnable(); } while(0)

//#define DECODE_USE_HARD_VALUES

//CLK=8MHz so everything is based on CLK/64=125kHz
#define MILLISECONDS(x)  ((uint16_t)((x)*125))

/* values extracted from the HDMI standard */
#define START_MIN  MILLISECONDS(3.5f)
#define START_AVG  MILLISECONDS(3.7f)
#define START_MAX  MILLISECONDS(3.9f)

#define START_LEN  MILLISECONDS(4.5f)

#define DATA_H_MIN  MILLISECONDS(0.4f)
#define DATA_H_AVG  MILLISECONDS(0.6f)
#define DATA_H_MAX  MILLISECONDS(0.85f)

#define DATA_L_MIN  MILLISECONDS(1.25f)
#define DATA_L_AVG  MILLISECONDS(1.5f)
#define DATA_L_MAX  MILLISECONDS(1.7f)

#define DATA_LEN  MILLISECONDS(2.4f)

#define ARBITATION  (DATA_LEN * 7)

/* not standard values, but usefull */
#define DATA_TRIGGER  ((DATA_H_AVG + DATA_L_AVG) / 2)
#define TIMEOUT    MILLISECONDS(5.0f)

#define tmrStart() do { TCCR1B |= 0b011; /* /64 */ } while(0)
#define tmrReset() do { TCNT1 = 0; } while(0)
#define tmrStop() do { TCCR1B &= ~0b111; TIMSK1 &= ~(_BV(OCIE1A)|_BV(OCIE1B)); TIFR1 |= (_BV(OCIE1A)|_BV(OCIE1B)); }while(0)
#define tmrGet() TCNT1

#define tmr0Get() OCR1A
#define tmr0(v)  do { OCR1A = v; TIFR1 = _BV(OCF1A); TIMSK1 |= _BV(OCIE1A);  } while(0)
#define tmr0Disable() do { TIMSK1 &= ~_BV(OCIE1A); } while(0)

volatile uint16_t _low=0, _high=0;

inline void cecBit(uint16_t low, uint16_t high)
{
  tmrReset();
  _low = low;
  _high = high;

  if(low)
  {
    tmr0(low);
    cecAssertLow();
  }
  else
    tmr0(high);
}

enum DecodeResult
{
  BIT_L     = 0,
  BIT_H     = 1,
  BIT_Start = 2,
  BIT_OOR   = 3
};

inline enum DecodeResult TimerDecode(void)
{
  uint16_t count = tmrGet();
#ifdef DECODE_USE_HARD_VALUES
  if(DATA_L_MIN <= count && count <= DATA_L_MAX)
    return BIT_L;
  else if(DATA_H_MIN <= count && count <= DATA_H_MAX)
    return BIT_H;
  else if(START_MIN <= count && count <= START_MAX)
    return BIT_Start;
  else
    return BIT_OOR;
#else
  if(count < DATA_TRIGGER)
    return BIT_H;
  else if(count < START_MIN)
    return BIT_L;
  else if(count <= START_MAX)
    return BIT_Start;
  else
    return BIT_OOR;
#endif
}

#define CEC_BroadcastAddr       0xF

#define CEC_MSG_BLOCKS    16 // Msg length
#define CEC_BLOCK_HEADER  0
#define CEC_BLOCK_OPCODE  1

#define CEC_BLOCK_BITS    8  // Data length
#define CEC_BIT_EOM       8
#define CEC_BIT_ACK       9

typedef struct
{
  volatile uint8_t   st;
  uint8_t   data[CEC_MSG_BLOCKS];
  uint16_t  eom;
  uint16_t  ack;
  /**
   * [0:3] : Bit index [0-9], Number of bits received
   * [4:7] : Block Index [0-15]
   */
  uint8_t   idx;
  uint8_t   tries;
} CEC_msg_t;

#define CEC_MSG_NONE    0x00  // Message is not used
#define CEC_MSG_START   0x01  // Start bit has been received !
#define CEC_MSG_HEADER  0x02  // Current block is HEADER  (data[0])
#define CEC_MSG_OPCODE  0x04  // Current block is OPCODE  (data[1], optional)
#define CEC_MSG_PAYLOAD 0x08  // Current block is PAYLOAD (data[2:15], optional)
#define CEC_MSG_RXED    0x10  // Message has been completely received ! (EOM bit has been set)
#define CEC_MSG_TXED    0x20
#define CEC_MSG_NACKED  0x40
#define CEC_MSG_ERR     0x80  // Sh*t happened. Reset message and try again.

/**
 *  CEC Message:
 *    A CEC Message can be up to 16 blocks long:
 *      0       : Header block
 *      1       : Opcode block
 *      [2:16]  : Payload block(s)
 *
 * A CEC Block is 10 bits long:
 *   [0:7] are data bits, MSB first
 *   8     is the End Of Message bit (EOM).
 *          When set, the current block is the last one for the CEC message
 *   9     is the Acknowledge bit    (ACK)
 *          Set to 1 by the Initiator (sender).
 *          For Unicast messages: It is cleared by the Follower (receiver), to acknowledge Block reception
 *                                It is then read by the Initiator.
 *                                  0 indicates a successfull transmission
 *                                  1 indicates an error (or that no device has received the message).
 *                                    FIXME: What now ?! Stop transmission I guess...
 *          For Broadcast messages: It is cleared by any Follower rejecting the message.
 *                                  It is then read by the Initiator.
 *                                    1 indicates a successful transmission. Next byte can be sent or whatever.
 *                                    0 indicates one or more Followers have rejected the message.
 *                                      FIXME: What now ?!
 *
 *  CEC Header Block:
 *    Data bytes are the (logical) addresses:
 *      [0:3] is the Destination address
 *      [4:7] is the Initiator   address
 *    If EOM is set, it's a polling message (used to PING an address).
 *        If the two addresses are the same (Destination == Initiator),
 *          the msg is used to check that the address is free
 *          in the Logical Addresses allocation step
 *
 *
 *  CEC Timings:
 *   - Data:
 *     - Start bit is 3.7ms low, 0.8ms high
 *     - Data/EOM:
 *       - 0 is       1.5ms      0.9ms
 *       - 1 is       0.6ms      1.8ms
 *        => Data bit length is 2.4ms
 *     - Ack must be a 1 bit when TXing
 *
 *   - Protocol:
 *     - Arbitration: TODO
 *
 *     - CEC line must be free (high) for the duration of (at least):
 *       - 7 data bits if sending a new frame after a previous one
 *       - 5           if sending a new frame after another Initiator
 *       - 3           when re-trying (after a failed transmission)
 */

void cecTxStart(void);
void cecRestart(void);
void cecMsgReset(CEC_msg_t* m);
void cecMsgPrint(CEC_msg_t* m, char eol);

#define cecMsgBit(m)   ((m).idx&0x0F)
#define cecMsgBlock(m) (((m).idx&0xF0)>>4)
#define cecMsgRxAddr(m) (((m).data[CEC_BLOCK_HEADER])&0xF)
#define cecMsgTxAddr(m) (((m).data[CEC_BLOCK_HEADER])>>4)

CEC_msg_t _cec_msg;

#define CEC_RX_QUEUE_SIZE 16
#define CEC_TX_QUEUE_SIZE 8
#define CEC_OPCODES_SIZE  32

typedef struct
{
  uint8_t   addr;
  uint8_t   mode;
  struct
  {
    uint8_t op;
    cec_cb  cb;
  }         opcodes[CEC_OPCODES_SIZE];
  uint8_t   nopcodes;
  cec_cb    hdlr;

  volatile uint8_t   st;
  CEC_msg_t rx[CEC_RX_QUEUE_SIZE];
  volatile uint8_t   rx_r,
            rx_w;
  CEC_msg_t tx[CEC_TX_QUEUE_SIZE];
  volatile uint8_t   tx_r,
            tx_w,
            tx_delay;
  uint8_t   last_error;
} CEC_device_t;

CEC_device_t _cec_dev;

// CEC LINE
#define CEC_DEV_FREE      0x00
#define CEC_DEV_PENDING   0x01
#define CEC_DEV_ARB_LOST  0x02
#define CEC_DEV_NACKED    0x03
#define CEC_DEV_BITT_ERR  0x04
#define CEC_DEV_TIMEOUT   0x05

// CEC TX
#define CEC_DEV_TX_START  0x04
#define CEC_DEV_TX        0x05
#define CEC_DEV_TX_ACK    0x06
//#define CEC_DEV_TX_NACK 0x07
#define CEC_DEV_TX_ACKED  0x09
#define CEC_DEV_TX_NACKED 0x0A

#define CEC_DEV_TXED      0x0B

#define CEC_DEV_RX_START  0x40
#define CEC_DEV_RX        0x50
#define CEC_DEV_RX_ACK    0x60
#define CEC_DEV_RX_NACK   0x70
//#define CEC_DEV_RX_ACKED  0x80
//#define CEC_DEV_RX_NACKED 0x90
#define CEC_DEV_RXED      0xB0

// *2.4ms
#define CEC_CNT_AFTER_TX  7 // 16.8ms
#define CEC_CNT_AFTER_RX  5 // 12.0ms
#define CEC_CNT_AFTER_ERR 3 //  7.2ms


void CEC_Init(void)
{
  // CEC Out
  DDRD |= _BV(CEC_OUT);
  cecRelease();

  // CEC In
  DDRD &= ~_BV(CEC_IN);
  //PORTD |= _BV(CEC_IN); // Pullup

  _cec_dev.mode = CEC_DEFAULT;
  _cec_dev.addr = 0xF;
  _cec_dev.rx_r = _cec_dev.rx_w = 0;
  _cec_dev.tx_r = _cec_dev.tx_w = 0;
  _cec_dev.tx_delay = 0;

  _cec_dev.last_error = 0;

  _cec_dev.nopcodes = 0;
  _cec_dev.hdlr = NULL;

  tmrStart();
  edgeIntEnable();
  edgeIntAny();
  cecRestart();
  sei();
}

void CEC_tx(const uint8_t* data, const uint8_t l, const uint8_t tries)
{
  uint8_t len = l;

  if(_cec_dev.tx_r >= _cec_dev.tx_w)
    _cec_dev.tx_r = _cec_dev.tx_w = 0;

  if(_cec_dev.tx_w >= CEC_TX_QUEUE_SIZE)
    return;

  if(len > 16)
    len = 16;

  CEC_msg_t *m = &_cec_dev.tx[_cec_dev.tx_w];
  cecMsgReset(m);
    // Set Data
  memcpy(m->data, data, len);
    // Adjust TX Address (if empty)
  if(cecMsgTxAddr(*m) == 0x0)
    m->data[CEC_BLOCK_HEADER] |= (_cec_dev.addr<<4)&0xF0;
  m->idx = (len-1) << 4;
    // Set EOM
  m->eom = 1<<(len-1);
  m->tries = tries;

  ++_cec_dev.tx_w;

  if(_cec_dev.st == CEC_DEV_FREE)
    cecRestart();
}

#define DEBUG_CEC_QUEUE     0
#define DEBUG_CEC_HANDLERS  0
#define DEBUG_CEC_RX        0
#define DEBUG_CEC_TX        0
#define DEBUG_CEC_LINE      0
#define DEBUG_CEC_ISR       0

void CEC_processQueue(void)
{
  while(_cec_dev.rx_r < _cec_dev.rx_w)
  {
    CEC_msg_t *m = &_cec_dev.rx[_cec_dev.rx_r];
    #if DEBUG_CEC_QUEUE
      cecMsgPrint(m,'\n');
    #endif
    uint8_t len = cecMsgBlock(*m);
    if(!len) len = 16;

    uint8_t st  = CEC_MSG_NONE;
    if((m->ack ^ (((uint32_t)1<<len)-1)) != 0)
      st = CEC_MSG_NACKED;

    cec_cb cb = _cec_dev.hdlr;
    if(len > 1)
    {
      #if DEBUG_CEC_HANDLERS >= 1
      dbg_s("Look for Handler for opcode 0x"); dbg_n(m->data[CEC_BLOCK_OPCODE]); dbg_s("... ");
      #endif
      uint8_t i=0;
      for(;i<_cec_dev.nopcodes;i++)
      {
        if(_cec_dev.opcodes[i].op ==
            m->data[CEC_BLOCK_OPCODE]
          )
        {
          #if DEBUG_CEC_HANDLERS >= 1
          dbg_s("Found !");
          #endif
          cb = _cec_dev.opcodes[i].cb;
          i = _cec_dev.nopcodes;
        }
      }
      #if DEBUG_CEC_HANDLERS >= 1
      dbg_c('\n');
      #endif
    }
    if(cb)
    {
      #if DEBUG_CEC_HANDLERS >= 1
      dbg_s("Call callback\n");
      #endif
      (*cb)(st, m->data, len);
    }
    ++_cec_dev.rx_r;
  }
}

void CEC_setMode(const uint8_t m)
{
  _cec_dev.mode = m;
}

int8_t CEC_registerLogicalAddr(const uint8_t addr, const uint8_t skipPoll)
{
  if(!skipPoll)
  {
    uint8_t data = addr&0xF; data |= data<<4; // Forge Polling Message
    CEC_tx(&data,1,2);
    while(_cec_dev.tx_r < _cec_dev.tx_w)
      _delay_ms(1);
    if(_cec_dev.last_error != CEC_DEV_NACKED)
      return -1;
  }
  _cec_dev.addr = addr & 0xF;
  return _cec_dev.addr;
}

void CEC_setDefaultHandler(cec_cb cb)
{
  _cec_dev.hdlr = cb;
}

void CEC_registerOpcode(const uint8_t op, cec_cb cb)
{
  #if DEBUG_CEC_HANDLERS >= 1
  dbg_s("Register opcode 0x"); dbg_n(op); dbg_c('\n');
  #endif
  _cec_dev.opcodes[_cec_dev.nopcodes].op = op;
  _cec_dev.opcodes[_cec_dev.nopcodes].cb = cb;
  ++_cec_dev.nopcodes;
}

void cecRestart(void)
{
  cecMsgReset(&_cec_msg);
  _cec_dev.st = CEC_DEV_FREE;

  if(((_cec_dev.mode & CEC_LISTEN_ONLY) != 0) ||
     _cec_dev.tx_r >= _cec_dev.tx_w
    )
    return;
  CEC_msg_t *m = &_cec_dev.tx[_cec_dev.tx_r];
  _cec_msg.st = CEC_MSG_START;
  _cec_dev.st = CEC_DEV_TX_START;
  _cec_dev.last_error = 0;

#if DEBUG_CEC_ISR >= 1
  dbg_c('9');
#endif
  uint8_t len = cecMsgBlock(*m)+1;
  memcpy(_cec_msg.data, m->data, len);
  _cec_msg.eom = m->eom;
#if DEBUG_CEC_TX >= 1
  dbg_s("\nTx ");
  cecMsgPrint(&_cec_msg,'\n');
#endif
  cecBit(START_AVG,START_LEN);
#if DEBUG_CEC_ISR >= 1
  dbg_c('8');
#endif
}

void cecWaitAndRestart(uint8_t n)
{
  if(_cec_dev.st == CEC_DEV_NACKED   ||
     _cec_dev.st == CEC_DEV_ARB_LOST ||
     _cec_dev.st == CEC_DEV_TIMEOUT
    )
  {
    _cec_dev.last_error = _cec_dev.st;
    if(!--(_cec_dev.tx[_cec_dev.tx_r].tries))
    {
      #if DEBUG_CEC_TX >= 1
      dbg_c('D');
      #endif
      ++_cec_dev.tx_r;
    }
    #if DEBUG_CEC_TX >= 1
    else
    {
      dbg_c('r'); dbg_c('0'+_cec_dev.tx[_cec_dev.tx_r].tries);
    }
    #endif
  }
  _cec_dev.st = CEC_DEV_PENDING;
  _cec_dev.tx_delay = n;
  cecBit(0, DATA_LEN);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// IntServiceRoutineCEC(void)
ISR(INT2_vect)
{
  #if DEBUG_CEC_ISR >= 1
  dbg_c('E');
  #endif
  if(!inRead()) // Low Level => Falling edge
  {
    #if DEBUG_CEC_RX >= 3
    dbg_c('F');
    #endif
    if(_cec_dev.st == CEC_DEV_FREE ||
       _cec_dev.st == CEC_DEV_PENDING
      )
      cecBit(0,TIMEOUT);

    //else if(_cec_dev.st < CEC_DEV_RX_START) // TXing
    //  return;

    tmrReset();
    if(_cec_dev.st == CEC_DEV_RX_NACK)
    {
      #if DEBUG_CEC_RX >= 2
      dbg_c('N');
      #endif
      cecBit(DATA_H_AVG,DATA_LEN);
      _cec_dev.st = CEC_DEV_RX;
    }
    else if(_cec_dev.st == CEC_DEV_RX_ACK)
    {
      #if DEBUG_CEC_RX >= 2
      dbg_c('A');
      #endif
      cecBit(DATA_L_AVG,DATA_LEN);
      _cec_dev.st = CEC_DEV_RX;
    }
    return;
  }

  #if DEBUG_CEC_RX >= 3
  dbg_c('R'); dbg_n(tmrGet());
  #endif

  enum DecodeResult result = TimerDecode();
  uint8_t rx_bit   = cecMsgBit(_cec_msg),
          rx_block = cecMsgBlock(_cec_msg);
  if(result == BIT_Start)
  {
    if(_cec_dev.st == CEC_DEV_FREE ||
       _cec_dev.st == CEC_DEV_PENDING
      )
    {
      #if DEBUG_CEC_RX >= 2
        dbg_c('<');
      #endif
      _cec_msg.st = CEC_MSG_START;
      _cec_dev.st = CEC_DEV_RX_START;
      cecMsgReset(&_cec_msg);
      return;
    }
    else if(_cec_dev.st == CEC_DEV_TX_START)
      return;

    dbg_c('S'); dbg_n(_cec_dev.st);
    _cec_dev.st = CEC_DEV_BITT_ERR;
    return cecWaitAndRestart(CEC_CNT_AFTER_ERR);
  }
  else if(result == BIT_OOR)
  {
    #if DEBUG_CEC_RX >= 2
    dbg_c('x'); dbg_n(_cec_dev.st);
    #endif
    return;
    //cecRestart();
    dbg_c('x'); dbg_n(_cec_dev.st);
    _cec_dev.st = CEC_DEV_BITT_ERR;
    return cecWaitAndRestart(CEC_CNT_AFTER_ERR);
  }

  if(_cec_dev.st < CEC_DEV_RX_START) // TXing, check value
  {
    uint16_t tmr0v = tmr0Get();
    if((tmr0v == DATA_L_AVG && result == BIT_H) ||
       (tmr0v == DATA_H_AVG && result == BIT_L)
      )
    {
      dbg_c('L');
      _cec_dev.st = CEC_DEV_ARB_LOST;
      return cecWaitAndRestart(CEC_CNT_AFTER_ERR);
    }
    // Getting here means the receiver has (N)ACKED
    if(_cec_dev.st == CEC_DEV_TX_ACK)
    {
      //dbg_n(result);
      if((cecMsgRxAddr(_cec_msg) != CEC_ADDR_BROADCAST))
      {
        if(result == BIT_L)
        {
          #if DEBUG_CEC_TX >= 2
          dbg_c('A');
          #endif
          _cec_dev.st = CEC_DEV_TX_ACKED;
        }
        else
        {
          #if DEBUG_CEC_TX >= 2
          dbg_c('N');
          #endif
          _cec_dev.st = CEC_DEV_TX_NACKED;
        }
      }
      else
      {
        if(result == BIT_H)
        {
          #if DEBUG_CEC_TX >= 2
          dbg_c('A');
          #endif
          _cec_dev.st = CEC_DEV_TX_ACKED;
        }
        else
        {
          #if DEBUG_CEC_TX >= 2
          dbg_c('N');
          #endif
          _cec_dev.st = CEC_DEV_TX_NACKED;
        }
      }
    }
    return;
  }
  switch(_cec_dev.st)
  {
    case CEC_DEV_FREE:
    case CEC_DEV_PENDING:
      break;
    case CEC_DEV_RX_START:
      _cec_msg.st = CEC_MSG_HEADER; // Meh ?
      _cec_dev.st = CEC_DEV_RX;
    case CEC_DEV_RX:
      if(rx_block > CEC_MSG_BLOCKS-1)
      {
        #if DEBUG_CEC_RX >= 2
        dbg_c('o');
        #endif
        break;
      }

      if(rx_bit < CEC_BLOCK_BITS)
      {
        _cec_msg.data[rx_block] |= (result&0x1)<<((CEC_BLOCK_BITS-1)-rx_bit);
        _cec_msg.idx++;
        #if DEBUG_CEC_RX >= 2
        //dbg_c('0'+(result&0x1));//'.');
        dbg_c('.');
        #endif
      }
      // Enough data bits received ! Next one is EOM
      else if(rx_bit == CEC_BIT_EOM)
      {
        _cec_msg.eom |= (result&0b1)<<rx_block;
        _cec_msg.idx++;
        #if DEBUG_CEC_RX >= 2
        dbg_c((result&0b1) ? 'E' : 'e');
        #endif

        // EOM bit has been received, Prepare ACK

        // Listen only, do not ACK/NACK msg
        if((_cec_dev.mode & CEC_LISTEN_ONLY) != 0)
          break;

        // Default is to NACK msg
        _cec_dev.st = CEC_DEV_RX_NACK;
        uint8_t rx_addr = cecMsgRxAddr(_cec_msg);

        // Broadcast msg, logic is inverted.
        if(rx_addr == CEC_ADDR_BROADCAST)
        {
          // FIXME What Now ?
          // Currently accepting all broadcast msgs
          #if DEBUG_CEC_RX >= 2
          dbg_c('b');
          #endif
          break;
        }

        // Msg is not for me, NACK
        if(rx_addr != _cec_dev.addr)
          break;

        // Block is HEADER or data, ACK
        // Block is OPCODE and ALLOW_ALL_OPCODE is set, ACK
        if((rx_block != CEC_BLOCK_OPCODE) ||
           (_cec_dev.mode & CEC_ALLOW_ALL_OPCODES) != 0
          )
        {
          _cec_dev.st = CEC_DEV_RX_ACK;
          break;
        }

        // Block is OPCODE.Check for registered OPCODE
        uint8_t i=0;
        for(;i<_cec_dev.nopcodes;i++)
        {
          // OPCODE is registered, ACK
          if(_cec_dev.opcodes[i].op ==
            _cec_msg.data[CEC_BLOCK_OPCODE]
            )
          {
            _cec_dev.st = CEC_DEV_RX_ACK;
            break;
          }
        }
      } // EOM was received ! Next one is ACK
      else if(rx_bit == CEC_BIT_ACK)
      {
        uint8_t rx_addr = cecMsgRxAddr(_cec_msg);
        #if DEBUG_CEC_RX >= 2
        dbg_c(result == BIT_L ? 'a' : 'n');
        #endif
        if((rx_addr != CEC_ADDR_BROADCAST && result == BIT_L) || 
           (rx_addr == CEC_ADDR_BROADCAST && result == BIT_H)
          )
            _cec_msg.ack |= (0b1)<<rx_block;
        
        // Increment block number. Also, reset bit count !
        _cec_msg.idx = (rx_block+1)<<4;
        // Update msg Status, mostly for DEBUG
        if(rx_block == CEC_BLOCK_HEADER)
        {
          _cec_msg.st  = CEC_MSG_OPCODE;
          #if DEBUG_CEC_RX >= 2
          dbg_c('H'); // Header received
          #endif
        }
        else if(rx_block == CEC_BLOCK_OPCODE)
        {
          _cec_msg.st  = CEC_MSG_PAYLOAD;
          #if DEBUG_CEC_RX >= 2
          dbg_c('O'); // OPCODE received
          #endif
        }
        #if DEBUG_CEC_RX >= 2
        else
          dbg_c('0'+rx_block); // Header received
        #endif

        // EOM was set, Message is complete !
        if(((_cec_msg.eom >> rx_block)&0b1) ||
           (((_cec_msg.ack >> rx_block)&0b1) == 0)
          )
        {
          #if DEBUG_CEC_RX >= 2
          dbg_c('>');
          dbg_c('\n');
          #endif
          #if DEBUG_CEC_RX >= 1
          dbg_s("Rxed ");
          cecMsgPrint(&_cec_msg,' ');
          dbg_c('\n');
          #endif

          _cec_msg.st = CEC_MSG_RXED;
          _cec_dev.st = CEC_DEV_RXED;

          if((rx_addr == _cec_dev.addr ||
              rx_addr == CEC_ADDR_BROADCAST ||
              ((_cec_dev.mode & CEC_PROMISCUIOUS) != 0)
             ) &&
             (_cec_dev.rx_w < CEC_RX_QUEUE_SIZE)
            )
          {
             if(_cec_dev.rx_r >= _cec_dev.rx_w)
              _cec_dev.rx_r = _cec_dev.rx_w = 0;
            memcpy(&_cec_dev.rx[_cec_dev.rx_w++], &_cec_msg, sizeof(CEC_msg_t));
          }
          return cecWaitAndRestart(CEC_CNT_AFTER_RX);

        }
      }
      #if DEBUG_CEC_RX >= 2
      else
        dbg_c('U');
      #endif
      break;

    default:
      dbg_c('#'); dbg_n(_cec_dev.st);
      return;
  }
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// IntServiceRoutineTimerA0(void)
ISR(TIMER1_COMPA_vect)
{
  //dbg_c('T'); dbg_n16(tmr0Get());
  if(_low)
  {
    #if DEBUG_CEC_ISR >= 1
      dbg_c('A');
    #endif
    cecRelease();
    _low = 0;
    if(_high)
      tmr0(_high);
    else
      tmr0Disable();
    return;
  }
  tmr0Disable();
  _high = 0;

  #if DEBUG_CEC_ISR >= 1
  dbg_c('B');
  #endif

  // Pre-handle this case
  if(_cec_dev.st == CEC_DEV_TX_ACKED)
  {
    #if DEBUG_CEC_TX >= 2
    //dbg_c('a');
    //dbg_n(_cec_msg.st);
    #endif
    _cec_dev.st = _cec_msg.st == CEC_MSG_TXED ? CEC_DEV_TXED : CEC_DEV_TX;
  }

  switch(_cec_dev.st)
  {
    case CEC_DEV_FREE:
    case CEC_DEV_RX:
      return;
    // If we get here, either no reply or NACK, so that's a NACK
    case CEC_DEV_TX_ACK:
    case CEC_DEV_TX_NACKED:
      #if DEBUG_CEC_TX >= 2
      dbg_c('R');
      #endif
      _cec_dev.st = CEC_DEV_NACKED;
      return cecWaitAndRestart(CEC_CNT_AFTER_ERR);

    case CEC_DEV_PENDING:
      if(--_cec_dev.tx_delay == 0)
      {
        #if DEBUG_CEC_LINE >= 1
        dbg_c('F');
        #endif
        cecRestart();
      }
      else
      {
        #if DEBUG_CEC_LINE >= 1
        dbg_c('*');
        #endif
        cecBit(0,DATA_LEN);
      }
      return;

    case CEC_DEV_TXED:
      #if DEBUG_CEC_TX >= 2
      dbg_c('\\');
      #endif
      ++_cec_dev.tx_r;
      return cecWaitAndRestart(CEC_CNT_AFTER_TX);

    case CEC_DEV_TX_START:
      #if DEBUG_CEC_TX >= 2
      dbg_c('/');
      #endif
      _cec_dev.st = CEC_DEV_TX;

    case CEC_DEV_TX:
    {
      uint8_t tx_bit   = cecMsgBit(_cec_msg),
              tx_block = cecMsgBlock(_cec_msg);

      uint8_t bit = 2;
      if(tx_block >= CEC_MSG_BLOCKS)
      {
        dbg_c('=');
        break;
      }
      if(tx_bit < CEC_BLOCK_BITS)
      {
        bit = (_cec_msg.data[tx_block]>>((CEC_BLOCK_BITS-1)-tx_bit))&0b1;
        _cec_msg.idx++;
        #if DEBUG_CEC_TX >= 2
        dbg_c(',');
        dbg_c('0'+bit);
        #endif
      }
      // Enough data bits Sent ! Next one is EOM
      else if(tx_bit == CEC_BIT_EOM)
      {
        bit = (_cec_msg.eom >> tx_block)&0b1;
        _cec_msg.idx++;
        #if DEBUG_CEC_TX >= 2
        dbg_c(bit ? 'E' : 'e');
        #endif
      } // EOM was sent ! Next one is ACK
      else if(tx_bit == CEC_BIT_ACK)
      {
        #if DEBUG_CEC_TX >= 2
        dbg_c('A');
        #endif
        // Increment block number. Also, reset bit count !
        _cec_msg.idx = (tx_block+1)<<4;
        if(tx_block == CEC_BLOCK_HEADER)
        {
          _cec_msg.st  = CEC_MSG_OPCODE;
          #if DEBUG_CEC_TX >= 2
          dbg_c('H'); // Header sent
          #endif
        }
        else if(tx_block == CEC_BLOCK_OPCODE)
        {
          _cec_msg.st  = CEC_MSG_PAYLOAD;
          #if DEBUG_CEC_TX >= 2
          dbg_c('O'); // OPCODE sent
          #endif
        }
        #if DEBUG_CEC_TX >= 2
        else
          dbg_c('0'+tx_block); // Data sent
        #endif

        // Do Ack
        bit = 1;
        _cec_dev.st = CEC_DEV_TX_ACK;

        if((_cec_msg.eom >> tx_block)&0b1)
        {
          //dbg_c('l');
          _cec_msg.st = CEC_MSG_TXED;
        }
      }
      if(bit < 2)
        cecBit(bit == 0 ? DATA_L_AVG : DATA_H_AVG,DATA_LEN);
      break;
    }

    default:
      dbg_c('+');
      dbg_n(_cec_dev.st);
  }
}


void cecMsgPrint(CEC_msg_t* m, char eol)
{

  dbg_s("[0x");   dbg_n(m->data[CEC_BLOCK_HEADER]>>4);
  dbg_s(" > 0x"); dbg_n(m->data[CEC_BLOCK_HEADER]&0xF);
  if(m->st != CEC_MSG_START) dbg_c((m->ack & 0b1) ? 'a': 'n');

  if((m->eom & 0b1) == 0)
  {
    dbg_s(": 0x");  dbg_n(m->data[CEC_BLOCK_OPCODE]);
    if(m->st != CEC_MSG_START) dbg_c((m->ack & 0b10) ? 'a': 'n');
    if((m->eom & 0b10) == 0)
    {
      dbg_s(", ");
      uint8_t i;
      for(i=CEC_BLOCK_OPCODE+1;i<CEC_MSG_BLOCKS;i++)
      {
        dbg_s("0x"); dbg_n(m->data[i]);
        if(m->st != CEC_MSG_START) dbg_c((m->ack & (1<<i)) ? 'a': 'n');
        if((m->eom & (1<<i)) == 0)
          dbg_c(' ');
        else
          break;
      }
    }
  }
  dbg_c(']');
  dbg_c(eol);
}

void cecMsgReset(CEC_msg_t* m)
{
  uint8_t i;
  m->st = CEC_MSG_NONE;
  for(i=0;i<16;i++)
    m->data[i] = 0;
  m->eom = 0x00;
  m->ack = 0x00;
  m->idx = 0x00;
}
