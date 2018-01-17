ptag                version 1.0      06/17/2016 
by Geordie Tait                     (ID# 10013837) 

Description
-----------

This directory contains utilities and a kernel patch necessary to implement process
tagging as specified. The utility ptag invokes a new system call (also called ptag)
which updates a linked list in the kernel task_struct with added or removed tag strings.
The tagstat utility reads process and tag information from /proc/ptag, which is created
and maintained by the kernel module tagstat (which must be loaded). The tagkill utility
(bash script) can be used to kill (-9) all processes with a given tag string.


-------------------
General Usage Notes
-------------------

./ptag [PID] [-a tag] [-r tag]

Examples:
* ./ptag 1 -a "cpsc457" - adds tag "cpsc457" to PID 1
* ./ptag 1 -r "cpsc457" - removes tag "cpsc457" from PID 1
* ./ptag 1 -a "tag1" -r "tag2" - adds and removes a tag in one command

./tagstat - displays all currently running processes with tags, and their statuses

./tagkill [tag] - attempts to kill (-9) all currently running processes with [tag]

---------
Compiling
---------

* patch -p1 < linux-2.6.32.CPSC457.patch

This will patch a linux kernel (2.6.32.22) with ptag and tagstat included. The kernel
must then be compiled with "make && make modules && make modules_install && make install".
The patch file is in the directory "kernel" within the current directory.

* make

Running this command in the (current) directory containing the source code will invoke
the makefile which will in turn execute gcc with the necessary modifiers/flags to
compile the utility programs.

-----------
Assumptions
-----------

* The user is using linux kernel v2.6.32.22
* The user can sudo to load 'tagstat' kernel module if not loaded
* The user does not wish to add the same tag to a PID multiple times
