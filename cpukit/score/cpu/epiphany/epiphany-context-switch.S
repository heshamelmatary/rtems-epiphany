/*
 * COPYRIGHT (c) 2014 Hesham ALMatary <heshamelmatary@gmail.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/asm.h>
#include <rtems/score/percpu.h>

#define PER_CPU_CONTROL_SIZE_LOG2 0x7
#define CPU_PER_CPU_CONTROL_SIZE 0
#define CPU_SIZEOF_POINTER 4

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

#define DISPATCH_NEEDED \
  (SYM(_Per_CPU_Information) + PER_CPU_DISPATCH_NEEDED)

#define HEIR \
  (SYM_Per_CPU_Information() + PER_CPU_OFFSET_HEIR)
  
.section .bsp_start_text,"a";
.align 4

PUBLIC(_CPU_Context_switch)
PUBLIC(_CPU_Context_restore)
PUBLIC(_CPU_Context_restore_fp)
PUBLIC(_CPU_Context_save_fp)
.global restore;

SYM(_CPU_Context_switch):
  gid
  //movfs r63, ctimer1
  str r0,[r0]
  str r1,[r0,1]
  str r2,[r0,2]
  str r3,[r0,3]
  str r4,[r0,4]
  str r5,[r0,5]
  str r6,[r0,6]
  str r7,[r0,7]
  str r8,[r0,8]
  str r9,[r0,9]
  str r10,[r0,10]
  str fp,[r0,11]
  str r12,[r0,12]
  str sp,[r0,13]
  str lr,[r0,14]
  
  str r15,[r0,15]
  str r16,[r0,16]
  str r17,[r0,17]
  str r18,[r0,18]
  str r19,[r0,19]
  str r20,[r0,20]
  str r21,[r0,21]
  str r22,[r0,22]
  str r23,[r0,23]
  str r24,[r0,24]
  str r25,[r0,25]
  str r26,[r0,26]
  str r27,[r0,27]
  
  str r28,[r0,28]
  str r29,[r0,29]
  str r30,[r0,30]
  str r31,[r0,31]
  str r32,[r0,32]
  str r33,[r0,33]
  str r34,[r0,34]
  str r35,[r0,35]
  str r36,[r0,36]
  str r37,[r0,37]
  str r38,[r0,38]
  str r39,[r0,39]
  str r40,[r0,40]
  str r41,[r0,41]
  str r42,[r0,42]
  str r43,[r0,43]
  str r44,[r0,44]
  str r45,[r0,45]
  str r46,[r0,46]
  str r47,[r0,47]
  str r48,[r0,48]
  str r49,[r0,49]
  str r50,[r0,50]
  str r51,[r0,51]
  str r52,[r0,52]
  str r53,[r0,53]
  str r54,[r0,54]
  str r55,[r0,55]
  str r56,[r0,56]
  str r57,[r0,57]
  str r58,[r0,58]
  str r59,[r0,59]
  str r60,[r0,60]
  str r61,[r0,61]
  str r62,[r0,62]
  str r63,[r0,63]
  
  movfs r27,status
  str r27, [r0,64]
  
  movfs r27,config
  str r27, [r0,65]
  
  movfs r27,iret
  str r27, [r0,66]
  
#ifdef RTEMS_SMP
  /*
   *   executing->is_executing = false;
   *   memory_barrier();
   */
  mov r27, #0
  str r27, [r0,67]
 
  /* Check if heir is executing */
  ldr r27, [r1,67]
  sub r27, r27, #0
  
  bne _check_dispatch_needed
  
  b   smp_restore
  /* TODO 
     *   if ( test_and_set( &heir->is_executing ) ) {
     *     do {
     *       Per_CPU_Control *cpu_self = _Per_CPU_Get_snapshot();
     *
     *       if ( cpu_self->dispatch_necessary ) {
     *         heir = _Thread_Get_heir_and_make_it_executing( cpu_self );
     *       }
     *     } while ( test_and_set( &heir->is_executing ) );
     *   }
     *
     *   restore( heir );
     * }
     * @endcode
     */
  
_check_dispatch_needed:
    
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
   
   
 /* Check if dispatch needed */
  add     r31, r18, #(PER_CPU_DISPATCH_NEEDED)
  ldr     r32, [r31]
  
  sub     r35, r32, #0
  beq     smp_restore
  
  /* cpu_self->dispatch_necessary = false; */
  mov r27, #0 
  str r27, [r31]
  
  /* heir = cpu_self->heir; */
  add     r31, r18, #(PER_CPU_OFFSET_HEIR)
  ldr     r32, [r31]
  
  add     r31, r18, #(PER_CPU_OFFSET_EXECUTING)
  
  str     r32, [r31]
  
  mov  r1, r32

smp_restore:
  
  mov r27, #1
  str r27, [r1,67]
#endif
  
SYM(restore):

  ldr r2,[r1,2]
  ldr r3,[r1,3]
  ldr r4,[r1,4]
  ldr r5,[r1,5]
  ldr r6,[r1,6]
  ldr r7,[r1,7]
  ldr r8,[r1,8]
  ldr r9,[r1,9]
  ldr r10,[r1,10]
  ldr fp,[r1,11]
  ldr r12,[r1,12]
  ldr sp,[r1,13]
  ldr lr,[r1,14]
  
  ldr r15,[r1,15]
  ldr r16,[r1,16]
  ldr r17,[r1,17]
  ldr r18,[r1,18]
  ldr r19,[r1,19]
  ldr r20,[r1,20]
  ldr r21,[r1,21]
  ldr r22,[r1,22]
  ldr r23,[r1,23]
  ldr r24,[r1,24]
  ldr r25,[r1,25]
  ldr r26,[r1,26]
  ldr r27,[r1,27]
  
  ldr r32,[r1,32]
  ldr r33,[r1,33]
  ldr r34,[r1,34]
  ldr r35,[r1,35]
  ldr r36,[r1,36]
  ldr r37,[r1,37]
  ldr r38,[r1,38]
  ldr r39,[r1,39]
  ldr r40,[r1,40]
  ldr r41,[r1,41]
  ldr r42,[r1,42]
  ldr r43,[r1,43]
  
  ldr r44,[r1,44]
  ldr r45,[r1,45]
  ldr r46,[r1,46]
  ldr r47,[r1,47]
  ldr r48,[r1,48]
  ldr r49,[r1,49]
  ldr r50,[r1,50]
  ldr r51,[r1,51]
  ldr r52,[r1,52]
  ldr r53,[r1,53]
  ldr r54,[r1,54]
  ldr r55,[r1,55]
  ldr r56,[r1,56]
  ldr r57,[r1,57]
  ldr r58,[r1,58]
  ldr r59,[r1,59]
  ldr r60,[r1,60]
  ldr r61,[r1,61]
  ldr r62,[r1,62]
  ldr r63,[r1,63]
  
  ldr r0,[r1,64]
  movts status, r0
  
  ldr r0,[r1,65]
  //movts config, r0
  
  ldr r0,[r1,66]
  movts iret, r0
  
  ldr r0,[r1]
  ldr r1,[r1,1]
  
  gie
  movfs r63, ctimer1
  jr lr

 SYM(_CPU_Context_restore):
  mov     r1, r0
  b       _restore
  nop

 SYM(_CPU_Context_restore_fp):
  nop

 SYM(_CPU_Context_save_fp):
  nop
