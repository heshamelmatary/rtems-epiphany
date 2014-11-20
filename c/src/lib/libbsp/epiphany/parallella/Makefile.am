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

nodist_include_HEADERS = ../../shared/include/coverhd.h \
  include/bspopts.h
include_bsp_HEADERS = ../shared/include/linker-symbols.h

###############################################################################
#                  Data                                                       #
###############################################################################

noinst_LIBRARIES = libbspstart.a

libbspstart_a_SOURCES = start/start.S

project_lib_DATA = start.$(OBJEXT)

project_lib_DATA += startup/linkcmds

###############################################################################
#                  LibBSP                                                     #
###############################################################################

#noinst_LIBRARIES += libbsp.a

# Startup
# #libbsp_a_SOURCES += startup/bspreset.c
# #libbsp_a_SOURCES += startup/bspstart.c
#
# ###############################################################################
# #                  Special Rules
# #
# ###############################################################################
#
# DISTCLEANFILES = include/bspopts.h
#
include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
