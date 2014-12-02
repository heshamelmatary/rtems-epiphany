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
  gid
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
  
  movfs r1,status
  str r1, [r0,64]
  
  movfs r1,config
  str r1, [r0,65]
  
  movfs r1,iret
  str r1, [r0,66]
  

SYM(restore):
  
  ldr r1,[r2,1]
  ldr r3,[r2,3]
  ldr r4,[r2,4]
  ldr r5,[r2,5]
  ldr r6,[r2,6]
  ldr r7,[r2,7]
  ldr r8,[r2,8]
  ldr r9,[r2,9]
  ldr r10,[r2,10]
  ldr fp,[r2,11]
  ldr r12,[r2,12]
  ldr sp,[r2,13]
  ldr lr,[r2,14]
  
  ldr r15,[r2,15]
  ldr r16,[r2,16]
  ldr r17,[r2,17]
  ldr r18,[r2,18]
  ldr r19,[r2,19]
  ldr r20,[r2,20]
  ldr r21,[r2,21]
  ldr r22,[r2,22]
  ldr r23,[r2,23]
  ldr r24,[r2,24]
  ldr r25,[r2,25]
  ldr r26,[r2,26]
  ldr r27,[r2,27]
  
  ldr r32,[r2,32]
  ldr r33,[r2,33]
  ldr r34,[r2,34]
  ldr r35,[r2,35]
  ldr r36,[r2,36]
  ldr r37,[r2,37]
  ldr r38,[r2,38]
  ldr r39,[r2,39]
  ldr r40,[r2,40]
  ldr r41,[r2,41]
  ldr r42,[r2,42]
  ldr r43,[r2,43]
  
  ldr r44,[r2,44]
  ldr r45,[r2,45]
  ldr r46,[r2,46]
  ldr r47,[r2,47]
  ldr r48,[r2,48]
  ldr r49,[r2,49]
  ldr r50,[r2,50]
  ldr r51,[r2,51]
  ldr r52,[r2,52]
  ldr r53,[r2,53]
  ldr r54,[r2,54]
  ldr r55,[r2,55]
  ldr r56,[r2,56]
  ldr r57,[r2,57]
  ldr r58,[r2,58]
  ldr r59,[r2,59]
  ldr r60,[r2,60]
  ldr r61,[r2,61]
  ldr r62,[r2,62]
  ldr r63,[r2,63]
  
  ldr r0,[r2,64]
  movts status, r0
  
  ldr r0,[r2,65]
  //movts config, r0
  
  ldr r0,[r2,66]
  movts iret, r0
  
  ldr r0,[r2]
  ldr r2,[r2,2]
  
  gie
  
  jr lr

 SYM(_CPU_Context_restore):
  mov     r2, r0
  b       _restore
  nop

 SYM(_CPU_Context_restore_fp):
  nop

 SYM(_CPU_Context_save_fp):
  nop
