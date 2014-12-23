/* Adapteva epiphany-core implementation of stdio support functions ()

   Copyright (c) 2011, 2012 Adapteva, Inc.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Adapteva nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. 

/*
 *  COPYRIGHT (c) 1989-2004.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <bsp.h>
//#include <bsp/console-polled.h>
//#include <rtems/libio.h>
#include <stdlib.h>
//#include <assert.h>
#include <stdio.h>

/*
 *  console_initialize_hardware
 *
 *  This routine initializes the console hardware.
 *
 */

/* external prototypes for monitor interface routines */

#define PARALLELLA_CTRL_BASE 0x8F800000

#define PARALLELLA_CONSOLE_REG_CTRL  (PARALLELLA_CTRL_BASE + 0)
#define PARALLELLA_CONSOLE_REG_DATA  (PARALLELLA_CTRL_BASE + 4)

#define PARALLELLA_CONSOLE_REG_CTRL_BIT_BUSY  0x1
#define PARALLELLA_CONSOLE_REG_CTRL_BIT_SEND  0x2
#define PARALLELLA_CONSOLE_REG_CTRL_BIT_RECV  0x4

#define PARALLELLA_REG(reg) (int *) (reg)

static void outbyte_console( char ) __attribute__ ((section(".console_drv")));
static char inbyte_console( void );
static void Parallella_output_char(char c) __attribute__ ((section(".console_drv")));
/*void console_initialize_hardware(void)
{
  *(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_CTRL)) = 0;
  *(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_DATA)) = 0;
}*/

/*static char asm_read(int CHAN, void *ADDR, int LEN)
{
	register int chan asm("r0") = CHAN;
	register void* addr asm("r1") = ADDR;
	register int len asm("r2") = LEN;
	register int result asm("r0");
	register int error asm("r3");
	asm ("trap 1" : "=r" (result), "=r" (error) :
	     "r" (chan), "r" (addr), "r" (len));

	return (char) result;
	
}*/

static void outbyte_console(char c)
{
  asm volatile ("gid");
  //(*(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_DATA)))++;
  while( *(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_CTRL))
         & PARALLELLA_CONSOLE_REG_CTRL_BIT_BUSY
       );
  
  /* Send character */
  *(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_DATA)) = (int) c;
  
  (*(PARALLELLA_REG(PARALLELLA_CONSOLE_REG_CTRL))) |= 
  PARALLELLA_CONSOLE_REG_CTRL_BIT_SEND;
  /* Signal Zynq that there is available data */
  
  asm volatile ("gie");
  //asm_write (STDOUT_FILENO, &c, 1);
}

/*static char inbyte_console( void )
{
  //char c = asm_read (STDIN_FILENO, &c, 1);
  //return c;
}
*/
/*
 *  console_outbyte_polled
 *
 *  This routine transmits a character using polling.
 */

/*void console_outbyte_polled(
  int  port,
  char ch
)
{
  outbyte_console( ch );
}
*/

/*
 *  console_inbyte_nonblocking
 *
 *  This routine polls for a character.
 */

/*int console_inbyte_nonblocking(
  int port
)
{
  char c;

  c = inbyte_console();
  if (!c)
    return -1;
  return c;
}*/

/*ssize_t write_parallella(
  int         minor,
  const char *bufarg,
  size_t      len     
)
{
  int nwrite = 0;
  const char *buf = bufarg; 

  while (nwrite < len) {
    outbyte_console( *buf++ );
    nwrite++;
  }
  return nwrite; 
}*/

/*
 *  To support printk
 */

#include <rtems/bspIo.h>

static void Parallella_output_char(char c) { outbyte_console(c ); }

BSP_output_char_function_type           BSP_output_char = Parallella_output_char;
BSP_polling_getchar_function_type       BSP_poll_char = NULL;
