/*
 *  COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/score/cpuopts.h>
#include <rtems/score/isrlevel.h>

/*
 * This file is a dirty hack.  A proper solution would be to add RTEMS support
 * for libatomic in GCC (see also libatomic/configure.tgt).
 */

#if defined(RTEMS_SMP)
static volatile uint32_t _EPIPHANY_The_one_lock = 0;

static inline uint32_t _EPIPHANY_Atomic_testset(
  volatile uint32_t *address,
  uint32_t value
)
{
  register int val asm ("r17");
  val = value;

  if(_EPIPHANY_The_one_lock == 0)
  {
    _EPIPHANY_The_one_lock = 1;
    return 0;
  } else {
    return _EPIPHANY_The_one_lock;
  }
  /*asm volatile (
  "mov r16, #0; \t \n"
  "testset r17, [%[address], r16]; \t \n" 
  :: [address] "r" (address):);
  */ 
  //value = val;
  //return val;
}
#endif

static ISR_Level _EPIPHANY_Acquire_the_one_lock( void )
{
  ISR_Level level;
  _ISR_Disable_without_giant( level );

#if defined(RTEMS_SMP)
  do {
    while ( _EPIPHANY_The_one_lock ) {
      /* Wait */
    
    }
  } while ( _EPIPHANY_Atomic_testset( &_EPIPHANY_The_one_lock, 1 ));
#endif

  return level;
}

static void _EPIPHANY_Release_the_one_lock( ISR_Level level )
{
#if defined(RTEMS_SMP)
  _EPIPHANY_The_one_lock = 0;
#endif
  _ISR_Enable_without_giant( level );
}

uint8_t __atomic_exchange_1( uint8_t *mem, uint8_t val, int model )
{
  uint8_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

/*
 * In order to get rid of the warning "conflicting types for built-in function
 * '__atomic_compare_exchange_4' [enabled by default]", you must port libatomic
 * to SPARC/RTEMS.  The libatomic is provided by GCC.
 */
uint32_t __atomic_exchange_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

/*
 * You get probably a warning here which can be ignored: "warning: conflicting
 * types for built-in function '__atomic_compare_exchange_4' [enabled by
 * default]"
 */
bool __atomic_compare_exchange_4(
  uint32_t *mem,
  uint32_t *expected,
  uint32_t desired,
  int success,
  int failure
)
{
  bool equal;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  equal = *mem == *expected;
  if ( equal ) {
    *mem = desired;
  }

  _EPIPHANY_Release_the_one_lock( level );

  return equal;
}

uint32_t __atomic_fetch_add_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = prev + val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

uint32_t __atomic_fetch_sub_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = prev - val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

uint32_t __atomic_fetch_and_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = prev & val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

uint32_t __atomic_fetch_or_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = prev | val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}

uint32_t __atomic_fetch_xor_4( uint32_t *mem, uint32_t val, int model )
{
  uint32_t prev;
  ISR_Level level;

  level = _EPIPHANY_Acquire_the_one_lock();

  prev = *mem;
  *mem = prev ^ val;

  _EPIPHANY_Release_the_one_lock( level );

  return prev;
}
