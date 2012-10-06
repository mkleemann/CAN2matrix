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

//! states of the FSM to send information to the instrument cluster
ic_comm_fsm_t ic_comm_states = IC_COMM_IDLE;

//! special startup sequence for instrument cluster (state AUDIO)
bool firstStart = true;

//! text information to be shown in cluster
uint8_t info[IC_COMM_INFO_LENGTH];

//! sequence number for sending
uint8_t seqTx = 0;


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
            ((0xB0 | (seqTx+1)) == msg->data[0]))
         {
            // get ack for info frames
            ++seqTx;
            // next info frame
            ic_comm_states = IC_COMM_INFO;
         }

         // get close of communication sequence fro minstrument cluster
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         10 23 02 01
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            (0 == (0xE0 & msg->data[0]))) // 1x/0x frames
         {
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;
            msg->header.len = 1;
            // add sequence number (+1) to acknowledge Bx
            msg->data[0] = 0xB0 | ((0x0F & msg->data[0]) + 1);
            can_send_message(CAN_CHIP1, msg);

            ic_comm_states = IC_COMM_STOP;
         }

         break;
      }

      case IC_COMM_INFO:
      {
         break;
      }

      case IC_COMM_STOP:
      {
         // reset sequence numbers
         seqRx = 0;
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
   firstStart = true;
}

/**
 * \brief setup text to send to instrument cluster
 * \param text - pointer to text buffer
 */
void ic_comm_set_text(uint8_t* text)
{
   uint8_t i;

   for(i = 0; i < IC_COMM_INFO_LENGTH; ++i)
   {
      info[i] = text[i];
   }
}


