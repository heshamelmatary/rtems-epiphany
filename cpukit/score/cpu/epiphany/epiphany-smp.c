/*
 *  This include file contains macros pertaining to the
 *  Epiphany processor family.
 *
 *  COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

#ifndef _EPIPHANY_SMP_H
#define _EPIPHANY_SMP_H

#include <assert.h>
#include <rtems/score/smp.h>
#include <rtems/score/epiphany-utility.h>
#include <rtems/score/smpimpl.h>

extern char bsp_processor_count[];

void bsp_inter_processor_interrupt(void *arg)
{
  //_SMP_Inter_processor_interrupt_handler();
}

uint32_t _CPU_SMP_Initialize(void)
{
  /* FIXME: For now use only linker count */
  
  uint32_t linker_count = (uint32_t) bsp_processor_count;

  return linker_count;
}

void _CPU_SMP_Finalize_initialization(uint32_t cpu_count)
{

}

void _CPU_SMP_Send_interrupt( uint32_t target_processor_index )
{

}

bool _CPU_SMP_Start_processor(uint32_t cpu_index)
{
  //return _Per_CPU_State_wait_for_non_initial_state(cpu_index, 0);
}

uint32_t _CPU_SMP_Get_current_processor( void )
{
  return _Epiphany_Get_Current_coreid();
}

inline void _CPU_SMP_Processor_event_broadcast( void )
{
    asm volatile ("" : : : "memory" );
}

inline void
epiphany_start_on_secondary_processor(void)
{

}
#endif /* _EPIPHANY_SMP_H */
