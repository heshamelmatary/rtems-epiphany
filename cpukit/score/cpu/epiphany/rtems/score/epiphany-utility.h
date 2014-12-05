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

#ifndef _EPIPHANY_UTILITY_H
#define _EPIPHANY_UTILITY_H

 uint32_t _Epiphany_Get_Current_coreid();
 
/* Epiphany uses 12 bits for defining core IDs, while RTEMS uses 
 * linear IDs. The following macro is used to map Epiphany IDs to
 * RTEMS linear IDs.
 */

static inline uint32_t epiphany_coreid_map(uint32_t epiphany_id)
{
  /* FIXME: move this to BSP as Parallella has another 64 core board 
   * and can be extended in future.
   */
  switch(epiphany_id)
  {
    case (0x808): return 0;  /* (32, 8) */
    case (0x809): return 1;  /* (32, 9) */
    case (0x80A): return 2;  /* (32,10) */
    case (0x80B): return 3;  /* (32,11) */
    case (0x848): return 4;  /* (33, 8) */
    case (0x849): return 5;  /* (33, 9) */
    case (0x84A): return 6;  /* (33,10) */
    case (0x84B): return 7;  /* (33,11) */
    case (0x888): return 8;  /* (34, 8) */
    case (0x889): return 9;  /* (34, 9) */
    case (0x88A): return 10; /* (34,10) */
    case (0x88B): return 11; /* (34,11) */
    case (0x8C8): return 12; /* (35, 8) */
    case (0x8C9): return 13; /* (35, 9) */
    case (0x8CA): return 14; /* (35,10) */
    case (0x8CB): return 15; /* (35,11) */
    default: return 0x1000; /* Error */
  }
  
  return 0x1000; /* Error */
}

inline uint32_t _Epiphany_Get_Current_coreid()
{
   uint32_t coreid;
   
   asm volatile ("movfs %0, coreid" : "=r" (coreid): );
   
   return epiphany_coreid_map(coreid);
}
#endif  /* _EPIPHANY_UTILITY_H */
