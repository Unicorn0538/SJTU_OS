#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/vmalloc.h> 

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

static long id;

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t  *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t  *pos);

static struct file_operations proc_ops={
	.owner = THIS_MODULE,
	.read = proc_read,
	.write =proc_write,
};


static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t  *pos)
{
	int rv=0;
	char buffer[BUFFER_SIZE];
	static int completed=0;
	struct task_struct *task_pid =  NULL;
	
	if (completed){
	      completed=0;
      		return 0;
    	}
	
	task_pid = pid_task(find_vpid(id),PIDTYPE_PID);
	
    	completed=1;
    	rv=sprintf(buffer, "command = %s, pid = %ld, state = %ld\n",task_pid->comm,id,task_pid->state);
    	raw_copy_to_user(usr_buf,buffer,rv);
    	return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t  *pos)
{
	char *k_mem;
	char buf[BUFFER_SIZE];
	k_mem = kmalloc(count,GFP_KERNEL);
	
	
	if(raw_copy_from_user(k_mem,usr_buf,count))
	{
		printk(KERN_INFO "ERROR!\n");
		return -1;
	}
	
	
	sscanf(k_mem,"%s",buf);
	kstrtol(buf,10,&id);
	kfree(k_mem);
	return count;
}
int proc_init(void)
{
    proc_create(PROC_NAME,0666,NULL,&proc_ops);
    printk(KERN_INFO "created!\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME,NULL);
    printk(KERN_INFO "removed!\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("project1");
MODULE_AUTHOR("Unicorn");

