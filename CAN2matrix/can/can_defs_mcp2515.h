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
 * \file can_defs_mcp2515.h
 *
 * \date Created: 28.11.2011 18:36:26
 * \author Matthias Kleemann
 *
 **/



#ifndef CAN_DEFS_MCP2515_H_
#define CAN_DEFS_MCP2515_H_


/**************************************************************************/
/* MCP2515 COMMAND SET                                                    */
/**************************************************************************/

/**
 * \addtogroup mcp_command_set MCP2515 Command Set
 * This is the command set used to control the MCP2515 CAN controller.
 * @{
 */

/*! \brief resets all registers to configuration mode */
#define MCP2515_RESET         0xC0
/*! \brief quick polling of filter match and message type */
#define MCP2515_RX_STATUS     0xB0
/*! \brief quick read status of RX/TX functions */
#define MCP2515_READ_STATUS   0xA0
/*! \brief read a RX buffer indicated by bit 0..1 */
#define MCP2515_READ_RX       0x90
/*! \brief begin TX sequence for a TX buffer (bit 0..2) */
#define MCP2515_RTS           0x80
/*! \brief load a TX buffer indicated by bit 0..2 */
#define MCP2515_LOAD_TX       0x40
/*! \brief modifies bit in MCP2515 register */
#define MCP2515_BITMODIFY     0x05
/*! \brief read data from MCP2515 */
#define MCP2515_READ          0x03
/*! \brief write data to MCP2515 */
#define MCP2515_WRITE         0x02

/*! @} */


/**************************************************************************/
/* MCP2515 CONTROL REGISTERS                                              */
/**************************************************************************/

/**
 * \addtogroup mcp_control_registers MCP2515 Control Registers
 * These are the registers the MCP2515 CAN controller provides to read and
 * write status, control and data information.
 * @{
 */

/*! \brief Receive Buffer 1 Control Register */
#define RXB1CTRL              0x70
/*! \brief Receive Buffer 0 Control Register */
#define RXB0CTRL              0x60
/*! \brief Transmit Buffer 2 Control Register */
#define TXB2CTRL              0x50
/*! \brief Transmit Buffer 1 Control Register */
#define TXB1CTRL              0x40
/*! \brief Transmit Buffer 0 Control Register */
#define TXB0CTRL              0x30
/*! \brief Error Flag Register */
#define EFLG                  0x2D
/*! \brief CAN Interrupt Flag Register */
#define CANINTF               0x2C
/*! \brief CAN Interrupt Enable Register */
#define CANINTE               0x2B
/*! \brief Configuration Register 1 */
#define CNF1                  0x2A
/*! \brief Configuration Register 2 */
#define CNF2                  0x29
/*! \brief Configuration Register 3 */
#define CNF3                  0x28
/*! \brief Receive Error Counter */
#define REC                   0x1D
/*! \brief Transmit Error Counter */
#define TEC                   0x1C
/*! \brief TXnRTS Pin Control and Status Register */
#define TXRTSCTRL             0x0D
/*! \brief RXnBF Pin Control and Status Register */
#define BFPCTRL               0x0C

/*!
 * \def CANSTAT(x)
 * \brief CAN Status Register(s)
 *
 * Multiple registers CANSTATx. Only one needs to be used (sets all), since
 * they are in the same location physically.
 */
#define CANSTAT(x)            CANSTAT ## x
/*! \sa CANSTAT(x) */
#define CANSTAT7              0x7E
/*! \sa CANSTAT(x) */
#define CANSTAT6              0x6E
/*! \sa CANSTAT(x) */
#define CANSTAT5              0x5E
/*! \sa CANSTAT(x) */
#define CANSTAT4              0x4E
/*! \sa CANSTAT(x) */
#define CANSTAT3              0x3E
/*! \sa CANSTAT(x) */
#define CANSTAT2              0x2E
/*! \sa CANSTAT(x) */
#define CANSTAT1              0x1E
/*! \sa CANSTAT(x) */
#define CANSTAT0              0x0E

/*!
 * \def CANCTRL(x)
 * \brief CAN Control Register(s)
 *
 * Multiple registers CANCTRLx. Only one needs to be used (sets all), since
 * they are in the same location physically.
 */
#define CANCTRL(x)            CANCTRL ## x
/*! \sa CANCTRL(x) */
#define CANCTRL7              0x7F
/*! \sa CANCTRL(x) */
#define CANCTRL6              0x6F
/*! \sa CANCTRL(x) */
#define CANCTRL5              0x5F
/*! \sa CANCTRL(x) */
#define CANCTRL4              0x4F
/*! \sa CANCTRL(x) */
#define CANCTRL3              0x3F
/*! \sa CANCTRL(x) */
#define CANCTRL2              0x2F
/*! \sa CANCTRL(x) */
#define CANCTRL1              0x1F
/*! \sa CANCTRL(x) */
#define CANCTRL0              0x0F

// reception filter mask registers

/**
 * \def RXM0SIDH
 * \brief Reception Mask Register 0 Standard ID Highbyte
 *
 * \def RXM0SIDL
 * \brief Reception Mask Register 0 Standard ID Lowbyte
 *
 * \def RXM0EID8
 * \brief Reception Mask Register 0 Extended ID Highbyte
 *
 * \def RXM0EID0
 * \brief Reception Mask Register 0 Extended ID Lowbyte
 */
#define RXM0SIDH              0x20
#define RXM0SIDL              0x21
#define RXM0EID8              0x22
#define RXM0EID0              0x23

/**
 * \def RXM1SIDH
 * \brief Reception Mask Register 1 Standard ID Highbyte
 *
 * \def RXM1SIDL
 * \brief Reception Mask Register 1 Standard ID Lowbyte
 *
 * \def RXM1EID8
 * \brief Reception Mask Register 1 Extended ID Highbyte
 *
 * \def RXM1EID0
 * \brief Reception Mask Register 1 Extended ID Lowbyte
 */
#define RXM1SIDH              0x24
#define RXM1SIDL              0x25
#define RXM1EID8              0x26
#define RXM1EID0              0x27

// reception filter registers

/**
 * \def RXF0SIDH
 * \brief Reception Filter Register 0 Standard ID Highbyte
 *
 * \def RXF0SIDL
 * \brief Reception Filter Register 0 Standard ID Lowbyte
 *
 * \def RXF0EID8
 * \brief Reception Filter Register 0 Extended ID Highbyte
 *
 * \def RXF0EID0
 * \brief Reception Filter Register 0 Extended ID Lowbyte
 */
#define RXF0SIDH              0x00
#define RXF0SIDL              0x01
#define RXF0EID8              0x02
#define RXF0EID0              0x03

/**
 * \def RXF1SIDH
 * \brief Reception Filter Register 1 Standard ID Highbyte
 *
 * \def RXF1SIDL
 * \brief Reception Filter Register 1 Standard ID Lowbyte
 *
 * \def RXF1EID8
 * \brief Reception Filter Register 1 Extended ID Highbyte
 *
 * \def RXF1EID0
 * \brief Reception Filter Register 1 Extended ID Lowbyte
 */
#define RXF1SIDH              0x04
#define RXF1SIDL              0x05
#define RXF1EID8              0x06
#define RXF1EID0              0x07

/**
 * \def RXF2SIDH
 * \brief Reception Filter Register 2 Standard ID Highbyte
 *
 * \def RXF2SIDL
 * \brief Reception Filter Register 2 Standard ID Lowbyte
 *
 * \def RXF2EID8
 * \brief Reception Filter Register 2 Extended ID Highbyte
 *
 * \def RXF2EID0
 * \brief Reception Filter Register 2 Extended ID Lowbyte
 */
#define RXF2SIDH              0x08
#define RXF2SIDL              0x09
#define RXF2EID8              0x0A
#define RXF2EID0              0x0B

/**
 * \def RXF3SIDH
 * \brief Reception Filter Register 3 Standard ID Highbyte
 *
 * \def RXF3SIDL
 * \brief Reception Filter Register 3 Standard ID Lowbyte
 *
 * \def RXF3EID8
 * \brief Reception Filter Register 3 Extended ID Highbyte
 *
 * \def RXF3EID0
 * \brief Reception Filter Register 3 Extended ID Lowbyte
 */
#define RXF3SIDH              0x10
#define RXF3SIDL              0x11
#define RXF3EID8              0x12
#define RXF3EID0              0x13

/**
 * \def RXF4SIDH
 * \brief Reception Filter Register 4 Standard ID Highbyte
 *
 * \def RXF4SIDL
 * \brief Reception Filter Register 4 Standard ID Lowbyte
 *
 * \def RXF4EID8
 * \brief Reception Filter Register 4 Extended ID Highbyte
 *
 * \def RXF4EID0
 * \brief Reception Filter Register 4 Extended ID Lowbyte
 */
#define RXF4SIDH              0x14
#define RXF4SIDL              0x15
#define RXF4EID8              0x16
#define RXF4EID0              0x17

/**
 * \def RXF5SIDH
 * \brief Reception Filter Register 5 Standard ID Highbyte
 *
 * \def RXF5SIDL
 * \brief Reception Filter Register 5 Standard ID Lowbyte
 *
 * \def RXF5EID8
 * \brief Reception Filter Register 5 Extended ID Highbyte
 *
 * \def RXF5EID0
 * \brief Reception Filter Register 5 Extended ID Lowbyte
 */
#define RXF5SIDH              0x18
#define RXF5SIDL              0x19
#define RXF5EID8              0x1A
#define RXF5EID0              0x1B

/*! @} */

/**************************************************************************/
/* MCP2515 BIT DEFINITIONS                                                */
/**************************************************************************/

/**
 * \addtogroup mcp_bit_definitions MCP2515 Bit Definitions
 * All named bits to be used writing and reading from MCP2515 registers.
 * @{
 */


/*!
 * \def BUKT2
 * Rollover enable bit. If set RXB0 message will rollover and be written to
 * RXB1 if RXB0 is full
 * \sa RXB0CTRL
 *
 * \def BUKT1
 * Copy of BUKT2 and used only internally by MCP2515. Read-Only
 * \sa RXB0CTRL
 * \sa BUKT2
 */
#define BUKT2                 2
#define BUKT1                 1

/**
 * \def FILHIT2
 * \brief Filter Hit bits.
 * Indicates which acceptance filter enabled reception of message.
 *
 * \code
 * FILHIT 2 1 0
 *        1 0 1 = Acceptance Filter 5 (RXF5)
 *        1 0 0 = Acceptance Filter 4 (RXF4)
 *        0 1 1 = Acceptance Filter 3 (RXF3)
 *        0 1 0 = Acceptance Filter 2 (RXF2)
 *        0 0 1 = Acceptance Filter 1 (RXF1) (Only if BUKT bit set in RXB0CTRL)
 *        0 0 0 = Acceptance Filter 0 (RXF0) (Only if BUKT bit set in RXB0CTRL)
 * \endcode
 *
 * \sa RXB1CTRL
 * \sa RXB0CTRL
 * \sa FILHIT1
 * \sa FILHIT0
 * \sa BUKT1
 * \sa BUKT2
 *
 * \def FILHIT1
 * Further documentation, see FILHIT2.
 * \sa FILHIT2
 * \sa FILHIT0
 */
#define FILHIT2               2
#define FILHIT1               1

/**
 * \def RXM1
 * \brief Receive Buffer Operating Mode bits
 *
 * \code
 * RXM 1 0
 *     1 1 = Turn mask/filters off; receive any message
 *     1 0 = Receive only valid messages with extended identifiers that meet filter criteria
 *     0 1 = Receive only valid messages with standard identifiers that meet filter criteria
 *     0 0 = Receive all valid messages using either standard or extended identifiers that meet filter criteria
 * \endcode
 *
 * \sa RXM0
 * \sa RXB1CTRL
 * \sa RXB0CTRL
 *
 * \def RXM0
 * \brief Receive Buffer Operating Mode bits
 *
 * Further documentaion, see RXM1
 * \sa RXM1
 * \sa RXB1CTRL
 * \sa RXB0CTRL
 *
 * \def FILHIT0
 * Further documentation, see FILHIT2.
 * \sa FILHIT2
 * \sa FILHIT1
 * \sa RXB1CTRL
 * \sa RXB0CTRL
 */
#define RXM1                  6
#define RXM0                  5
#define RXRTR                 3
#define FILHIT0               0

// TXBxCTRL
#define ABTF                  6
#define MLOA                  5
#define TXERR                 4
#define TXREQ                 3
#define TXP1                  1
#define TXP0                  0

// EFLG
#define RX1OVR                7
#define RX0OVR                6
#define TXBO                  5
#define TXEP                  4
#define RXEP                  3
#define TXWAR                 2
#define RXWAR                 1
#define EWARN                 0

// CANINTE
#define MERRE                 7
#define WAKIE                 6
#define ERRIE                 5
#define TX2IE                 4
#define TX1IE                 3
#define TX0IE                 2
#define RX1IE                 1
#define RX0IE                 0

// CANINTF
#define MERRF                 7
#define WAKIF                 6
#define ERRIF                 5
#define TX2IF                 4
#define TX1IF                 3
#define TX0IF                 2
#define RX1IF                 1
#define RX0IF                 0

// CNF1
#define SJW1                  7
#define SJW0                  6
#define BRP5                  5
#define BRP4                  4
#define BRP3                  3
#define BRP2                  2
#define BRP1                  1
#define BRP0                  0

// CNF2
#define BTLMODE               7
#define SAM                   6
#define PHSEG12               5
#define PHSEG11               4
#define PHSEG10               3
#define PRSEG2                2
#define PRSEG1                1
#define PRSEG0                0

// CNF3
#define SOF                   7
#define WAKFIL                6
#define PHSEG22               2
#define PHSEG21               1
#define PHSEG20               0

// TXRTSCTRL
#define B2RTS                 5
#define B1RTS                 4
#define B0RTS                 3
#define B2RTSM                2
#define B1RTSM                1
#define B0RTSM                0

// BFPCRL
#define B1BFS                 5
#define B0BFS                 4
#define B1BFE                 3
#define B0BFE                 2
#define B1BFM                 1
#define B0BFM                 0

// multiple registers CANSTATx
#define OPMOD2                7
#define OPMOD1                6
#define OPMOD0                5
#define ICOD2                 3
#define ICOD1                 2
#define ICOD0                 1

// multiple registers CANCTRLx
#define REQOP2                7
#define REQOP1                6
#define REQOP0                5
#define ABAT                  4
#define OSM                   3
#define CLKEN                 2
#define CLKPRE1               1
#define CLKPRE0               0

// command READ_STATUS
#define TXB2CNTRL_TXREQ       6
#define TXB1CNTRL_TXREQ       4
#define TXB0CNTRL_TXREQ       2

// load TX buffer addresses
#define TXB2ADDR              4
#define TXB1ADDR              2
#define TXB0ADDR              0

// RX buffer status
// buffer information as bit - CANINTF.RXnIF bits are mapped to bits 7 and 6
#define RXB_STATUSMASK        0x60
#define RXB1                  7
#define RXB0                  6
// msg type as value - the extended ID bit is mapped to bit 4. The RTR bit
// is mapped to bit 3
#define RXB_EXT               4
#define RXB_RTR               3
#define RXB_MSGTYPEMASK       0x18
#define RXB_STDFRAME          0x00
#define RXB_STDREMOTEFRAME    0x01
#define RXB_EXTDATAFRAME      0x02
#define RXB_EXTREMOTEFRAME    0x03
// filter match as value
#define RXB_FILTERMATCHMASK   0x07
#define RXB_RXF0              0x00
#define RXB_RXF1              0x01
#define RXB_RXF2              0x02
#define RXB_RXF3              0x03
#define RXB_RXF4              0x04
#define RXB_RXF5              0x05
#define RXB_RXF0_RO_RXB1      0x06
#define RXB_RXF1_RO_RXB1      0x07

// RXBnSIDL (n = 0, 1)
#define SRR                   4
#define IDE                   3

// TXBnDLC and RXBxDLC
#define RTR                   6
#define DLC3                  3
#define DLC2                  2
#define DLC1                  1
#define DLC0                  0

/*! @} */

/**************************************************************************/
/* MCP2515 MODES OF OPERATION                                             */
/**************************************************************************/

/**
 * \addtogroup mcp_mode_of_operation MCP2515 Modes of Operation
 * \brief MCP2515 supports different modes of operation
 * @{
 */

/*!
 * \def MODE_SELECT_MASK
 * \brief selection mask for any mode settings
 *
 * The mask is used to prvide easy access to the operation mode bits
 * of the CANCTRL register.
 *
 * \sa CANCTRL(x)
 */
#define MODE_SELECT_MASK      ((1 << REQOP2) | (1 << REQOP1) | (1 << REQOP0))

/**
 * \def NORMAL_MODE
 * \brief standard operation mode
 *
 * This is the standard operation mode. In this mode the controller actively
 * monitors and reacts to the CAN stream. This is the only mode to transmit
 * CAN frames to the bus.
 * \sa CANCTRL(x)
 *
 * \def SLEEP_MODE
 * \brief mode to reduce power consumption
 *
 * During sleep mode the power consumption is reduced to a minimum.
 * Nevertheless the controller registers can be accessed via SPI the
 * whole time, to allow wake up and other operations while the chip
 * sleeps (on CAN).
 *
 * The internal oscillator is stopped during sleep.
 *
 * When setting up the wakup interrupt, any activity on the bus can wake up
 * the controller.
 * \sa CANCTRL(x)
 * \sa WAKIE, WAKIF
 * \sa CANINTE, CANINTF
 *
 * \def LOOPBACK_MODE
 * \brief internal CAN loopback
 *
 * This mode is used for testing during development. The transmit buffer
 * will be put directly to the receive buffer without actually sending
 * anything on the bus. Filters can be applied too.
 * \sa CANCTRL(x)
 *
 * \def LISTEN_ONLY_MODE
 * \brief listen only to the bus
 *
 * This mode can be used for monitoring or baud rate detection applications.
 *
 * The mode is also set, when waking up. So no messages are accidentally sent
 * to the CAN bus after sleep mode.
 * \sa CANCTRL(x)
 * \sa SLEEP_MODE
 *
 * \def CONFIG_MODE
 * \brief mode to configure the controller
 *
 * This mode can be entered anytime and automatically after power-up. The
 * controller needs to be configured prior activation. It's the only mode
 * to set the following control registers:
 * - CNF1, CNF2, CNF3
 * - TXRTSCTRL
 * - Filter registers
 * - Mask registers
 *
 * \sa CANCTRL(x)
 * \sa CNF1, CNF2, CNF3
 * \sa TXRTSCTRL
 */
#define NORMAL_MODE           0
#define SLEEP_MODE            (1 << REQOP0)
#define LOOPBACK_MODE         (1 << REQOP1)
#define LISTEN_ONLY_MODE      (1 << REQOP1) | (1 << REQOP0)
#define CONFIG_MODE           (1 << REQOP2)

/*! @} */



#endif /* CAN_DEFS_MCP2515_H_ */