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
 * \file CAN2matrix.c
 *
 * \date Created: 28.11.2011 18:12:46
 * \author Matthias Kleemann
 **/


#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
//#include <stdlib.h>

#include "can/can_mcp2515.h"
#include "adc/adc.h"
#include "comm/comm_matrix.h"
#include "leds/leds.h"
#include "timer/timer.h"
//#include "uart/uart.h"
#include "CAN2matrix.h"

//! counter to evaluate timing to send CAN messages
volatile uint8_t send_it         = 0;
//! flag to send tick to instrument cluster
volatile bool    trigger50ms     = false;
//! current state of FSM
volatile state_t fsmState        = INIT;

/**
 * @brief main loop
 *
 * The main loop consists of the FSM and calls all necessary init sequences
 * before entering it. Any error in the init process will result in
 * entering the error state. This is indicated by a blinking LED.
 **/
#ifdef __DOXYGEN__
int main(void)
#else
int __attribute__((OS_main)) main(void)
#endif
{
   initHardware();

   if (true == initCAN())
   {
      // start timers and enable ADC
      startTimer1();
      startTimer2();
      adc_enable();
      // start normal operation
      fsmState = RUNNING;

      led_on(sleepLed);

      // enable all (configured) interrupts
      sei();

      while (1)
      {
         switch (fsmState)
         {
            case RUNNING:
            {
               run();
               break;
            }

            case WAKEUP:
            {
               wakeUp();
               fsmState = RUNNING;
               break;
            }

            case SLEEP_DETECTED:
            {
               sleepDetected();
               fsmState = SLEEPING;
               break;
            }

            case SLEEPING:
            {
               sleeping();
               // set state WAKEUP here, too avoid race conditions
               // with pending interrupt
               fsmState = WAKEUP;
               break;
            }

            default:
            {
               errorState();
               fsmState = ERROR;
               break;
            }
         }
      }
   }

   errorState();
}


/***************************************************************************/
/* STATES OF FSM                                                           */
/***************************************************************************/

/**
 * @brief Deactivate CAN and timers
 *
 * Sleep trigger was detected (no CAN activity on master bus). All timers are
 * stopped and ADC disabled. CAN controllers are put to sleep and AVR is
 * preparing for sleep mode.
 */
void sleepDetected()
{
   // stop timer for now
   stopTimer1();
   stopTimer2();

   // stop adc to save power
   adc_disable();

#ifdef ___TEST_TX_ABORT__
   // abort any pending CAN frames to be transmitted on CAN
   can_abort_all_transmissions(CAN_CHIP1);
   // wait for SPI
   _delay_ms(1);
   can_abort_all_transmissions(CAN_CHIP2);
#endif

   // (re)set global flags
   send_it  = 0;

   // put MCP25* to sleep for CAN2 and activate after activity on CAN1
   // This has to be done before master CAN goes to sleep, because
   // the clock of the slave chip may be taken from master CAN chip's
   // CLKOUT pin.
   mcp2515_sleep(CAN_CHIP2, INT_SLEEP_MANUAL_WAKEUP);
   // wait for SPI
   _delay_ms(1);

   // put MCP2515 to sleep and wait for activity interrupt
   mcp2515_sleep(CAN_CHIP1, INT_SLEEP_WAKEUP_BY_CAN);

   // low power consumption
   led_all_off();
}

/**
 * @brief enter AVR sleep mode
 *
 * AVR enters sleep mode and also wakes up in this state, so some intial
 * steps to set wakeup interrupt need to be done here.
 *
 * The three \c _NOP(); instructions are a safety, since older AVRs may
 * skip the next couple of instructions after sleep mode.
 *
 * Also a precaution is the disabling of the wake up interrupt, to avoid
 * several interrupts to happen, if the signal lies too long on the
 * external interrupt pin.
 */
void sleeping()
{
   cli();

   // enable wakeup interrupt INT0
   GICR  |= EXTERNAL_INT0_ENABLE;

   // let's sleep...
   set_sleep_mode(AVR_SLEEP_MODE);
   // sleep_mode() has a possible race condition in it, so splitting it
   sleep_enable();
   sei();
   sleep_cpu();
   sleep_disable();

   // just in case...
   _NOP();
   _NOP();
   _NOP();

   // disable interrupt: precaution, if signal lies too long on pin
   GICR  &= ~(EXTERNAL_INT0_ENABLE);
}

/**
 * @brief wake up CAN and reinitialize the timers
 *
 * Now the AVR has woken up. Timers needs to be restarted, ADC to be enabled
 * again and the CAN controllers will also need to enter their working mode.
 */
void wakeUp()
{
   cli();
   // wakeup all CAN busses
   mcp2515_wakeup(CAN_CHIP1, INT_SLEEP_WAKEUP_BY_CAN);
   // wait a little for the CAN controller to be ready (Tosc * 128)
   _delay_ms(1);
   mcp2515_wakeup(CAN_CHIP2, INT_SLEEP_MANUAL_WAKEUP);
   // wait a little for the CAN controller to be ready (Tosc * 128)
   _delay_ms(1);

   restartTimer1();
   restartTimer2();

   adc_enable();

   sei();

   // debugging ;-)
   led_on(sleepLed);
}

/**
 * @brief do all the work.
 */
void run()
{
   can_t msg;
   can_error_t error;

   /**** GET MESSAGES FROM CAN1 ***********************************/

   handleCan1Reception(&msg);
   _delay_ms(1);

   /**** PUT MESSAGES TO CAN2 *************************************/

   handleCan2Transmission(&msg);
   _delay_ms(1);

   /**** GET MESSAGES FROM CAN2 ***********************************/

   handleCan2Reception(&msg);
   _delay_ms(1);

   /**** PUT MESSAGES TO CAN1 *************************************/

   handleCan1Transmission(&msg);
   _delay_ms(1);

   /**** CHECK CAN STATUS *****************************************/
   error = can_get_general_bus_errors(CAN_CHIP1);
   if(CAN_ERR_NO_ERROR == error)
   {
      led_off(errCan1LED);
   }
   else
   {
      led_on(errCan1LED);
   }

   error = can_get_general_bus_errors(CAN_CHIP2);
   if(CAN_ERR_NO_ERROR == error)
   {
      led_off(errCan2LED);
   }
   else
   {
      led_on(errCan2LED);
   }
}

/**
 * @brief Error state
 *
 * Call this when an illegal state is reached. Only some status LEDs will
 * blink to show the error, but the system stops to work.
 */
void errorState()
{
   // error handling, e.g. init failed
   stopTimer1();
   restartTimer2();     // may be stopped, due to sleep mode
   while (1)
   {
      if (0 == (trigger50ms % 10))  // approx. 500ms
      {
         trigger50ms = false;
         led_toggle(sleepLed);
      }
   }
}

/**
 * @brief Initialize Hardware
 *
 * Setting up the peripherals to the AVR and the wake-up interrupt
 * trigger.
 *
 * * \ref page_timers to trigger events
 *
 * * \ref page_spi to communicate to the MCP2515
 *
 * * \ref page_status_leds to show the (non-)errors of your way
 *
 * * \ref page_adc for connecting the analog and digital world
 */
void initHardware()
{
   // init LED output
   led_init();

   // set timer for bussleep detection
   initTimer1(TimerCompare);

   // set timer for CAN 100ms trigger
   initTimer2(TimerCompare);

   // initialize the hardware SPI with default values set in spi/spi_config.h
   spi_pin_init();
   spi_master_init();

   // initialize adc
   adc_init();

   // initialize uart
//   uart_init();

   // set wakeup interrupt trigger on low level
   MCUCR |= EXTERNAL_INT0_TRIGGER;
}

/**
 * @brief Initialize the CAN controllers
 *
 * Calls can_init_mcp2515 for each attached CAN controller and setting up
 * bit rate. If an error occurs some status LEDs will indicate it.
 *
 * See chapter \ref page_can_bus for further details.
 *
 * @return true if all is ok. Otherwise false is returned.
 */
bool initCAN()
{
   bool retVal = true;

   // init can interface 1
   if (false == can_init_mcp2515(CAN_CHIP1, CAN_BITRATE_100_KBPS, NORMAL_MODE))
   {
      // signal error on initialization
      led_on(errCan1LED);
      retVal = false;
   }
   // wait for SPI
   _delay_ms(1);
   // init can interface 2
   if (false == can_init_mcp2515(CAN_CHIP2, CAN_BITRATE_125_KBPS, NORMAL_MODE))
   {
      // signal error on initialization
      led_on(errCan2LED);
      retVal = false;
   }

   return retVal;
}


/***************************************************************************/
/* INTERRUPT SERVICE ROUTINES                                              */
/***************************************************************************/

/**
 * @brief interrupt service routine for Timer1 capture
 *
 * Timer1 input capture interrupt (~15s 4MHz@1024 prescale factor)
 **/
ISR(TIMER1_CAPT_vect)
{
   fsmState = SLEEP_DETECTED;
}

/**
 * @brief interrupt service routine for Timer2 compare
 *
 * Timer2 compare match interrupt handler --> set as 25ms (4x25ms = 100ms)
 **/
ISR(TIMER2_COMP_vect)
{
   ++send_it;
   trigger50ms = (0 == (send_it % 2));   // ~50ms;
}

/**
 * @brief interrupt service routine for external interrupt 0
 *
 * External Interrupt0 handler to wake up from CAN activity
 **/
ISR(INT0_vect)
{
   // needs to be here, even if empty
}

/***************************************************************************/
/* HELPER ROUTINES                                                         */
/***************************************************************************/

/**
 * @brief handles CAN1 reception
 * @param msg - pointer to message struct
 */
void handleCan1Reception(can_t* msg)
{
   if (can_check_message_received(CAN_CHIP1))
   {
      // try to read message
      if (can_get_message(CAN_CHIP1, msg))
      {
         // reset timer counter, since there is activity on master CAN bus
         setTimer1Count(0);

         // fetch information from CAN1
         fetchInfoFromCAN1(msg);

         // signal activity
         led_toggle(rxCan1LED);
      }
   }
}

/**
 * @brief handles CAN2 reception
 * @param msg - pointer to message struct
 */
void handleCan2Reception(can_t* msg)
{
   // empty read buffers and get information
   if (can_check_message_received(CAN_CHIP2))
   {
      // try to read message
      if (can_get_message(CAN_CHIP2, msg))
      {
         // fetch information from CAN2
         fetchInfoFromCAN2(msg);
      }
   }
}

/**
 * @brief handle CAN1 transmission
 * @param msg - pointer to message struct
 */
void handleCan1Transmission(can_t* msg)
{
   // handle reset trigger wisely, when putting in some code here!
}

/**
 * @brief handle CAN2 transmission
 * @param msg - pointer to message struct
 */
void handleCan2Transmission(can_t* msg)
{
   if(true == trigger50ms)
   {
      trigger50ms = false;
      // signal activity
      led_toggle(txCan2LED);
      sendCan2(msg);
      // sample and set dim value
      uint16_t dimValue = adc_get();
      setDimValue(dimValue);
   }
}



