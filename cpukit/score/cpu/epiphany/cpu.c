/*
 *  EPIPHANY CPU Dependent Source
 *
 *  COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/wkspace.h>
#include <bsp/linker-symbols.h>
#include <rtems/score/cpu.h>

void _init(void)
{

}

void _fini(void)
{

}
/**
 * @brief Performs processor dependent initialization.
 */
void _CPU_Initialize(void)
{
  /* Do nothing */
}

/**
 * @brief Sets the hardware interrupt level by the level value.
 *
 * @param[in] level for Epiphany can only range over two values:
 * 0 (enable interrupts) and 1 (disable interrupts). In future
 * implementations if fast context switch is implemented, the level
 * can range from 0 to 15. @see OpenRISC architecture manual.
 *
 */
void _CPU_ISR_Set_level(uint32_t level)
{
}

uint32_t  _CPU_ISR_Get_level( void )
{
}

void _CPU_ISR_install_raw_handler(
  uint32_t   vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
}

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
}

void _CPU_Install_interrupt_stack( void )
{
}

void _CPU_Thread_Idle_body( void )
{
  for(;;);
}
