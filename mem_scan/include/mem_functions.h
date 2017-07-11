#ifndef MACH_H
#define MACH_H

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_region.h>

#endif

#ifndef ADDRESS_H
#define ADDRESS_H

#include "../include/address_list.h"

#endif

mach_port_t get_task_for_pid( int pid, kern_return_t *kern_return );
void fill_active_memory_regions( address_list_t *list, mach_port_t task );
void scan_memory_regions( address_list_t *list, mach_port_t task, uint32_t value, address_list_t *results );