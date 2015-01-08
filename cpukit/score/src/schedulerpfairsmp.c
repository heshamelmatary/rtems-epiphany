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
#include <rtems/score/schedulersimple.h>
#include <rtems/score/rbtree.h>
#include <rtems/score/rbtreeimpl.h>
#include <rtems/score/thread.h>

//#include <rtems/score/schedulerpfairtypes.h>

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
  Thread_Control *thread1 =
    RTEMS_CONTAINER_OF( n1, Thread_Control, RBNode );
    //RTEMS_CONTAINER_OF( n1, Scheduler_pfair_SMP_Node, Node );
  Thread_Control *thread2 =
    RTEMS_CONTAINER_OF( n1, Thread_Control, RBNode );
    //RTEMS_CONTAINER_OF( n2, Scheduler_pfair_SMP_Node, Node );
  Priority_Control value1 = thread1->current_priority;
  Priority_Control value2 = thread2->current_priority;

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
  _RBTree_Initialize_empty( &self->Ready );
}

void _Scheduler_pfair_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_SMP_Node *node = _Scheduler_SMP_Thread_get_own_node( thread );

  _Scheduler_SMP_Node_initialize( node, thread );
}

static void _Scheduler_pfair_SMP_Allocate_processor(
  Thread_Control *scheduled,
  Thread_Control *victim
)
{
  Per_CPU_Control *cpu_of_scheduled = scheduled->Scheduler.cpu;
  Per_CPU_Control *cpu_of_victim = victim->Scheduler.cpu;
  Thread_Control *heir;

   _Per_CPU_Acquire( cpu_of_scheduled );

  //if ( scheduled->is_executing ) {
   // heir = cpu_of_scheduled->heir;
   // cpu_of_scheduled->heir = scheduled;
  //} else {
    heir = scheduled;
  //}

   _Per_CPU_Release( cpu_of_scheduled );

  if ( heir != victim ) {
      const Per_CPU_Control *cpu_of_executing = _Per_CPU_Get();
    heir->Scheduler.cpu = cpu_of_victim;
    cpu_of_victim->heir = heir;
    cpu_of_victim->dispatch_necessary = true;
    if ( cpu_of_victim != cpu_of_executing ) {
      _Per_CPU_Send_interrupt( cpu_of_victim );
    }
  }
}

static Thread_Control *_Scheduler_pfair_SMP_Get_lowest_scheduled(
  Scheduler_Context *context
)
{
  Scheduler_SMP_Context *self = _Scheduler_SMP_Get_self( context );
  Thread_Control *lowest_ready = NULL;
  Chain_Control *scheduled = &self->Scheduled;

  if ( !_Chain_Is_empty( scheduled ) ) {
    lowest_ready = (Thread_Control *) _Chain_Last( scheduled );
  }

  return lowest_ready;
}

static Thread_Control *_Scheduler_pfair_SMP_Get_highest_ready(
  Scheduler_Context *context
)
{
  Thread_Control *highest_ready = NULL;
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( context );
    
  //_SMP_lock_Acquire(&self->smp_lock_ready_queue);
  if ( !_RBTree_Is_empty(&self->Ready) ) {

    RBTree_Node *first = _RBTree_First( &self->Ready, RBT_LEFT );
  Thread_Control *highest_ready =
    RTEMS_CONTAINER_OF( first, Thread_Control, RBNode );

    //highest_ready = sched_info->thread;
  }
  //_SMP_lock_Release(&self->smp_lock_ready_queue);
}

static void _Scheduler_pfair_SMP_Move_from_scheduled_to_ready(
  Scheduler_Context *context,
  RBTree_Control *ready_chain,
  Thread_Control *scheduled_to_ready
)
{ 
   RBTree_Node *node = &(scheduled_to_ready->RBNode); 
   _Chain_Extract_unprotected( &scheduled_to_ready->Object.Node );
  //_SMP_lock_Acquire(&self->smp_lock_ready_queue);

  _RBTree_Insert( ready_chain, node, _Scheduler_pfair_SMP_Compare, false );
  //_SMP_lock_Release(&self->smp_lock_ready_queue);
}

static void _Scheduler_pfair_SMP_Move_from_ready_to_scheduled(
  Scheduler_Context *context,
  Chain_Control *scheduled_chain,
  Thread_Control *ready_to_scheduled
)
{
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( context );

  //_SMP_lock_Acquire(&self->smp_lock_ready_queue);
   RBTree_Node *node = &(ready_to_scheduled->RBNode);
   _RBTree_Extract(&self->Ready, node );
  //_SMP_lock_Release(&self->smp_lock_ready_queue);
   _Scheduler_simple_Insert_priority_fifo( scheduled_chain, ready_to_scheduled );
}

static void _Scheduler_pfair_SMP_Insert(
  RBTree_Control *chain,
  Thread_Control *thread,
  RBTree_Node *node
)
{
  //_SMP_lock_Acquire(&self->smp_lock_ready_queue);
   _RBTree_Insert( chain, node, _Scheduler_pfair_SMP_Compare, false);
  //_SMP_lock_Release(&self->smp_lock_ready_queue);
}

static void _Scheduler_pfair_SMP_ChainInsert(
  Chain_Control *chain,
  Thread_Control *thread,
  Chain_Node_order order
)
{
  _Chain_Insert_ordered_unprotected( chain, &thread->Object.Node, order );
}

static void _Scheduler_pfair_SMP_Enqueue_ordered(
  Scheduler_Context *context,
  Thread_Control *thread,
  Chain_Node_order order,
  RBTree_Node *node
)
{

  /*
   * The scheduled chain has exactly processor count nodes after
   * initialization, thus the lowest priority scheduled thread exists.
   */
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( context );
  
  Scheduler_SMP_Context *smp_self = _Scheduler_SMP_Get_self( context );
  
  RBTree_Node *node_thread = &(thread->RBNode);

    Thread_Control *highest_ready = _Scheduler_pfair_SMP_Get_highest_ready(context);
    
    Thread_Control *lowest_scheduled =  _Scheduler_pfair_SMP_Get_lowest_scheduled(self);

     if ( ( *order )( &thread->Object.Node, &lowest_scheduled->Object.Node ) ) {
        _Scheduler_pfair_SMP_Allocate_processor( thread, lowest_scheduled );

        _Scheduler_pfair_SMP_ChainInsert( &smp_self->Scheduled, thread, order );

        _Scheduler_pfair_SMP_Move_from_scheduled_to_ready(
          context,
          &self->Ready,
          lowest_scheduled
        );
     } else {
      _Scheduler_pfair_SMP_Insert( &self->Ready, thread, node);
      }
}


void _Scheduler_pfair_SMP_Enqueue_priority_fifo(Scheduler_Context *context, Thread_Control *thread )
{
  RBTree_Node *node = &(thread->RBNode);

  _Scheduler_pfair_SMP_Enqueue_ordered(context,
				       thread,//Thread
				       _Scheduler_simple_Insert_priority_fifo_order,//Chain Order
				       node // RB Tree Node
				       );
}

void _Scheduler_pfair_SMP_Enqueue_priority_lifo(Scheduler_Context *context, Thread_Control *thread )
{
  _Scheduler_pfair_SMP_Enqueue_priority_fifo(context, thread);
}

void _Scheduler_pfair_SMP_Extract(Scheduler_Control *scheduler, Thread_Control *thread )
{
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( scheduler );
  
  Scheduler_SMP_Context *smp_self = _Scheduler_SMP_Get_self( self );
  
  _Chain_Extract_unprotected( &thread->Object.Node );
  //_SMP_lock_Acquire(&self->smp_lock_ready_queue);
  
  //if ( thread->is_scheduled ) {
    RBTree_Node *first = _RBTree_First(&self->Ready, RBT_LEFT);
    Thread_Control *highest_ready = 
       RTEMS_CONTAINER_OF( first, Thread_Control, RBNode );

    _Scheduler_pfair_SMP_Allocate_processor( highest_ready, thread );

    _Scheduler_pfair_SMP_Move_from_ready_to_scheduled(
	      self,
      	&smp_self->Scheduled,
      	highest_ready
    );
 // }
}

void _Scheduler_pfair_SMP_Yield(const Scheduler_Control *scheduler, Thread_Control *thread )
{
  ISR_Level level;
  Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_self( scheduler );
    
  _ISR_Disable( level );

  _Scheduler_pfair_SMP_Extract(self, thread );
  _Scheduler_pfair_SMP_Enqueue_priority_fifo(self, thread );

  _ISR_Enable( level );
}

static void  _Scheduler_pfair_SMP_Schedule_highest_ready(
  const Scheduler_Context *context,
  Thread_Control *victim
)
{
  Thread_Control *highest_ready =  _Scheduler_pfair_SMP_Get_highest_ready(context);
  Scheduler_SMP_Context *smp_self = _Scheduler_SMP_Get_self( context );
 _Scheduler_pfair_SMP_Allocate_processor(highest_ready, victim);
 _Scheduler_pfair_SMP_Move_from_ready_to_scheduled(
  context,
  &smp_self->Scheduled,
  highest_ready);
}

static void _Scheduler_pfair_SMP_helper_Schedule(
  Scheduler_Context *context,
  Thread_Control *thread
)
{
   _Scheduler_pfair_SMP_Schedule_highest_ready(
      context,
      thread
    );
}

void _Scheduler_pfair_SMP_Schedule(const Scheduler_Control *scheduler, Thread_Control *thread )
{
   Scheduler_pfair_SMP_Context *self =
    _Scheduler_pfair_SMP_Get_context( scheduler );
    
  _Scheduler_pfair_SMP_helper_Schedule(self,
				thread
				);
}

void _Scheduler_pfair_SMP_Start_idle(
  Scheduler_Context *context,
  Thread_Control *thread,
  Per_CPU_Control *cpu
)
{
  //Scheduler_global_EDF_Control *self = _Scheduler_global_EDF_Instance();
  //thread->is_scheduled = true;
  Scheduler_SMP_Context *smp_self = _Scheduler_SMP_Get_self( context );
  thread->Scheduler.cpu = cpu;
  _Chain_Append_unprotected( &smp_self->Scheduled, &thread->Object.Node );
}

Scheduler_Void_or_thread _Scheduler_pfair_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority,
  bool                     prepend_it
)
{
  Scheduler_pfair_SMP_Context *context =
    _Scheduler_pfair_SMP_Get_context( scheduler );
  //Scheduler_EDF_Node *node = _Scheduler_EDF_Thread_get_node( the_thread );
  
  _RBTree_Extract( &context->Ready, &the_thread->RBNode );
  _RBTree_Insert(
    &context->Ready,
    &the_thread->RBNode,
    _Scheduler_pfair_SMP_Compare,
    false
  );

  SCHEDULER_RETURN_VOID_OR_NULL;
}

void _Scheduler_pfair_SMP_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority
)
{
  Scheduler_pfair_SMP_Context *context =
    _Scheduler_pfair_SMP_Get_context( scheduler );
  //Scheduler_EDF_Node *node = _Scheduler_EDF_Thread_get_node( the_thread );
  
  _RBTree_Extract( &context->Ready, &the_thread->RBNode );
  _RBTree_Insert(
    &context->Ready,
    &the_thread->RBNode,
    _Scheduler_pfair_SMP_Compare,
    false
  );
}

Thread_Control *_Scheduler_pfair_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  _Scheduler_pfair_SMP_Enqueue_priority_fifo(context, thread);
}

/** Calculate sub-task deadline */
void _Scheduler_pfair_SMP_SubTask_deadline(Scheduler_pfair_SMP_Per_Thread *task)
{
  task->deadline_subtask = ((task->subtask_num * task->Tp) / task->Te) + 
                          (task->Tp % task->Te)? 0 : 1;
}

/* Calculate successor bit */
void _Scheduler_pfair_SMP_SubTask_successor_bit(Scheduler_pfair_SMP_Per_Thread *task)
{
  task->successor_bit = task->deadline_subtask - (task->subtask_num - 1) * (task->Tp / task->Te);
}

void _Scheduler_pfair_SMP_Budget_Algorithm_callout(Thread_Control *thread)
{
  thread->pfair_per_thread_info.subtask_num++;
  
  _Scheduler_pfair_SMP_SubTask_deadline(&thread->pfair_per_thread_info);
  _Scheduler_pfair_SMP_SubTask_successor_bit(&thread->pfair_per_thread_info);
  thread->cpu_time_budget = 1;
  
   Priority_Control new_priority;

    /* Initializing or shifting deadline. */
    new_priority = (_Watchdog_Ticks_since_boot + thread->pfair_per_thread_info.deadline_subtask)
                   & ~SCHEDULER_PFAIR_PRIO_MSB;
  
  thread->real_priority = new_priority;
  _Thread_Change_priority(thread, new_priority, true);
  
  //_Scheduler_pfair_SMP_Change_priority();
  
  //_Scheduler_pfair_SMP_Schedule();
}

void _Scheduler_pfair_SMP_Thread_init(Thread_Control *thread, float weight, )
{
  thread->budget_callout   = _Scheduler_pfair_SMP_Budget_Algorithm_callout;
  thread->budget_algorithm = THREAD_CPU_BUDGET_ALGORITHM_CALLOUT;
  thread->is_preemptible   = true;
  thread->pfair_per_thread_info.deadline_subtask = 1;
  thread->pfair_per_thread_info.subtask_num      = 1;
}

/*
_______________________________________________________________
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
*/
