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
 * \file comm_can_ids.h
 *
 * \date Created: 18.10.2012 06:09:15
 * \author Matthias Kleemann
 *
 */

#ifndef COMM_CAN_IDS_H_
#define COMM_CAN_IDS_H_

/***************************************************************************/
/* Definition of car CAN message ids                                       */
/***************************************************************************/

/**
 * \addtogroup matrix_can_ids_master CAN IDs of Master (CAN #1)
 * \brief all CAN ids for the first CAN bus used
 *
 * All verified CAN IDs are part of the communication on the entertainment
 * CAN bus. Convenience or motor CAN signals are not part here.
 *
 * @{
 */


/**
 * \def CANID_1_IGNITION
 * \brief ignition by key status
 *
 * \code
 * byte 0: IGN Status
 *  bit 0: ACC/key in - on 1; off 0
 *  bit 1: terminal 15   - ignition on 1; off 0
 *  bit 2: terminal X    - consumers relevant for start are switched off
 *  bit 3: terminal 50   - starter on 1; off 0
 *  bit 4: not used
 *  bit 5: not used
 *  bit 6: not used
 *  bit 7: terminal ER   - engine runs 1; turned off 0
 *
 * byte 1: Diagnosis Error Status
 *  bit 0..6: not used
 *  bit    7: at least one error entry is available
 * \endcode
 */
#define CANID_1_IGNITION               0x271

/**
 * \def CANID_1_WHEEL_GEAR_DATA
 * \brief wheel count and speed (gateway)
 *
 * \code
 * byte 0:
 *  bit 1: reverse gear/reverse lights on
 *
 * byte 1..2:
 *  bit 0    : signal source 1 - ABS; 0 - Speedometer
 *  bit 1..15: speed
 *
 * example:
 *    "xx 01 00 xx xx xx xx" =  0.00 km/h
 *    "xx DB 01 xx xx xx xx" =  2.37 km/h
 *    "xx 71 17 xx xx xx xx" = 30.00 km/h
 *
 * byte 3..4:
 *  bit 0..10: wheel impulse count
 *  bit 11   : at least one overrun (1) or reset/no overrun (0)
 *  bit 12   : ABS active
 *  bit 13   : speed instead of wheel impulse count (defect)
 *
 * byte 5:
 *  bit 0..7 : temperature (outside) in 0.5�C
 *             -50..77�C (1..254)
 *             0x00 - not available
 *             0xFF - error
 *
 * byte 6:
 *  bit 0..7 : temperature (outside) in 0.5�C
 *             -50..77�C (1..254)
 *             0x00 - not available
 *             0xFF - error
 * \endcode
 *
 * \todo verify gear box status values PRND/tip
 */
#define CANID_1_WHEEL_GEAR_DATA        0x351

/**
 * \def CANID_1_RPM_STATUS
 * \brief Engine RPM (gateway)
 *
 * \code
 * byte 1..2: engine RPM in 0.25 rpm
 * byte 3   : engine temperature
 * byte 5   : engine fan status in %
 * \endcode
 */
#define CANID_1_RPM_STATUS             0x353

/**
 * \def CANID_1_LANGUAGE_AND_UNIT
 * \brief also a gateway signal
 */
#define CANID_1_LANGUAGE_AND_UNIT      0x653

/**
 * \def CANID_1_PDC_STATUS
 * \brief park distance control values
 *
 * \code
 * 0xFF      : no object in range
 * 0xFE      : sensor not available
 * 0x00..0xFD: distance in cm.
 *
 * byte 0: front left
 * byte 1: front right
 * byte 2: rear left
 * byte 3: rear right
 * byte 4: front mid left
 * byte 5: front mid right
 * byte 6: rear mid left
 * byte 7: rear mid right
 * \endcode
 */
#define CANID_1_PDC_STATUS             0x54B

/**
 * \def CANID_1_TIME_AND_ODO
 * \brief time and odometer
 *
 * byte 1..3 - Odometer: The values are sent in reverse order, meaning that
 * the byte 3 is the highest byte and byte 1 the lowest. A value of 125302km
 * (hex 01E976) translates to a byte sequence of "xx 76 E9 01 xx xx xx xx"
 * in the message.
 *
 *
 * byte 4..7 - Time
 *  byte 7: seconds (2x byte 7 + bit 7 of byte 6)
 *  byte 6: minutes (bit 0..6 >> 1)
 * \todo decode time information
 */
#define CANID_1_TIME_AND_ODO           0x65D

/**
 * \def CANID_1_COM_DISP_START
 * \brief start signal for central display communication (Instrument Cluster)
 */
#define CANID_1_COM_DISP_START         0x2E8

/**
 * \def CANID_1_COM_RADIO_START
 * \brief start signal for central display communication (Radio)
 */
#define CANID_1_COM_RADIO_START        0x4D9

/**
 * \def CANID_1_COM_RADIO_2_CLUSTER
 * \brief communication channel from Radio to Instrument Cluster
 */
#define CANID_1_COM_RADIO_2_CLUSTER    0x6B9

/**
 * \def CANID_1_COM_CLUSTER_2_RADIO
 * \brief communication channel from Instrument Cluster to Radio
 */
#define CANID_1_COM_CLUSTER_2_RADIO    0x699

/*! @} */


/***************************************************************************/
/* Definition of radio CAN message ids                                     */
/***************************************************************************/


/**
 * \addtogroup matrix_can_ids_slave CAN IDs of Slave (CAN #2)
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
 * \def CANID_2_VEH_CONFIG
 * \brief vehicle configuration, e.g. brand
 */
#define CANID_2_VEH_CONFIG             0x2D3

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
 * \def CANID_2_MEDIA_STATUS
 * \brief current status of media present
 *
 *- media type
 *- media mode
 */
#define CANID_2_MEDIA_STATUS           0x291

/**
 * \def CANID_2_RADIO_STATUS
 * \brief current status of head unit
 *
 *- head unit mode
 *- track number
 *- track time
 */
#define CANID_2_RADIO_STATUS           0x293

/**
 * \def CANID_2_MEDIA_INFO_DATA
 * \brief media info text
 *
 *- byte0: bit 0..2 type of message
 *- byte0: bit 3    start of sequence
 *- byte0: bit 4..7 sequence counter
 *
 *- byte1: group number
 *
 *- byte 2..7: text information (zero ended)
 */
#define CANID_2_MEDIA_INFO_DATA        0x294

/**
 * \def CANID_2_RADIO_STATION_NAME
 * \brief short radio station name
 */
#define CANID_2_RADIO_STATION_NAME     0x295

/**
 * \def CANID_2_ODO_AND_TEMP
 * \brief Odometer and ambient temperature
 *
 * \code
 * byte 0..3: odometer in 0.1km
 * byte 5..6: average ambient temperature in 0.01�C (-40�C..85�C - 0..12500)
 * \endcode
 */
#define CANID_2_ODO_AND_TEMP           0x214

/*! @} */

/***************************************************************************/
/* Bit Definitions                                                         */
/***************************************************************************/

/**
 * \addtogroup matrix_bit_defs_can_1 Bit Definitions of CAN #1
 * \brief bit definitions for CAN signals used on CAN #1
 *
 * Clamps/contact identifier according to German guidelines in Automotive
 * Industry. See: http://de.wikipedia.org/wiki/Klemmenbezeichnung
 * @{
 */

/**
 * \def IGN_1_CL_ACC
 * \brief terminal "ACC" - key in/ACC
 *
 * \def IGN_1_CL_15
 * \brief terminal 15 - ignition on
 *
 * \def IGN_1_CL_X
 * \brief terminal X - consumers not relevant for engine start are switched off
 *
 * \def IGN_1_CL_50
 * \brief terminal 50 - engine start
 *
 * \def IGN_1_CL_ER
 * \brief terminal "ER" - engine runs
 */
#define IGN_1_CL_ACC                   0
#define IGN_1_CL_15                    1
#define IGN_1_CL_X                     2
#define IGN_1_CL_50                    3
#define IGN_1_CL_ER                    7

/**
 * \def IGN_1_ACC_Status
 * \brief switched ACC and key in
 *
 * \def IGN_1_START_Status
 * \brief switch to start engine
 *
 * \def IGN_1_ON
 * \brief ignition on status
 */
#define IGN_1_ACC_Status       (1 << IGN_1_CL_ACC)
#define IGN_1_START_Status    ((1 << IGN_1_CL_50) | (1 << IGN_1_CL_X))
#define IGN_1_ON               (1 << IGN_1_CL_15)

/*! @} */


/**
 * \addtogroup matrix_bit_defs_can_2 Bit Definitions of CAN #2
 * \brief bit definitions for CAN signals used on CAN #2
 * @{
 */

/**
 * \def IGN_2_KeyIn
 * \brief key status (in/out)
 *
 * \def IGN_2_Stat1
 * \brief ignition status 1
 *
 * \def IGN_2_Stat2
 * \brief ignition status 2
 *
 * \def IGN_2_Stat3
 * \brief ignition status 3
 */
#define IGN_2_KeyIn                    0
#define IGN_2_Stat1                    5
#define IGN_2_Stat2                    6
#define IGN_2_Stat3                    7

/**
 * \def IGN_2_ACC_On_IGN_Off
 * \brief ACC on and ignition off
 *
 * \def IGN_2_IGN_Start
 * \brief ignition set to: start engine
 *
 * \def IGN_2_ON
 * \brief ignition on
 */
#define IGN_2_ACC_On_IGN_Off  ((1 << IGN_2_Stat2) | (1 << IGN_2_Stat1))
#define IGN_2_IGN_Start       ((1 << IGN_2_Stat3) | (1 << IGN_2_Stat2))
#define IGN_2_ON               (1 << IGN_2_Stat3)

/**
 * \def DIM_2_DAY_NIGHT
 * \brief bit definition for day/night mod ein CAN message
 *
 * \def DIM_2_DAY_MODE
 * \brief value for day mode
 *
 * \def DIM_2_NIGHT_MODE
 * \brief value for night mode
 *
 */
#define DIM_2_DAY_NIGHT                0
#define DIM_2_DAY_MODE                 0
#define DIM_2_NIGHT_MODE               (1 << DIM_2_DAY_NIGHT)

/**
 * \def LANG_GERMAN_CAN2
 * \brief Language set German (CAN #2)
 *
 * \def LANG_ENGLISH_US_CAN2
 * \brief Language set US-English (CAN #2)
 *
 * \def LANG_FRENCH_CAN2
 * \brief Language set French (CAN #2)
 *
 * \def LANG_ITALIAN_CAN2
 * \brief Language set Italian (CAN #2)
 *
 * \def LANG_SPANISH_CAN2
 * \brief Language set Spanish (CAN #2)
 *
 * \def LANG_PORTUGESE_CAN2
 * \brief Language set Portugese (CAN #2)
 *
 * \def LANG_DUTCH_CAN2
 * \brief Language set Dutch (CAN #2)
 *
 * \def LANG_ENGLISH_UK_CAN2
 * \brief Language set UK-English (CAN #2)
 *
 * \def LANG_NO_CHANGE_CAN2
 * \brief Language did not change (CAN #2)
 */
#define LANG_GERMAN_CAN2               0
#define LANG_ENGLISH_US_CAN2           1
#define LANG_FRENCH_CAN2               2
#define LANG_ITALIAN_CAN2              3
#define LANG_SPANISH_CAN2              4
#define LANG_PORTUGESE_CAN2            6
#define LANG_DUTCH_CAN2                7
#define LANG_ENGLISH_UK_CAN2           8
#define LANG_NO_CHANGE_CAN2            15

/**
 * \def CONFIG_STATUS_NOT_PROGRAMMED
 * \brief config status not taken over by unit
 *
 * \def CONFIG_STATUS_PROGRAMMED
 * \brief config status taken over by unit
 *
 * \def CONFIG_STATUS_REQUEST
 * \brief config status request
 *
 * \def CONFIG_STATUS_SNA
 * \brief config status unavailable
 */
#define CONFIG_STATUS_NOT_PROGRAMMED   0
#define CONFIG_STATUS_PROGRAMMED       1
#define CONFIG_STATUS_REQUEST          2
#define CONFIG_STATUS_SNA              3

/**
 * \def VEH_BRAND_CHRYSLER
 * \brief vehicle is a Chrysler
 *
 * \def VEH_BRAND_DODGE
 * \brief vehicle is a Dodge
 *
 * \def VEH_BRAND_JEEP
 * \brief vehicle is a Jeep
 *
 * \def VEH_BRAND_VW
 * \brief vehicle is a Volkswagen
 *
 * \def VEH_BRAND_SNA
 * \brief signal not available
 */
#define VEH_BRAND_CHRYSLER             1
#define VEH_BRAND_DODGE                2
#define VEH_BRAND_JEEP                 3
#define VEH_BRAND_VW                   6
#define VEH_BRAND_SNA                  15

/**
 * \def MEDIA_TEXT_SEQUENCE_COUNT_MASK
 * \brief media info message sequence counter mask
 *
 * \def MEDIA_TEXT_SEQUENCE_START_FLAG
 * \brief start of media info group
 *
 * \def MEDIA_TEXT_MESSAGE_TYPE_MASK
 * \brief media message type mask
 */
#define MEDIA_TEXT_SEQUENCE_COUNT_MASK 0xF0
#define MEDIA_TEXT_SEQUENCE_START_FLAG 0x04
#define MEDIA_TEXT_MESSAGE_TYPE_MASK   0x07

/**
 * \def MEDIA_SOURCE_RADIO_AM
 * \brief source: AM radio
 *
 * \def MEDIA_SOURCE_RADIO_FM
 * \brief source: FM radio
 *
 * \def MEDIA_SOURCE_RADIO_MW
 * \brief source: MW radio
 *
 * \def MEDIA_SOURCE_RADIO_LW
 * \brief source: LW radio
 *
 * \def MEDIA_SOURCE_CD_DVD
 * \brief source: internal disc (CD/DVD)
 *
 * \def MEDIA_SOURCE_HDD
 * \brief source: internal HDD
 *
 * \def MEDIA_SOURCE_AUX
 * \brief source: auxiliary port
 *
 * \def MEDIA_SOURCE_BLUETOOTH_AUDIO
 * \brief source: bluetooth audio
 *
 * \def MEDIA_SOURCE_RADIO_OFF
 * \brief radio switched off
 *
 * \def MEDIA_SOURCE_RADIO_LOCKED
 * \brief radio locked (antitheft)
 *
 * \def MEDIA_SOURCE_SNA
 * \brief signal not available (no information)
 */
#define MEDIA_SOURCE_RADIO_AM          0x00
#define MEDIA_SOURCE_RADIO_FM          0x01
#define MEDIA_SOURCE_RADIO_MW          0x02
#define MEDIA_SOURCE_RADIO_LW          0x03
#define MEDIA_SOURCE_CD_DVD            0x04
#define MEDIA_SOURCE_HDD               0x05
#define MEDIA_SOURCE_AUX               0x06
#define MEDIA_SOURCE_BLUETOOTH_AUDIO   0x08
#define MEDIA_SOURCE_RADIO_OFF         0x1D
#define MEDIA_SOURCE_RADIO_LOCKED      0x1E
#define MEDIA_SOURCE_SNA               0x1F

/**
 * \def MEDIA_DISC_NOT_PRESENT
 * \brief no disc present
 *
 * \def MEDIA_DISC_PRESENT_NOT_ACCESSED
 * \brief disc present, but not accessed yet
 *
 * \def MEDIA_DISC_PRESENT_UNKNOWN
 * \brief unknown disc present
 *
 * \def MEDIA_CD_AUDIO
 * \brief audio CD present
 *
 * \def MEDIA_CD_VIDEO
 * \brief video CD present
 *
 * \def MEDIA_CD_DATA_AUDIO
 * \brief audio data CD present
 *
 * \def MEDIA_CD_DATA_VIDEO
 * \brief video data CD present
 *
 * \def MEDIA_CD_DATA_PICTURE
 * \brief picture data CD present
 *
 * \def MEDIA_DVD_AUDIO
 * \brief audio DVD present
 *
 * \def MEDIA_DVD_VIDEO
 * \brief video DVD present
 *
 * \def MEDIA_DVD_DATA_AUDIO
 * \brief audio data DVD present
 *
 * \def MEDIA_DVD_DATA_VIDEO
 * \brief video data DVD present
 *
 * \def MEDIA_DVD_DATA_PICTURE
 * \brief picture data DVD present
 *
 * \def MEDIA_TRANSFER
 * \brief media transfer to HDD active
 *
 * \def MEDIA_HOT_ERROR
 * \brief disc high temperature error
 *
 * \def MEDIA_READ_ERROR
 * \brief disc read error
 *
 * \def MEDIA_DVD_REGION_MISMATCH
 * \brief DVD region code mismatch
 *
 * \def MEDIA_DVD_REGION_NOT_PROGRAMMED
 * \brief DVD region cod enot programmed
 *
 * \def MEDIA_ERROR
 * \brief general media error
 */
#define MEDIA_DISC_NOT_PRESENT            0x00
#define MEDIA_DISC_PRESENT_NOT_ACCESSED   0x01
#define MEDIA_DISC_PRESENT_UNKNOWN        0x02
#define MEDIA_CD_AUDIO                    0x03
#define MEDIA_CD_VIDEO                    0x04
#define MEDIA_CD_DATA_AUDIO               0x05
#define MEDIA_CD_DATA_VIDEO               0x06
#define MEDIA_CD_DATA_PICTURE             0x07
#define MEDIA_DVD_AUDIO                   0x08
#define MEDIA_DVD_VIDEO                   0x09
#define MEDIA_DVD_DATA_AUDIO              0x0A
#define MEDIA_DVD_DATA_VIDEO              0x0B
#define MEDIA_DVD_DATA_PICTURE            0x0C
#define MEDIA_TRANSFER                    0x0E
#define MEDIA_HOT_ERROR                   0x0F
#define MEDIA_READ_ERROR                  0x10
#define MEDIA_DVD_REGION_MISMATCH         0x11
#define MEDIA_DVD_REGION_NOT_PROGRAMMED   0x12
#define MEDIA_ERROR                       0x1F


/**
 * \def MEDIA_STATUS_NONE
 * \brief no status known for current media
 *
 * \def MEDIA_STATUS_PLAY
 * \brief media is playing
 *
 * \def MEDIA_STATUS_LOAD
 * \brief media is loading (e.g. CD, DVD)
 *
 * \def MEDIA_STATUS_READ
 * \brief media is being read (e.g. data)
 *
 * \def MEDIA_STATUS_EJECT
 * \brief media is ejecting (e.g. CD, DVD)
 *
 * \def MEDIA_STATUS_PAUSE
 * \brief media is paused
 *
 * \def MEDIA_STATUS_STOP
 * \brief media is stopped
 *
 * \def MEDIA_STATUS_SCAN
 * \brief media scan running (e.g. radio, CD)
 *
 * \def MEDIA_STATUS_TUNE
 * \brief radio is tuning
 *
 * \def MEDIA_STATUS_ERROR
 * \brief media general error status
 *
 * \def MEDIA_STATUS_TRANSFER
 * \brief media is transferring (e.g. CD to HDD)
 *
 * \def MEDIA_STATUS_SNA
 * \brief media status not available
 */
#define MEDIA_STATUS_NONE                 0x00
#define MEDIA_STATUS_PLAY                 0x01
#define MEDIA_STATUS_LOAD                 0x02
#define MEDIA_STATUS_READ                 0x03
#define MEDIA_STATUS_EJECT                0x04
#define MEDIA_STATUS_PAUSE                0x05
#define MEDIA_STATUS_STOP                 0x06
#define MEDIA_STATUS_SCAN                 0x07
#define MEDIA_STATUS_TUNE                 0x08
#define MEDIA_STATUS_ERROR                0x09
#define MEDIA_STATUS_TRANSFER             0x0A
#define MEDIA_STATUS_SNA                  0x0F

/**
 * \def MEDIA_BT_NOT_PRESENT
 * \brief bluetooth media not present
 *
 * \def MEDIA_BT_PRESENT_NOT_ACCESSED
 * \brief bluetooth media present, but not accessed
 *
 * \def MEDIA_BT_PRESENT_UNKNOWN
 * \brief unknown bluetooth media present
 *
 * \def MEDIA_BT_AUDIO
 * \brief bluetooth audio
 *
 * \def MEDIA_BT_PRESENT_NOT_CONNECTED
 * \brief bluetooth media present, but not connected
 *
 * \def MEDIA_BT_ERROR
 * \brief general bluetooth media error
 */
#define MEDIA_BT_NOT_PRESENT              0x00
#define MEDIA_BT_PRESENT_NOT_ACCESSED     0x01
#define MEDIA_BT_PRESENT_UNKNOWN          0x02
#define MEDIA_BT_AUDIO                    0x03
#define MEDIA_BT_PRESENT_NOT_CONNECTED    0x05
#define MEDIA_BT_ERROR                    0x0F

/*! @} */


#endif /* COMM_CAN_IDS_H_ */

