#include <stdio.h>
#include <e-hal.h>
#include <stdlib.h>

/* RTEMS uses this address as a shared buffer between
 * Zynq and Epiphany,
 */

/* TODO: Define definitions shared with RTEMS 
 * in a separate header file
 */

#define SHARED_BUF_BASE 0x01800000
#define SHARED_BUF_OFF  0x0
#define SHARED_BUF_SIZE 1024

#define PARALLELLA_CONSOLE_REG_CTRL_BIT_BUSY  0x1
#define PARALLELLA_CONSOLE_REG_CTRL_BIT_AVL   0x2

void main()
{

  e_platform_t platform;
  e_epiphany_t workgroup_boot;
  e_epiphany_t workgroup1;
  e_epiphany_t workgroup2;
  e_mem_t      e_mem;
  int rtems_console_ctrl, rtems_console_data = 0;
  
  unsigned int row, col, coreid;

  /* Initialize the Epiphany Chip */
  if( e_init(NULL) != E_OK )
  {
    printf("Faile to initialize Epiphany Chip. Exiting... \n");
    exit(0);
  }
 
  e_reset_system();
  
  /* Get platform information */
  if( e_get_platform_info(&platform) != E_OK )
  {
    printf("Failed to get platform info. Exiting... \n");
    exit(0);
  }
  
  row = 0;
  col = 0;  
  coreid = (row + platform.row) * 64 + (col + platform.col);

  if( e_open(&workgroup_boot, row, col, 1, 1) != E_OK )
  {
    printf("Failed to open workgroup \n");
    exit(0);
  }

  if( e_open(&workgroup1, 0, 1, 1, 3) != E_OK )
  {
    printf("Failed to open workgroup \n");
    exit(0);
  }

  if( e_open(&workgroup2, 1, 0, 3, 4) != E_OK )
  {
    printf("Failed to open workgroup \n");
    exit(0);
  }

  if ( (e_reset_group(&workgroup_boot) & e_reset_group(&workgroup1) & e_reset_group(&workgroup2)) != E_OK )
  {
    printf("Failed to reset the workgroup \n");
    exit(0);
  }

  /* Allocate shared memory buffer for communication between
   * RTEMS and Linux/Zynq
   */
  if( e_alloc(&e_mem, SHARED_BUF_BASE, SHARED_BUF_SIZE) != E_OK )
  {
    printf("Failed to allocate shared memory \n");
    exit(0);
  }

  printf("*** Loading the program to Epiphany ***\n");
   
  /* Load the boot processor and start it immediately */
  if ( e_load_group("smp01.srec", &workgroup_boot, 0, 0, 1, 1, E_FALSE) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }
  
  if ( e_load_group("smp01.srec", &workgroup1, 0, 0, 1, 3, E_FALSE) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }
  if ( e_load_group("smp01.srec", &workgroup2, 0, 0, 3, 4, E_FALSE) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }

  e_start_group(&workgroup_boot);
  //usleep(1000000);
  //e_start_group(&workgroup1);
  //e_start(&workgroup2, 0, 0);
  //usleep(1000000);
  
  e_start(&workgroup1, 0, 0);
  int x = 0;
  /*for(x = 0 ; x<3; x++)
  {
    usleep(10000);
    e_start(&workgroup1, 0, x);
  }*/

  /*for(x = 4 ; x<16; x++)
  {
    usleep(10000);
    e_start(&workgroup2, (x/4) - 1   ,x % 4);
  }*/
  //e_start_group(&workgroup2);
  //usleep(1000000);
  //usleep(100000);
 
  /*if ( e_start(&workgroup, 0, 0) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }
  
  usleep(100000);

  if ( e_start(&workgroup, 0, 1) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }
  
  usleep(10000);

  if ( e_start(&workgroup, 0, 2) != E_OK )
  {
    printf("Failed to start the workgroup program \n");
    exit(0);
  }
  */
  /*if( e_load("smp01.srec", &workgroup, 0, 2, E_FALSE) != E_OK )
  {
    printf("Failed to load the program \n");
    exit(0);
  }
  */

  //usleep(100000);
 
  /* TODO Poll for data until console closed from Epiphany */
  while (1)
  {
    e_read(&e_mem, 0, 0, 0x0, &rtems_console_ctrl, 4);

    /* Snop for data, and lock console if there are data avaiable until handling it */
    if (rtems_console_ctrl & PARALLELLA_CONSOLE_REG_CTRL_BIT_AVL)
    {
      rtems_console_ctrl |= PARALLELLA_CONSOLE_REG_CTRL_BIT_BUSY;
      e_write(&e_mem, 0, 0, 0x0, &rtems_console_ctrl, 4);

      /* Output the coming data from Epiphnay */
      e_read(&e_mem, 0, 0, 0x4, &rtems_console_data, 4);
      putc( (char) rtems_console_data, stderr);

      /* Make console avaiable again for future use */
      
      rtems_console_ctrl &= ~PARALLELLA_CONSOLE_REG_CTRL_BIT_BUSY;
      rtems_console_ctrl &= ~PARALLELLA_CONSOLE_REG_CTRL_BIT_AVL;

      e_write(&e_mem, 0, 0, 0x0, &rtems_console_ctrl, 4);
    }
  }
  /* Free the allocated shared memory */

  printf("*** Freeing the allocated memory ***\n");

  e_free(&e_mem);

  /* Close the workgroup */
  if ( e_close(&workgroup_boot) != E_OK)
  {
    printf("Failed to close the workgroup \n");
    exit(0);
  }

  /* Shutdown the system */
  if ( e_finalize() != E_OK )
  {
    printf("Failed to finalize the system. \n");
    exit(0);
  }
}
