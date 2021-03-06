/**
 * @file
 *
 * @ingroup ScoreCPU
 *
 * @brief Epiphany exception support implementation.
 */

/*
 *  COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/cpu.h>
#include <rtems/asm.h>
#include <rtems/score/percpu.h>

.extern bsp_start_vector_table_begin
.extern __Thread_Dispatch

.section .text,"a";
.align 4

.global _ISR_Handler;
PUBLIC(_ISR_Handler)
.type    _ISR_Handler,%function

//#define ISR_NEST_LEVEL SYM(_Per_CPU_Information)

  /*
   *  If this CPU target lets RTEMS allocates the interrupt stack, then
   *  we need to have places in the per CPU table to hold them.
   */
#define PER_CPU_CONTROL_SIZE_LOG2 0x7
#define CPU_PER_CPU_CONTROL_SIZE 0
#define CPU_SIZEOF_POINTER 4

  #define PER_CPU_INTERRUPT_STACK_LOW \
    CPU_PER_CPU_CONTROL_SIZE
  #define PER_CPU_INTERRUPT_STACK_HIGH \
    PER_CPU_INTERRUPT_STACK_LOW + CPU_SIZEOF_POINTER
  #define PER_CPU_END_STACK             \
    PER_CPU_INTERRUPT_STACK_HIGH + CPU_SIZEOF_POINTER

  #define INTERRUPT_STACK_LOW \
    (SYM(_Per_CPU_Information) + PER_CPU_INTERRUPT_STACK_LOW)
  #define INTERRUPT_STACK_HIGH \
    (SYM(_Per_CPU_Information) + PER_CPU_INTERRUPT_STACK_HIGH)

/*
 *  These are the offsets of the required elements in the per CPU table.
 */
#define PER_CPU_ISR_NEST_LEVEL \
  PER_CPU_END_STACK
#define PER_CPU_THREAD_DISPATCH_DISABLE_LEVEL \
  PER_CPU_ISR_NEST_LEVEL + 4
#define PER_CPU_OFFSET_EXECUTING \
  PER_CPU_THREAD_DISPATCH_DISABLE_LEVEL + 4
#define PER_CPU_OFFSET_HEIR \
  PER_CPU_OFFSET_EXECUTING + CPU_SIZEOF_POINTER
#define PER_CPU_DISPATCH_NEEDED \
  PER_CPU_OFFSET_HEIR + CPU_SIZEOF_POINTER

#define THREAD_DISPATCH_DISABLE_LEVEL \
  (SYM(_Per_CPU_Information) + PER_CPU_THREAD_DISPATCH_DISABLE_LEVEL)
#define ISR_NEST_LEVEL \
  (SYM(_Per_CPU_Information) + PER_CPU_ISR_NEST_LEVEL)
#define DISPATCH_NEEDED \
  (SYM(_Per_CPU_Information) + PER_CPU_DISPATCH_NEEDED)

_ISR_Handler:

  //movfs r63, ctimer1
  /* Reserve space for CPU_Exception_frame */ 
  add sp, sp, #-280
  
  str r0,[sp]
  str r1,[sp,1]
  
  str r2,[sp,2]
  
  movfs r1,config
  str r1, [sp,65]
  
  str r3,[sp,3]
  str r4,[sp,4]
  str r5,[sp,5]
  str r6,[sp,6]
  str r7,[sp,7]
  str r8,[sp,8]
  str r9,[sp,9]
  str r10,[sp,10]
  str fp,[sp,11]
  str r12,[sp,12]
  str lr,[sp,14]
  
  str r15,[sp,15]
  str r16,[sp,16]
  str r17,[sp,17]
  str r18,[sp,18]
  str r19,[sp,19]
  str r20,[sp,20]
  str r21,[sp,21]
  str r22,[sp,22]
  str r23,[sp,23]
  str r24,[sp,24]
  str r25,[sp,25]
  str r26,[sp,26]
  str r27,[sp,27]
  
  str r28,[sp,28]
  str r29,[sp,29]
  str r30,[sp,30]
  str r31,[sp,31]
  str r32,[sp,32]
  str r33,[sp,33]
  str r34,[sp,34]
  str r35,[sp,35]
  str r36,[sp,36]
  str r37,[sp,37]
  str r38,[sp,38]
  str r39,[sp,39]
  str r40,[sp,40]
  str r41,[sp,41]
  str r42,[sp,42]
  str r43,[sp,43]
  str r44,[sp,44]
  str r45,[sp,45]
  str r46,[sp,46]
  str r47,[sp,47]
  str r48,[sp,48]
  str r49,[sp,49]
  str r50,[sp,50]
  str r51,[sp,51]
  str r52,[sp,52]
  str r53,[sp,53]
  str r54,[sp,54]
  str r55,[sp,55]
  str r56,[sp,56]
  str r57,[sp,57]
  str r58,[sp,58]
  str r59,[sp,59]
  str r60,[sp,60]
  str r61,[sp,61]
  str r62,[sp,62] ;; /* FIXME: Currently holds vector number */
  str r63,[sp,63] ;; /* FIXME: Currently hold _ISR_Handler addr */
  
  movfs r1,status
  str r1, [sp,64]
  
  movfs r1,iret
  str r1, [sp,66]
  
  
  /* Save interrupted task stack pointer */
  add r1, sp, #288
  str r1,[sp,13]
 
  mov  r18, %low(__Per_CPU_Information)
	movt r18, %high(__Per_CPU_Information)
	
#ifdef RTEMS_SMP 
   movfs r17, coreid
   
   /* Mapping from Epiphany IDs to 0-16 IDs macro */
   mov   r19, #0x003
   mov   r20, #0x0F0
   and   r19, r17, r19
   and   r20, r17, r20
   lsr   r20, r20, #4 
   add   r17, r19, r20
   /* Mapping from Epiphany IDs to 0-16 IDs macro */
   
   lsl  r17, r17, #(PER_CPU_CONTROL_SIZE_LOG2)
   add  r18, r18, r17
#endif
   
	 
	 add  r6, r18, #(PER_CPU_ISR_NEST_LEVEL)
	 add  r8, r18, #(PER_CPU_THREAD_DISPATCH_DISABLE_LEVEL)
  
  ldr     r5, [r6]
  ldr     r7, [r8]
  add     r5, r5, #1
  add     r7, r7, #1
  str     r5, [r6]
  str     r7, [r8]
  
  
  /* Keep sp (Exception frame address) in r32 - Callee saved */
  mov   r32, sp
  
  /* Keep __Per_CPU_Information address in r33 - Callee saved */
  mov   r33, r18
  
  /* Call the exception handler from vector table */

  /* First function arg for C handler is vector number,
   * and the second is a pointer to exception frame.
   */
  mov    r0,  r62
  mov    r27, r62
  mov    r2,  sp  ;; /* The compiler expects second arg in r2! */
  lsl    r27, r27, #2
  mov    r26, %low(_bsp_start_vector_table_begin)
  movt   r15, #0
  add    r27, r27, r26
  ldr    r27, [r27]
  
  /* Do not switch stacks if we are in a nested interrupt. At
   * this point r5 should be holding ISR_NEST_LEVEL value.
   */
  
  sub    r37, r5, #1
  bgtu    jump_to_c_handler
  
   /* Switch to RTEMS dedicated interrupt stack */
  add     sp, r18, #(PER_CPU_INTERRUPT_STACK_HIGH)
  ldr     sp, [sp]
  
jump_to_c_handler:
  /* Red zone ? */
  //add sp, sp, #-128
  jalr r27


  /* Switch back to the interrupted task stack */
  mov  sp, r32
  
  /* Get the address of __Per_CPU_Information */
  mov r18, r33
  
  /* Decrement nesting level */
  /* Enable multitasking */
  add  r6, r18, #(PER_CPU_ISR_NEST_LEVEL)
	add  r8, r18, #(PER_CPU_THREAD_DISPATCH_DISABLE_LEVEL)
  
  ldr     r5, [r6]
  ldr     r7, [r8]
  sub     r5, r5, #1
  sub     r7, r7, #1
  str     r5, [r6]
  str     r7, [r8]
  
  /* Check if _ISR_Nest_level > 0 */ 
  sub     r37, r5, #0
  bgtu    exception_frame_restore

  /* Check if _Thread_Dispatch_disable_level > 0 */
  sub     r37, r7, #0
  bgtu    exception_frame_restore

  /* Check if dispatch needed */
  add     r31, r18, #(PER_CPU_DISPATCH_NEEDED)
  ldr     r31, [r31]
  
  sub     r35, r31, #0
  beq     exception_frame_restore
  
  mov     r35, %low(__Thread_Dispatch)
  movt    r35, %high(__Thread_Dispatch)
  jalr    r35
  
exception_frame_restore:
  
  ldr r1,[sp,1]
  
  ldr r3,[sp,3]
  ldr r4,[sp,4]
  ldr r5,[sp,5]
  ldr r6,[sp,6]
  ldr r7,[sp,7]
  ldr r8,[sp,8]
  ldr r9,[sp,9]
  ldr r10,[sp,10]
  ldr fp,[sp,11]
  ldr r12,[sp,12]

  ldr lr,[sp,14]

  ldr r15,[sp,15]
  ldr r16,[sp,16]
  ldr r17,[sp,17]
  ldr r18,[sp,18]
  ldr r19,[sp,19]
  ldr r20,[sp,20]
  ldr r21,[sp,21]
  ldr r22,[sp,22]
  ldr r23,[sp,23]
  ldr r24,[sp,24]
  ldr r25,[sp,25]
  ldr r26,[sp,26]
  ldr r27,[sp,27]
  
  ldr r28,[sp,28]
  ldr r29,[sp,29]
  ldr r30,[sp,30]
  ldr r31,[sp,31]
  ldr r32,[sp,32]
  ldr r34,[sp,34]
  ldr r36,[sp,36]
  ldr r38,[sp,38]
  
  ldr r39,[sp,39]
  ldr r40,[sp,40]
  ldr r41,[sp,41]
  ldr r42,[sp,42]
  ldr r43,[sp,43]
  
  ldr r44,[sp,44]
  ldr r45,[sp,45]
  ldr r46,[sp,46]
  ldr r47,[sp,47]
  ldr r48,[sp,48]
  ldr r49,[sp,49]
  ldr r50,[sp,50]
  ldr r51,[sp,51]
  ldr r52,[sp,52]
  ldr r53,[sp,53]
  ldr r54,[sp,54]
  ldr r55,[sp,55]
  ldr r56,[sp,56]
  ldr r57,[sp,57]
  ldr r58,[sp,58]
  ldr r59,[sp,59]
  ldr r60,[sp,60]
  ldr r61,[sp,61]
  ldr r62,[sp,62]
  ldr r63,[sp,63]
  
  ldr r0,[sp,64]
  movts status, r0
  
  ldr r0, [sp,65]
  movts config, r0
  
  ldr   r0, [sp,66]
  movts iret, r0
  
  ldr r0,[sp]
  
  ldr r2,[sp,2]
  
  /* Unwind exception frame */
  add sp, sp, #280
  
  ldr r62, [sp, #4]
  ldr r62, [sp, #4]
  //ldr r63, [sp, #8]
  
  add sp, sp, #8
  
  //movfs r62, ctimer1
  //sub   r63, r63, r62

  rti
