
rtems_tests_PROGRAMS = smpschedpfair01
smpschedpfair01_SOURCES = init.c tasks.c system.h ../../support/src/locked_print.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -DSMPTEST 

LINK_OBJS = $(smpschedpfair01_OBJECTS)
LINK_LIBS = $(smpschedpfair01_LDLIBS)

smp01$(EXEEXT): $(smpschedpfair01_OBJECTS) $(smpschedpfair01_DEPENDENCIES)
	@rm -f smpschedpfair01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
