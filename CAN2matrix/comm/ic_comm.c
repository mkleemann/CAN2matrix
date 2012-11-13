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

#include "../can/can_mcp2515.h"

#include "comm_can_ids.h"
#include "ic_comm.h"

#include <avr/eeprom.h>
#include <stdlib.h>




/**** PATTERN STORAGE ******************************************************/

/**
 * \brief start sequence for each text information including placeholder
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
uint8_t ic_comm_text_segment[IC_COMM_TEXT_SEQ_LENGTH] =
{
   0x57, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x2E,
   0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
   0x2E
};

/**
 * \brief sequence for startup
 *
 * \note The 3rd byte (0x39) may be of interest, since it is also sent by
 *       the next sequence with AUDIO indicator.
 */
uint8_t ic_comm_std_pattern_start[IC_COMM_STD_PATTERN_START_LENGTH] =
{
   0x10, 0x15, 0x39, 0x00, 0x01, 0x01
};

/**
 * \brief sequence for audio startup
 *
 * \note There is no end of sequence marker here.
 */
uint8_t ic_comm_std_pattern_start_audio[IC_COMM_STD_PATTERN_START_AUDIO_LENGTH] =
{
   0x20, 0x02, 0x80, 0x39, 0x20, 0x41, 0x55, 0x44,   // byte 4..7 "9 AUD"
   0x11, 0x49, 0x4F                                  // byte 2..3 "IO"
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
 */
uint8_t ic_comm_std_pattern_media[IC_COMM_STD_PATTERN_MEDIA_LENGTH] =
//! Pattern #1 - 47 bytes
{ 0x20, 0x09, 0x02, 0x57, 0x0B, 0x03, 0x21, 0x00,  // right 6 chars
  0x21, 0x00, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
  0x22, 0x2E, 0x57, 0x08, 0x03, 0x02, 0x00, 0x00,  // left 3 chars
  0x03, 0x00, 0x2E, 0x2E, 0x2E, 0x57, 0x0D, 0x03,
  0x24, 0x0C, 0x00, 0x0A, 0x00, 0x2E, 0x2E, 0x2E,  // center 8 chars
  0x15, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x08
};

/**
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
 */
uint8_t ic_comm_std_pattern_system[IC_COMM_STD_PATTERN_SYSTEM_LENGTH] =
//! Pattern #2 - 19 bytes
{ 0x20, 0x09, 0x02, 0x57, 0x0B, 0x03, 0x11, 0x00,  // center 6 chars
  0x21, 0x05, 0x00, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,  // vertically centered
  0x12, 0x2E, 0x08
};

/**
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
 */
uint8_t ic_comm_std_pattern_pdc[IC_COMM_STD_PATTERN_PDC_LENGTH] =
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

/**
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
uint8_t ic_comm_std_pattern_traffic[IC_COMM_STD_PATTERN_TRAFFIC_LENGTH] =
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

//! PDC value storage
uint8_t pdcValues[IC_COMM_PDC_DATA_LENGTH] =
{
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

//! text information storage for row #1, if available
uint8_t textRow1[IC_COMM_MAX_LENGTH_OF_ROW] =
{
   0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
   0x2E, 0x2E
};

//! dynamically set length of text information in row #1 storage
uint8_t lengthRow1 = IC_COMM_MAX_LENGTH_OF_ROW;

//! text information storage for row #2, if available
uint8_t textRow2[IC_COMM_MAX_LENGTH_OF_ROW] =
{
   0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
   0x2E, 0x2E
};

//! dynamically set length of text information in row #2 storage
uint8_t lengthRow2 = IC_COMM_MAX_LENGTH_OF_ROW;

//! mode for choosing correct pattern
ic_comm_infotype_t mode = INFO_TYPE_SETUP;

//! pointer to sequence within active pattern
uint8_t seqPointerActive = 0;

//! pointer to standard pattern
uint8_t* pattern = ic_comm_std_pattern_start;

//! length of standard pattern
uint8_t  patternLength = IC_COMM_STD_PATTERN_START_LENGTH;

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
         // set start pattern
         pattern = ic_comm_std_pattern_start;
         patternLength = IC_COMM_STD_PATTERN_START_LENGTH;
         ic_comm_startCommSeq(msg);
         ic_comm_cur_state = IC_COMM_SEQ_START;
         break;
      }

      case IC_COMM_INIT_2:
      {
         // set start audio pattern
         pattern = ic_comm_std_pattern_start_audio;
         patternLength = IC_COMM_STD_PATTERN_START_AUDIO_LENGTH;
         ic_comm_startCommSeq(msg);
         ic_comm_cur_state = IC_COMM_SEQ_START;
         ic_comm_end_state = IC_COMM_IDLE;
         break;
      }

      case IC_COMM_IDLE:
      {
         ic_comm_startCommSeq(msg);
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
            ic_comm_send2Cluster(msg);

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
            // now setup information for pattern
            ic_comm_patternSetup();
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
            ic_comm_send2Cluster(msg);

            ic_comm_cur_state = IC_COMM_SEQ_END;
         }
         else
         {
            msg->msgId = CANID_1_COM_RADIO_2_CLUSTER;

            // byte 0: IC_COMM_SOF  | seqCntTx  -> next info message
            // byte 0: IC_COMM_EOF  | seqCntTx  -> wait for ack, stop
            // byte 0: IC_COMM_W4NF | seqCntTx  -> wait for ack, next
            msg->header.len = ic_comm_getNextMsg(msg->data);
            ic_comm_send2Cluster(msg);

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
         ic_comm_send2Cluster(msg);

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
void ic_comm_send2Cluster(can_t* msg)
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
   uint8_t length = 8;
   uint8_t i;
   bool    endOfSeq = (0x10 == (IC_COMM_FRAME_MASK & data[0]));

   // end of sequence
   if(true == endOfSeq)
   {
      // set correct length for next data packet (less than 8 bytes)
      length = patternLength % 8;
   }

   // copy data
   for(i = seqPointerActive; i < (seqPointerActive + length); ++i)
   {
      data[i - seqPointerActive] = pattern[i];
   }

   // end of sequence: length < 8 is not reliable here!
   if(true == endOfSeq)
   {
      // restart with next information frame
      seqPointerActive = 0;
   }
   else
   {
      // next data packet
      seqPointerActive += 8;
   }

   return(length);
}

/**
 * \brief set PDC values
 * \param data - pointer to 8 byte array of values
 * \sa IC_COMM_PDC_DATA_LENGTH
 * \note All 8 possible values are set here, despite their availability.
 */
void ic_comm_setPDCValues(uint8_t* data)
{
   int i;
   for(i = 0; i < IC_COMM_PDC_DATA_LENGTH; ++i)
   {
      pdcValues[i] = data[i];
   }
}

/**
 * \brief set state machine to new start
 */
void ic_comm_restart(void)
{
   ic_comm_cur_state = IC_COMM_INIT_1;
   ic_comm_end_state = IC_COMM_INIT_2;
   seqPointerActive = 0;
}

/**
 * \brief send start of communication sequence to instrument cluster
 * \param msg - pointer to CAN message struct
 */
void ic_comm_startCommSeq(can_t* msg)
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
   ic_comm_send2Cluster(msg);
}

/**
 * \brief set type of information, so the pattern is set accordingly
 * \param type of information
 * \return true, if type was setup correctly
 * \note The startup pattern are set automatically at startup or restart.
 */
bool ic_comm_setType(ic_comm_infotype_t type)
{
   bool retVal = false;
   // safeguard: only allowed if idle
   if(IC_COMM_IDLE == ic_comm_cur_state)
   {
      // set new mode
      mode = type;
      // set right pattern
      switch(type)
      {
         case INFO_TYPE_MEDIA_CD:
         case INFO_TYPE_MEDIA_DVD:
         case INFO_TYPE_MEDIA_HDD:
         case INFO_TYPE_MEDIA_RADIO_FM:
         case INFO_TYPE_MEDIA_RADIO_AM:
         {
            pattern = ic_comm_std_pattern_media;
            patternLength = IC_COMM_STD_PATTERN_MEDIA_LENGTH;
            break;
         }

         case INFO_TYPE_PDC:
         {
            pattern = ic_comm_std_pattern_pdc;
            patternLength = IC_COMM_STD_PATTERN_PDC_LENGTH;
            break;
         }

         case INFO_TYPE_TRAFFIC:
         {
            pattern = ic_comm_std_pattern_traffic;
            patternLength = IC_COMM_STD_PATTERN_TRAFFIC_LENGTH;
            break;
         }

         case INFO_TYPE_FREETEXT: // even, if it's done dynamically
         case INFO_TYPE_MEDIA_AUX:
         case INFO_TYPE_SETUP:
         default:
         {
            // system pattern as default to avoid confusion, even if not used
            pattern = ic_comm_std_pattern_system;
            patternLength = IC_COMM_STD_PATTERN_SYSTEM_LENGTH;
            break;
         }
      }
      retVal = true;
   }
   return(retVal);
}

/**
 * \brief setup for information in pattern requested
 * \todo insert freetext pattern setup
 */
void ic_comm_patternSetup(void)
{
   uint8_t i;
   switch(mode)
   {
      // media pattern
      case INFO_TYPE_MEDIA_CD:
      case INFO_TYPE_MEDIA_DVD:
      case INFO_TYPE_MEDIA_HDD:
      case INFO_TYPE_MEDIA_RADIO_FM:
      case INFO_TYPE_MEDIA_RADIO_AM:
      {
         // right aligned 6 characters from textRow1 storage 3..9
         for(i = 0; i < 5; ++i)
         {
            ic_comm_std_pattern_media[i + 11] = textRow1[i + 3];
         }
         ic_comm_std_pattern_media[17] = textRow1[9];

         // left aligned 3 characters from textRow1 storage 0..2
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_media[i + 26] = textRow1[i];
         }

         // centered 8 characters from textRow2 0..7
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_media[i + 37] = textRow2[i];
         }
         for(i = 0; i < 5; ++i)
         {
            ic_comm_std_pattern_media[i + 41] = textRow2[i+3];
         }
         break;
      }

      // PDC pattern
      case INFO_TYPE_PDC:
      {
         char temp[4];

         // left aligned 3 characters from pdcValues[2]
         utoa(pdcValues[2], temp, 10);
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_pdc[i + 11] = temp[i];
         }

         // right aligned 3 characters from pdcValues[3]
         utoa(pdcValues[3], temp, 10);
         for(i = 0; i < 2; ++i)
         {
            ic_comm_std_pattern_pdc[i + 22] = temp[i];
         }
         ic_comm_std_pattern_pdc[25] = temp[2];

         // left aligned 3 characters from pdcValues[6]
         utoa(pdcValues[6], temp, 10);
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_pdc[i + 45] = temp[i];
         }

         // right aligned 3 characters from pdcValues[7]
         utoa(pdcValues[7], temp, 10);
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_pdc[i + 57] = temp[i];
         }
         break;
      }

      // traffic pattern
      case INFO_TYPE_TRAFFIC:
      {
         // centered 8 characters from textRow2 0..7
         for(i = 0; i < 5; ++i)
         {
            ic_comm_std_pattern_traffic[i + 27] = textRow2[i];
         }
         for(i = 0; i < 3; ++i)
         {
            ic_comm_std_pattern_traffic[i + 33] = textRow2[i + 5];
         }
         break;
      }

      // freetext pattern
      case INFO_TYPE_FREETEXT:
      {
         break;
      }

      // system pattern
      case INFO_TYPE_MEDIA_AUX:
      {
         ic_comm_setupSystemPattern("AUX IN");
         break;
      }

      // system pattern
      case INFO_TYPE_SETUP:
      {
         ic_comm_setupSystemPattern("SETUP ");
         break;
      }

      // system pattern
      default:
      {
         ic_comm_setupSystemPattern("UPS...");
         break;
      }
   }
}

/**
 * \brief setup text of system pattern
 * \param data - 6 character text w/o ending 0
 */
void ic_comm_setupSystemPattern(char* data)
{
   uint8_t i;
   // centered 6 characters
   for(i = 0; i < 5; ++i)
   {
      ic_comm_std_pattern_system[i + 11] = (uint8_t)data[i];
   }
   ic_comm_std_pattern_system[17] = (uint8_t)data[5];
}

