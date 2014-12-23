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
#include <stdarg.h>
#include <stdio.h>

//extern char bsp_processor_count[];
extern char bsp_start_vector_table_begin[];
//extern char smp_ram_console_driver_shared_begin[];
//extern char smp_ram_console_driver_shared_size[];
//extern char smp_ram_clock_driver_shared_begin[];
extern char smp_ram_clock_driver_shared_size[];

void epiphany_clock_at_tick(void) __attribute__((section(".clk_drv")));

/*static void printNum(
  long long num,
  unsigned base,
  bool sign,
  unsigned maxwidth,
  char lead
) __attribute__ ((section(".console_drv")));
*/

//void printk(const char *fmt, ...);

/*void vprintk(
  const char *fmt,
  va_list     ap
) __attribute__ ((section(".console_drv")));
*/

void bsp_inter_processor_interrupt(void *arg);

void bsp_start_on_secondary_processor(void);

void _Epiphany_Send_interrupt (uint32_t coreid, IRQ_PER_CORE_T interrupt_type)
{ 
  uint32_t ilats_cpu_addr = EPIPHANY_GET_REG_ABSOLUTE_ADDR(
                    coreid, EPIPHANY_PER_CORE_REG_ILATST
                   );
  uint32_t ilats_reg = read_epiphany_reg(ilats_cpu_addr);
  
  /* Set ILATS bit that maps to the interrupt required*/   
  ilats_reg |= 1 << interrupt_type;
  write_epiphany_reg(ilats_cpu_addr, ilats_reg);
}

static rtems_isr Epiphany_test_message_handler()
{
  if (*((uint32_t *)0x8F800008))
    asm volatile ("idle");
  //printf("Hi, I have been interrupted \n");
}

void bsp_inter_processor_interrupt(void *arg)
{
  //_SMP_Inter_processor_interrupt_handler();
}

void bsp_start_on_secondary_processor(void)
{
  /* FIXME: No need to get the absolute address of vector table 
   * since this function executes on the local secondary processor
   * and see the local address space 
   */
  uint32_t *vector_table = bsp_start_vector_table_begin;
  *((uint32_t *) 0x58) = 0; 
  
  /* TODO: If cpu == 1 > console driver, if cpu == 2, clock driver, etc */
  //memcpy(0x4000, 0x8e000000, smp_ram_console_driver_shared_size);
  
  memcpy(0x4000 /*+ smp_ram_console_driver_shared_size*/, 0x8e000000 /*+ smp_ram_clock_driver_shared_begin -
  0x4000*/, 
  smp_ram_clock_driver_shared_size);
  
  //free_local_mem_pointer += smp_ram_console_driver_shared_size;
  
  //vector_table[SW_INT] = printk_smp_interrupt_handler;
  vector_table[SMP_MESSAGE] = Epiphany_test_message_handler; // FIXME
  vector_table[TIMER0] = epiphany_clock_at_tick;
  //epiphany_clock_handler_install(epiphany_clock_at_tick, (void *) 0);
  epiphany_clock_initialize();
  
  asm volatile ("gie");
  asm volatile ("idle");
}
