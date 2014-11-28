/*
  File: e_ctimer_wait.c

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


#include <rtems/score/e_ctimers.h>

void e_wait(e_ctimer_id_t timer, unsigned int clicks)
{
	// Program ctimer and start counting
	e_ctimer_stop(timer);
	e_ctimer_set(timer, clicks);
	e_ctimer_start(timer, E_CTIMER_CLK);

	// Wait until ctimer is idle
	while (e_ctimer_get(timer)) { };

	return;
}
