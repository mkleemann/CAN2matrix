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
 * \file CAN2matrix.h
 *
 * \date Created: 28.11.2011 18:13:35
 * \author Matthias Kleemann
 **/



#ifndef CAN2MATRIX_H_
#define CAN2MATRIX_H_

/***************************************************************************/
/* TYPE DEFINITIONS                                                        */
/***************************************************************************/

/**
 * @brief defines all states of the FSM
 */
typedef enum
{
   //! initialize all hardware
   INIT           = 0,
   //! do all the work
   RUNNING        = 1,
   //! prepare sleep mode (AVR and CAN)
   SLEEP_DETECTED = 2,
   //! sleeping
   SLEEPING       = 3,
   //! wake up (AVR and CAN)
   WAKEUP         = 4,
   //! an error occurred, stop working
   ERROR          = 5
} state_t;

/***************************************************************************/
/* DEFINITIONS                                                             */
/***************************************************************************/

/*** DEBUGGING *************************************************************/

// use this to cut off all CAN handling
//#define ___SIMULATION___

// use only CAN1 - read messages and put them back on bus with msgId += 10
//#define ___SINGLE_CAN___

/**
 * @brief INT0 trigger definition
 *
 * \code
 * ISC01 ISC00 Description
 *     0     0 The low level of INT0 generates an interrupt request
 *     0     1 Any logical change on INT0 generates an interrupt request
 *     1     0 The falling edge of INT0 generates an interrupt request
 *     1     1 The rising edge of INT0 generates an interrupt request
 * \endcode
 */
#define EXTERNAL_INT0_TRIGGER    0

/**
 * @brief setup for enabling the INT0 interrupt
 */
#define EXTERNAL_INT0_ENABLE     (1 << INT0)


/**
 * @brief INT1 trigger definition
 *
 * \code
 * ISC11 ISC10 Description
 *     0     0 The low level of INT1 generates an interrupt request
 *     0     1 Any logical change on INT1 generates an interrupt request
 *     1     0 The falling edge of INT1 generates an interrupt request
 *     1     1 The rising edge of INT1 generates an interrupt request
 * \endcode
 */
#define EXTERNAL_INT1_TRIGGER    (1 << ISC11)

/**
 * @brief setup for enabling the INT1 interrupt
 */
#define EXTERNAL_INT1_ENABLE     (1 << INT1)

/**
 * @brief setup AVR sleep mode: power down
 *
 * \code
 * SM2 SM1 SM0 Sleep Mode
 *   0   0   0 Idle
 *   0   0   1 ADC Noise Reduction
 *   0   1   0 Power-down
 *   0   1   1 Power-save
 *   1   1   0 Standby
 * \code
 */
#define AVR_SLEEP_MODE           (1 << SM1)
//#define AVR_SLEEP_MODE           SLEEP_MODE_PWR_DOWN

/***************************************************************************/
/* STATES OF FSM                                                           */
/***************************************************************************/

/**
 * @brief Go to sleep mode. Deactivate CAN and set the sleep mode.
 */
void sleepDetected(void);


/**
 * @brief enter AVR sleep mode
 */
void sleeping(void);

/**
 * @brief wake up CAN and reinitialize the timers
 */
void wakeUp(void);

/**
 * @brief do all the work.
 */
void run(void);

/**
 * @brief Error state. Call this when an illegal state is reached.
 */
void errorState(void);

/**
 * @brief Initialize LED, Timer and SPI.
 */
void initHardware(void);

/**
 * @brief Initialize the CAN controllers
 *
 * @return true if all is ok. Otherwise false is returned.
 */
bool initCAN(void);

/***************************************************************************/
/* HELPER ROUTINES                                                         */
/***************************************************************************/

#ifndef ___SIMULATION___
/**
 * @brief handles CAN1 reception
 * @param pointer to message struct
 */
void handleCan1Reception(can_t* msg);

/**
 * @brief handles CAN2 reception
 * @param pointer to message struct
 */
void handleCan2Reception(can_t* msg);

/**
 * @brief handle CAN1 transmission
 * @param pointer to message struct
 */
void handleCan1Transmission(can_t* msg);

/**
 * @brief handle CAN2 transmission
 * @param pointer to message struct
 */
void handleCan2Transmission(can_t* msg);

/**
 * @brief sends message to CAN2 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param pointer to CAN message
 */
void sendCan2Message(can_t* msg);
#endif

#endif /* CAN2MATRIX_H_ */