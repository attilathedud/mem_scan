#include "../include/mem_functions.h"
#include "../include/address_list.h"

mach_port_t get_task_for_pid( int pid, kern_return_t *kern_return )
{
    mach_port_t task;

    if( pid <= 0 )
        return -1;

    *kern_return = task_for_pid( mach_task_self(), pid, &task );
    if( *kern_return != KERN_SUCCESS ) 
    {
        return -1;
    }

    return task;
}

void fill_active_memory_regions( mach_port_t task )
{
    kern_return_t kern_return;

    mach_vm_address_t address = 0;

    mach_vm_size_t size;
    natural_t depth = 0;
    vm_region_submap_info_data_64_t info;
    mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;

    while( address <= 0x7fffffffffff )
    {
        count = VM_REGION_SUBMAP_INFO_COUNT_64;

        kern_return = mach_vm_region_recurse ( task, &address, &size, &depth, (vm_region_recurse_info_t) &info, &count );

        if( info.is_submap )
        {
            ++depth;
        }
        else
        {
            add_address_to_list( address, size );
            address += size;
        }
    }
}

