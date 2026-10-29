#!/bin/bash

CPU=$(lscpu | grep "Model name" | head -1 | cut -d: -f2 | xargs)
ARCH=$(uname -m)
CORES=$(nproc)
RAM=$(free -h | awk '/^Mem:/ {print $2}')
KERNEL=$(uname -r)
BUILD=$(uname -v)
DISK=$(lsblk -dn -o NAME,SIZE,TYPE | awk '$3=="disk" {printf "%s %s; ",$1,$2}')
SHELL_NAME=$(basename "$SHELL")
PID=$$

echo "+--------------------------------------------------------------------------+"
echo "| USER SPACE (user mode)                                                   |"
echo "|   shell: $SHELL_NAME (PID $PID)"
echo "|   my programs: file_creator, file_reader, dir_list (lib + sys versions)"
echo "|   libc: fopen, fprintf, printf, opendir -> buffers, then calls below"
echo "+--------------------------------------------------------------------------+"
echo "| SYSTEM CALL INTERFACE (user -> kernel mode switch)                       |"
echo "|   openat()  read()  write()  close()  getdents64()  newfstatat()  brk()  |"
echo "+--------------------------------------------------------------------------+"
echo "| KERNEL (kernel mode) Linux $KERNEL"
echo "|   build: $BUILD"
echo "|   scheduler, memory manager, VFS + filesystems, device drivers"
echo "|   loaded modules: $(lsmod | awk 'NR>1 {print $1}' | head -3 | paste -sd ', ' -)"
echo "+--------------------------------------------------------------------------+"
echo "| HARDWARE                                                                 |"
echo "|   CPU: ${CPU:-Unknown CPU} ($CORES cores, $ARCH)"
echo "|   RAM: $RAM"
echo "|   disks: $DISK"
echo "|   network, display, keyboard and other virtual/physical devices          |"
echo "+--------------------------------------------------------------------------+"
