/**
 * ----------------------------------------------------------------------------
 * "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
 * <m.kleemann@gmx.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
 * like beer much.)
 *
 * Matthias Kleemann
 * ----------------------------------------------------------------------------
 **/

/*
 * CAN2matrix.c
 *
 * Created: 28.11.2011 18:12:46
 *  Author: MKleemann
 */

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
#include "CAN2matrix.h"
#include "matrix.h"

#define ___AVR_SLEEP_ON___

static volatile uint8_t send_it      = 0;
static volatile bool    send100ms    = false;
static volatile bool    send500ms    = false;
static volatile state_t fsmState     = INIT;

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
               run();
               break;

            case WAKEUP:
               wakeUp();
               // woken up
               fsmState = RUNNING;
               break;

            case SLEEP_DETECTED:
               // Set status first here. After wakeup we leave this function.
               fsmState = SLEEPING;
               sleepDetected();
               break;

            case SLEEPING:
               // This state might be used, even when AVR is sleeping!
               break;

            default:
               errorState();
               // set status
               fsmState = ERROR;
               break;
         }
      }
   }

   errorState();
} /* end of main() */


/***************************************************************************/
/* HELPER ROUTINES                                                         */
/***************************************************************************/
/**
 * @brief sends message to CAN2 and filling up converted data
 *
 * Note: Set message id before calling this function.
 *
 * @param pointer to CAN message
 */
#ifndef ___SIMULATION___
void sendCan2Message(can_t* msg)
{
   fillInfoToCAN2(msg);

   // send message
   can_send_message(CAN_CHIP2, msg);

   // signal activity
   led_toggle(txCan2LED);
}
#endif

/**
 * @brief Go to sleep mode. Deactivate CAN and set the sleep mode.
 */
void sleepDetected()
{
   cli();
   // stop timer for now
   stopTimer1();
   stopTimer2();

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
 * @brief Wake up CAN and reinitialize the timer
 */
void wakeUp()
{
   cli();
#ifndef ___SIMULATION___
   // wakeup all CAN busses
   mcp2515_wakeup(CAN_CHIP1);
#ifndef ___SINGLE_CAN___
   _delay_ms(10);       // wait for clock of CAN1 to be ready
   mcp2515_wakeup(CAN_CHIP2);
#endif
#endif

   restartTimer1();
   restartTimer2();

   sei();

   // debugging ;-)
   led_on(sleepLed);
}

/**
 * @brief Do all the work.
 */
void run()
{
#ifndef ___SIMULATION___
   /**** GET MESSAGES FROM CAN1 ***********************************/
   can_t msg;

   if (can_check_message_received(CAN_CHIP1))
   {
      // try to read message
      if (can_get_message(CAN_CHIP1, &msg))
      {
         // reset timer counter, since there is activity on master CAN bus
         setTimer1Count(0);

         // fetch information from CAN1
         fetchInfoFromCAN1(&msg);
#ifdef ___SINGLE_CAN___
         msg.msgId += 10;
         can_send_message(CAN_CHIP1, &msg);
#endif
         // signal activity
         led_toggle(rxCan1LED);
      } /* end of if message is read */
   } /* end of if check message received on CAN1 */

   /**** PUT MESSAGES ON CAN2 *************************************/
#ifndef ___SINGLE_CAN___
   if (send100ms)    // approx. 100ms 4MHz@1024 prescale factor
   {
      send100ms = false;

      msg.msgId = CANID_2_IGNITION;
      sendCan2Message(&msg);

      msg.msgId = CANID_2_WHEEL_DATA;  // should be 50ms, but keep it
      sendCan2Message(&msg);
   } /* end of if 100ms tick */

   if (send500ms)   // approx. 500ms 4MHz@1024 prescale factor
   {
      send500ms = false;

      msg.msgId = CANID_2_REVERSE_GEAR;
      sendCan2Message(&msg);
   } /* end of if 500ms tick */

   /**** GET MESSAGES FROM CAN2 ***********************************/

   // empty read buffers and get information
   if (can_check_message_received(CAN_CHIP2))
   {
      // try to read message
      if (can_get_message(CAN_CHIP2, &msg))
      {
         // fetch information from CAN2
         fetchInfoFromCAN2(&msg);
      } /* end of if message is read */
   } /* end of if check message received on CAN2 */
#endif
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
      } /* end of if 500ms tick */
   } /* end of while(1) - error handling */
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
   } /* end of init CAN1 failed */
#ifndef ___SINGLE_CAN___
   else if (false == can_init_mcp2515(CAN_CHIP2, CAN_BITRATE_125_KBPS))
   {
      // signal error on initialization
      led_on(errCan2LED);
      retVal = false;
   } /* end of else init failed CAN2 */
#endif
#endif

   return retVal;
}


/***************************************************************************/
/* INTERRUPT SERVICE ROUTINES                                              */
/***************************************************************************/

// Timer1 input capture interrupt (~15s 4MHz@1024 prescale factor)
ISR(TIMER1_CAPT_vect)
{
   fsmState = SLEEP_DETECTED;
}

// Timer2 compare match interrupt handler
// --> set as 25ms (4x25ms = 100ms)
ISR(TIMER2_COMP_vect)
{
   ++send_it;
   send100ms = (0 == (send_it % 4));   // ~100ms
   send500ms = (0 == (send_it % 20));  // ~500ms
}

// External Interrupt0 handler to wake up from CAN activity
ISR(INT0_vect)
{
#ifndef ___AVR_SLEEP_ON___
   // disable interrupt: precaution, if signal lies too long on pin
   GICR  &= ~(EXTERNAL_INT0_ENABLE);
#endif
   led_toggle(errCan2LED);
   fsmState = WAKEUP;
}

