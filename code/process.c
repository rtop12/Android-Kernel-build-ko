
	#include "process.h"
	#include <linux/sched.h>
	#include <linux/sched/mm.h>
	#include <linux/sched/task.h>
	#include <linux/module.h>
	#include <linux/mm.h>
	#include <linux/version.h>
	#include <linux/pid.h>
	#include <linux/fs.h>
	#include <linux/dcache.h>
	#include <linux/rwsem.h>
	
	#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
	#define mmap_read_lock(mm)    down_read(&(mm)->mmap_sem)
	#define mmap_read_unlock(mm)  up_read(&(mm)->mmap_sem)
	#endif
	
	#define ARC_PATH_MAX 256
	
	uintptr_t get_module_base(pid_t pid, char *name)
	{
		struct pid *pid_struct;
		struct task_struct *task;
		struct mm_struct *mm;
		struct vm_area_struct *vma;
		uintptr_t base_addr = 0;
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0))
		struct vma_iterator vmi;
	#endif
	
		pid_struct = find_get_pid(pid);
		if (!pid_struct)
			return 0;
	
		task = get_pid_task(pid_struct, PIDTYPE_PID);
		put_pid(pid_struct);
		if (!task)
			return 0;
	
		mm = get_task_mm(task);
		put_task_struct(task);
		if (!mm)
			return 0;
	
		mmap_read_lock(mm);
	
	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0))
		vma_iter_init(&vmi, mm, 0);
		for_each_vma(vmi, vma)
	#else
		for (vma = mm->mmap; vma; vma = vma->vm_next)
	#endif
		{
			if (vma->vm_file) {
				char buf[ARC_PATH_MAX];
				char *path_nm;
	
				path_nm = d_path(&vma->vm_file->f_path, buf, ARC_PATH_MAX - 1);
				if (!IS_ERR(path_nm)) {
					const char *basename = kbasename(path_nm);
					if (strcmp(basename, name) == 0) {
						base_addr = vma->vm_start;
						break;
					}
				}
			}
		}
	
		mmap_read_unlock(mm);
		mmput(mm);
		return base_addr;
	}
	