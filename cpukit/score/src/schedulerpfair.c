/**
 * @file
 *
 * @brief Scheduler Pfair Initialize and Support
 * @ingroup ScoreScheduler
 */

/*
 *  Copyright (C) 2015 Hesham ALMatary.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulerpfairimpl.h>

void _Scheduler_pfair_Initialize( const Scheduler_Control *scheduler )
{
  Scheduler_pfair_Context *context =
    _Scheduler_pfair_Get_context( scheduler );

  _Priority_bit_map_Initialize( &context->Bit_map );
  _Scheduler_priority_Ready_queue_initialize( &context->Ready[ 0 ] );
}

void _Scheduler_pfair_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_SMP_Node *node = _Scheduler_SMP_Thread_get_own_node( thread );

  _Scheduler_SMP_Node_initialize( node, thread );
}
