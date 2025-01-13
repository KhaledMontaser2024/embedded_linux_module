#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/rtc.h>
#define PROC_NAME "kernel_stats"

// Function to show data in the proc file
static int proc_show(struct seq_file *m, void *v) {
    struct task_struct *task;
    struct sysinfo si;
    struct timespec time;
    struct rtc_time rtc_tm;


    // Gather task details
    seq_printf(m, "Task List:\n");
    for_each_process(task) {
        seq_printf(m, "  PID: %d | Name: %s | State: %ld\n",
                   task->pid, task->comm, task->state);
    }
    
    // Gather memory statistics
    si_meminfo(&si);	
    seq_printf(m, "Memory Usage:\n");
    seq_printf(m, "  Total RAM: %lu MB\n", si.totalram * si.mem_unit / 1024 / 1024);
    seq_printf(m, "  Free RAM: %lu MB\n", si.freeram * si.mem_unit / 1024 / 1024);
    seq_printf(m, "  Used RAM: %lu MB\n\n", (si.totalram - si.freeram) * si.mem_unit / 1024 / 1024);


    // CPU Load Average
    seq_printf(m, "Load Averages:\n");
    seq_printf(m, "  1 min: %lu\n", si.loads[0] / 65536);
    seq_printf(m, "  5 min: %lu\n", si.loads[1] / 65536);
    seq_printf(m, "  15 min: %lu\n\n", si.loads[2] / 65536);

    // system time 
    getnstimeofday(&time);  // This gets the current time in 'timespec' format
    seq_printf(m, "System Time:\n");
    seq_printf(m, "Current time: %ld.%09ld\n", time.tv_sec, time.tv_nsec);

    // Print real-time clock (RTC) time
    rtc_time_to_tm(time.tv_sec, &rtc_tm);
    seq_printf(m, "Current RTC Time: %04d-%02d-%02d %02d:%02d:%02d\n",
        rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday,
        rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

    return 0;
}

// Open function
static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, proc_show, NULL);
}

// Define file operations for /proc entry
static const struct file_operations proc_fops = {
    .owner   = THIS_MODULE,
    .open    = proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};

// Module initialization
static int __init kernel_stats_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Module cleanup
static void __exit kernel_stats_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("khaled Montaser");
MODULE_DESCRIPTION("Kernel module to gather system statistics");
MODULE_VERSION("1.0");

module_init(kernel_stats_init);
module_exit(kernel_stats_exit);
