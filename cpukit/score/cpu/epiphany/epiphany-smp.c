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
extern char bsp_start_vector_table_begin[];

static inline void Epiphany_test_message_handler()
{
  printf("Hi, I have been interrupted \n");
}
static inline 
_Epiphany_Send_interrupt (uint32_t coreid, IRQ_PER_CORE_T interrupt_type)
{ 
  uint32_t ilats_cpu_addr = EPIPHANY_GET_REG_ABSOLUTE_ADDR(
                    coreid, EPIPHANY_PER_CORE_REG_ILATST
                   );
  uint32_t ilats_reg = read_epiphany_reg(ilats_cpu_addr);
  
  /* Set ILATS bit that maps to the interrupt required*/   
  ilats_reg |= 1 << interrupt_type;
  write_epiphany_reg(ilats_cpu_addr, ilats_reg);
}

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
  _SMP_Set_test_message_handler(Epiphany_test_message_handler);
}

void _CPU_SMP_Send_interrupt( uint32_t target_processor_index )
{
  /* Use message interrupt to notify the core that there is 
   * a message for it.
   */
   uint32_t epiphany_coreid = rtems_coreid_to_epiphany_map(
                              target_processor_index);
                              
   _Epiphany_Send_interrupt(epiphany_coreid, SMP_MESSAGE);
}
   
   
bool _CPU_SMP_Start_processor(uint32_t rtems_cpu_index)
{
   uint32_t abs_core_ivt_addr = 
            (uint32_t) rtems_coreid_to_epiphany_map(rtems_cpu_index)
          | (uint32_t) bsp_start_vector_table_begin;
          
   uint32_t epiphany_coreid = rtems_coreid_to_epiphany_map(
                              rtems_cpu_index);
                              
  /* Set up message handler */
  _SMP_Set_test_message_handler(Epiphany_test_message_handler);
  
  /* Send HW interrupt to start the processor */
  //_Epiphany_Send_interrupt(epiphany_coreid, START);
   
   //return _Per_CPU_State_wait_for_non_initial_state(rtems_cpu_index, 0);
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
bsp_start_on_secondary_processor(void)
{
  uint32_t cpu_index_self = _CPU_SMP_Get_current_processor();
  uint32_t abs_core_ivt_addr = 
            (uint32_t) rtems_coreid_to_epiphany_map(cpu_index_self)
          | (uint32_t) bsp_start_vector_table_begin;
  /* Set up message handler */
  _SMP_Set_test_message_handler(Epiphany_test_message_handler);
  
  asm volatile ("gid");
  _SMP_Start_multitasking_on_secondary_processor();
}
#endif /* _EPIPHANY_SMP_H */
