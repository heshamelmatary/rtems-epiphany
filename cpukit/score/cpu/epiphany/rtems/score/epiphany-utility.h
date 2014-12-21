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

/* TODO: Move this to irq.h */
typedef enum
{
  START,
  SW_EXCEPTION,
  MEM_FAULT,
  TIMER0,
  TIMER1,
  SMP_MESSAGE,
  DMA0,
  DMA1,
  SW_INT,
} IRQ_PER_CORE_T;

/* TODO: move this to different SMP place */

#define CONSOLE_SMP_INTR_OFF_ARG0 0x50
#define CONSOLE_SMP_INTR_OFF_ARG1 0x54

/*  Per-core IO mapped register addresses 
 *  @see Epiphany architecture reference.
 */
#define EPIPHANY_PER_CORE_REG_CONFIG      0xF0400
#define EPIPHANY_PER_CORE_REG_STATUS      0xF0404
#define EPIPHANY_PER_CORE_REG_PC          0xF0408
#define EPIPHANY_PER_CORE_REG_DEBUGSTATUS 0xF040C
#define EPIPHANY_PER_CORE_REG_LC          0xF0414
#define EPIPHANY_PER_CORE_REG_LS          0xF0418
#define EPIPHANY_PER_CORE_REG_LE          0xF041C
#define EPIPHANY_PER_CORE_REG_IRET        0xF0420
#define EPIPHANY_PER_CORE_REG_IMASK       0xF0424
#define EPIPHANY_PER_CORE_REG_ILAT        0xF0428
#define EPIPHANY_PER_CORE_REG_ILATST      0xF042C
#define EPIPHANY_PER_CORE_REG_ILATCL      0xF0430
#define EPIPHANY_PER_CORE_REG_IPEND       0xF0434
#define EPIPHANY_PER_CORE_REG_FSTATUS     0xF0440
#define EPIPHANY_PER_CORE_REG_DEBUGCMD    0xF0448
#define EPIPHANY_PER_CORE_REG_RESETCORE   0xF070C

/* Event timer registers */
#define EPIPHANY_PER_CORE_REG_CTIMER0     0xF0438
#define EPIPHANY_PER_CORE_REG_CTIMER1     0xF043C

/* Processor control registers */
#define EPIPHANY_PER_CORE_REG_MEMSTATUS   0xF0604
#define EPIPHANY_PER_CORE_REG_MEMPROTECT  0xF0608

/* DMA Registers */
#define EPIPHANY_PER_CORE_REG_DMA0CONFIG  0xF0500
#define EPIPHANY_PER_CORE_REG_DMA0STRIDE  0xF0504
#define EPIPHANY_PER_CORE_REG_DMA0COUNT   0xF0508
#define EPIPHANY_PER_CORE_REG_DMA0SRCADDR 0xF050C
#define EPIPHANY_PER_CORE_REG_DMA0DSTADDR 0xF0510
#define EPIPHANY_PER_CORE_REG_DMA0AUTO0   0xF0514
#define EPIPHANY_PER_CORE_REG_DMA0AUTO1   0xF0518
#define EPIPHANY_PER_CORE_REG_DMA0STATUS  0xF051C
#define EPIPHANY_PER_CORE_REG_DMA1CONFIG  0xF0520
#define EPIPHANY_PER_CORE_REG_DMA1STRIDE  0xF0524
#define EPIPHANY_PER_CORE_REG_DMA1COUNT   0xF0528
#define EPIPHANY_PER_CORE_REG_DMA1SRCADDR 0xF052C
#define EPIPHANY_PER_CORE_REG_DMA1DSTADDR 0xF0530
#define EPIPHANY_PER_CORE_REG_DMA1AUTO0   0xF0534
#define EPIPHANY_PER_CORE_REG_DMA1AUTO1   0xF0538
#define EPIPHANY_PER_CORE_REG_DMA1STATUS  0xF053C

/* Mesh Node Control Registers */
#define EPIPHANY_PER_CORE_REG_MESHCONFIG  0xF0700
#define EPIPHANY_PER_CORE_REG_COREID      0xF0704
#define EPIPHANY_PER_CORE_REG_MULTICAST   0xF0708
#define EPIPHANY_PER_CORE_REG_CMESHROUTE  0xF0710
#define EPIPHANY_PER_CORE_REG_XMESHROUTE  0xF0714
#define EPIPHANY_PER_CORE_REG_RMESHROUTE  0xF0718

/*  This macros constructs an address space of epiphany cores 
 *  from their IDs.
 */
#define EPIPHANY_COREID_TO_MSB_ADDR(id) (id) << 20

/*  Construct a complete/absolute IO mapped address register from
 *  core ID and register name
 */
#define EPIPHANY_GET_REG_ABSOLUTE_ADDR(coreid, reg) \
        (EPIPHANY_COREID_TO_MSB_ADDR(coreid) | (reg))

/* FIXME: Some registers are 64-bit while others are 32, for now
 * just use 32-bit registers 
 */
#define EPIPHANY_REG(reg) (uint32_t *) (reg)

void printk_smp_interrupt_handler();

static inline uint32_t epiphany_coreid_to_rtems_map(uint32_t coreid);

/* Read register with its absolute address */
static inline uint32_t read_epiphany_reg(volatile uint32_t reg_addr)
{
  return *(EPIPHANY_REG(reg_addr));
}

/* Write register with its abolute address */
static inline void write_epiphany_reg(volatile uint32_t reg_addr, uint32_t val)
{
  *(EPIPHANY_REG(reg_addr)) = val;
}

/*  Epiphany uses 12 bits for defining core IDs, while RTEMS uses 
 *  linear IDs. The following function converts RTEMS linear IDs to
 *  Epiphany correspodning ones
 */ 
static inline uint32_t rtems_coreid_to_epiphany_map(uint32_t rtems_id)
{
  switch(rtems_id)
  {
    case (0) : return 0x808;  /* (32, 8) */
    case (1) : return 0x809;  /* (32, 9) */
    case (2) : return 0x80A;  /* (32,10) */
    case (3) : return 0x80B;  /* (32,11) */
    case (4) : return 0x848;  /* (33, 8) */
    case (5) : return 0x849;  /* (33, 9) */
    case (6) : return 0x84A;  /* (33,10) */
    case (7) : return 0x84B;  /* (33,11) */
    case (8) : return 0x888;  /* (34, 8) */
    case (9) : return 0x889;  /* (34, 9) */
    case (10): return 0x88A; /* (34,10) */
    case (11): return 0x88B; /* (34,11) */
    case (12): return 0x8C8; /* (35, 8) */
    case (13): return 0x8C9; /* (35, 9) */
    case (14): return 0x8CA; /* (35,10) */
    case (15): return 0x8CB; /* (35,11) */
    default: return 0x1000; /* Error */
  }
  
  return 0x1000; /* Error */
}

/* Epiphany uses 12 bits for defining core IDs, while RTEMS uses 
 * linear IDs. The following function is used to map Epiphany IDs to
 * RTEMS linear IDs.
 */
inline uint32_t epiphany_coreid_to_rtems_map(uint32_t epiphany_id)
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

static inline uint32_t _Epiphany_Get_Current_coreid()
{
   uint32_t coreid;
   
   asm volatile ("movfs %0, coreid" : "=r" (coreid): );
   
   return epiphany_coreid_to_rtems_map(coreid);
}
#endif  /* _EPIPHANY_UTILITY_H */
