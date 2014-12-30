#ifndef _RTEMS_TMTEST27
#error "This is an RTEMS internal file you must not include directly."
#endif

#ifndef __tm27_h
#define __tm27_h

/**
 * @name Interrupt mechanisms for Time Test 27 
 * @{
 */

#define MUST_WAIT_FOR_INTERRUPT 0

#define Install_tm27_vector( handler ) /* empty */

#define Cause_tm27_intr() /* empty */

#define Clear_tm27_intr() /* empty */

#define Lower_tm27_intr() /* empty */

#endif
