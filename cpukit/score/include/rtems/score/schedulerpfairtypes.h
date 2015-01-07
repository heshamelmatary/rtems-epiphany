typedef struct {
  RBTree_Node node; 
  uint32_t subtask_num;
  uint32_t deadline_subtask;
  uint32_t weight;
  uint32_t successor_bit;
  
} Scheduler_pfair_SMP_Per_Thread;
