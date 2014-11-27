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

  .extern bsp_start
  .extern bsp_section_bss_begin
  .extern bsp_section_bss_end

  .extern  bsp_start_vector_table_end
  .extern  bsp_start_vector_table_size
  .extern  bsp_vector_table_size
  .extern  bsp_section_stack_begin
  
  .global .normal_start;
	.global _external_start;
	.global .sw_exception;
	
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
         b .normal_start

		.balign 4 ; 0x10
         b .normal_start

		.balign 4 ; 0x14 
		 b .normal_start

		.balign 4 ; 0x18
		 b .normal_start

		.balign 4 ; 0x1c
		 b .normal_start

		.balign 4 ; 0x20
		 b .normal_start

		.balign 4 ; 0x24
		 b .normal_start

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
	mov	sp,%low(bsp_section_stack_begin)
	movt	sp,%high(bsp_section_stack_begin)
	mov	fp,sp

	;; Zero the data space
	mov	r0,%low(bsp_section_bss_begin)
	movt	r0,%high(bsp_section_bss_begin)
	mov	r1,%low(bsp_section_bss_end)
	movt	r1,%high(bsp_section_bss_end)
	mov	r2,#0
	mov	r3,#0
.L0_init_:
	strd	r2,[r0],+#1
	sub	r5,r1,r0
	bne	.L0_init_

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
