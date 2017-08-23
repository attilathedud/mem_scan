#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

#ifndef ADDRESS_H
#define ADDRESS_H
#include "address_list.h"
#endif

/*!
*   Given a pid, return the task mach_port for that process and fill in kern_return with the detailed
*   return code.
*/
mach_port_t get_task_for_pid( int pid, kern_return_t *kern_return );

/*!
*   Given a task and upper_limit, iterate through all active memory regions and add them to the passed list.
*/
void fill_active_memory_regions( address_list_t *list, mach_port_t task, mach_vm_address_t upper_limit );

/*!
*   Given a memory list, task, and value, iterate through the memory regions, copy each into a buffer,
*   and then scan for the passed value. If the value is found, add it to the passed results list.
*/
void scan_memory_regions( address_list_t *list, mach_port_t task, uint32_t value, address_list_t *results );

/*!
*   Given a task, address, and value, write value to the address.
*/
mem_return_t write_memory( mach_port_t task, unsigned long address, uint32_t value, kern_return_t *kern_return );