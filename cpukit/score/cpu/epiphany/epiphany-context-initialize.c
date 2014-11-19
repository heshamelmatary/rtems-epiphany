/*
 * COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 * COPYRIGHT (c) 1989-2006
 * On-Line Applications Research Corporation (OAR).
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <string.h>

#include <rtems/score/cpu.h>
#include <rtems/score/interr.h>

void _CPU_Context_Initialize(
  Context_Control *context,
  void *stack_area_begin,
  size_t stack_area_size,
  uint32_t new_level,
  void (*entry_point)( void ),
  bool is_fp,
  void *tls_area
)
{
  /* Decrement 200 byte to account for red-zone */
  uint32_t stack = ((uint32_t) stack_area_begin) - 200;
  uint32_t sr;
  uint32_t stack_high = stack + stack_area_size;

  asm volatile ("movfs %0, config \n" : "=r" (sr):);

  memset(context, 0, sizeof(*context));

  context->r[13] = stack_high;
  context->[11] = stack_high;
  context->r[14] = (uint32_t) entry_point;
  context->status = sr;
}
