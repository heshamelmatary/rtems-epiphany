/*
  File: e_irq_clear.c

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
#include <rtems/score/e_coreid.h>
#include <rtems/score/e_ic.h>

void e_irq_clear(unsigned row, unsigned col, e_irq_type_t irq)
{
	unsigned *ilatcl;

//	if ((row == E_SELF) || (col == E_SELF))
//		ilatcl = (unsigned *) E_ILATCL;
//	else
	ilatcl = (unsigned *) e_get_global_address(row, col, (void *) E_REG_ILATCL);

	*ilatcl = 1 << (irq - E_SYNC);

	return;
}
