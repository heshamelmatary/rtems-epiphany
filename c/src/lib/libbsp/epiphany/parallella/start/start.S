# C Startup for EPIPHANY

# Copyright (c) 2011, Adapteva, Inc.
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of Adapteva nor the names of its contributors may be
#    used to endorse or promote products derived from this software without
#    specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

#include <bsp/linker-symbols.h>
#include <rtems/score/percpu.h>

#define CPU_SIZEOF_POINTER 4
#define PRE_ISR_HANDLER(interrupt)  \
  gid;                               \
  add sp, sp, #-8;                   \
  str r62, [sp, #4];                 \
  str r63, [sp, #8];                 \
  mov r62, #interrupt;               \
  mov r63, %low(_ISR_Handler);       \
  movt r63, %high(_ISR_Handler);     \
  jr  r63 
  
  .extern bsp_start
  .extern bsp_section_bss_begin
  .extern bsp_section_bss_end
  .extern _ISR_Handler
  .extern  _bsp_start_on_secondary_processor
  .extern  bsp_start_vector_table_end
  .extern  bsp_start_vector_table_size
  .extern  bsp_vector_table_size
  .extern  bsp_section_stack_begin
  .extern _bsp_start_vector_table_begin;
  .extern _Clock_isr;
  .global _EPIPHANY_Exception_default;
  .global _bsp_start_vector_table_begin;
  .global .normal_start;
	.global _external_start;
	.global .sw_exception;
	.global .clock_isr;
	.global __init;
	.global __fini;
	
	.type   _external_start, %function
	
.section .vector, "ax";
        .global _start;
        .type   _start, %function;
_start:
		.balign 4 ;
         b .normal_start

    .balign 4 ; 0x4
         b .sw_exception 

		.balign 4 ; 0x8
         b .normal_start

		.balign 4 ; 0xc
         b .clock_isr

		.balign 4 ; 0x10
         b .timer1_isr

		.balign 4 ; 0x14 
		    b .msg_int

		.balign 4 ; 0x18
		 b .normal_start

		.balign 4 ; 0x1c
		 b .normal_start

		.balign 4 ; 0x20
		 b .normal_start

		.balign 4 ; 0x24
		 b .normal_start

_bsp_start_vector_table_begin:

  .balign 4 ;
     .word .normal_start /* Reset */
  .balign 4 ; 
     .word _EPIPHANY_Exception_default /* SW exception */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* Data Page Fault */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* Timer 0 */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* Timer 1 */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* Message int */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* DMA0 int */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* DMA1 int */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* WAND */
  .balign 4 ;
     .word _EPIPHANY_Exception_default /* User interrupt */

.size  _start, .-_start

.section .start,"ax"     ;;

.balign 4
.type   .normal_start, %function
.normal_start:
	mov  r3,%low(_external_start)
	movt r3,%high(_external_start)
	jalr r3
.size  .normal_start, .-.normal_start

.align  4
.type   _external_start, %function
_external_start:

	;; Initialise the stack pointer and frame pointer.
	nop
	nop 
	mov	sp,%low(bsp_section_main_begin)
	movt	sp,%high(bsp_section_main_begin)
	mov	fp,sp

#ifdef RTEMS_SMP
  /* Check wether this is the boot processor (index 0)
   * or not, if it's not, just allocate stack for it idle.
   * and make it idle
   */
   movfs r17, coreid
   
   /* Mapping from Epiphany IDs to 0-16 IDs macro */
   mov   r19, #0x003
   mov   r20, #0x0F0
   and   r19, r17, r19
   and   r20, r17, r20
   lsr   r20, r20, #4  
   add   r17, r19, r20
   /* Mapping from Epiphany IDs to 0-16 IDs macro */
   
   mov   r16, #0
   sub   r16, r17, r16
   beq   cpu0
   
   mov  r18, %low(__Per_CPU_Information)
	 movt r18, %high(__Per_CPU_Information)
	
   /* Set up stack for seconday processor */
   lsl  r17, r17, #(PER_CPU_CONTROL_SIZE_LOG2)
   add  r18, r18, r17
   
   add  r18, r18, #(PER_CPU_INTERRUPT_STACK_HIGH)
   ldr  r18, [r18]
	 mov  sp, r18 /* Set processor's stack pointer */
	 mov  fp, sp  /* Set processor's frame pointer */
   
   /* Clear the reset interrupt flag */
  mov    r0, %low(.secondary_processor_start)
  movt   r0, %high(.secondary_processor_start)
  movts  iret, r0
  rti
  
.secondary_processor_start:
  mov  r0, %low(_bsp_start_on_secondary_processor)
  movt r0, %high(_bsp_start_on_secondary_processor)
  jr   r0 /* No return */
#endif

cpu0:
	;; Zero the data space
	mov	r0,%low(bsp_section_bss_begin)
	movt	r0,%high(bsp_section_bss_begin)
	mov	r1,%low(bsp_section_stack_begin)
	movt	r1,%high(bsp_section_stack_begin)
	mov	r2,#0
	mov	r3,#0
.L0_init_:
	strd	r2,[r0],+#1
	sub	r5,r1,r0
	bne	.L0_init_

  /* Clear the reset interrupt flag */
  mov    r0, %low(_jump_to_c)
  movt   r0, %high(_jump_to_c)
  movts  iret, r0
  rti
  
_jump_to_c:
  ;; jump to bootcard
	mov	r0,#0
	mov	r1,#0
	mov	r2,#0
	mov r3,%low(_boot_card)
	movt r3,%high(_boot_card)
	jalr r3
	;;bl	_boot_card

	;; Should never reach here
	idle

.size   _external_start, .-_external_start

.balign 4
.type   .sw_exception, %function
.sw_exception:
  idle
  
.balign 4
.type   .clock_isr, %function
.clock_isr:
  gid;                               
  add sp, sp, #-8;                   
  str r62, [sp, #4];                 
  str r63, [sp, #8];                 
  mov r62, 3;               
  mov r63, %low(_ISR_Handler);       
  movt r63, %high(_ISR_Handler);     
  jr  r63

.balign 4
.type   .timer1_isr, %function
.timer1_isr:
  gid;                               
  add sp, sp, #-8;                   
  str r62, [sp, #4];                 
  str r63, [sp, #8];                 
  mov r62, 4;               
  mov r63, %low(_ISR_Handler);       
  movt r63, %high(_ISR_Handler);     
  jr  r63
  
.balign 4
.type   .msg_int, %function
.msg_int:
  gid;                               
  add sp, sp, #-8;                   
  str r62, [sp, #4];                 
  str r63, [sp, #8];                 
  mov r62, 5;               
  mov r63, %low(_ISR_Handler);       
  movt r63, %high(_ISR_Handler);     
  jr  r63 

.balign 4
.type   _EPIPHANY_Exception_default, %function
_EPIPHANY_Exception_default:
  idle
 
.balign 4
.type   secondary_processor_wait, %function
secondary_processor_wait: 
 /* Clear the reset interrupt flag */
  mov    r0, %low(_wait)
  movt   r0, %high(_wait)
  movts  iret, r0
  rti
 _wait:
  idle
