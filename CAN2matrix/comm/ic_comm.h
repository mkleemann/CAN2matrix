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
 * \file ic_comm.h
 *
 * \date Created: 03.10.2012 13:51:10
 * \author Matthias Kleemann
 *
 */


#ifndef IC_COMM_H_
#define IC_COMM_H_


/***************************************************************************/
/* DEFINITIONS                                                             */
/***************************************************************************/

/**
 * \addtogroup ic_comm_definitions Definitions for Communication with Instrument Cluster
 * \brief definitions and types for communication with instrument cluster
 * @{
 */


/**
 * \def IC_COMM_ALIGN_LEFT
 * \brief left alignment
 */
#define IC_COMM_ALIGN_LEFT       0x00

/**
 * \def IC_COMM_ALIGN_CENTER
 * \brief left alignment
 */
#define IC_COMM_ALIGN_CENTER     0x10

/**
 * \def IC_COMM_ALIGN_RIGHT
 * \brief left alignment
 */
#define IC_COMM_ALIGN_RIGHT      0x20

/***************************************************************************/
/* TYPE DEFINITIONS                                                        */
/***************************************************************************/

/**
 * \brief States of FSM
 */
typedef enum
{
   //! idle state (default)
   IC_COMM_IDLE = 0,
   //! start communication: \ref c2m_comm_ic_seq_radio
   //! starting with 4D9/2E8
   IC_COMM_START = 1,
   //! preamble: \ref c2m_comm_ic_seq_radio
   //! starting with 6B9/699 and A0/A1 data
   IC_COMM_PREAMBLE = 2,
   //! wait for cluster info, e.g. acknowledges: \ref c2m_comm_ic_seq_radio_normal
   //! Bx messages and 699: 10 23 02 01
   IC_COMM_WAIT_4_CLUSTER = 3,
   //! send information frames: \ref c2m_comm_ic_seq_radio_normal
   IC_COMM_INFO = 4,
   //! stop communication: \ref c2m_comm_ic_seq_radio_normal
   //! A8 message to stop
   IC_COMM_STOP = 5
} ic_comm_fsm_t;

/*! @} */

/***************************************************************************/
/* FUNCTION DEFINITIONS                                                    */
/***************************************************************************/

/**
 * \brief FSM for communicating with instrument cluster
 * \param msg - pointer to CAN message
 *
 * FSM needs to be called after any other CAN activity for that cycle. It
 * manipulates the message received to sent the buffer as needed.
 */
void ic_comm_fsm(can_t* msg);

/**
 * \brief reset flags for next startup
 */
void ic_comm_reset4start();

#endif /* IC_COMM_H_ */
