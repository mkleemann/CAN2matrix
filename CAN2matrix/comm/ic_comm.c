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

#include "comm_can_ids.h"
#include "ic_comm.h"

#include <avr/eeprom.h>


/**** EEPROM VARIABLES ******************************************************/

/**
 * \brief sequence for startup
 *
 * \note The 3rd byte (0x39) may be of interest, since it is also sent by
 *       the next sequence with AUDIO indicator.
 */
uint8_t EEMEM ic_comm_startup_seq[IC_COMM_START_SEQ_LENGTH] = {
   0x10, 0x15, 0x39, 0x00, 0x01, 0x01
};

/**
 * \brief sequence for audio startup
 *
 * \note There is no end of sequence marker here.
 */
uint8_t EEMEM ic_comm_startup_audio_seq[IC_COMM_AUDIO_SEQ_LENGTH] = {
   0x20, 0x02, 0x80, 0x39, 0x20, 0x41, 0x55, 0x44,   // byte 6..7 "AUD"
   0x11, 0x49, 0x4F                                  // byte 2..3 "IO"
};

/**
 * \brief normal media info sequence
 *
 * 2nd row is populated with 8 character free text, e.g. station info. The
 * top row is populated with 5 byte media info (which media) and 5 byte
 * other information, e.g. track number or traffic programme indicator.
 *
 * The default text is setup to "." (0x2E) as placeholder. This will be
 * overwritten in the frame setup.
 *
 * It looks like that:
 * \code
 * +----------------------------------------------------------------+
 * |  ***** ***     *     **  *   *   ***   ***   ***   ***   ***   |
 * |    *   *  *   * *   *  * *  *   *   * *   * *   * *   * *   *  |
 * |    *   *   * *   * *     * *    *   * *   * *   * *   * *   *  |
 * |    *   *  *  *   * *     **      ****  ****  ****  ****  ****  |
 * |    *   ***   ***** *     * *        *     *     *     *     *  |
 * |    *   *  *  *   *  *  * *  *   *   * *   * *   * *   * *   *  |
 * |    *   *   * *   *   **  *   *   ***   ***   ***   ***   ***   |
 * |                                                                |
 * |                                                                |
 * |       ***** ***   ***** ***** ***** ***** *   * *****          |
 * |       *     *  *  *     *       *   *      * *    *            |
 * |       *     *   * *     *       *   *       *     *            |
 * |       ***   *  *  ***   ***     *   ***     *     *            |
 * |       *     ***   *     *       *   *       *     *            |
 * |       *     *  *  *     *       *   *      * *    *            |
 * |       *     *   * ***** *****   *   ***** *   *   *            |
 * |                                                                |
 * |                                                                |
 * |                                                                |
 * |                                                                |
 * +----------------------------------------------------------------+
 * \endcode
 */
uint8_t EEMEM ic_comm_text_seq[IC_COMM_TEXT_SEQ_LENGTH] = {
   0x20, 0x09, 0x02, 0x57, 0x0D, 0x03, 0x06, 0x00,   // startpos 6:10; 8 bytes
   0x21, 0x0A, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
   0x22, 0x2E, 0x2E, 0x2E, 0x57, 0x0A, 0x03, 0x02,   // startpos 2:0 ; 5 bytes
   0x03, 0x00, 0x00, 0x00, 0x2E, 0x2E, 0x2E, 0x2E,
   0x24, 0x2E, 0x57, 0x0A, 0x03, 0x20, 0x00, 0x00,   // startpos 32:0; 5 bytes
   0x15, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x08

};


/**** VARIABLES *************************************************************/

//! states of the FSM to send information to the instrument cluster
ic_comm_fsm_t ic_comm_states = IC_COMM_IDLE;

//! special startup sequence for instrument cluster (state AUDIO)
ic_comm_stage_t stage = IC_COMM_START_FRAME;

//! sequence number for sending
uint8_t seqTx = 0;

//! number of bytes in frame
uint8_t bytesInFrame = 0;

//! pointer to next message in frame
uint8_t nextMsg = 0;

//! buffer to setup a communication frame
uint8_t frame[IC_COMM_MAX_LENGTH_OF_FRAME];

//! pointer to free text
uint8_t* freeText = 0;

//! pointer to info text
uint8_t* infoText = 0;


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
   stage = IC_COMM_START_FRAME;
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
/*
   uint8_t i;
*/
   switch(stage)
   {
      case IC_COMM_START_FRAME:
      {
         eeprom_read_block(frame,
                           &ic_comm_startup_seq,
                           IC_COMM_START_SEQ_LENGTH);
         bytesInFrame = IC_COMM_START_SEQ_LENGTH;
         // next stage
         stage = IC_COMM_AUDIO_SETUP_FRAME;
         break;
      }

      case IC_COMM_AUDIO_SETUP_FRAME:
      {
         eeprom_read_block(frame,
                           &ic_comm_startup_seq,
                           IC_COMM_AUDIO_SEQ_LENGTH);
         bytesInFrame = IC_COMM_AUDIO_SEQ_LENGTH;
         // next stage
         stage = IC_COMM_NORMAL_OP;
         break;
      }

      case IC_COMM_NORMAL_OP:
      {
         // read sequence from eeprom
         eeprom_read_block(frame,
                           &ic_comm_startup_seq,
                           IC_COMM_TEXT_SEQ_LENGTH);
         bytesInFrame = IC_COMM_TEXT_SEQ_LENGTH;
         // now setup text fragments

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

/**
 * \brief get current state
 * \return current fsm state
 */
ic_comm_fsm_t getCurFsmState(void)
{
   return(ic_comm_states);
}

/**
 * \brief get current stage of operation
 * \return current stage of operation
 */
ic_comm_stage_t getCurStage(void)
{
   return(stage);
}

/**
 * \brief get media info for showing in instrument cluster
 * \param data - pointer to media info
 */
void setInfoText(uint8_t* data)
{
   infoText = data;
}

/**
 * \brief get freetext for showing in instrument cluster
 * \param data - pointer to free text
 */
void setFreeText(uint8_t* data)
{
   freeText = data;
}

