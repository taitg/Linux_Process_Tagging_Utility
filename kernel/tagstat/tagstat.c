/* CPSC 457 Spring 2016
 * Assignment 2
 *
 * Geordie Tait 10013837
 *
 * This is a kernel module designed to print all processes tagged with
 * ptag as virtual file /proc/ptag
 *
 * Based on code from:
 * http://www.linuxdevcenter.com/pub/a/linux/2007/07/05/devhelloworld-a-simple-introduction-to-device-drivers-under-linux.html?page=2
 * http://linuxgazette.net/133/saha.html
 * http://pointer-overloading.blogspot.ca/2013/09/linux-creating-entry-in-proc-file.html
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/kernel.h>

/* tagstat_show:
 * Called when /proc/ptag is read and displays processes, tags, and status
 */
static int tagstat_show(struct seq_file *m, void *v) {
	struct task_struct *ts;
	struct ptag_node *node;
	char *status;

    // iterate through all processes
	for_each_process(ts) {
        // iterate through tags for each process
		list_for_each_entry(node, &ts->ptag_list, list) {
            // print the PID and tag
			seq_printf(m, "%d : %s ", ts->pid, node->ptag);
            // determine the process status
			switch(ts->state) {
				case -1:
					status = "Unrunnable";
					break;
				case TASK_RUNNING:
					status = "Running";
					break;
				case TASK_INTERRUPTIBLE:
					status = "Interruptible";
					break;
				case TASK_UNINTERRUPTIBLE:
					status = "Uninterruptible";
					break;
				case TASK_STOPPED:
					status = "Stopped";
					break;
				case TASK_TRACED:
					status = "Traced";
					break;
				default:
					status = "Unknown";
			}
            // print the process status
			seq_printf(m, "(%s)\n", status);
		}
	}

	return 0;
}

/* tagstat_open:
 * Called when /proc/ptag is opened
 */
static int tagstat_open(struct inode *inode, struct file *file) {
	return single_open(file, tagstat_show, NULL);
}

/* tagstat_fops:
 * Defines file operations for /proc/ptag
 */
static const struct file_operations tagstat_fops = {
	.owner = THIS_MODULE,
	.open = tagstat_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

/* tagstat_init:
 * Called when the module is loaded
 */
static int __init tagstat_init(void) {
	proc_create("ptag", 0, NULL, &tagstat_fops);
	return 0;
}

/* tagstat_exit:
 * Called when the module is unloaded
 */
static void __exit tagstat_exit(void) {
	remove_proc_entry("ptag", NULL);
}

// module init and exit definitions
module_init(tagstat_init);
module_exit(tagstat_exit);

MODULE_LICENSE("GPL");
