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
 * \file uart_config.h
 *
 * USART is setup without interrupt usage! ATmega 8 is used here.
 *
 * \date Created: 28.11.2011 18:17:15
 * \author Matthias Kleemann
 **/



#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_


#ifndef UART_BAUDRATE
/**
 * \brief Set a default baud rate, if not yet set
 *
 * Set numeric values as UL, otherwise errors may occur in any
 * pre processor calculation using it.
 */
   #define UART_BAUDRATE  9600UL
#endif

/**
 * \brief definition of a newline on various OSs
 *
 * \code
 * Windows: "rn"
 * Linux  : "n"
 * MacOS  : "r"
 * \endcode
 */
#define CR "\r\n"


#endif
