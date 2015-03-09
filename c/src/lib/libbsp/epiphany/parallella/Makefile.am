##
#
# @brief Makefile of LibBSP for the Parallella board.
#
#
ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

include_bspdir = $(includedir)/bsp
include_libcpudir = $(includedir)/libcpu

dist_project_lib_DATA = bsp_specs

###############################################################################
#                  Header                                                     #
###############################################################################

include_HEADERS = include/bsp.h
include_HEADERS += include/tm27.h

include_HEADERS += ../../shared/include/coverhd.h
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h
#include_bsp_HEADERS = ../shared/include/linker-symbols.h

#include_bsp_HEADERS += ../../../libbsp/shared/include/mm.h
#include_bsp_HEADERS += ../../shared/include/utility.h
#include_bsp_HEADERS += ../../shared/include/irq-generic.h
#include_bsp_HEADERS += ../../shared/include/irq-info.h
#include_bsp_HEADERS += ../../shared/include/stackalloc.h
#include_bsp_HEADERS += ../../shared/include/console-polled.h

nodist_include_HEADERS = include/bspopts.h

###############################################################################
#                  Data                                                       #
###############################################################################
noinst_LIBRARIES = libbspstart.a

libbspstart_a_SOURCES = start/start.S

project_lib_DATA = start/start.$(OBJEXT)

project_lib_DATA += startup/linkcmds

###############################################################################
#                  LibBSP                                                     #
###############################################################################

noinst_LIBRARIES += libbsp.a

# Startup
libbsp_a_SOURCES = ../../shared/bspreset.c
#libbsp_a_SOURCES += startup/bspstart.c

# Shared
libbsp_a_SOURCES += ../../shared/bootcard.c
#libbsp_a_SOURCES += ../../shared/bspclean.c
libbsp_a_SOURCES += ../../shared/bspgetworkarea.c
#libbsp_a_SOURCES += ../../shared/bsplibc.c
#libbsp_a_SOURCES += ../../shared/bsppost.c
#libbsp_a_SOURCES += ../../shared/bsppredriverhook.c
#libbsp_a_SOURCES += ../../shared/bsppretaskinghook.c
#libbsp_a_SOURCES += ../../shared/gnatinstallhandler.c
#libbsp_a_SOURCES += ../../shared/src/stackalloc.c

# clock
libbsp_a_SOURCES += clock/clockdrv.c ../../../shared/clockdrv_shell.h

# console
#libbsp_a_SOURCES += ../../shared/console-polled.c

# Interprocessor support */
libbsp_a_SOURCES += startup/bsp_interprocessor_support.c

# SMP
if HAS_SMP
libbsp_a_SOURCES += startup/bspsmp.c
endif

# debugio
libbsp_a_SOURCES += console/console-io.c
# ###############################################################################
# #                  Special Rules
# #
# ###############################################################################
#
DISTCLEANFILES = include/bspopts.h
#
include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
