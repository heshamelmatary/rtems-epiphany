#ifndef _RTEMS_SCORE_SCHEDULERPFAIRTYPES_H
#define _RTEMS_SCORE_SCHEDULERPFAIRTYPES_H

#include <rtems/score/rbtree.h>

typedef struct {
  RBTree_Node node; 
  uint32_t subtask_num;
  uint32_t deadline_subtask;
  uint32_t Te;
  uint32_t Tp;
  uint32_t successor_bit;
  
} Scheduler_pfair_SMP_Per_Thread;

#endif 
