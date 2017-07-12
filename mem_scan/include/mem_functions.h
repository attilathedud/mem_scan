#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

#ifndef ADDRESS_H
#define ADDRESS_H
#include "address_list.h"
#endif

mach_port_t get_task_for_pid( int pid, kern_return_t *kern_return );
void fill_active_memory_regions( address_list_t *list, mach_port_t task );
void scan_memory_regions( address_list_t *list, mach_port_t task, uint32_t value, address_list_t *results );
mem_return_t write_memory( mach_port_t task, unsigned long address, uint32_t value, kern_return_t *kern_return );