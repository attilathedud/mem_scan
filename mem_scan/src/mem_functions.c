#include "../include/mem_functions.h"

/*!
*   Given a pid, return the task mach_port for that process and fill in kern_return with the detailed
*   return code.
*/
mach_port_t get_task_for_pid( int pid, kern_return_t *kern_return )
{
    mach_port_t task = RETURN_INVALID_PID;

    if( pid <= 0 )
        return RETURN_INVALID_PID;

    // mach_task_self() is the calling task.
    *kern_return = task_for_pid( mach_task_self(), pid, &task );
    if( *kern_return != KERN_SUCCESS ) 
    {
        return RETURN_INVALID_PID;
    }

    return task;
}

/*!
*   Given a task and upper_limit, iterate through all active memory regions and add them to the passed list.
*/
void fill_active_memory_regions( address_list_t *list, mach_port_t task, mach_vm_address_t upper_limit )
{
    kern_return_t kern_return = 0;

    mach_vm_address_t address = 0;

    mach_vm_size_t size = 0;
    natural_t depth = 0;
    vm_region_submap_info_data_64_t info = { 0 };
    mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;

    upper_limit = ( upper_limit == 0 ? 0x7fffffffffff : upper_limit );

    while( address <= upper_limit )
    {
        count = VM_REGION_SUBMAP_INFO_COUNT_64;

        kern_return = mach_vm_region_recurse( task, &address, &size, &depth, (vm_region_recurse_info_t) &info, &count );

        if( info.is_submap )
        {
            ++depth;
        }
        else
        {
            add_address_to_list( list, address, size );
            address += size;
        }
    }
}

/*!
*   Given a memory list, task, and value, iterate through the memory regions, copy each into a buffer,
*   and then scan for the passed value. If the value is found, add it to the passed results list.
*/
void scan_memory_regions( address_list_t *list, mach_port_t task, uint32_t value, address_list_t *results )
{
    kern_return_t kern_return = 0;

    mach_vm_size_t bytes_read = 0;
    unsigned int buffer_size = 0x1000;

    address_node_t *cur_entry = list->head;
    while( cur_entry != NULL )
    {
        bytes_read = 0;
        
        // Due to vm_read's speed, copy blocks into a buffer and then scan through that buffer.
        while( bytes_read < cur_entry->region_size )
        {
            unsigned char buffer[ buffer_size ];
            uint32_t size = 0;
            pointer_t buffer_pointer = 0;

            kern_return = vm_read( task, cur_entry->address + bytes_read, 
                ( cur_entry->region_size < buffer_size ) ? cur_entry->region_size : buffer_size, 
                &buffer_pointer, &size );

            if( kern_return == KERN_SUCCESS )
            {
                memcpy( buffer, (const void *)buffer_pointer, size );

                unsigned int buffer_position = 0;
                while( buffer_position <= buffer_size )
                {
                    uint32_t num = 0;
                    memcpy( &num, &buffer[ buffer_position ], sizeof( uint32_t ) );

                    if( num == value )
                    {
                        add_address_to_list( results, ( cur_entry->address + bytes_read + buffer_position ), sizeof( uint32_t ) );
                    }

                    buffer_position += sizeof( uint32_t );
                }
            }

            bytes_read += buffer_size;
        }

        cur_entry = cur_entry->next;
    }
}

/*!
*   Given a task, address, and value, write value to the address.
*/
mem_return_t write_memory( mach_port_t task, unsigned long address, uint32_t value, kern_return_t *kern_return )
{
    // Unprotect the address first so we can write to it.
    *kern_return = vm_protect( task, address, sizeof( uint32_t ), 0, VM_PROT_READ | VM_PROT_WRITE );
    if( *kern_return != KERN_SUCCESS ) 
    {
        return RETURN_VM_PROTECT_ERROR;
    }

    *kern_return = vm_write( task, address, (pointer_t)( unsigned char * )&value, sizeof( uint32_t ) );
    if( *kern_return != KERN_SUCCESS ) 
    {
        return RETURN_VM_WRITE_ERROR;
    }

    return 0;
}