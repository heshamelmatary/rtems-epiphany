#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc src/read_rtems_poll.c -o exe/read_rtems_poll.elf -I ${EINCS} -L ${ELIBS} -le-hal

# TODO build RTEMS samples here 

# Convert rtems program to SREC file
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/ticker.exe exe/ticker.srec

