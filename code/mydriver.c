
#include "mydriver.h"

static ssize_t menu_write(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos) {
    char kbuf[128];
    size_t len = min(count, sizeof(kbuf) - 1);

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';
    printk(KERN_INFO "[Menu.ko] 收到指令: %s\n", kbuf);
    return len;
}

static ssize_t menu_read(struct file *file, char __user *buf,
                         size_t count, loff_t *ppos) {
    const char *msg = "Menu.ko 已就绪 (适配Android 14 / Kernel 6.1)\n";
    int len = strlen(msg);

    if (*ppos >= len)
        return 0;

    len = min(count, (size_t)len);
    if (copy_to_user(buf, msg, len))
        return -EFAULT;

    *ppos += len;
    return len;
}

static struct file_operations menu_fops = {
    .owner = THIS_MODULE,
    .read  = menu_read,
    .write = menu_write,
};

static int __init menu_init(void) {
    if (!proc_create(PROC_NAME, 0666, NULL, &menu_fops)) {
        printk(KERN_ERR "[Menu.ko] 创建 /proc/%s 失败\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "[Menu.ko] 加载成功，节点: /proc/%s\n", PROC_NAME);
    return 0;
}

static void __exit menu_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "[Menu.ko] 已卸载\n");
}

module_init(menu_init);
module_exit(menu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rtop12");
MODULE_DESCRIPTION("通过 /proc/Menu 与终端通信的内核模块");
