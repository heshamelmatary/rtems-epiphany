rtems_tests_PROGRAMS = smpschedpfair02
smpschedpfair02_SOURCES = init.c system.h ../../support/src/locked_print.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -DSMPTEST 

LINK_OBJS = $(smpschedpfair02_OBJECTS)
LINK_LIBS = $(smpschedpfair02_LDLIBS)

smp02$(EXEEXT): $(smpschedpfair02_OBJECTS) $(smpschedpfair02_DEPENDENCIES)
	@rm -f smpschedpfair02$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
