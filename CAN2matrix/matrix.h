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
 * \file matrix.h
 *
 * \date Created: 28.11.2011 18:25:36
 * \author Matthias Kleemann
 *
 * \todo verify CAN signals in car
**/




#ifndef MATRIX_H_
#define MATRIX_H_

#include "can/can_mcp2515.h"


/***************************************************************************/
/* Definition of car CAN message ids                                       */
/***************************************************************************/

/**
 * \addtogroup matrix_can_ids_master CAN ids of master (CAN #1)
 * \brief all CAN ids for the first CAN bus used
 *
 * \note Currently a mix of infotainment and convenience CAN bus signals.
 *
 * @{
 */


//! ignition by key status
//! another id could be 0x2C3 - verify
#define CANID_1_IGNITION               0x271

//! wheel count values for navigation
//! also via gateway signal 0x351 (old!)
#define CANID_1_WHEEL_DATA             0x455

//! gear box status via gateway signal
#define CANID_1_REVERSE_GEAR           0x351

//! status of lights
//! to be verified
#define CANID_1_LIGHT_STATUS           0x531

//! automatic and manual dimming
#define CANID_1_DIMMING                0x635

//! also a gateway signal
#define CANID_1_LANGUAGE_AND_UNIT      0x653

//! park distance control values
//! to be verified
#define CANID_1_PDC_STATUS             0x54B

//! time and odometer
//! to be verified, but may also available via cluster
#define CANID_1_TIME_AND_ODO           0x65D

//! vehicle identification number
//! to be verified
#define CANID_1_VIN                    0x65F

//! steering wheel angle
//! to be verified
#define CANID_1_STEERING_WHEEL         0x3C3

//! switch status, e.g. lights, horn, wipers, BC, ...
//! to be verified
#define CANID_1_TURN_SIGNAL_CTRL       0x2C1

//! cruise control status
//! to be verified
#define CANID_1_CRUISE_CONTROL         0x289

//! instrument cluster status, e.g. warning lights
//! to be verified
#define CANID_1_COM_CLUSTER_STATUS1    0x621

//! instrument cluster status, e.g. date/time
//! to be verified
#define CANID_1_COM_CLUSTER_STATUS2    0x623

//! multi function display status (sent by display!)
//! to be verified
#define CANID_1_COM_DISPLAY_STATUS     0x62F

//! start signal for display communication
//! to be verified
#define CANID_1_COM_DISP_START         0x2E8

//! communication channels
//! to be verified
#define CANID_1_COM_RADIO_CLUSTER1     0x680
//! communication channels
//! to be verified
#define CANID_1_COM_RADIO_CLUSTER2     0x681
//! communication channels
//! to be verified
#define CANID_1_COM_NAVI_CLUSTER1      0x682
//! communication channels
//! to be verified
#define CANID_1_COM_NAVI_CLUSTER2      0x683
//! communication channels
//! to be verified
#define CANID_1_COM_PHONE_CLUSTER      0x685
//! communication channels
//! to be verified
#define CANID_1_COM_COMPASS_CLUSTER    0x689

//! radio status signal, e.g. on/off, locked, ...
//! to be verified
#define CANID_1_RADIO_STATUS           0x661

//! navigation status, e.g. wake up, bus sleep, ...
//! to be verified
#define CANID_1_NAVI_STATUS            0x436

/*! @} */


/***************************************************************************/
/* Definition of radio CAN message ids                                     */
/***************************************************************************/


/**
 * \addtogroup matrix_can_ids_slave CAN ids of slave (CAN #2)
 * \brief all CAN ids for the second CAN bus used
 * @{
 */

/**
 * \def CANID_2_IGNITION
 * \brief ignition and key status
 */
#define CANID_2_IGNITION               0x20B

/**
 * \def CANID_2_WHEEL_DATA
 * \brief wheel pulse counter
 */
#define CANID_2_WHEEL_DATA             0x211

/**
 * \def CANID_2_REVERSE_GEAR
 * \brief gear box status, e.g. for rear view camera and navigation
 */
#define CANID_2_REVERSE_GEAR           0x20E

/**
 * \def CANID_2_LIGHT_STATUS
 * \brief light status and dimming for keys
 */
#define CANID_2_LIGHT_STATUS           0x309

/**
 * \def CANID_2_DIMMING
 * \brief CAN id: dimming for main display
 *
 * Message contains 3 bytes:
 * \code
 * byte  bit(s)   value          range             step
 *    0  0        day/night
 *    1  0..7     display level  0..200 (0..100%)  0.5% steps
 *    2  0..7     interior level 0..200 (0..100%)  0.5% steps
 * \endcode
 *
 */
#define CANID_2_DIMMING                0x308

/**
 * \def CANID_2_LANGUAGE_AND_UNIT
 * \brief switches units and language
 */
#define CANID_2_LANGUAGE_AND_UNIT      0x2B0

/**
 * \def CANID_2_CONFIG_STATUS2
 * \brief config status 2 - rear view camera present
 */
#define CANID_2_CONFIG_STATUS2         0x21E

/**
 * \def CANID_2_NAVI_TURN_BY_TURN
 * \brief navigation turn-by-turn information for instrument cluster
 */
#define CANID_2_NAVI_TURN_BY_TURN      0x2D4

/**
 * \def CANID_2_RADIO_STATION_NAME
 * \brief radio station name
 */
#define CANID_2_RADIO_STATION_NAME     0x295

/*! @} */

/***************************************************************************/
/* Bit Definitions                                                         */
/***************************************************************************/

/**
 * \addtogroup matrix_bit_defs_can_1 Bit Definitions of CAN #1
 * \brief bit definitions for CAN signals used on CAN #1
 * @{
 */

//! Ignition CAN1
//! clamp S
#define IGN_1_CL_S                     0
//! clamp 15
#define IGN_1_CL_15                    1
//! clamp X
#define IGN_1_CL_X                     2
//! clamp 50
#define IGN_1_CL_50                    3
//! clamp P
#define IGN_1_CL_P                     4
//! clamp 15 SV
#define IGN_1_CL_15SV                  6
//! clamp L
#define IGN_1_CL_L                     7

//! combinations for CAN1
//! key status (in/out)
#define IGN_1_KEY_Status       (1 << IGN_1_CL_S)
//! switch ACC
#define IGN_1_ACC_Status      ((1 << IGN_1_CL_L) | (1 << IGN_1_CL_15SV))
//! switch to start engine
#define IGN_1_START_Status    ((1 << IGN_1_CL_50) | (1 << IGN_1_CL_X))
//! ignition on status
#define IGN_1_ON               (1 << IGN_1_CL_15)

/*! @} */


/**
 * \addtogroup matrix_bit_defs_can_2 Bit Definitions of CAN #2
 * \brief bit definitions for CAN signals used on CAN #2
 * @{
 */

//! Ignition CAN2
//! key status (in/out)
#define IGN_2_KeyIn                    0
//! ignition status 1
#define IGN_2_Stat1                    5
//! ignition status 2
#define IGN_2_Stat2                    6
//! ignition status 3
#define IGN_2_Stat3                    7

//! combinations for CAN2
//! ACC on and ignition off
#define IGN_2_ACC_On_IGN_Off  ((1 << IGN_2_Stat2) | (1 << IGN_2_Stat1))
//! ignition set to: start engine
#define IGN_2_IGN_Start       ((1 << IGN_2_Stat3) | (1 << IGN_2_Stat2))
//! ignition on
#define IGN_2_ON               (1 << IGN_2_Stat3)

//! dimming for CAN2
#define DIM_2_DAY_NIGHT                0
//! use bit definitions for setup in message
//! day mode setup
#define DIM_2_DAY_MODE                 0
//! night mode setup
#define DIM_2_NIGHT_MODE       (1 << DIM_2_DAY_NIGHT)

/*! @} */


/***************************************************************************/
/* DEFINITIONS                                                             */
/***************************************************************************/

/**
 * \addtogroup matrix_common_definitions Definitions for The Matrix
 * \brief Definitions for The Matrix to use for calculating CAN values
 * @{
 */

//! when to switch from day to night mode and vice versa (hysteresis)
//! upper limit
#define DAY_NIGHT_UPPER_LIMIT          0x60
//! when to switch from day to night mode and vice versa (hysteresis)
//! lower limit
#define DAY_NIGHT_LOWER_LIMIT          0x40

//! averaging interval (steps) for dim value
#define DIM_STEPS_2_AVERAGE            16

/*! @} */

/***************************************************************************/
/* functions for matrix operations                                         */
/***************************************************************************/

/**
 * @brief fetch information from CAN1 and put to storage
 * @param msg - CAN message to extract
 */
void fetchInfoFromCAN1(can_t* msg);

/**
 * @brief fetch information from CAN2 and put to storage
 * @param msg - CAN message to extract
 */
void fetchInfoFromCAN2(can_t* msg);

/**
 * @brief put information from storage to CAN1
 * @param msg - CAN message to fill
 */
void fillInfoToCAN1(can_t* msg);

/**
 * @brief put information from storage to CAN2
 * @param msg - CAN message to fill
 */
void fillInfoToCAN2(can_t* msg);


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
void transferIgnStatus(can_t* msg);

/**
 * @brief transfer wheel count values to storage
 *
 * Direction from CAN1 to CAN2
 *
 * @param msg - pointer to CAN message
 */
void transferWheelCount(can_t* msg);

/**
 * @brief transfer gear box status (reverse gear)
 *
 * Direction from CAN1 to CAN2
 *
 * @param msg - pointer to CAN message
 */
void transferGearStatus(can_t* msg);

/***************************************************************************/
/* Helpers to be called by main routine                                    */
/***************************************************************************/

/**
 * @brief send CAN1 message every 100ms
 * @param msg - pointer to message struct
 */
void sendCan1_100ms(can_t* msg);

/**
 * @brief send CAN1 message every 500ms
 * @param msg - pointer to message struct
 */
void sendCan1_500ms(can_t* msg);

/**
 * @brief send CAN1 message every 1000ms
 * @param msg - pointer to message struct
 */
void sendCan1_1000ms(can_t* msg);

/**
 * @brief sends message to CAN1 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param msg - pointer to CAN message with set msg id
 */
void sendCan1Message(can_t* msg);

/**
 * @brief send CAN2 message every 100ms
 * @param msg - pointer to message struct
 */
void sendCan2_100ms(can_t* msg);

/**
 * @brief send CAN2 message every 500ms
 * @param msg - pointer to message struct
 */
void sendCan2_500ms(can_t* msg);

/**
 * @brief send CAN2 message every 1000ms
 * @param msg - pointer to message struct
 */
void sendCan2_1000ms(can_t* msg);

/**
 * @brief sends message to CAN2 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param msg - pointer to CAN message with set msg id
 */
void sendCan2Message(can_t* msg);

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
void setDimValue(uint16_t value);

#endif /* MATRIX_H_ */

