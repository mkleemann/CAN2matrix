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
 *
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
   0x20, 0x02, 0x80, 0x39, 0x20, 0x41, 0x55, 0x44,   // byte 4..7 "9 AUD"
   0x11, 0x49, 0x4F                                  // byte 2..3 "IO"
};

/**
 * \brief start sequence for each text information
 *
 * For further information see section \ref c2m_comm_ic_seq_radio_format
 *
 *- byte 0: start of text sequence
 *- byte 1: length marker
 *- byte 2: mode(?), e.g. overwrite existing text
 *- byte 3: start position x-axis
 *- byte 4: unused
 *- byte 5: start position y-axis
 *- byte 6: unused
 *- byte 7..16: text (max. 10 characters)
 *
 * The default text is setup to "." (0x2E) as placeholder. This will be
 * overwritten in the text setup.
 *
 * For full example see \ref c2_ic_comm_media_view
 */
uint8_t EEMEM ic_comm_text_segment[IC_COMM_TEXT_SEQ_LENGTH] = {
   0x57, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x2E,
   0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
   0x2E
};

/**
 * \var ic_comm_std_pattern_media
 * \brief Media Information Pattern
 *
 *- first row: 3 characters left (media type)
 *- first row: 6 characters right (track or frequency information)
 *- second row: 8 characters centered (free text)
 *
 * For formatting and sequences see \ref c2m_comm_ic_seq_radio_format.
 *
 * Text information to be filled is pre-defined as "." (0x2E). Position
 * information can be overwritten too.
 *
 * \var ic_comm_std_pattern_system
 * \brief System Information Pattern
 *
 *- vertically and horizontally centered: 6 characters (free text)
 *
 * The freetext contains something like "SETUP!" or "AUX IN"
 *
 * For formatting and sequences see \ref c2m_comm_ic_seq_radio_format.
 *
 * Text information to be filled is pre-defined as "." (0x2E). Position
 * information can be overwritten too.
 *
 * \var ic_comm_std_pattern_pdc
 * \brief PDC Pattern
 *
 *- first row: 3 characters left and right
 *- second row: 3 characters left and right (more towards center)
 *- vertically and horizontally centered: "PDC"
 *
 * For formatting and sequences see \ref c2m_comm_ic_seq_radio_format.
 *
 * Text information to be filled is pre-defined as "." (0x2E). Position
 * information can be overwritten too.
 *
 * \var ic_comm_std_pattern_traffic
 * \brief Traffic Programme Pattern
 *
 *- first row: 7 characters centered: "TRAFFIC"
 *- second row: 8 characters centered (free text)
 *
 * For formatting and sequences see \ref c2m_comm_ic_seq_radio_format.
 *
 * Text information to be filled is pre-defined as "." (0x2E). Position
 * information can be overwritten too.
 *
 */
uint8_t EEMEM ic_comm_std_pattern_media[] =
   //! Pattern #1 - 47 bytes
   { 0x20, 0x09, 0x02, 0x57, 0x0B, 0x03, 0x21, 0x00,  // right 6 chars
     0x21, 0x00, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
     0x22, 0x2E, 0x57, 0x08, 0x03, 0x02, 0x00, 0x00,  // left 3 chars
     0x03, 0x00, 0x2E, 0x2E, 0x2E, 0x57, 0x0D, 0x03,
     0x24, 0x0C, 0x00, 0x0A, 0x00, 0x2E, 0x2E, 0x2E,  // center 8 chars
     0x15, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x08
   };

uint8_t EEMEM ic_comm_std_pattern_system[] =
   //! Pattern #2 - 19 bytes
   { 0x20, 0x09, 0x02, 0x57, 0x0B, 0x03, 0x11, 0x00,  // center 6 chars
     0x21, 0x05, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,  // vertically centered
     0x12, 0x2E, 0x08
   };

uint8_t EEMEM ic_comm_std_pattern_pdc[] =
   //! Pattern #3 - 61 bytes
   { 0x20, 0x09, 0x02, 0x57, 0x08, 0x03, 0x02, 0x00,  // left 3 chars
     0x21, 0x00, 0x00, 0x2E, 0x2E, 0x2E, 0x57, 0x08,  // first row
     0x22, 0x03, 0x31, 0x00, 0x00, 0x00, 0x2E, 0x2E,  // right 3 chars
     0x03, 0x2E, 0x57, 0x08, 0x03, 0x18, 0x00, 0x05,  // centered "PDC"
     0x24, 0x00, 'P' , 'D' , 'C' , 0x57, 0x08, 0x03,  // second row
     0x25, 0x06, 0x00, 0x0A, 0x00, 0x2E, 0x2E, 0x2E,  // left 3 chars
     0x26, 0x57, 0x08, 0x03, 0x2D, 0x00, 0x0A, 0x00,  // right 3 chars
     0x17, 0x2E, 0x2E, 0x2E, 0x08
   };

uint8_t EEMEM ic_comm_std_pattern_traffic[] =
   //! Pattern #4 - 37 bytes
   {
      0x20, 0x09, 0x02, 0x57, 0x0C, 0x03, 0x0E, 0x00,  // center 7 chars
      0x21, 0x00, 0x00, 'T' , 'R' , 'A' , 'F' , 'F' ,
      0x22, 'I' , 'C' , 0x57, 0x0D, 0x03, 0x0C, 0x00,  // center 8 chars
      0x03, 0x0A, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
      0x14, 0x2E, 0x2E, 0x2E, 0x08
   };


/**** VARIABLES *************************************************************/

//! states of the FSM to send information to the instrument cluster
ic_comm_fsm_t ic_comm_cur_state = IC_COMM_INIT_1;

//! state for end of sequence return
ic_comm_fsm_t ic_comm_end_state = IC_COMM_INIT_2;

//! sequence counter for transmission
uint8_t seqCntTx = 0;


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
   switch(ic_comm_cur_state)
   {
      case IC_COMM_INIT_1:
      {
         ic_comm_cur_state = IC_COMM_SEQ_START;
         break;
      }

      case IC_COMM_INIT_2:
      {

         ic_comm_cur_state = IC_COMM_SEQ_START;
         ic_comm_end_state = IC_COMM_IDLE;
         break;
      }

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
         send2Cluster(msg);

         ic_comm_cur_state = IC_COMM_SEQ_START;
         ic_comm_end_state = IC_COMM_IDLE;
         break;
      }

      case IC_COMM_SEQ_START:
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
            send2Cluster(msg);

            ic_comm_cur_state = IC_COMM_SEQ_PREAMBLE;
         }
         break;
      }

      case IC_COMM_SEQ_PREAMBLE:
      {
         // get acknowledge to preamble
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         A1 04 8A 85 43 94
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            (0xA1 == msg->data[0]))
         {
            ic_comm_cur_state = IC_COMM_SEQ_INFO;
         }
         break;
      }

      case IC_COMM_SEQ_WAIT:
      {
         // get acknowledge packet from instrument cluster
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     <-- 699 ---     |         Bx
         if((CANID_1_COM_CLUSTER_2_RADIO == msg->msgId) &&
            ((0xB0 | seqCntTx) == msg->data[0]))
         {
            // next info frame
            ic_comm_cur_state = IC_COMM_SEQ_INFO;
         }

         break;
      }

      case IC_COMM_SEQ_INFO:
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
            send2Cluster(msg);

            ic_comm_cur_state = IC_COMM_SEQ_END;
         }
         else
         {
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;

            // byte 0: IC_COMM_SOF  | seqCntTx  -> next info message
            // byte 0: IC_COMM_EOF  | seqCntTx  -> wait for ack, stop
            // byte 0: IC_COMM_W4NF | seqCntTx  -> wait for ack, next
            msg->header.len = ic_comm_getNextMsg(msg->data);
            send2Cluster(msg);

            // next sequence, also for ack!
            ++seqCntTx;

            // frame over? data[0] == 0x/1x
            if(0x20 != (IC_COMM_FRAME_MASK & msg->data[0]))
            {
               // wait for instrument cluster response
               ic_comm_cur_state = IC_COMM_SEQ_WAIT;
            }
         }
         break;
      }

      case IC_COMM_SEQ_END:
      {
         // reset sequence numbers
         seqCntTx = 0;
         //              CAN    Instrument     Example
         // Radio        ID      Cluster
         //   _                     _
         //   |     --- 6B9 -->     |         A8
         msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;
         msg->header.len = 1;
         msg->data[0] = 0xA8;
         send2Cluster(msg);

         ic_comm_cur_state = ic_comm_end_state;
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
 * \brief send CAN message to instrument cluster
 * \param msg - pointer to CAN message
 */
void send2Cluster(can_t* msg)
{
   while(0 == can_send_message(CAN_CHIP1, msg))
   {
      _delay_us(10);
   }
}

/**
 * \brief prepare next info message
 * \param data - pointer to data in CAN message
 * \return length of buffer copied
 */
uint8_t ic_comm_getNextMsg(uint8_t* data)
{
   // sequence number
   data[0] = seqCntTx;

   return(1);
}
