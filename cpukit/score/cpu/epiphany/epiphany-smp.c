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

uint32_t _CPU_SMP_Initialize(void)
{
  /* FIXME: For now use only linker count */
  
  uint32_t linker_count = (uint32_t) bsp_processor_count;
  
  return linker_count;
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

uint32_t _CPU_SMP_Get_current_processor( void )
{
  return _Epiphany_Get_Current_coreid();
}

inline void _CPU_SMP_Processor_event_broadcast( void )
{
    asm volatile ("" : : : "memory" );
}

#endif /* _EPIPHANY_SMP_H */
