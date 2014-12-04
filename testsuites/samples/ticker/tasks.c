/*  Test_task
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_id          tid;
  rtems_time_of_day time;
  uint32_t          task_index;
  rtems_status_code status;
  status = rtems_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "task ident" ); 

  task_index = task_number( tid );
  for ( ; ; ) {
    status = rtems_clock_get_tod( &time );
    if ( time.second >= 35 ) {
      TEST_END();
      rtems_test_exit( 0 );
    }
   int x = 9;
    put_name( Task_name[ task_index ], FALSE );
    //print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    printk("Time in hours is %d \n", x);
    //printk(" - rtems_clock_get_tod - %d:%d:%d   %d/%d/%d\n", \ 
  //     time.hour, time.minute, time.second, \
    //   time.month, time.day, time.year);

    /*printk(" - rtems_clock_get_tod - " PRIu32 ":%02" PRIu32 ":%02" PRIu32 " %02" PRIu32 "/%02" PRIu32 "/%04" PRIu32 "\n", \ 
       time.hour, time.minute, time.second, \
       time.month, time.day, time.year);
    */
    status = rtems_task_wake_after(
      task_index * 5 * rtems_clock_get_ticks_per_second()
    );
    directive_failed( status, "wake after" ); 
  }
}
