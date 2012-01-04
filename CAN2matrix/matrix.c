/**
 * ----------------------------------------------------------------------------
 * "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
 * <m.kleemann@gmx.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
 * like beer much.)
 *
 * Matthias Kleemann
 * ----------------------------------------------------------------------------
 **/

/*
 * matrix.c
 *
 * Created: 03.01.2012 07:38:54
 *  Author: MKleemann
 */

#include <avr/io.h>
#include "matrix.h"

/***************************************************************************/
/* Definition of global variables to store CAN values.                     */
/***************************************************************************/
typedef struct
{
   uint8_t wheel1U;     // wheel signal 1 upper byte (as-is)
   uint8_t wheel1L;     // wheel signal 1 lower byte (as-is)
   uint8_t wheel2U;     // wheel signal 2 upper byte (as-is)
   uint8_t wheel2L;     // wheel signal 2 lower byte (as-is)
   uint8_t wheel3U;     // wheel signal 3 upper byte (as-is)
   uint8_t wheel3L;     // wheel signal 3 lower byte (as-is)
   uint8_t wheel4U;     // wheel signal 4 upper byte (as-is)
   uint8_t wheel4L;     // wheel signal 4 lower byte (as-is)
   uint8_t ignition;    // ignition key status (destination)
   uint8_t gearBox;     // gear box status (destination)
   uint8_t headlights;  // headlights status (destination) on/off
   uint8_t dimLevel;    // dimming of display (destination) 0..255
} storeVals_t;

volatile storeVals_t storage;


/***************************************************************************/
/* fetch/fill functions for CAN (check IDs)                                */
/***************************************************************************/

/**
 * @brief fetch information from CAN1 and put to storage
 * @param CAN message to extract
 */
void fetchInfoFromCAN1(can_t* msg)
{
   switch(msg->msgId)
   {
      case CANID_1_IGNITION:
      {
         transferIgnStatus(msg);
         break;
      } /* end of case CANID_1_IGNITION */
      case CANID_1_WHEEL_DATA:
      {
         transferWheelCount(msg);
         break;
      } /* end of case CANID_1_WHEEL_DATA */
      case CANID_1_REVERSE_GEAR:
      {
         transferGearStatus(msg);
         break;
      } /* end of case CANID_1_REVERSE_GEAR */
      default:
      {
         // do nothing!
         break;
      } /* end of default */
   } /* end of switch msgId */
}

/**
 * @brief fetch information from CAN2 and put to storage
 * @param CAN message to extract
 */
void fetchInfoFromCAN2(can_t* msg)
{
}

/**
 * @brief put information from storage to CAN1
 * @param CAN message to fill
 */
void fillInfoToCAN1(can_t* msg)
{
}

/**
 * @brief put information from storage to CAN2
 * @param CAN message to fill
 */
void fillInfoToCAN2(can_t* msg)
{
   // remove any old values
   for(int i = 0; i < 8; ++i)
   {
      msg->data[i] = 0;
   } /* end of for 0..7 */

   switch(msg->msgId)
   {
      case CANID_2_IGNITION:
      {
         // fill in length of message
         msg->header.len = 2;
         // main ignition status
         msg->data[0] = storage.ignition;
         // 0: start not active; 1: normal start
         msg->data[1] = (storage.ignition & 0x80) ? 1 : 0;
         break;
      } /* end of case CANID_2_IGNITION */
      case CANID_2_WHEEL_DATA:
      {
         // message is 8 bytes long
         msg->header.len = 8;
         // byte 0/1: engine PRM    : not used here
         // byte 2/3: vehicle speed : not used here
         // byte 4/5: wheel count left
         // byte 6/7: wheel count right
         msg->data[4] = storage.wheel1U;
         msg->data[5] = storage.wheel1L;
         msg->data[6] = storage.wheel2U;
         msg->data[7] = storage.wheel2L;
         break;
      } /* end of case CANID_2_WHEEL_DATA */
      case CANID_2_REVERSE_GEAR:
      {
         // message is 7 bytes long
         msg->header.len = 7;
         // gear box status
         msg->data[2] = storage.gearBox;
         break;
      } /* end of case CANID_2_REVERSE_GEAR */
      default:
      {
         // do nothing!
         break;
      } /* end of default */
   } /* end of switch msgId */
}


/***************************************************************************/
/* Helpers to match bytes and bits                                         */
/***************************************************************************/

/**
 * @brief match different IGN status messages
 *
 * Direction from CAN1 to CAN2!
 *
 * @param pointer to CAN message
 */
void transferIgnStatus(can_t* msg)
{
   uint8_t status = 0;
   uint8_t byte1  = msg->data[0];

   // check key in event
   // Note: Byte2 (start status) is set to SNA (7) or normal start (1) when
   //       sending destination message.
   status |= (byte1 & IGN_1_KEY_Status);  // bit 0 - Key In Ignition (IGN_2_KeyIn)

   // check ACC status
   if(byte1 & IGN_1_ACC_Status)
   {
      status |= IGN_2_ACC_On_IGN_Off;     // bits 5-7 - IGN off and ACC on
   } /* end of if ACC */

   // check IGN start status
   if(byte1 & IGN_1_START_Status)
   {
      status |= IGN_2_IGN_Start;          // bits 5-7 - IGN start
   } /* end of if IGN start */

   // check IGN on status
   if(byte1 & IGN_1_ON)
   {
      status |= IGN_2_ON;                 // bit 5-7 - IGN on
   } /* end of if IGN on */

   // store information
   storage.ignition = status;
}

/**
 * @brief transfer wheel count values to storage
 *
 * Direction from CAN1 to CAN2
 *
 * @param pointer to CAN message
 */
void transferWheelCount(can_t* msg)
{
   // only 10 bits per wheel for count value
   storage.wheel1U = msg->data[0] & 0x3;  // FL
   storage.wheel1L = msg->data[1];
   storage.wheel2U = msg->data[2] & 0x3;  // FR
   storage.wheel2L = msg->data[3];
   storage.wheel3U = msg->data[4] & 0x3;  // RL
   storage.wheel3L = msg->data[5];
   storage.wheel4U = msg->data[6] & 0x3;  // RR
   storage.wheel4L = msg->data[7];
}

/**
 * @brief transfer gear box status (reverse gear)
 *
 * Direction from CAN1 to CAN2
 *
 * @param pointer to CAN message
 */
void transferGearStatus(can_t* msg)
{
   uint8_t status = 0;  // position P
   uint8_t byte8  = msg->data[7];

   // get information (automatic PRND)
   if(7 == byte8)
   {
      status |= 0x01;
   } /* end of if R */
   else if(6 == byte8)
   {
      status |= 0x02;
   } /* end of else if N */
   else if(8 != byte8)
   {
      status |= 0x04;
   } /* end of else if D/S/tip (not P) */
   // else status = 0

   // store information
   storage.gearBox = status;
}

