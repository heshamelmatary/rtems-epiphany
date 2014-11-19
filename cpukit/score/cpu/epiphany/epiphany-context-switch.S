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

.text
.align 4

PUBLIC(_CPU_Context_switch)
PUBLIC(_CPU_Context_restore)
PUBLIC(_CPU_Context_restore_fp)
PUBLIC(_CPU_Context_save_fp)

SYM(_CPU_Context_switch):
  strd lr,[r0]
  strd r4,[r0,1]
  strd r6,[r0,2]
  strd r8,[r0,3]
  strd r10,[r0,4]
  strd r32,[r0,5]
  strd r34,[r0,6]
  strd r36,[r0,7]
  strd r38,[r0,8]
  str sp,[r0,18]
  movfs r63,status
  str r63, [r0,64]

SYM(restore):

  ldrd lr,[r1] ; return address / r15
  ldrd r4,[r1,1]
  ldrd r6,[r1,2]
  ldrd r8,[r1,3]
  ldrd r10,[r1,4]
  ldrd r32,[r1,5]
  ldrd r34,[r1,6]
  ldrd r36,[r1,7]
  ldrd r38,[r1,8]
  ldr sp,[r1,18]
  ldr r63,[r1,63]
  movts config, r63
  
  jr lr

 SYM(_CPU_Context_restore):
  mov     r1, r0
  b       restore
  nop

 SYM(_CPU_Context_restore_fp):
  nop

 SYM(_CPU_Context_save_fp):
  nop
