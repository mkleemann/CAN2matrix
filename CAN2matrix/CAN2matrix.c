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


#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

#include "util/util.h"
#include "spi/spi.h"
#include "can/can_mcp2515.h"
#include "timer/timer.h"
#include "leds/leds.h"
#include "adc/adc.h"
#include "CAN2matrix.h"
#include "matrix.h"

#define ___AVR_SLEEP_ON___

//! counter to evluate timing to send CAN messages
static volatile uint8_t send_it      = 0;
//! flag to send 100ms cycle CAN messages
static volatile bool    send100ms    = false;
//! flag to send 500ms cycle CAN messages
static volatile bool    send500ms    = false;
//! current state of FSM
static volatile state_t fsmState     = INIT;

/**
 * @brief main loop
 **/
int main(void)
{
   initHardware();

   if (true == initCAN())
   {
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
} /* end of main() */


/***************************************************************************/
/* STATES OF FSM                                                           */
/***************************************************************************/

/**
 * @brief Deactivate CAN and timers.
 */
void sleepDetected()
{
   // stop timer for now
   stopTimer1();
   stopTimer2();

   // stop adc to save power
   adc_disable();

   // (re)set global flags
   send_it  = 0;

#ifndef ___SIMULATION___
   // put MCP25* to sleep for CAN2 and activate after activity on CAN1
   // This has to be done before master CAN goes to sleep, because
   // the clock of the slave chip may be taken from master CAN chip's
   // CLKOUT pin.
#ifndef ___SINGLE_CAN___
   mcp2515_sleep(CAN_CHIP2, INT_SLEEP_MANUAL_WAKEUP);
#endif

   // put MCP2515 to sleep and wait for activity interrupt
   mcp2515_sleep(CAN_CHIP1, INT_SLEEP_WAKEUP_BY_CAN);
#endif

   // low power consumption
   led_all_off();
}

/**
 * @brief enter AVR sleep mode
 *
 * AVR also wakes up in this function, so some intial steps need to
 * be done here.
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

   // error? Maybe timer interrupt cause state change...testing
   if(SLEEPING != fsmState)
   {
      led_all_on();
      fsmState = SLEEPING;
   }

   // enable wakeup interrupt INT0
   GICR  |= EXTERNAL_INT0_ENABLE;

#ifdef ___AVR_SLEEP_ON___
   // let's sleep...
   set_sleep_mode(AVR_SLEEP_MODE);
   // sleep_mode() has a possible race condition in it, so splitting it
   sleep_enable();
   sei();
   sleep_cpu();
   sleep_disable();

   // disable interrupt: precaution, if signal lies too long on pin
   GICR  &= ~(EXTERNAL_INT0_ENABLE);

   // just in case...
   _NOP();
   _NOP();
   _NOP();
#else
   sei();
#endif
}

/**
 * @brief wake up CAN and reinitialize the timers
 */
void wakeUp()
{
   cli();
#ifndef ___SIMULATION___
   // wakeup all CAN busses
   mcp2515_wakeup(CAN_CHIP1);
#ifndef ___SINGLE_CAN___
   _delay_ms(100);       // wait for clock of CAN1 to be ready
   mcp2515_wakeup(CAN_CHIP2);
#endif
#endif

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
#ifndef ___SIMULATION___
   can_t msg;

   /**** GET MESSAGES FROM CAN1 ***********************************/

   handleCan1Reception(&msg);
   _delay_us(1);

   /**** PUT MESSAGES TO CAN2 *************************************/

#ifndef ___SINGLE_CAN___
   handleCan2Transmission(&msg);
   _delay_us(1);

   /**** GET MESSAGES FROM CAN2 ***********************************/

   handleCan2Reception(&msg);
   _delay_us(1);
#endif

   /**** PUT MESSAGES TO CAN1 *************************************/

   handleCan1Transmission(&msg);
   _delay_us(1);

   // check and set dim value
   uint16_t dimValue = adc_get();
   setDimValue(dimValue);

#else
   if (send500ms)   // approx. 500ms 4MHz@1024 prescale factor
   {
      send500ms = false;
      led_toggle(errCan1LED);
   } /* end of if 500ms tick */
#endif
}

/**
 * @brief Error state. Call this when an illegal state is reached.
 */
void errorState()
{
   // error handling, e.g. init failed
   stopTimer1();
   restartTimer2();     // may be stopped, due to sleep mode
   while (1)
   {
      if (send500ms)    // approx. 500ms 4MHz@1024 prescale factor
      {
         send500ms = false;
         led_toggle(sleepLed);
      }
   }
}

/**
 * @brief Initialize LED, Timer and SPI.
 */
void initHardware()
{
   // init LED output
   led_init();

   // set timer for bussleep detection
   initTimer1(TimerCompare);
   startTimer1();

   // set timer for CAN 100ms trigger
   initTimer2(TimerCompare);
   startTimer2();

   // initialize the hardware SPI with default values set in spi/spi_config.h
   spi_pin_init();
   spi_master_init();

   // initialize adc and enable
   adc_init();
   adc_enable();

   // set wakeup interrupt trigger on low level
   MCUCR |= EXTERNAL_INT0_TRIGGER;
}

/**
 * @brief Initialize the CAN controllers
 *
 * @return true if all is ok. Otherwise false is returned.
 */
bool initCAN()
{
   bool retVal = true;

#ifndef ___SIMULATION___
   // init can interface 1
   if (false == can_init_mcp2515(CAN_CHIP1, CAN_BITRATE_100_KBPS))
   {
      // signal error on initialization
      led_on(errCan1LED);
      retVal = false;
   }
#ifndef ___SINGLE_CAN___
   // init can interface 2
   else if (false == can_init_mcp2515(CAN_CHIP2, CAN_BITRATE_125_KBPS))
   {
      // signal error on initialization
      led_on(errCan2LED);
      retVal = false;
   }
#endif
#endif

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
   send100ms = (0 == (send_it % 4));   // ~100ms
   send500ms = (0 == (send_it % 20));  // ~500ms
}

/**
 * @brief interrupt service routine for external interrupt 0
 *
 * External Interrupt0 handler to wake up from CAN activity
 **/
ISR(INT0_vect)
{
#ifndef ___AVR_SLEEP_ON___
   // disable interrupt: precaution, if signal lies too long on pin
   GICR  &= ~(EXTERNAL_INT0_ENABLE);
#endif
   led_toggle(errCan2LED);
   fsmState = WAKEUP;
}

/***************************************************************************/
/* HELPER ROUTINES                                                         */
/***************************************************************************/

#ifndef ___SIMULATION___
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
#ifdef ___SINGLE_CAN___
   if (send500ms)   // approx. 500ms 4MHz@1024 prescale factor
   {
      send500ms = false;

      sendCan1_500ms(msg);
   }
#endif
}

/**
 * @brief handle CAN2 transmission
 * @param msg - pointer to message struct
 */
void handleCan2Transmission(can_t* msg)
{
   if (send100ms)    // approx. 100ms 4MHz@1024 prescale factor
   {
      send100ms = false;

      sendCan2_100ms(msg);

      // signal activity
      led_toggle(txCan2LED);
   }

   if (send500ms)   // approx. 500ms 4MHz@1024 prescale factor
   {
      send500ms = false;

      // check and set dim value
      uint16_t dimValue = adc_get();
      setDimValue((uint8_t) dimValue);  // ADC config set to 8bit resolution

      sendCan2_500ms(msg);

      // signal activity
      led_toggle(txCan2LED);
   }
}


#endif

