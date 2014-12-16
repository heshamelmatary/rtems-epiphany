/**
 *  @file
 *
 *  @brief Read From a File
 *  @ingroup libcsupport
 */

/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/libio_.h>
#include <rtems/seterr.h>

/**
 *  POSIX 1003.1b 6.4.1 - Read From a File
 */
ssize_t read(
  int         fd,
  void       *buffer,
  size_t      count
)
{
  return 0;
}

#if defined(RTEMS_NEWLIB) && !defined(HAVE__READ_R)

#include <reent.h>

/**
 *  This is the Newlib dependent reentrant version of read().
 */
ssize_t _read_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd,
  void          *buf,
  size_t         nbytes
)
{
  return read( fd, buf, nbytes );
}
#endif
