include $(top_srcdir)/automake/compile.am

CLEANFILES =
DISTCLEANFILES =

include_rtemsdir = $(includedir)/rtems

include_rtems_HEADERS = rtems/asm.h

include_rtems_scoredir = $(includedir)/rtems/score

include_rtems_score_HEADERS =
include_rtems_score_HEADERS += rtems/score/cpu.h
include_rtems_score_HEADERS += rtems/score/cpu_asm.h
include_rtems_score_HEADERS += rtems/score/types.h
include_rtems_score_HEADERS += rtems/score/epiphany.h
include_rtems_score_HEADERS += rtems/score/epiphany-utility.h
include_rtems_score_HEADERS += rtems/score/cpuatomic.h

#e-lib library 
include_rtems_score_HEADERS += e-lib/include/e_common.h 
include_rtems_score_HEADERS += e-lib/include/e_ctimers.h
include_rtems_score_HEADERS += e-lib/include/e_ic.h
include_rtems_score_HEADERS += e-lib/include/e-lib.h
include_rtems_score_HEADERS += e-lib/include/e_mutex.h
include_rtems_score_HEADERS += e-lib/include/e_shm.h
include_rtems_score_HEADERS += e-lib/include/e_types.h
include_rtems_score_HEADERS += e-lib/include/e_coreid.h
include_rtems_score_HEADERS += e-lib/include/e_dma.h
include_rtems_score_HEADERS += e-lib/include/e_lib.h
include_rtems_score_HEADERS += e-lib/include/e_mem.h
include_rtems_score_HEADERS += e-lib/include/e_regs.h
include_rtems_score_HEADERS += e-lib/include/e_trace.h
include_rtems_score_HEADERS += e-lib/include/a_trace_shared.h

noinst_LIBRARIES = libscorecpu.a

libscorecpu_a_SOURCES =
libscorecpu_a_SOURCES += cpu.c
libscorecpu_a_SOURCES += epiphany-context-switch.S
libscorecpu_a_SOURCES += epiphany-context-initialize.c
libscorecpu_a_SOURCES += epiphany-exception-handler.S
libscorecpu_a_SOURCES += epiphany-smp.c
libscorecpu_a_SOURCES += epiphany-atomic.c

#e-lib library 
libscorecpu_a_SOURCES += e-lib/src/e_coreid_config.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_coords_from_coreid.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_from_coords.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_get_coreid.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_get_global_address.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_is_oncore.c
libscorecpu_a_SOURCES += e-lib/src/e_coreid_neighbor_id.c
libscorecpu_a_SOURCES += e-lib/src/e_ctimer_get.s
libscorecpu_a_SOURCES += e-lib/src/e_ctimer_set.s
libscorecpu_a_SOURCES += e-lib/src/e_ctimer_start.s
libscorecpu_a_SOURCES += e-lib/src/e_ctimer_stop.c
libscorecpu_a_SOURCES += e-lib/src/e_ctimer_wait.c
libscorecpu_a_SOURCES += e-lib/src/e_dma_busy.c
libscorecpu_a_SOURCES += e-lib/src/e_dma_copy.c
libscorecpu_a_SOURCES += e-lib/src/e_dma_set_desc.c
libscorecpu_a_SOURCES += e-lib/src/e_dma_start.c
libscorecpu_a_SOURCES += e-lib/src/e_dma_wait.c
libscorecpu_a_SOURCES += e-lib/src/e_irq_attach.c
libscorecpu_a_SOURCES += e-lib/src/e_irq_clear.c
libscorecpu_a_SOURCES += e-lib/src/e_irq_global_mask.c
libscorecpu_a_SOURCES += e-lib/src/e_irq_mask.c
libscorecpu_a_SOURCES += e-lib/src/e_irq_set.c
libscorecpu_a_SOURCES += e-lib/src/e_mem_read.c
libscorecpu_a_SOURCES += e-lib/src/e_mem_write.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_barrier.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_barrier_init.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_init.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_lock.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_trylock.c
libscorecpu_a_SOURCES += e-lib/src/e_mutex_unlock.c
libscorecpu_a_SOURCES += e-lib/src/e_reg_read.c
libscorecpu_a_SOURCES += e-lib/src/e_reg_write.c
libscorecpu_a_SOURCES += e-lib/src/e_shm.c
libscorecpu_a_SOURCES += e-lib/src/e_trace.c
libscorecpu_a_SOURCES += e-lib/src/e_trace_dma.c

libscorecpu_a_CPPFLAGS = $(AM_CPPFLAGS)

all-local: $(PREINSTALL_FILES)

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
