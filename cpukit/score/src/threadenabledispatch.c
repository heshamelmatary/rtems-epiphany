/**
 * @file
 *
 * @brief Enable Dispatching of Threads
 *
 * @ingroup ScoreThread
 */

/*
 *   _Thread_Enable_dispatch
 *
 *
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/threaddispatch.h>

#if defined (__THREAD_DO_NOT_INLINE_ENABLE_DISPATCH__ )
void _Thread_Enable_dispatch( void )
{
  if ( _Thread_Dispatch_decrement_disable_level() )
    return;
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
  _Thread_Dispatch();
  }
}
#endif
