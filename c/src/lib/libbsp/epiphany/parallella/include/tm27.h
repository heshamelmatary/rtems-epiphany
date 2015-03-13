#ifndef _RTEMS_TMTEST27
#error "This is an RTEMS internal file you must not include directly."
#endif

#ifndef __tm27_h
#define __tm27_h

#include <rtems/score/epiphany-utility.h>
/**
 * @name Interrupt mechanisms for Time Test 27 
 * @{
 */

#define MUST_WAIT_FOR_INTERRUPT 1

#define Install_tm27_vector( handler ) *((uint32_t *) 0x3C) = handler

#define Cause_tm27_intr() write_epiphany_reg((0x808 << 20 | EPIPHANY_PER_CORE_REG_ILATST), 1 << SMP_MESSAGE)

#define Clear_tm27_intr() /* empty */

#define Lower_tm27_intr() /* empty */

#endif
