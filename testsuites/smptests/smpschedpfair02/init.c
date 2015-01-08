/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CONFIGURE_INIT
#include "system.h"

#include <rtems/score/schedulerpfairsmp.h>

#include <inttypes.h>

const char rtems_test_name[] = "SMP SCHED PFAIR 1";

void Loop() {
  volatile int i;

  for (i=0; i<300000; i++);
}

rtems_task Init(
  rtems_task_argument argument
)
{
 // char               ch;
  uint32_t           cpu_self;
  rtems_id           id;
  rtems_status_code  status;
  //bool               allDone;

  uint32_t Tp, Te, Up, Ue;

  Tp = 10;
  Te = 5;
  Up = 10;
  Ue = 3;
 
  cpu_self = rtems_get_current_processor();

  /* XXX - Delay a bit to allow debug messages from
   * startup to print.  This may need to go away when
   * debug messages go away.
   */ 
  Loop();

  TEST_BEGIN();

  locked_print_initialize();

      status =  _Scheduler_pfair_SMP_Task_create(
        rtems_build_name( 'T', 'A', 'T', ' ' ),
        2,
        RTEMS_MINIMUM_STACK_SIZE,
        RTEMS_DEFAULT_MODES | RTEMS_TIMESLICE,
        RTEMS_DEFAULT_ATTRIBUTES,
        &id,
        Tp,
				Te        
      );

      directive_failed( status, "task create" );

      locked_printf(" CPU %" PRIu32 " start task TA%c\n", cpu_self, 'T');
      status = rtems_task_start( id, Test_task1, 1 );
      directive_failed( status, "task start" );

      status = _Scheduler_pfair_SMP_Task_create(
        rtems_build_name( 'T', 'A', 'U', ' ' ),
        4,
        RTEMS_MINIMUM_STACK_SIZE,
        RTEMS_TIMESLICE,
        RTEMS_DEFAULT_ATTRIBUTES,
        &id,
        Up,
				Ue
      );

      directive_failed( status, "task create" );

      locked_printf(" CPU %" PRIu32 " start task TA%c\n", cpu_self, 'U');
      status = rtems_task_start( id, Test_task2, 2 );
      directive_failed( status, "task start" );

      //rtems_test_exit( 0 );
}

rtems_task Test_task1(
  rtems_task_argument task_index
)
{
  uint32_t          cpu_num;
  char              name[5];
  char             *p;

  /* Get the task name */
  p = rtems_object_get_name( RTEMS_SELF, 5, name );
  rtems_test_assert( p != NULL );

   /* Get the CPU Number */
  cpu_num = rtems_get_current_processor();

  /* Print that the task is up and running. */
  Loop();
  locked_printf(" CPU %" PRIu32 " running Task %s\n", cpu_num, name);

  /* Set the flag that the task is up and running */
  //TaskRan[cpu_num] = true;


  /* Drop into a loop which will keep this task on
   * running on the cpu.
   */
  while(1);
}


rtems_task Test_task2(
  rtems_task_argument task_index
)
{
  uint32_t          cpu_num;
  char              name[5];
  char             *p;

  /* Get the task name */
  p = rtems_object_get_name( RTEMS_SELF, 5, name );
  rtems_test_assert( p != NULL );

   /* Get the CPU Number */
  cpu_num = rtems_get_current_processor();

  /* Print that the task is up and running. */
  Loop();
  locked_printf(" CPU %" PRIu32 " running Task %s\n", cpu_num, name);

  /* Set the flag that the task is up and running */
  //TaskRan[cpu_num] = true;


  /* Drop into a loop which will keep this task on
   * running on the cpu.
   */
  while(1);
}
