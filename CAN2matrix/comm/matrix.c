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
 * \file matrix.c
 *
 * \date Created: 03.01.2012 07:38:54
 * \author Matthias Kleemann
 **/


#include <avr/io.h>
#include <stdbool.h>
#include "matrix.h"


/***************************************************************************/
/* Definition of global variables to store CAN values.                     */
/***************************************************************************/
/**
 * @brief storage struct to keep values to send to CAN2
 */
typedef struct
{
   //! wheel signal (as-is)
   uint8_t wheel[2];
   //! engine RPM (as-is)
   uint8_t rpm[2];
   //! vehicle speed
   uint8_t speed[2];
   //! ignition key status (destination)
   uint8_t ignition;
   //! gear box status (destination)
   uint8_t gearBox;
   //! headlights status (destination) on/off
   uint8_t headlights;
   //! dimming of display (destination) 0..255
   uint8_t dimLevel;
   //! odometer value
   uint8_t odo[3];
   //! ambient temperature
   uint8_t temp;
} storeVals_t;

//! temporary storage for any values comming via CAN
volatile storeVals_t storage;
//! average of dimming values for CAN transmission
volatile uint16_t    dimAverage  = 0x7F00;
//! night mode detection flag (together with dimming)
volatile bool        nightMode   = false;


/***************************************************************************/
/* fetch/fill functions for CAN (check IDs)                                */
/***************************************************************************/

/**
 * @brief fetch information from CAN1 and put to storage
 * @param msg - CAN message to extract
 */
void fetchInfoFromCAN1(can_t* msg)
{
   switch(msg->msgId)
   {
      case CANID_1_IGNITION:
      {
         transferIgnStatus(msg);
         break;
      }

      case CANID_1_WHEEL_GEAR_DATA:
      {
         transferWheelGearTemp(msg);
         break;
      }

      case CANID_1_TIME_AND_ODO:
      {
         uint8_t i;
         uint8_t tmp = 0;
         for(i = 0; i < 3; ++i)
         {
            // store odo value and change resolution from 1.0km to 0.1km
            storage.odo[i] = (msg->data[i+1] << 1) | tmp;
            // get MSB to put into LSB of next byte
            if(i < 2)
            {
               tmp = (msg->data[i+1] & 0x80) >> 7;
            }
         }
         break;
      }

      case CANID_1_RPM_STATUS:
      {
         // 0.25 rpm to 1.0 rpm
         storage.rpm[0] = (msg->data[1] >> 2) | (msg->data[2] << 6);
         storage.rpm[1] = msg->data[2] >> 2;
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
 * @brief fetch information from CAN2 and put to storage
 * @param msg - CAN message to extract
 */
void fetchInfoFromCAN2(can_t* msg)
{
}

/**
 * @brief put information from storage to CAN1
 * @param msg - CAN message to fill
 */
void fillInfoToCAN1(can_t* msg)
{
#if 0  // currently nothing to do here
   // remove any old values
   for(int i = 0; i < 8; ++i)
   {
      msg->data[i] = 0;
   }

   switch(msg->msgId)
   {

      default:
      {
         // do nothing!
         break;
      }
   }
#endif
}

/**
 * @brief put information from storage to CAN2
 * @param msg - CAN message to fill
 *
 * \todo add values of temperature
 */
void fillInfoToCAN2(can_t* msg)
{
   // remove any old values
   for(int i = 0; i < 8; ++i)
   {
      msg->data[i] = 0;
   }

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
      }

      case CANID_2_WHEEL_DATA:
      {
         // message is 8 bytes long
         msg->header.len = 8;
         // byte 0/1: engine PRM
         msg->data[0] = storage.rpm[0];
         msg->data[1] = storage.rpm[1];
         // byte 2/3: vehicle speed
         msg->data[2] = storage.speed[0];
         msg->data[3] = storage.speed[1];
         // byte 4/5: wheel count left
         msg->data[4] = storage.wheel[0];
         msg->data[5] = storage.wheel[1];
         // byte 6/7: wheel count right
         msg->data[6] = storage.wheel[0];
         msg->data[7] = storage.wheel[1];
         break;
      }

      case CANID_2_REVERSE_GEAR:
      {
         // message is 7 bytes long
         msg->header.len = 7;
         // gear box status
         msg->data[2] = storage.gearBox;
         break;
      }

      case CANID_2_DIMMING:
      {
         // message is 3 bytes long
         msg->header.len = 3;
         // byte 1 bit 0 - day/night switch
         nightMode    = ((false == nightMode) &&
                         (storage.dimLevel < DAY_NIGHT_LOWER_LIMIT)) ||
                        ((true == nightMode) &&
                         (storage.dimLevel < DAY_NIGHT_UPPER_LIMIT));
         msg->data[0] = (nightMode) ? DIM_2_DAY_MODE : DIM_2_NIGHT_MODE;
         msg->data[1] = storage.dimLevel; // radio
         msg->data[2] = storage.dimLevel; // interior
         break;
      }

      case CANID_2_ODO_AND_TEMP:
      {
         uint8_t i;
         // message is 7 bytes long
         msg->header.len = 7;
         for(i = 0; i < 3; ++i)
         {
            msg->data[i] = storage.odo[i];
         }
         // temperature needs to be stored here
         //msg->data[4] = storage.temp;
         //msg->data[5] = storage.temp;
         break;
      }

      default:
      {
         // do nothing!
         break;
      }
   }
}


/***************************************************************************/
/* Helpers to match bytes and bits                                         */
/***************************************************************************/

/**
 * @brief match different IGN status messages
 *
 * Direction from CAN1 to CAN2!
 *
 * @param msg - pointer to CAN message
 */
void transferIgnStatus(can_t* msg)
{
   uint8_t status = 0;
   uint8_t byte1  = msg->data[0];

   // Note: Byte2 (start status) is set to SNA (7) or normal start (1) when
   //       sending destination message.

   // check Key In/ACC status
   if(byte1 & IGN_1_ACC_Status)
   {
      // bit  0   - Key In Ignition
      // bits 5-7 - IGN off and ACC on
      status = (IGN_2_ACC_On_IGN_Off | IGN_2_KeyIn);
   }

   // check IGN start status
   if(byte1 & IGN_1_START_Status)
   {
      // bit  0   - Key In Ignition
      // bits 5-7 - IGN start
      status = (IGN_2_IGN_Start | IGN_2_KeyIn);
   }

   // check IGN on status
   if(byte1 & IGN_1_ON)
   {
      // bit  0   - Key In Ignition
      // bit 5-7 - IGN on
      status = (IGN_2_ON | IGN_2_KeyIn);
   }

   // store information
   storage.ignition = status;
}

/**
 * @brief transfer wheel/reverse/temperature status to storage
 *
 * Direction from CAN1 to CAN2
 *
 * @param msg - pointer to CAN message
 *
 * \todo add definitions for destination gear box (CAN2)
 * \todo make function interrupt save
 */
void transferWheelGearTemp(can_t* msg)
{
   // store information: bit 1: 1 - reverse; 0 - not reverse (assume D(rive))
   storage.gearBox = (msg->data[0] & 0x02) ? 0x01 : 0x04;
   // store speed information: CAN1 uses approx. half of the resolution of
   // CAN2 speed signal.
   storage.speed[0] = msg->data[1] << 1;
   storage.speed[1] = msg->data[2] << 1 | ((msg->data[0] & 0x80) >> 7);
   // only 10 bits per wheel for count value
   storage.wheel[0] = msg->data[3];
   storage.wheel[1] = msg->data[4] & 0x3;
   // store temperature too
   storage.temp = msg->data[5];
}


/***************************************************************************/
/* Helpers to be called by main routine                                    */
/***************************************************************************/

/**
 * @brief send CAN1 message every 100ms
 * @param msg - pointer to message struct
 */
void sendCan1_100ms(can_t* msg)
{
}

/**
 * @brief send CAN1 message every 500ms
 * @param msg - pointer to message struct
 */
void sendCan1_500ms(can_t* msg)
{
}

/**
 * @brief send CAN1 message every 1000ms
 * @param msg - pointer to message struct
 */
void sendCan1_1000ms(can_t* msg)
{
}

/**
 * @brief sends message to CAN1 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param msg - pointer to CAN message with set msg id
 */
void sendCan1Message(can_t* msg)
{
   fillInfoToCAN1(msg);
   can_send_message(CAN_CHIP1, msg);
}


/**
 * @brief send CAN2 message every 100ms
 * @param msg - pointer to message struct
 */
void sendCan2_100ms(can_t* msg)
{
   msg->msgId = CANID_2_IGNITION;
   sendCan2Message(msg);

   msg->msgId = CANID_2_WHEEL_DATA;  // should be 50ms, but keep it
   sendCan2Message(msg);

   msg->msgId = CANID_2_ODO_AND_TEMP;
   sendCan2Message(msg);
}

/**
 * @brief send CAN2 message every 500ms
 * @param msg - pointer to message struct
 */
void sendCan2_500ms(can_t* msg)
{
   msg->msgId = CANID_2_DIMMING;
   sendCan2Message(msg);

   msg->msgId = CANID_2_REVERSE_GEAR;
   sendCan2Message(msg);
}

/**
 * @brief send CAN2 message every 1000ms
 * @param msg - pointer to message struct
 */
void sendCan2_1000ms(can_t* msg)
{
}

/**
 * @brief sends message to CAN2 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param msg - pointer to CAN message with set msg id
 */
void sendCan2Message(can_t* msg)
{
   fillInfoToCAN2(msg);

   can_send_message(CAN_CHIP2, msg);
}

/**
 * @brief gets a dim value to be sent via CAN
 * @param value - dim value 0..65535 (left aligned from ADC)
 *
 * This function uses an integral to get an averaged value to set
 * the dimlevel of the target unit (CAN). Since nobody wants to have
 * a too fast changing value - also for detecting darkness and switch
 * to night mode - this needs some thought. The formula to use is:
 * \code
 *    dimAverage = value/DIM_STEPS_2_AVERAGE + dimAverage - dimAverage/DIM_STEPS_2_AVERAGE;
 * \endcode
 *
 */
void setDimValue(uint16_t value)
{
   // integral for averaging dim values
   dimAverage = value/DIM_STEPS_2_AVERAGE + dimAverage - dimAverage/DIM_STEPS_2_AVERAGE;

   // set dim level for upper 8bit of 10bit left aligned average value
   //
   // | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | x | x | x | x | x | x |
   // | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | x | x | x | x | x | x |
   // |          upper byte           |          lower byte           |
   // |                 value                 |       not used        |
   // |        dimming average        |           discarded           |
   storage.dimLevel = dimAverage >> 8;
}

