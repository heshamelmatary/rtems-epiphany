include $(top_srcdir)/automake/multilib.am
include $(top_srcdir)/automake/compile.am

noinst_LIBRARIES = libcsupport.a
libcsupport_a_CPPFLAGS = $(AM_CPPFLAGS)

include_rtemsdir = $(includedir)/rtems
include_rtems_HEADERS = include/console.h
include_rtems_HEADERS += include/clockdrv.h
include_rtems_HEADERS += include/rtems/framebuffer.h
include_rtems_HEADERS += include/iosupp.h
include_rtems_HEADERS += include/ringbuf.h
include_rtems_HEADERS += include/rtc.h
include_rtems_HEADERS += include/rtems/tod.h
include_rtems_HEADERS += include/spurious.h
include_rtems_HEADERS += include/vmeintr.h

## motorola

include_rtems_motoroladir = $(includedir)/rtems/motorola

include_rtems_motorola_HEADERS = include/motorola/mc68230.h
include_rtems_motorola_HEADERS += include/motorola/mc68681.h

## rtems
include_rtems_HEADERS += include/rtems/assoc.h
include_rtems_HEADERS += include/rtems/deviceio.h
include_rtems_HEADERS += include/rtems/error.h
include_rtems_HEADERS += include/rtems/libcsupport.h
include_rtems_HEADERS += include/rtems/libio.h
include_rtems_HEADERS += include/rtems/libio_.h
include_rtems_HEADERS += include/rtems/malloc.h
include_rtems_HEADERS += include/rtems/termiostypes.h
include_rtems_HEADERS += include/rtems/gxx_wrappers.h

## zilog

include_rtems_zilogdir = $(includedir)/rtems/zilog

include_rtems_zilog_HEADERS = include/zilog/z8036.h
include_rtems_zilog_HEADERS += include/zilog/z8530.h
include_rtems_zilog_HEADERS += include/zilog/z8536.h

##  General stuff
ERROR_C_FILES = src/error.c src/__assert.c

ASSOCIATION_C_FILES = #src/assoclocalbyname.c \
    #src/assoclocalbyremotebitfield.c src/assoclocalbyremote.c \
    #src/assocnamebad.c src/assocnamebylocalbitfield.c \
    #src/assocnamebylocal.c src/assocnamebyremotebitfield.c \
    #src/assocnamebyremote.c src/assocptrbylocal.c src/assocptrbyname.c \
    #src/assocptrbyremote.c src/assocremotebylocalbitfield.c \
    #src/assocremotebylocal.c src/assocremotebyname.c src/libio_helper_null.c

BASE_FS_C_FILES = 

TERMIOS_C_FILES = 

SYSTEM_CALL_C_FILES = src/syscall_stubs.c src/write.c \
    src/write_r.c src/libio_helper_null.c \
    src/ioctl.c \
    src/link.c src/unlink.c src/utime.c \
    src/fcntl.c \
    src/_rename_r.c src/statvfs.c src/utimes.c

#SYSTEM_CALL_C_FILES = src/syscall_stubs.c src/write.c \
#    src/write_r.c src/libio_helper_null.c \
#    src/ioctl.c src/mkdir.c src/mknod.c src/mkfifo.c src/rmdir.c \
#    src/chdir.c src/chmod.c src/fchdir.c src/fchmod.c src/fchown.c src/chown.c \
#    src/link.c src/unlink.c src/umask.c src/ftruncate.c src/utime.c \
#    src/fcntl.c src/fpathconf.c src/getdents.c src/fsync.c src/fdatasync.c \
#    src/pipe.c src/dup.c src/dup2.c src/symlink.c src/readlink.c \
#    src/chroot.c src/sync.c src/_rename_r.c src/statvfs.c src/utimes.c src/lchown.c
#
## Until sys/uio.h is moved to libcsupport, we have to have networking
## enabled to compile these.  Hopefully this is a temporary situation.
if NEWLIB
SYSTEM_CALL_C_FILES += src/readv.c src/writev.c
endif

DIRECTORY_SCAN_C_FILES =
## Newlib SHOULD have provided this one
DIRECTORY_SCAN_C_FILES += #src/readdir_r.c
## Comment out when using these from newlib's unix directory
DIRECTORY_SCAN_C_FILES += #src/getcwd.c

ID_C_FILES = #src/getegid.c src/geteuid.c src/getgid.c src/getgroups.c \
    #src/getlogin.c src/getpgrp.c src/getpid.c src/getppid.c src/getuid.c \
    #src/seteuid.c src/setgid.c src/setuid.c src/setegid.c src/setpgid.c \
    #src/setsid.c

MALLOC_C_FILES = src/malloc_initialize.c src/calloc.c src/malloc.c \
     src/rtems_heap_null_extend.c
#    src/realloc.c src/_calloc_r.c src/_malloc_r.c \
#    src/free.c src/_free_r.c \
#    src/_realloc_r.c src/mallocfreespace.c \
#    src/mallocgetheapptr.c src/mallocsetheapptr.c \
#    src/mallocinfo.c src/malloc_walk.c \
#    src/posix_memalign.c \
#    src/rtems_memalign.c src/malloc_deferred.c \
#    src/malloc_dirtier.c src/malloc_p.h src/rtems_malloc.c \
#    src/rtems_heap_extend_via_sbrk.c \
#    src/rtems_heap_extend.c \
#    src/rtems_heap_greedy.c
#MALLOC_C_FILES += src/cachealignedalloc.c
#MALLOC_C_FILES += src/cachecoherentalloc.c

PASSWORD_GROUP_C_FILES = #src/pwdgrp.c
PASSWORD_GROUP_C_FILES += #src/getgrent.c
PASSWORD_GROUP_C_FILES += #src/getgrnam.c
PASSWORD_GROUP_C_FILES += #src/getpwent.c

TERMINAL_IDENTIFICATION_C_FILES = #src/ctermid.c
## 20 Nov 2008: Now using these from newlib's posix directory
TERMINAL_IDENTIFICATION_C_FILES += #src/isatty.c src/isatty_r.c
## Comment out when using these from newlib's unix directory
TERMINAL_IDENTIFICATION_C_FILES += #src/ttyname.c

LIBC_GLUE_C_FILES = src/newlibc_reent.c src/newlibc_init.c src/newlibc_exit.c \
    #src/__getpid.c src/__gettod.c src/__times.c \
    #src/truncate.c src/access.c src/lstat.c src/pathconf.c \
    #src/newlibc_reent.c src/newlibc_init.c src/newlibc_exit.c \
    #src/kill_noposix.c src/utsname.c src/realpath.c

BSD_LIBC_C_FILES = #src/strlcpy.c src/strlcat.c src/issetugid.c

libcsupport_a_SOURCES = src/gxx_wrappers.c src/getchark.c src/printk.c \
    src/printk_plugin.c src/putk.c src/vprintk.c \
    $(BSD_LIBC_C_FILES) $(BASE_FS_C_FILES) $(MALLOC_C_FILES) \
    $(ERROR_C_FILES) $(ASSOCIATION_C_FILES)
    #src/sup_fs_location.c \
    #src/printk_plugin.c src/putk.c src/vprintk.c \
    #src/rtems_putc.c \
    #src/printf_plugin.c \
    #src/sup_fs_eval_path.c \
    #src/sup_fs_eval_path_generic.c \
    #src/sup_fs_check_permissions.c \
    #src/sup_fs_next_token.c \
    #src/sup_fs_exist_in_same_instance.c \
    #src/sup_fs_mount_iterate.c \
    #src/sup_fs_node_type.c \
    #src/sup_fs_deviceio.c \
    #src/clonenode.c \
    #src/freenode.c 

libcsupport_a_SOURCES += $(LIBC_GLUE_C_FILES) $(PASSWORD_GROUP_C_FILES) \
    $(TERMINAL_IDENTIFICATION_C_FILES) $(SYSTEM_CALL_C_FILES) \
    $(DIRECTORY_SCAN_C_FILES) $(ID_C_FILES) src/envlock.c \
    $(TERMIOS_C_FILES) #src/getpagesize.c src/getrusage.c

libcsupport_a_SOURCES += #src/flockfile.c src/funlockfile.c src/ftrylockfile.c

EXTRA_DIST = src/TODO src/CASES src/README

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
