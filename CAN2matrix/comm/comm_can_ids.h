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
 *  bit 0..7 : temperature (outside) in 0.5°C
 *             -50..77°C (1..254)
 *             0x00 - not available
 *             0xFF - error
 *
 * byte 6:
 *  bit 0..7 : temperature (outside) in 0.5°C
 *             -50..77°C (1..254)
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

/**
 * \def CANID_2_ODO_AND_TEMP
 * \brief Odometer and ambient temperature
 *
 * \code
 * byte 0..3: odometer in 0.1km
 * byte 5..6: average ambient temperature in 0.01°C (-40°C..85°C - 0..12500)
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

//! Ignition CAN1
//! terminal "ACC" - key in/ACC
#define IGN_1_CL_ACC                   0
//! terminal 15 - ignition on
#define IGN_1_CL_15                    1
//! terminal X - consumers not relevant for engine start are switched off
#define IGN_1_CL_X                     2
//! terminal 50 - engine start
#define IGN_1_CL_50                    3
//! terminal "ER" - engine runs
#define IGN_1_CL_ER                    7

//! combinations for CAN1
//! switched ACC and key in
#define IGN_1_ACC_Status       (1 << IGN_1_CL_ACC)
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

// Ignition CAN2
//! key status (in/out)
#define IGN_2_KeyIn                    0
//! ignition status 1
#define IGN_2_Stat1                    5
//! ignition status 2
#define IGN_2_Stat2                    6
//! ignition status 3
#define IGN_2_Stat3                    7

// combinations for CAN2
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

//! GERMAN for destination CAN (#2)
#define LANG_GERMAN_CAN2               0
//! US_ENGLISH for destination CAN (#2)
#define LANG_ENGLISH_US_CAN2           1
//! FRENCH for destination CAN (#2)
#define LANG_FRENCH_CAN2               2
//! ITALIAN for destination CAN (#2)
#define LANG_ITALIAN_CAN2              3
//! SPANISH for destination CAN (#2)
#define LANG_SPANISH_CAN2              4
//! PORTUGESE for destination CAN (#2)
#define LANG_PORTUGESE_CAN2            6
//! DUTCH for destination CAN (#2)
#define LANG_DUTCH_CAN2                7
//! UK_ENGLISH for destination CAN (#2)
#define LANG_ENGLISH_UK_CAN2           8
//! NO_CHANGE for destination CAN (#2)
#define LANG_NO_CHANGE_CAN2           15


/*! @} */



#endif /* COMM_CAN_IDS_H_ */

