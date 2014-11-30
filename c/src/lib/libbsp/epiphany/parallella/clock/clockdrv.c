/**
 * @file
 *
 * @ingroup epiphany_clock
 *
 * @brief epiphany clock support.
 */

/*
 * Epiphany Clock driver
 *
 * COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE
 */

#include <rtems.h>
#include <bsp.h>
#include <rtems/score/e_lib.h> /* e-lib Epiphany library */

/* The number of clock cycles before generating a tick timer interrupt. */
#define TTMR_NUM_OF_CLOCK_TICKS_INTERRUPT     0x0000FFFF
#define EPIPHANY_CLOCK_CYCLE_TIME_NANOSECONDS  10

/* CPU counter */
static CPU_Counter_ticks cpu_counter_ticks;

/* This prototype is added here to Avoid warnings */
void Clock_isr(void *arg);

static void epiphany_clock_at_tick(void)
{
  unsigned int val = 0xFFFFFFFF; 
  unsigned int event_type = E_CTIMER_CLK;
  /* The following e-lib function would not work properly 
     because gcc emits the wrong code with wrong argument
     registers ordering.
   */
  //x = e_ctimer_set(E_CTIMER_0, 0x00000000u);
   
  /* Embed assembly code for setting timer0 */
  asm volatile ("movts ctimer0, %[val] \t \n" :: [val] "r" (val));
  
  /* The following e-lib function would not work properly 
     because gcc emits the wrong code with wrong argument
     registers ordering.
   */
  //x = e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
  
  /* Embed assembly code for setting timer0 qouted from e-lib */
  asm volatile ("movfs r3, config; \t \n"
                "mov   r63, %%low(0xffffff0f);\t \n"
                "mov   r63, %%high(0xffffff0f);\t \n"
                "lsl   r1, %[event_type], 0x4; \t \n"
                "and   r3, r3, r63; \t \n"
                "movts config, r3; \t \n"
                "orr   r3, r3, r1; \t \n"
                "movts config, r3; \t \n"
                :: [event_type] "r" (event_type));
  
  cpu_counter_ticks += TTMR_NUM_OF_CLOCK_TICKS_INTERRUPT;
}

/* Use timer0 on each eCPU for scheduling purposes */
static void epiphany_clock_handler_install(proc_ptr new_isr, proc_ptr old_isr)
{
   old_isr = NULL;
   /*e_irq_attach(E_TIMER0_INT, (sighandler_t) new_isr);*/ 
}

static void epiphany_clock_initialize(void)
{
  unsigned int x = 0xDEADBEEF;
  
  unsigned int val = 0xFFFFFFFF; 
  unsigned int event_type = E_CTIMER_CLK;
  /* The following e-lib function would not work properly 
     because gcc emits the wrong code with wrong argument
     registers ordering.
   */
  //x = e_ctimer_set(E_CTIMER_0, 0x00000000u);
  
  /* Embed assembly code for setting timer0 */
  asm volatile ("movts ctimer0, %[val] \t \n" :: [val] "r" (val));

  /* The following e-lib function would not work properly 
     because gcc emits the wrong code with wrong argument
     registers ordering.
   */
  //x = e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
  
  /* Embed assembly code for setting timer0 qouted from e-lib */
  asm volatile ("movfs r3, config; \t \n"
                "mov   r63, %%low(0xffffff0f);\t \n"
                "mov   r63, %%high(0xffffff0f);\t \n"
                "lsl   r1, %[event_type], 0x4; \t \n"
                "and   r3, r3, r63; \t \n"
                "movts config, r3; \t \n"
                "orr   r3, r3, r1; \t \n"
                "movts config, r3; \t \n"
                :: [event_type] "r" (event_type));
  
  cpu_counter_ticks = 0;
}

 static void epiphany_clock_cleanup(void)
{
   e_ctimer_stop(E_CTIMER_0);
}

/*
 *  Return the nanoseconds since last tick
 */
static uint32_t epiphany_clock_nanoseconds_since_last_tick(void)
{
  return
  TTMR_NUM_OF_CLOCK_TICKS_INTERRUPT * EPIPHANY_CLOCK_CYCLE_TIME_NANOSECONDS;
}

CPU_Counter_ticks _CPU_Counter_read(void)
{
  uint32_t ticks_since_last_timer_interrupt;

  ticks_since_last_timer_interrupt = TTMR_NUM_OF_CLOCK_TICKS_INTERRUPT - 
  e_ctimer_get(E_CTIMER_0);

  return cpu_counter_ticks + ticks_since_last_timer_interrupt;
}

CPU_Counter_ticks _CPU_Counter_difference(
  CPU_Counter_ticks second,
  CPU_Counter_ticks first
)
{
  return second - first;
}
#define Clock_driver_support_at_tick() epiphany_clock_at_tick()

#define Clock_driver_support_initialize_hardware() epiphany_clock_initialize()

#define Clock_driver_support_install_isr(isr, old_isr) \
  do {                                                 \
    old_isr = NULL;                                    \
    epiphany_clock_handler_install(isr, old_isr);       \
  } while (0)

#define Clock_driver_support_shutdown_hardware() epiphany_clock_cleanup()

#define Clock_driver_nanoseconds_since_last_tick \
  epiphany_clock_nanoseconds_since_last_tick

#include "../../../shared/clockdrv_shell.h"
