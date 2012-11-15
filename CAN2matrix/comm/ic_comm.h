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
 * \def IC_COMM_VALIGN_TOP
 * \brief vertical alignment top
 *
 * Offset of 0 pixels from top.
 *
 * \def IC_COMM_VALIGN_CENTER
 * \brief vertical center alignment
 *
 * Offset of 5 pixels from top
 *
 * \def IC_COMM_VALIGN_BOTTOM
 * \brief vertical alignment bottom
 *
 * Offset of 10 pixels from top.
 */
#define IC_COMM_VALIGN_TOP         0x00
#define IC_COMM_VALIGN_CENTER      0x05
#define IC_COMM_VALIGN_BOTTOM      0x0A

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
 * \def IC_COMM_FRAME_SIZE
 * \brief number of CAN messages for one frame
 */
#define IC_COMM_FRAME_SIZE          4

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
 * \def IC_COMM_STD_PATTERN_START_LENGTH
 * \brief sequence length of start frame
 *
 * \def IC_COMM_STD_PATTERN_START_AUDIO_LENGTH
 * \brief sequence length of audio setup frame
 *
 * \def IC_COMM_TEXT_SEQ_LENGTH
 * \brief sequence length of normal information frame
 */
#define IC_COMM_STD_PATTERN_START_LENGTH          6
#define IC_COMM_STD_PATTERN_START_AUDIO_LENGTH    11
#define IC_COMM_TEXT_SEQ_LENGTH                   17

/**
 * \def IC_COMM_STD_PATTERN_MEDIA_LENGTH
 * \brief length of standard sequence pattern "Media"
 *
 * \def IC_COMM_STD_PATTERN_SYSTEM_LENGTH
 * \brief length of standard sequence pattern "System"
 *
 * \def IC_COMM_STD_PATTERN_PDC_LENGTH
 * \brief length of standard sequence pattern "PDC"
 *
 * \def IC_COMM_STD_PATTERN_TRAFFIC_LENGTH
 * \brief length of standard sequence pattern "Traffic"
 *
 */
#define IC_COMM_STD_PATTERN_MEDIA_LENGTH     47
#define IC_COMM_STD_PATTERN_SYSTEM_LENGTH    19
#define IC_COMM_STD_PATTERN_PDC_LENGTH       61
#define IC_COMM_STD_PATTERN_TRAFFIC_LENGTH   37

/**
 * \def IC_COMM_PDC_DATA_LENGTH
 * \brief array size of PDC values
 * \sa CANID_1_PDC_STATUS
 */
#define IC_COMM_PDC_DATA_LENGTH     8

/**
 * \def IC_COMM_MAX_LENGTH_OF_ROW
 * \brief information storage buffer
 */
#define IC_COMM_MAX_LENGTH_OF_ROW   10

/***************************************************************************/
/* TYPE DEFINITIONS                                                        */
/***************************************************************************/

/**
 * \brief States of FSM
 *
 * See \ref c2m_comm_ic_state_machine for details.
 */
typedef enum
{
   //! startup stage #1
   IC_COMM_INIT_1 = 0,
   //! startup stage #2
   IC_COMM_INIT_2 = 1,
   //! idle state (default)
   IC_COMM_IDLE = 2,
   //! start communication: \ref c2m_comm_ic_seq_radio
   //! starting with 4D9/2E8
   IC_COMM_SEQ_START = 3,
   //! preamble: \ref c2m_comm_ic_seq_radio
   //! starting with 6B9/699 and A0/A1 data
   IC_COMM_SEQ_PREAMBLE = 4,
   //! wait for cluster info, e.g. acknowledges: \ref c2m_comm_ic_seq_radio_normal
   //! Bx messages and 699: 10 23 02 01
   IC_COMM_SEQ_WAIT = 5,
   //! send information frames: \ref c2m_comm_ic_seq_radio_normal
   IC_COMM_SEQ_INFO = 6,
   //! stop communication: \ref c2m_comm_ic_seq_radio_normal
   //! A8 message to stop
   IC_COMM_SEQ_END = 7
} ic_comm_fsm_t;


/**
 * \brief information type for sequence handling
 */
typedef enum
{
   //! media information: HDD
   INFO_TYPE_MEDIA_HDD = 0,
   //! media information: Radio FM
   INFO_TYPE_MEDIA_RADIO_FM = 1,
   //! media information: Radio AM
   INFO_TYPE_MEDIA_RADIO_AM = 2,
   //! media information: CD
   INFO_TYPE_MEDIA_CD = 3,
   //! media information: DVD
   INFO_TYPE_MEDIA_DVD = 4,
   //! media information: AUX
   INFO_TYPE_MEDIA_AUX = 5,
   //! information: Traffic Programme
   INFO_TYPE_TRAFFIC = 6,
   //! information: PDC
   INFO_TYPE_PDC = 7,
   //! information: Setup
   INFO_TYPE_SETUP = 8,
   //! information: two rows freetext (max. 2x10 characters)
   INFO_TYPE_FREETEXT = 9
} ic_comm_infotype_t;


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
 * \brief send CAN message to instrument cluster
 * \param msg - pointer to CAN message
 */
void ic_comm_send2Cluster(can_t* msg);

/**
 * \brief prepare next info message
 * \param data - pointer to data in CAN message
 * \return length of buffer copied
 */
uint8_t ic_comm_getNextMsg(uint8_t* data);

/**
 * \brief set PDC values
 * \param data - pointer to 8 byte array of values
 * \sa IC_COMM_PDC_DATA_LENGTH
 * \note All 8 possible values are set here, despite their availability.
 */
void ic_comm_setPDCValues(uint8_t* data);


/**
 * \brief set state machine to new start
 */
void ic_comm_restart(void);

/**
 * \brief send start of communication sequence to instrument cluster
 * \param msg - pointer to CAN message struct
 */
void ic_comm_startCommSeq(can_t* msg);

/**
 * \brief set type of information, so the pattern is set accordingly
 * \param type of information
 * \return true, if type was setup correctly
 * \note The startup pattern are set automatically at startup or restart.
 */
bool ic_comm_setType(ic_comm_infotype_t type);

/**
 * \brief setup for information in pattern requested
 */
void ic_comm_patternSetup(void);

/**
 * \brief setup text of system pattern
 * \param data - 6 character text w/o ending 0
 */
void ic_comm_setupSystemPattern(char* data);

/**
 * \brief get current state of state machine
 * \return current state
 */
ic_comm_fsm_t ic_comm_getState(void);

#endif /* IC_COMM_H_ */
