/*
  File: e_mutex_barrier.c

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

#include <rtems/score/e_mutex.h>
#include <rtems/score/e_coreid.h>

#if 0
void e_barrier(e_barrier_t bar_array[], e_barrier_t *tgt_bar_array[])
{
	int corenum, numcores, i;

	numcores = e_group_config.group_rows * e_group_config.group_cols;
	corenum  = e_group_config.core_row * e_group_config.group_cols + e_group_config.core_col;

	// Barrier as a Flip-Flop

	// Flip pass
	if (corenum == 0)
	{
		// set "my" slot
		bar_array[corenum] = 1;
		// poll on all slots
		for (i=1; i<numcores; i++)
			while (bar_array[i] == 0) {};
	} else {
		// set "my" remote slot
		*(tgt_bar_array[corenum]) = 1;
	}

	// Flop pass
	if (corenum == 0)
	{
		// clear all local slots
		for (i=0; i<numcores; i++)
			bar_array[i] = 0;
		// set remote slots
		for (i=1; i<numcores; i++)
			*(tgt_bar_array[i]) = 1;
	} else {
		// poll on "my" local slot
		while (bar_array[corenum] == 0) {};
		// clear "my" local slot
		bar_array[corenum] = 0;
	}

	return;
}
#else
void e_barrier(volatile e_barrier_t bar_array[], e_barrier_t *tgt_bar_array[])
{
	int corenum, numcores, i;

	numcores = e_group_config.group_rows * e_group_config.group_cols;
	corenum  = e_group_config.core_row * e_group_config.group_cols + e_group_config.core_col;

	// Barrier as a Flip-Flop

	if (corenum == 0)
	{
		// Flip pass
		// set "my" slot
		bar_array[corenum] = 1;
		// poll on all slots
		for (i=1; i<numcores; i++)
			while (bar_array[i] == 0) {};

		// Flop pass
		// clear all local slots
		for (i=0; i<numcores; i++)
			bar_array[i] = 0;
		// set remote slots
		for (i=1; i<numcores; i++)
			*(tgt_bar_array[i]) = 1;
	} else {
		// Flip pass
		// set "my" remote slot
		*(tgt_bar_array[0]) = 1;

		// Flop pass
		// poll on "my" local slot
		while (bar_array[0] == 0) {};
		// clear "my" local slot
		bar_array[0] = 0;
	}

	return;
}
#endif
