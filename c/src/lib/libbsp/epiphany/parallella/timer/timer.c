#include <rtems.h>
#include <bsp.h>
#include <rtems/btimer.h>
#include <rtems/score/epiphany-utility.h>

extern char bsp_start_vector_table_begin[];

bool benchmark_timer_find_average_overhead;

static benchmark_timer1_interrupt_handler(void)
{
  unsigned int val = 0xFFFFFFFF;
  unsigned int event_type = 0x1; /* FIXME: Use macros or enum for event types */

  /* Embed assembly code for setting timer1 */
  asm volatile ("movts ctimer1, %[val] \t \n" :: [val] "r" (val));
  
  /* Embed assembly code for setting timer1 qouted from e-lib */
  asm volatile ("movfs r16, config; \t \n"
                "mov   r17, %%low(0xffffff0f);\t \n"
                "movt   r17, %%high(0xffffff0f);\t \n"
                "lsl   r18, %[event_type], 0x4; \t \n"
                "and   r16, r16, r17; \t \n"
                "movts config, r16; \t \n"
                "orr   r16, r16, r18; \t \n"
                "movts config, r16; \t \n"
                :: [event_type] "r" (event_type));
}


/* Start eCore tiemr 1 usef for profiling and timing analysis */
void benchmark_timer_initialize( void )
{
  unsigned int val = 0xFFFFFFFF;
  unsigned int event_type = 0x1;
  

  /* Install interrupt handler for timer 1 */
     
  proc_ptr *table =
    (proc_ptr *) bsp_start_vector_table_begin;
     
  table[TIMER1] = benchmark_timer1_interrupt_handler;
   
  /* Embed assembly code for setting timer1 */
  asm volatile ("movts ctimer1, %[val] \t \n" :: [val] "r" (val));

  /* Embed assembly code for setting timer1 qouted from e-lib */
  asm volatile ("movfs r16, config; \t \n"
                "mov   r17, %%low(0xffffff0f);\t \n"
                "movt  r17, %%high(0xffffff0f);\t \n"
                "lsl   r18, %[event_type], 0x4; \t \n"
                "and   r16, r16, r17; \t \n"
                "movts config, r16; \t \n"
                "orr   r16, r16, r18; \t \n"
                "movts config, r16; \t \n"
                :: [event_type] "r" (event_type));
}

/*
 *  The following controls the behavior of benchmark_timer_read().
 *
 *  AVG_OVEREHAD is the overhead for starting and stopping the timer.  It
 *  is usually deducted from the number returned.
 *
 *  LEAST_VALID is the lowest number this routine should trust.  Numbers
 *  below this are "noise" and zero is returned.
 */

#define AVG_OVERHEAD      0  /* It typically takes X.X microseconds */
                             /* (Y countdowns) to start/stop the timer. */
                             /* This value is in microseconds. */
#define LEAST_VALID       1  /* Don't trust a clicks value lower than this */

benchmark_timer_t benchmark_timer_read( void )
{
  uint32_t timer_val;
  uint32_t total;
  
  asm volatile ("movfs %0 ,ctimer1; \t \n" : "=r" (timer_val):);
  
  total = (0xFFFFFFFF - timer_val) / 1000; /* Convert to uS */
  
  if ( benchmark_timer_find_average_overhead == true )
    return total;          /* in XXX microsecond units */
  else {
    if ( total < LEAST_VALID )
      return 0;            /* below timer resolution */
  /*
   *  Somehow convert total into microseconds
   */
      return (total - AVG_OVERHEAD);
    }
}

void benchmark_timer_disable_subtracting_average_overhead(
  bool find_flag
)
{
  benchmark_timer_find_average_overhead = find_flag;
}
