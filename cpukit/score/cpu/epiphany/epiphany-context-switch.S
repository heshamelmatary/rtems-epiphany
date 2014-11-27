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

.section .text,"a";
.align 4

PUBLIC(_CPU_Context_switch)
PUBLIC(_CPU_Context_restore)
PUBLIC(_CPU_Context_restore_fp)
PUBLIC(_CPU_Context_save_fp)

SYM(_CPU_Context_switch):
  
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
  str r32,[r0,32]
  str r34,[r0,34]
  str r36,[r0,36]
  str r38,[r0,38]
  
  movfs r1,status
  str r1, [r0,64]
  

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
  ldr r32,[r1,32]
  ldr r34,[r1,34]
  ldr r36,[r1,36]
  ldr r38,[r1,38]
  
  ldr r0,[r1,64]
  movts config, r0
  
  ldr r0,[r1]
  ldr r1,[r1,1]
  
  jr lr

 SYM(_CPU_Context_restore):
  mov     r1, r0
  b       _restore
  nop

 SYM(_CPU_Context_restore_fp):
  nop

 SYM(_CPU_Context_save_fp):
  nop
