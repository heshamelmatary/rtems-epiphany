#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc src/read_rtems_poll.c -o exe/read_rtems_poll.elf -I ${EINCS} -L ${ELIBS} -le-hal
gcc src/rtems_smp2.c -o exe/load_rtems_smp.elf -I ${EINCS} -L ${ELIBS} -le-hal

# TODO build RTEMS samples here 

# Convert rtems program to SREC file
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/hello.exe exe/hello.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/ticker.exe exe/ticker.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smp01.exe exe/smp01.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smp03.exe exe/smp03.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smp05.exe exe/smp05.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smp07.exe exe/smp07.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smp08.exe exe/smp08.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/spcpucounter01.exe exe/spcpucounter01.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/tm06.exe exe/tm06.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/tmoverhd.exe exe/tmoverhd.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smpload01.exe exe/smpload01.srec
e-objcopy --srec-forceS3 --output-target srec /home/linaro/Downloads/smpscheduler01.exe exe/smpscheduler01.srec

