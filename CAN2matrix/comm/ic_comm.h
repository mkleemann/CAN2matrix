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
 * \def IC_COMM_ALIGN_V_TOP
 * \brief vertical alignment top
 *
 * Offset of 0 pixels from top.
 *
 * \def IC_COMM_ALIGN_V_CENTER
 * \brief vertical center alignment
 *
 * Offset of 5 pixels from top
 *
 * \def IC_COMM_ALIGN_V_BOTTOM
 * \brief vertical alignment bottom
 *
 * Offset of 10 pixels from top.
 */
#define IC_COMM_ALIGN_V_TOP         0x00
#define IC_COMM_ALIGN_V_CENTER      0x05
#define IC_COMM_ALIGN_V_BOTTOM      0x0A

/**
 * \def IC_COMM_ALIGN_H_TAB_WIDTH
 * \brief horizontal tab width
 *
 * The tab or character width consists of 5 pixels of character width and
 * one pixel for the gap between characters. This means "wide" characters.
 * Some of them are written in only 3 pixels width.
 */
#define IC_COMM_ALIGN_H_TAB_WIDTH   6

/**
 * \def IC_COMM_POS_OFFSET
 * \brief most left position to start text (10 characters)
 *
 * Display area is 20x64px, so starting at position 2 leaves 60 pixels for the
 * character to show in and leave a 2px border on the right side. Any
 * character is assumed 6px in width, including the spacing.
 */
#define IC_COMM_POS_OFFSET          2

/**
 * \def IC_COMM_SOF
 * \brief start of frame
 *
 * \def IC_COMM_EOF
 * \brief end of frame
 *
 * \def IC_COMM_W4NF
 * \brief wait for next frame
 */
#define IC_COMM_SOF                 0x20
#define IC_COMM_EOF                 0x10
#define IC_COMM_W4NF                0x00

/**
 * \def IC_COMM_FRAME_MASK
 * \brief mask for frame message signature
 *
 * \def IC_COMM_EOF_MASK
 * \brief mask for frame message signature (1x/0x)
 *
 * Test
 * \code
 * if(0 == (byte & IC_COMM_EOF_MASK))
 * { ... }
 * \endcode
 * and the value is either 0x or 1x.
 *
 * \def IC_COMM_FRAME_SEQ_MASK
 * \brief mask for sequence number
 *
 * Sequence number is:
 * \code
 * (byte | IC_COMM_FRAME_SEQ_MASK)
 * \endcode
 *
 */
#define IC_COMM_FRAME_MASK          0xF0
#define IC_COMM_EOF_MASK            0xE0
#define IC_COMM_FRAME_SEQ_MASK      0x0F

/**
 * \def IC_COMM_SET_REMOVE_OLD_TEXT
 * \brief remove old text information prior writing new one
 */
#define IC_COMM_SET_REMOVE_OLD_TEXT 0x20

/**
 * \def IC_COMM_MAX_LENGTH_OF_FRAME
 * \brief length of messages for 2 communication frames
 *
 * A communication frame for the instrument cluster consists of 4 CAN
 * messages maximum. The buffer fits for 2 frames.
 */
#define IC_COMM_MAX_LENGTH_OF_FRAME 64

/**
 * \def IC_COMM_START_SEQ_LENGTH
 * \brief sequence length of start frame
 *
 * \def IC_COMM_AUDIO_SEQ_LENGTH
 * \brief sequence length of audio setup frame
 *
 * \def IC_COMM_TEXT_SEQ_LENGTH
 * \brief sequence length of normal information frame
 */
#define IC_COMM_START_SEQ_LENGTH    6
#define IC_COMM_AUDIO_SEQ_LENGTH    11
#define IC_COMM_TEXT_SEQ_LENGTH     48

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

/**
 * \brief enumeration of sequence types for radio communication
 *
 * For easy access to the sequence in the eeprom.
 */
typedef enum {
   //! radio startup sequence
   IC_COMM_SEQ_START = 0,
   //! audio startup sequence
   IC_COMM_SEQ_AUDIO = 1,
   //! media information sequence
   IC_COMM_SEQ_MEDIA = 2,
   //! media information with traffic programme sequence
   IC_COMM_SEQ_MEDIA_TP = 3,
   //! traffic programme information sequence
   IC_COMM_SEQ_TRAFFIC = 4,
   //! 1st line only, 8 bytes, left aligned
   IC_COMM_SEQ_FIRST = 5,
   //! 2nd line only, 8 bytes, left aligned
   IC_COMM_SEQ_SECOND = 6,
   //! 2nd line with traffic programme sequence
   IC_COMM_SEQ_SECOND_TP = 7,
   //! max number of sequences
   IC_COMM_SEQ_MAX = 8
} ic_comm_seq_t;


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

/**
 * \brief setup for frame to send to cluster
 *
 * Fill frame buffer with control and information sequences.
 */
void ic_comm_framesetup(void);

/**
 * \brief get next message from frame buffer
 * \param data - pointer to destination buffer
 * \return length of buffer copied
 */
uint8_t ic_comm_getNextMsg(uint8_t* data);

#endif /* IC_COMM_H_ */
