#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

/* Copied from libgloss */

int stat (const char  *file, struct stat *st)
{
  return 0;
}

ssize_t read (int fildes, void *ptr, size_t len)
{
  return 0;
}

off_t lseek (int  fildes, off_t offset, int  whence)
{
  return 0;
}

int  fstat (int fildes, struct stat *st)
{
  return 0;
}

pid_t fork (void)
{
  errno = ENOSYS;
  return -1; 

}

void exit (int  rc) 
{
} 

int access(const char *fn, int flags)
{
  struct stat s;
  if (stat(fn, &s))
    return -1; 
  if (s.st_mode & S_IFDIR)
    return 0;
  if (flags & W_OK)
  {
    if (s.st_mode & S_IWRITE)
      return 0;
    return -1; 
  }
  return 0;
}

/*int sbrk (int  incr)
{
  extern char bsp_section_work_begin[];//set by linker
  extern char bsp_section_work_end[];//set by linker

  static char *heap_end;    
  char        *prev_heap_end;

  if (0 == heap_end) {
    heap_end = bsp_section_work_begin;  
  }

  prev_heap_end  = heap_end;
  heap_end      += incr;
if( heap_end < bsp_section_work_end) {

  } else {
    errno = ENOMEM;
    return (char*)-1;
  }
  return (void *) prev_heap_end;

}
*/

/*int open (const char *file, int flags, mode_t mode)
{
  return 0;
}*/

int close (int  fildes)
{
  return  0;
}


#include <reent.h>

int _fstat_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd,
  struct stat   *buf
)
{
  return fstat( fd, buf );
}

/**
 *  *  This is the Newlib dependent reentrant version of read().
 *   */
ssize_t _read_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd,
  void          *buf,
  size_t         nbytes
)
{
  return read( fd, buf, nbytes );
}

/*
 *  *  _lseek_r
 *   *
 *    *  This is the Newlib dependent reentrant version of lseek().
 *     */

off_t _lseek_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd,
  off_t          offset,
  int            whence
)
{
  return lseek( fd, offset, whence );
}

int _close_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd
)
{
  return close( fd );
}
