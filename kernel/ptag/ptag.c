/* CPSC 457 Spring 2016
 * Assignment 2
 *
 * Geordie Tait 10013837
 *
 * New system call for linux kernel which adds and removes tags
 * from processes based on pid
 *
 * Based on code from:
 * http://www.makelinux.net/ldd3/chp-11-sect-5
 * http://stackoverflow.com/questions/26807284/c-linux-built-in-linked-list-in-kernel-data-structures-usage
 * http://stackoverflow.com/questions/939778/linux-api-to-list-running-processes
 */

#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/list.h>

#define EPODRF -1

/* sys_ptag system call:
 * Adds or removes a tag from a pid
 * Args:
 *  pid - the process ID
 *  tag - tag string to add or remove
 *  mode - 0 for add, 1 for remove
 * Return:
 *  0 - success
 *  1 - failure, invalid PID
 *  2 - failure, tag already exists
 *  3 - failure, tag does not exist
 */
asmlinkage long sys_ptag(pid_t pid, char *tag, int mode) {

	struct ptag_node *node, *next;
	struct task_struct *ts;
	int pid_exists = 0;

    // check if PID exists
	for_each_process(ts) {
		if (ts->pid == pid)
			pid_exists = 1;
	}

    // print error message and return 1 if invalid PID
	if (pid_exists == 0) {
		printk("%d is not a valid PID\n", pid);
		return 1;
	} else {
        // get the task_struct from PID
		ts = pid_task(find_vpid((pid_t) pid), PIDTYPE_PID);
        // lock the task
		task_lock(ts);

        // add a new tag
		if (mode == 0) {
			int exists = 0;
            // check if the tag exists already
			list_for_each_entry(node, &ts->ptag_list, list) {
				if (strcmp(node->ptag,tag) == 0)
					exists = 1;
			}
			if (exists == 0) {
                // allocate kernel memory for new tag node
				node = kmalloc(sizeof(node), GFP_KERNEL);
				node->ptag = kmalloc(sizeof(tag), GFP_KERNEL);
				strcpy(node->ptag, tag);
                // add the new node
				list_add_tail(&node->list, &ts->ptag_list);
				printk("Tag %s attached to process %d\n", tag, pid);
			} else {
                // if tag exists, print error message, unlock, and return 2
				printk("Tag %s already attached to process %d\n", tag, pid);
				task_unlock(ts);
				return 2;
			}

        // remove a tag
		} else if (mode == 1) {
			int success = 0;
            // iterate through process's tags and remove if matching
			list_for_each_entry_safe(node, next, &ts->ptag_list, list) {
				if (strcmp(node->ptag,tag) == 0) {
                    // delete tag node
					list_del(&node->list);
					//kfree(&node);
					printk("Tag %s removed from process %d\n", tag, pid);
					success = 1;
				}
			}
			if (success == 0) {
                // if tag doesn't match, print error, unlock, and return 3
				printk("Tag %s not associated with process %d\n", tag, pid);
				task_unlock(ts);
				return 3;
			}
		}
        // unlock task
		task_unlock(ts);
	}
    // successs: return 0
	return 0;
}
