/**
 * @file
 *
 * @ingroup epiphany parallella
 *
 * @brief BSP SMP functions
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

#include <bsp.h>
#include <bsp/bootcard.h>
#include <bsp/fatal.h>
#include <rtems/bspIo.h>
#include <rtems/score/smpimpl.h>
#include <rtems/score/epiphany-utility.h>
#include <stdlib.h>

extern char bsp_processor_count[];
extern char bsp_start_vector_table_begin[];

static rtems_isr Epiphany_test_message_handler()
{
  printf("Hi, I have been interrupted \n");
}

void bsp_inter_processor_interrupt(void *arg)
{
  _SMP_Inter_processor_interrupt_handler();
}

void bsp_start_on_secondary_processor(void)
{
  /* FIXME: No need to get the absolute address of vector table 
   * since this function executes on the local secondary processor
   * and see the local address space 
   */
  uint32_t cpu_index_self = _CPU_SMP_Get_current_processor();
  uint32_t *abs_core_ivt_addr = 
            (uint32_t) rtems_coreid_to_epiphany_map(cpu_index_self)
          | (uint32_t) bsp_start_vector_table_begin;

  abs_core_ivt_addr[SMP_MESSAGE] = bsp_inter_processor_interrupt;
  
  /* Set up message test message handler */
  _SMP_Set_test_message_handler(Epiphany_test_message_handler);
  
  /* Disable interrupts */
  asm volatile ("gid");
  
  
  _SMP_Start_multitasking_on_secondary_processor();
}


bool _CPU_SMP_Start_processor( uint32_t rtems_cpu_index )
{
  /* Wait for all other secondary processors to complete initialization */
  return _Per_CPU_State_wait_for_non_initial_state(rtems_cpu_index, 0);
}

void _CPU_SMP_Finalize_initialization( uint32_t cpu_count )
{
  (void) cpu_count;

  /* Nothing to do */
}
