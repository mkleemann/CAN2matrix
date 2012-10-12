/**
 * ----------------------------------------------------------------------------
 *
 * "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
 * <dev@layer128.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
 * like beer much.)
 *
 * Matthias Kleemann
 *
 * ----------------------------------------------------------------------------
 *
 *
 * \file ic_comm.c
 *
 * \date Created: 03.10.2012 13:50:54
 * \author Matthias Kleemann
 */

#include "matrix.h"
#include "ic_comm.h"

#include <stdbool.h>
#include <avr/eeprom.h>


/**** VARIABLES *************************************************************/

//! states of the FSM to send information to the instrument cluster
ic_comm_fsm_t ic_comm_states = IC_COMM_IDLE;

//! special startup sequence for instrument cluster (state AUDIO)
bool firstStart = true;

//! sequence number for sending
uint8_t seqTx = 0;

//! number of bytes in frame
uint8_t bytesInFrame = 0;

//! pointer to next message in frame
uint8_t nextMsg = 0;

/**
 * \brief buffer to setup a communication frame
 */
uint8_t frame[IC_COMM_MAX_LENGTH_OF_FRAME];


/**** EEPROM VARIABLES ******************************************************/

/**
 * \brief offset to start pattern in EEPROM
 */
uint8_t EEMEM ic_comm_eep_start[IC_COMM_EEP_START_LENGTH]   = { 0x09, 0x02 };

/**
 * \brief offset to format pattern in EEPROM
 */
uint8_t EEMEM ic_comm_eep_format[IC_COMM_EEP_FORMAT_LENGTH] = { 0x57, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00 };

/**
 * \brief offset to stop pattern in EEPROM
 */
uint8_t EEMEM ic_comm_eep_stop[IC_COMM_EEP_STOP_LENGTH]     = { 0x08 };



/**** FUNCTIONS *************************************************************/

/**
 * \brief FSM for communicating with instrument cluster
 * \param msg - pointer to CAN message
 *
 * FSM needs to be called after any other CAN activity for that cycle. It
 * manipulates the message received to sent the buffer as needed.
 */
void ic_comm_fsm(can_t* msg)
{
   switch(ic_comm_states)
   {
      case IC_COMM_IDLE:
      {
         // communication request to instrument cluster
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     --- 4D9 -->     |         08 C0 B9
         msg->msgId = CANID_1_COM_RADIO_START;
         msg->header.len = 3;
         msg->data[0] = 0x08;
         msg->data[1] = 0xC0;
         msg->data[2] = 0xB9;
         can_send_message(CAN_CHIP1, msg);

         ic_comm_states = IC_COMM_START;
         break;
      }

      case IC_COMM_START:
      {
         // ack for communication request by instrument cluster
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 2E8 ---     |         39 D0 99
         if((CANID_1_COM_DISP_START == msg->msgId) &&
            (0x39 == msg->data[0]))
         {
            // send preamble
            //              CAN    Instrument     Example
            // Radio        ID      Cluster
            //   _                     _
            //   |     --- 6B9 -->     |         A0 04 54 54 4A B2
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;
            msg->header.len = 6;
            msg->data[0] = 0xA0;
            msg->data[1] = 0x04;
            msg->data[2] = 0x54;
            msg->data[3] = 0x54;
            msg->data[4] = 0x4A;
            msg->data[5] = 0xB2;
            can_send_message(CAN_CHIP1, msg);

            ic_comm_states = IC_COMM_PREAMBLE;
         }
         break;
      }

      case IC_COMM_PREAMBLE:
      {
         // get acknowledge to preamble
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         A1 04 8A 85 43 94
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            (0xA1 == msg->data[0]))
         {
            ic_comm_states = IC_COMM_INFO;
         }
         break;
      }

      case IC_COMM_WAIT_4_CLUSTER:
      {
         // get acknowledge packet from instrument cluster
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         Bx
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            ((0xB0 | seqTx) == msg->data[0]))
         {
            // next info frame
            ic_comm_states = IC_COMM_INFO;
         }

         break;
      }

      case IC_COMM_INFO:
      {
         // last frame/data packet from instrument cluster, e.g.
         // "close of communication" sequence
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         10 23 02 01
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            (0x10 == (IC_COMM_FRAME_MASK & msg->data[0])))
         {
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;
            msg->header.len = 1;
            // add sequence number (+1) to acknowledge Bx
            msg->data[0] = 0xB0 | ((0x0F & msg->data[0]) + 1);
            can_send_message(CAN_CHIP1, msg);

            ic_comm_states = IC_COMM_STOP;
         }
         else
         {
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;

            // byte 0: IC_COMM_SOF  | seqTx  -> next info message
            // byte 0: IC_COMM_EOF  | seqTx  -> wait for ack, stop
            // byte 0: IC_COMM_W4NF | seqTx  -> wait for ack, next
            msg->header.len = ic_comm_getNextMsg(msg->data);
            can_send_message(CAN_CHIP1, msg);

            // next sequence, also for ack!
            ++seqTx;

            // frame over? data[0] == 0x/1x
            if(0x20 != (IC_COMM_FRAME_MASK & msg->data[0]))
            {
               // wait for instrument cluster response
               ic_comm_states = IC_COMM_WAIT_4_CLUSTER;
            }
         }
         break;
      }

      case IC_COMM_STOP:
      {
         // reset sequence numbers
         seqTx = 0;
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     --- 6B9 -->     |         A8
         msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;
         msg->header.len = 1;
         msg->data[0] = 0xA8;
         can_send_message(CAN_CHIP1, msg);

         ic_comm_states = IC_COMM_IDLE;
         break;
      }

      default:
      {
         // do nothing!
         break;
      }
   }
}

/**
 * \brief reset flags for next startup
 */
void ic_comm_reset4start()
{
   // reset for clean startup in communication
   firstStart = true;
   bytesInFrame = 0;
   nextMsg = 0;
   seqTx = 0;
   ic_comm_states = IC_COMM_IDLE;
}

/**
 * \brief setup for frame to send to cluster
 *
 * Fill frame buffer with control and information sequences.
 */
void ic_comm_framesetup(void)
{
   uint8_t* text = getInfoText();
   uint8_t i;

   // start with frame: signal and sequence
   frame[bytesInFrame] = IC_COMM_SOF;                                    // 0
   ++bytesInFrame;

   // information start sequence (once in frame)
   eeprom_read_block(&frame[bytesInFrame],                               // 1..2
                     ic_comm_eep_start,
                     IC_COMM_EEP_START_LENGTH);
   bytesInFrame += IC_COMM_EEP_START_LENGTH;

   eeprom_read_block(&frame[bytesInFrame],                               // 3..9
                     ic_comm_eep_format,
                     IC_COMM_EEP_FORMAT_LENGTH);
   bytesInFrame += IC_COMM_EEP_FORMAT_LENGTH;

   // byte 4: setup length of first string (len + 5)
   frame[4] = 8 + 5; // test 8 byte sequence                             // 4
   // byte 6: format
   frame[6] = 0x06;  // test left                                        // 6

   // next CAN message
   frame[8]  = IC_COMM_SOF | 1;                                          // 8

   // move information                                                   // 10
   frame[10] = frame[9];
   // byte 9: line indicator
   frame[9] = 0x0A;  // test second line                                 // 9
   // one byte more
   ++bytesInFrame;

   // first string: 1..5 bytesInFrame
   // testing here!
   for(i = 0; i < 5; ++i, ++bytesInFrame)                                // 11..15
   {
      frame[bytesInFrame] = text[i];
   }

   // next CAN message
   frame[bytesInFrame]  = IC_COMM_EOF | 2;                               // 16
   // one byte more
   ++bytesInFrame;

   // first string: 1..5 bytesInFrame
   // testing here!
   for(i = 5; i < 8; ++i, ++bytesInFrame)                                // 17..19
   {
      frame[bytesInFrame] = text[i];
   }

   eeprom_read_block(&frame[bytesInFrame],                               // 20
                     ic_comm_eep_stop,
                     IC_COMM_EEP_STOP_LENGTH);
   bytesInFrame += IC_COMM_EEP_STOP_LENGTH;
}

/**
 * \brief get next message from frame buffer
 * \param data - pointer to destination buffer
 * \return length of buffer copied
 */
uint8_t ic_comm_getNextMsg(uint8_t* data)
{
   uint8_t length = 8;
   uint8_t i;

   // copy data
   for(i = nextMsg; i < nextMsg + 8; ++i)
   {
      data[i - nextMsg] = frame[i];
   }

   // set pointer to next message
   nextMsg += 8;

   if(0 == (IC_COMM_EOF_MASK & data[0]))  // 1x/0x
   {
      // last number of bytes in CAN message
      length = bytesInFrame % 8;
      // restart frame
      nextMsg = 0;
   }

   return(length);
}




