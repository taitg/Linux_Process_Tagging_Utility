ptag                version 1.0      06/17/2016 
by Geordie Tait                     (ID# 10013837) 

CPSC 457 Spring 2016 Assignment 2
University of Calgary 

-----------
Description
-----------

This directory contains utilities and a kernel patch necessary to implement process
tagging as specified. The utility ptag invokes a new system call (also called ptag)
which updates a linked list in the kernel task_struct with added or removed tag strings.
The tagstat utility reads process and tag information from /proc/ptag, which is created
and maintained by the kernel module tagstat (which must be loaded). The tagkill utility
(bash script) can be used to kill (-9) all processes with a given tag string.


* PLEASE NOTE:

I had everything compiled, installed and working yesterday, but today everything went wrong.
I'm not sure what changed, but my kernel would not find many modules and thus would not install.
Upon restoring to a previous snapshot and trying to recompile (several times), my VM ran out of
space on the virtual disk and could no longer compile. I cannot even boot the VM at the moment,
nor can I even delete snapshots (I only have 3 total) because "there is not enough space to
merge" them.

--> See screenshot image in this directory

As a result, I missed out on a lot of polishing work and was not able to generate a
typescript demonstrating that it works. But it does work.

I have included my files for ptag and tagstat (kernel code) in the "kernel" directory,
as well as a patch that I created when it was working for me correctly. I wish I could
include more of the kernel files that I modified (e.g. fork.c) but I cannot find a way to
access them now. I will speak to my TA ASAP about getting it working again.

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
