
#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <linux/kernel.h>
#include <linux/types.h>

bool read_process_memory(pid_t pid, uintptr_t addr, void *buffer, size_t size);
bool write_process_memory(pid_t pid, uintptr_t addr, void *buffer, size_t size);

#endif /* _MEMORY_H_ */
