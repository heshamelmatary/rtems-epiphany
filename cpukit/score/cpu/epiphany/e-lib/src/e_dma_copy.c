/*
  File: e_dma_copy.c

  This file is part of the Epiphany Software Development Kit.

  Copyright (C) 2013 Adapteva, Inc.
  See AUTHORS for list of contributors.
  Support e-mail: <support@adapteva.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License (LGPL)
  as published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  and the GNU Lesser General Public License along with this program,
  see the files COPYING and COPYING.LESSER.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#include <rtems/score/e_common.h>
#include <rtems/score/e_dma.h>


e_dma_desc_t _dma_copy_descriptor_ SECTION(".data_bank0");

unsigned dma_data_size[8] =
{
	E_DMA_DWORD,
	E_DMA_BYTE,
	E_DMA_HWORD,
	E_DMA_BYTE,
	E_DMA_WORD,
	E_DMA_BYTE,
	E_DMA_HWORD,
	E_DMA_BYTE,
};

#define local_mask (0xfff00000)


int e_dma_copy(void *dst, void *src, size_t n)
{
	e_dma_id_t chan;
	unsigned   index;
	unsigned   shift;
	unsigned   stride;
	unsigned   config;
	int        ret_val;

	chan  = E_DMA_1;

	index = (((unsigned) dst) | ((unsigned) src) | ((unsigned) n)) & 7;

	config = E_DMA_MASTER | E_DMA_ENABLE | dma_data_size[index];
	if ((((unsigned) dst) & local_mask) == 0)
		config = config | E_DMA_MSGMODE;
	shift = dma_data_size[index] >> 5;
	stride = 0x10001 << shift;

	// TODO: add e_dma_wait()!!!
	_dma_copy_descriptor_.config       = config;
	_dma_copy_descriptor_.inner_stride = stride;
	_dma_copy_descriptor_.count        = 0x10000 | (n >> shift);
	_dma_copy_descriptor_.outer_stride = stride;
	_dma_copy_descriptor_.src_addr     = src;
	_dma_copy_descriptor_.dst_addr     = dst;

	ret_val = e_dma_start(&_dma_copy_descriptor_, chan);

	while (e_dma_busy(chan));

	return ret_val;
}

