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

#include <rtems/score/schedulerpfairsmpimpl.h>
#include <rtems/score/rbtree.h>

int _Scheduler_pfair_SMP_Priority_compare (
  Priority_Control p1,
  Priority_Control p2
)
{
  Watchdog_Interval time = _Watchdog_Ticks_since_boot;

  /*
   * Reorder priorities to separate deadline driven and background tasks.
   *
   * The background tasks have p1 or p2 > SCHEDULER_PFAIR_PRIO_MSB.
   * The deadline driven tasks need to have subtracted current time in order
   * to see which deadline is closer wrt. current time.
   */
  if (!(p1 & SCHEDULER_PFAIR_PRIO_MSB))
    p1 = (p1 - time) & ~SCHEDULER_PFAIR_PRIO_MSB;
  if (!(p2 & SCHEDULER_PFAIR_PRIO_MSB))
    p2 = (p2 - time) & ~SCHEDULER_PFAIR_PRIO_MSB;

  return ((p1<p2) - (p1>p2));
}

RBTree_Compare_result _Scheduler_pfair_SMP_Compare(
  const RBTree_Node* n1,
  const RBTree_Node* n2
)
{
  Scheduler_pfair_SMP_Node *pfair1 =
    RTEMS_CONTAINER_OF( n1, Scheduler_pfair_SMP_Node, Node );
  Scheduler_pfair_SMP_Node *pfair2 =
    RTEMS_CONTAINER_OF( n2, Scheduler_pfair_SMP_Node, Node );
  Priority_Control value1 = pfair1->thread->current_priority;
  Priority_Control value2 = pfair2->thread->current_priority;

  /*
   * This function compares only numbers for the red-black tree,
   * but priorities have an opposite sense.
   */
  return (-1)*_Scheduler_pfair_SMP_Priority_compare(value1, value2);
}

static Scheduler_pfair_SMP_Context *
_Scheduler_pfair_SMP_Get_context( const Scheduler_Control *scheduler )
{
  return (Scheduler_pfair_SMP_Context *) _Scheduler_Get_context( scheduler );
}

void _Scheduler_pfair_SMP_Initialize( const Scheduler_Control *scheduler )
{
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_context( scheduler );

  _Scheduler_SMP_Initialize( &self->Base );
  _RBTree_Initialize_empty( &context->Ready );
}

void _Scheduler_pfair_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_SMP_Node *node = _Scheduler_SMP_Thread_get_own_node( thread );

  _Scheduler_SMP_Node_initialize( node, thread );
}

void _Scheduler_pfair_SMP_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control *thread,
  Priority_Control new_priority
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );
  Scheduler_Node *node = _Scheduler_Thread_get_node( thread );

  _Scheduler_pfair_SMP_Do_update( context, node, new_priority );
}

static Scheduler_Node *_Scheduler_pfair_SMP_Get_highest_ready(
  Scheduler_Context *context,
  Scheduler_Node    *node
)
{
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( context );

  (void) node;

  return (Scheduler_Node *) _Scheduler_priority_Ready_queue_first(
    &self->Bit_map,
    &self->Ready[ 0 ]
  );
}

void _Scheduler_pfair_SMP_Block(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  _Scheduler_SMP_Block(
    context,
    thread,
    _Scheduler_pfair_SMP_Extract_from_ready,
    _Scheduler_pfair_SMP_Get_highest_ready,
    _Scheduler_pfair_SMP_Move_from_ready_to_scheduled,
    _Scheduler_SMP_Allocate_processor_lazy
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_ordered(
  Scheduler_Context    *context,
  Scheduler_Node       *node,
  Thread_Control       *needs_help,
  Chain_Node_order      order,
  Scheduler_SMP_Insert  insert_ready,
  Scheduler_SMP_Insert  insert_scheduled
)
{
  return _Scheduler_SMP_Enqueue_ordered(
    context,
    node,
    needs_help,
    order,
    insert_ready,
    insert_scheduled,
    _Scheduler_pfair_SMP_Move_from_scheduled_to_ready,
    _Scheduler_SMP_Get_lowest_scheduled,
    _Scheduler_SMP_Allocate_processor_lazy
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_lifo(
  Scheduler_Context *context,
  Scheduler_Node    *node,
  Thread_Control    *needs_help
)
{
  return _Scheduler_pfair_SMP_Enqueue_ordered(
    context,
    node,
    needs_help,
    _Scheduler_SMP_Insert_priority_lifo_order,
    _Scheduler_pfair_SMP_Insert_ready_lifo,
    _Scheduler_SMP_Insert_scheduled_lifo
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_fifo(
  Scheduler_Context *context,
  Scheduler_Node    *node,
  Thread_Control    *needs_help
)
{
  return _Scheduler_pfair_SMP_Enqueue_ordered(
    context,
    node,
    needs_help,
    _Scheduler_SMP_Insert_priority_fifo_order,
    _Scheduler_pfair_SMP_Insert_ready_fifo,
    _Scheduler_SMP_Insert_scheduled_fifo
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_scheduled_ordered(
  Scheduler_Context *context,
  Scheduler_Node *node,
  Chain_Node_order order,
  Scheduler_SMP_Insert insert_ready,
  Scheduler_SMP_Insert insert_scheduled
)
{
  return _Scheduler_SMP_Enqueue_scheduled_ordered(
    context,
    node,
    order,
    _Scheduler_pfair_SMP_Extract_from_ready,
    _Scheduler_pfair_SMP_Get_highest_ready,
    insert_ready,
    insert_scheduled,
    _Scheduler_pfair_SMP_Move_from_ready_to_scheduled,
    _Scheduler_SMP_Allocate_processor_lazy
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_scheduled_lifo(
  Scheduler_Context *context,
  Scheduler_Node *node
)
{
  return _Scheduler_pfair_SMP_Enqueue_scheduled_ordered(
    context,
    node,
    _Scheduler_SMP_Insert_priority_lifo_order,
    _Scheduler_pfair_SMP_Insert_ready_lifo,
    _Scheduler_SMP_Insert_scheduled_lifo
  );
}

static Thread_Control *_Scheduler_pfair_SMP_Enqueue_scheduled_fifo(
  Scheduler_Context *context,
  Scheduler_Node *node
)
{
  return _Scheduler_pfair_SMP_Enqueue_scheduled_ordered(
    context,
    node,
    _Scheduler_SMP_Insert_priority_fifo_order,
    _Scheduler_pfair_SMP_Insert_ready_fifo,
    _Scheduler_SMP_Insert_scheduled_fifo
  );
}

Thread_Control *_Scheduler_pfair_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  return _Scheduler_SMP_Unblock(
    context,
    thread,
    _Scheduler_pfair_SMP_Enqueue_fifo
  );
}

Thread_Control *_Scheduler_pfair_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Priority_Control         new_priority,
  bool                     prepend_it
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  return _Scheduler_SMP_Change_priority(
    context,
    thread,
    new_priority,
    prepend_it,
    _Scheduler_pfair_SMP_Extract_from_ready,
    _Scheduler_pfair_SMP_Do_update,
    _Scheduler_pfair_SMP_Enqueue_fifo,
    _Scheduler_pfair_SMP_Enqueue_lifo,
    _Scheduler_pfair_SMP_Enqueue_scheduled_fifo,
    _Scheduler_pfair_SMP_Enqueue_scheduled_lifo
  );
}

Thread_Control *_Scheduler_pfair_SMP_Ask_for_help(
  const Scheduler_Control *scheduler,
  Thread_Control          *offers_help,
  Thread_Control          *needs_help
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  return _Scheduler_SMP_Ask_for_help(
    context,
    offers_help,
    needs_help,
    _Scheduler_pfair_SMP_Enqueue_fifo
  );
}

Thread_Control *_Scheduler_pfair_SMP_Yield(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  return _Scheduler_SMP_Yield(
    context,
    thread,
    _Scheduler_pfair_SMP_Extract_from_ready,
    _Scheduler_pfair_SMP_Enqueue_fifo,
    _Scheduler_pfair_SMP_Enqueue_scheduled_fifo
  );
}

Scheduler_Void_or_thread _Scheduler_pfair_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority,
  bool                     prepend_it
)
{
  Scheduler_EDF_Context *context =
    _Scheduler_EDF_Get_context( scheduler );
  Scheduler_EDF_Node *node = _Scheduler_EDF_Thread_get_node( the_thread );

  _RBTree_Extract( &context->Ready, &node->Node );
  _RBTree_Insert(
    &context->Ready,
    &node->Node,
    _Scheduler_EDF_Compare,
    false
  );  

  SCHEDULER_RETURN_VOID_OR_NULL;
}

