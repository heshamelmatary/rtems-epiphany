/**
 *  @file  rtems/score/schedulerpfair.h
 *
 *  @brief Thread Manipulation with the Priority-Based Scheduler
 *
 *  This include file contains all the constants and structures associated
 *  with the manipulation of threads for the pfaif-based scheduler.
 */

/*
 *  Copryight (c) 2015 Hesham ALMatary.
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_SCHEDULERPFAIR_H
#define _RTEMS_SCORE_SCHEDULERPFAIR_H

#include <rtems/score/scheduler.h>
#include <rtems/score/schedulerpriority.h>
#include <rtems/score/schedulersmp.h>
#include <rtems/score/rbtree.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ScoreSchedulerPF 
 *
 * @ingroup ScoreScheduler
 */
/**@{*/

/**
 *  Entry points for the PF Scheduler.
 */
#define SCHEDULER_PFAIR_SMP_ENTRY_POINTS \
  { \
    _Scheduler_pfair_SMP_Initialize,       /* initialize entry point */ \
    _Scheduler_default_Schedule,         /* schedule entry point */ \
    _Scheduler_pfair_SMP_Yield,            /* yield entry point */ \
    _Scheduler_pfair_SMP_Block,            /* block entry point */ \
    _Scheduler_pfair_SMP_Unblock,          /* unblock entry point */ \
    _Scheduler_pfair_SMP_Change_priority,  /* change priority entry point */ \
    _Scheduler_pfair_SMP_Ask_for_help, \
    _Scheduler_pfair_SMP_Node_initialize,  /* node initialize entry point */ \
    _Scheduler_default_Node_destroy,       /* node destroy entry point */ \
    _Scheduler_pfair_SMP_Update_priority,  /* update priority entry point */ \
    _Scheduler_priority_Priority_compare,  /* compares two priorities */ \
    _Scheduler_default_Release_job,        /* new period of task */ \
    _Scheduler_default_Tick,               /* tick entry point */ \
    _Scheduler_SMP_Start_idle              /* start idle entry point */ \
    SCHEDULER_OPERATION_DEFAULT_GET_SET_AFFINITY \
  }

/**
 * This is just a most significant bit of Priority_Control type. It
 * distinguishes threads which are deadline driven (priority
 * represented by a lower number than @a SCHEDULER_EDF_PRIO_MSB) from those
 * ones who do not have any deadlines and thus are considered background
 * tasks.
 */
#define SCHEDULER_PFAIR_PRIO_MSB 0x80000000

typedef struct {
  Scheduler_SMP_Context    Base;
  //Priority_bit_map_Control Bit_map;
  RBTree_Control Ready;
} Scheduler_pfair_SMP_Context;

/**
 * @brief Data for ready queue operations.
 */
typedef struct {
  RBTree_Control ready_tree;
} Scheduler_pfair_SMP_Ready_queue;

/**
 * @brief Scheduler node specialization for pfair schedulers.
 */
typedef struct {
  /**
   * @brief SMP scheduler node.
   */
  Scheduler_SMP_Node Base;

  RBTree_Node Node;
  /**
   * @brief The associated ready queue of this node.
   */
  RBTree_Control Ready_tree;
  //Scheduler_priority_Ready_queue Ready_queue;
} Scheduler_pfair_SMP_Node;

typedef struct {
  uint32_t subtask_num = 1;
  uint32_t dealine_subtask;
  uint32_t weight;
  uint32_t successor_bit;
  
} Scheduler_pfair_Per_Task;

/** Calculate sub-task deadline */
void _Scheduler_pfair_SMP_SubTask_deadline(Scheduler_pfair_Per_Task *task)
{
  task->dealine_subtask = (task->subtask_num / task->weight) + 
                          (task->subtask_num % task->weight)? 0 : 1;
}

/* Calculate successor bit */
void _Scheduler_pfair_SMP_SubTask_successor_bit(Scheduler_pfair_Per_Task *task)
{
  task->successor_bit = task->dealine_subtask - (subtask_num - 1) / task->weight;
}

/**
 * @brief Initializes the pfair scheduler.
 * This routine initializes the pfair scheduler.
 */
void _Scheduler_pfair_SMP_Initialize( const Scheduler_Control *scheduler );

void _Scheduler_pfair_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
);

/**
 *  @brief Removes @a the_thread from the scheduling decision.
 *
 *  This routine removes @a the_thread from the scheduling decision,
 *  that is, removes it from the ready queue.  It performs
 *  any necessary scheduling operations including the selection of
 *  a new heir thread.
 *
 *  @param[in] the_thread is the thread to be blocked
 */
void _Scheduler_pfair_SMP_Block(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 *  @brief Sets the heir thread to be the next ready thread.
 *
 *  This kernel routine sets the heir thread to be the next ready thread
 *  by invoking the_scheduler->ready_queue->operations->first().
 */
void _Scheduler_pfair_SMP_Schedule(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 *  @brief Updates the scheduler node to reflect the new priority of the
 *  thread.
 */
void _Scheduler_pfair_SMP_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority
);

/**
 *  @brief Add @a the_thread to the scheduling decision.
 *
 *  This routine adds @a the_thread to the scheduling decision,
 *  that is, adds it to the ready queue and
 *  updates any appropriate scheduling variables, for example the heir thread.
 *
 *  @param[in] the_thread will be unblocked
 */
Scheduler_Void_or_thread _Scheduler_pfair_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

Scheduler_Void_or_thread _Scheduler_pfair_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority,
  bool                     prepend_it
);

Thread_Control *_Scheduler_pfair_SMP_Ask_for_help(
  const Scheduler_Control *scheduler,
  Thread_Control          *needs_help,
  Thread_Control          *offers_help
);

/**
 *  @brief The specified THREAD yields.
 *
 *  This routine is invoked when a thread wishes to voluntarily
 *  transfer control of the processor to another thread in the queue.
 *
 *  This routine will remove the specified THREAD from the ready queue
 *  and place it immediately at the rear of this chain.  Reset timeslice
 *  and yield the processor functions both use this routine, therefore if
 *  reset is true and this is the only thread on the queue then the
 *  timeslice counter is reset.  The heir THREAD will be updated if the
 *  running is also the currently the heir.
 *
 *  - INTERRUPT LATENCY:
 *    + ready chain
 *    + select heir
 *
 *  @param[in,out] thread The yielding thread.
 */
Scheduler_Void_or_thread _Scheduler_pfair_SMP_Yield(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 *  @brief Compare two priorities.
 *
 *  This routine compares two priorities.
 */
int _Scheduler_pfair_SMP_Priority_compare(
  Priority_Control   p1,
  Priority_Control   p2
);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
