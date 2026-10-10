#ifndef _DEVCFG_H
#define _DEVCFG_H

#include <linux/version.h>

#define DEVCFG_NODE     "devcfg"
#define DEVCFG_TRIGGER  "Menu"
#define DEVCFG_TRIGGER_LEN 4
#define DEVCFG_BUF_SIZE 16

ssize_t devcfg_write(struct file *file, const char __user *ubuf,
                     size_t cnt, loff_t *ppos);

int  devcfg_init(void);
void devcfg_exit(void);

#endif
