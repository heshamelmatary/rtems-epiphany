/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <stdlib.h>

#include <bsp.h>
#include <rtems/rtems/tasks.h>
#include <rtems/rtems/clock.h>

int *footprint = 0x8F800008;

Context_Control ctx __attribute__ ((section (".start")));

static inline void benchmark_timer_initialize()
{
   uint32_t event_type = 0x1;
   unsigned int val = 0xFFFFFFFF;

   asm volatile ("movts ctimer1, %[val] \t \n" :: [val] "r" (val));
   asm volatile ("movfs r16, config; \t \n"
                "mov   r17, %%low(0xfffff0ff);\t \n"
                "movt   r17, %%high(0xfffff0ff);\t \n"
                "lsl   r18, %[event_type], 0x8; \t \n"
                "and   r16, r16, r17; \t \n"
                "movts config, r16; \t \n"
                "orr   r16, r16, r18; \t \n"
                "movts config, r16; \t \n"
                :: [event_type] "r" (event_type)); 
}

rtems_task Test_task(
  rtems_task_argument task_index
)
{
  rtems_status_code status;
  rtems_interval    ticks;
  struct timespec   uptime;
  asm volatile ("movfs r63 ,ctimer1; \t \n" ::);
  ticks = task_index * (5 * rtems_clock_get_ticks_per_second());
  for ( ; ; ) {
    status = rtems_task_wake_after( ticks );

    status = rtems_clock_get_uptime( &uptime );
    if ( uptime.tv_sec >= 35 ) {
      //printk( "*** END OF LOW MEMORY CLOCK TICK TEST (delay) ***\n" );
      //(*footprint)++;
      //_Epiphany_Send_interrupt(0x809, 5);
      rtems_shutdown_executive( 0 );
    }
    /*printk( "TA%d - rtems_clock_uptime - %d:%d\n", 
      task_index, uptime.tv_sec, uptime.tv_nsec 
    );*/
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
  //*footprint = 0;
  rtems_status_code status;
  rtems_id          id;
  int               i;

  //printk( "\n\n*** LOW MEMORY CLOCK TICK TEST (delay) ***\n" );
  benchmark_timer_initialize();
  for (i=1 ; i<=2 ; i++ ) {
    status = rtems_task_create(
      rtems_build_name( 'T', 'A', 0x30+1, ' ' ), 1, 0, RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES, &id
    );
    status = rtems_task_start( id, Test_task, i );
  }
  
  while( 1 )
    ;
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_TERMIOS_DISABLED
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 0
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE 512 
#define CONFIGURE_MAXIMUM_PRIORITY 15
#define CONFIGURE_DISABLE_CLASSIC_API_NOTEPADS
#define CONFIGURE_IDLE_TASK_BODY Init
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_MAXIMUM_TASKS             2

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/

