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
 * \file comm_matrix.h
 *
 * \date Created: 28.11.2011 18:25:36
 * \author Matthias Kleemann
 *
 * \todo verify CAN signals in car
**/

#ifndef MATRIX_H_
#define MATRIX_H_



/***************************************************************************/
/* DEFINITIONS                                                             */
/***************************************************************************/

/**
 * \addtogroup matrix_common_definitions Definitions for The Matrix
 * \brief Definitions for The Matrix to use
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

//! timeout counter for PDC mode (value * 50msec)
#define PDC_TIMEOUT_COUNT              200

/*! @} */



/***************************************************************************/
/* functions for matrix operations                                         */
/***************************************************************************/

/**
 * \brief fetch information from CAN1 and put to storage
 * \param msg - CAN message to extract
 */
void fetchInfoFromCAN1(can_t* msg);

/**
 * \brief fetch information from CAN2 and put to storage
 * \param msg - CAN message to extract
 */
void fetchInfoFromCAN2(can_t* msg);

/**
 * \brief put information from storage to CAN1
 * \param msg - CAN message to fill
 */
void fillInfoToCAN1(can_t* msg);

/**
 * \brief put information from storage to CAN2
 * \param msg - CAN message to fill
 */
void fillInfoToCAN2(can_t* msg);


/***************************************************************************/
/* Helpers to match bytes and bits                                         */
/***************************************************************************/

/**
 * \brief match different IGN status messages
 *
 * Direction from CAN1 to CAN2!
 *
 * \param msg - pointer to CAN message
 */
void transferIgnStatus(can_t* msg);

/**
 * \brief transfer wheel/reverse/temperature status to storage
 *
 * Direction from CAN1 to CAN2
 *
 * \param msg - pointer to CAN message
 *
 * \todo add definitions for destination gear box (CAN2)
 */
void transferWheelGearTemp(can_t* msg);


/***************************************************************************/
/* Helpers to be called by main routine                                    */
/***************************************************************************/

/**
 * \brief send CAN1 message every 100ms
 * \param msg - pointer to message struct
 */
void sendCan1_100ms(can_t* msg);

/**
 * \brief send CAN1 message every 500ms
 * \param msg - pointer to message struct
 */
void sendCan1_500ms(can_t* msg);

/**
 * \brief send CAN1 message every 1000ms
 * \param msg - pointer to message struct
 */
void sendCan1_1000ms(can_t* msg);

/**
 * \brief sends message to CAN1 and filling up converted data
 *
 * \note Set message id before calling this function.
 *
 * \param msg - pointer to CAN message with set msg id
 */
void sendCan1Message(can_t* msg);

/**
 * \brief send CAN2 message every 100ms
 * \param msg - pointer to message struct
 */
void sendCan2_100ms(can_t* msg);

/**
 * \brief send CAN2 message every 500ms
 * \param msg - pointer to message struct
 */
void sendCan2_500ms(can_t* msg);

/**
 * \brief send CAN2 message every 1000ms
 * \param msg - pointer to message struct
 */
void sendCan2_1000ms(can_t* msg);

/**
 * \brief sends message to CAN2 and filling up converted data
 *
 * \note Set message id before calling this function.
 *
 * \param msg - pointer to CAN message with set msg id
 */
void sendCan2Message(can_t* msg);

/**
 * \brief gets a dim value to be sent via CAN
 * \param value - dim value 0..65535 (left aligned from ADC)
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

/**
 * \brief trigger for any IC communication, timed update
 */
void tick4ICComm(void);

/**
 * \brief set instrument cluster communication to an end
 */
void stopICComm(void);

/**
 * \brief set instrument cluster to start sequence
 */
void restartICComm(void);


#endif /* MATRIX_H_ */

