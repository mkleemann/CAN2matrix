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
 * \file uart.h
 *
 * \date Created: 28.11.2011 18:16:09
 * \author Matthias Kleemann
 *
 **/



#ifndef UART_H_
#define UART_H_

#include "uart_config.h"

/***************************************************************************/
/* DEFINITIONS                                                             */
/***************************************************************************/

#ifndef F_CPU
//! setup a default CPU clock rate, if not set yet
   #define F_CPU     4000000UL
   #warning "F_CPU was not defined yet, and is now set here to some default value!"
#endif // F_CPU

/**
 * \def R_UBRR
 * \brief calculate UBRRH/UBRRL register values
 *
 * \def BAUD_REAL
 * \brief real baud rate
 *
 * \def BAUD_ERROR
 * \brief error in per mill
 *
 * A value of 1000 means "no error". A comparison if the value is
 * >990 and <1010 ensures, that the systematic error is less than 1%. This
 * is done in preprocessor already and shows up while compiling.
 */
#define R_UBRR     ((F_CPU + UART_BAUDRATE * 8)/(16 * UART_BAUDRATE) - 1)
#define BAUD_REAL  (F_CPU/(16*(R_UBRR + 1)))
#define BAUD_ERROR ((BAUD_REAL * 1000) / UART_BAUDRATE)

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematic error. Baudrate is more than 1%, which is too high!
#endif


/***************************************************************************/
/* PROTOTYPES                                                              */
/***************************************************************************/

/**
 * @brief initializes UART interface
 */
void uart_init(void);

/**
 * @brief  put byte to UART interface
 * @param  data byte to be transmitted to UART
 */
void uart_putc(const uint8_t data);

/**
 * @brief  put null terminated string to UART interface
 * @param  data pointer (null terminated string)
 */
void uart_puts(const char *data);

/**
 * @brief  read byte from UART, wait until something happens
 * @return received byte
 */
uint8_t uart_getc_wait(void);


/**
 * @brief  read byte from UART, but don't wait for it
 * @return received byte, -1 otherwise
 */
int uart_getc_nowait(void);


#endif