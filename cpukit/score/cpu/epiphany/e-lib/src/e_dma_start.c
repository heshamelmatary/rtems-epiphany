/*
  File: e_dma_start.c

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

#include <rtems/score/e_regs.h>
#include <rtems/score/e_types.h>
#include <rtems/score/e_dma.h>

int e_dma_start(e_dma_desc_t *descriptor, e_dma_id_t chan)
{
	unsigned        start;
	e_return_stat_t ret_val;

	ret_val = E_ERR;

	if ((chan | 1) != 1)
	{
		return E_ERR;
	}

	/* wait for the DMA engine to be idle */
	while (e_dma_busy(chan));

	start = ((int)(descriptor) << 16) | E_DMA_STARTUP;

	switch (chan)
	{
	case E_DMA_0:
		e_reg_write(E_REG_DMA0CONFIG, start);
		ret_val = E_OK;
		break;
	case E_DMA_1:
		e_reg_write(E_REG_DMA1CONFIG, start);
		ret_val = E_OK;
		break;
	}

	return ret_val;
}
