#include "../include/address_list.h"

address_node_t *head = NULL;
address_node_t *last_entry = NULL;

int add_address_to_list( mach_vm_address_t address, mach_vm_size_t region_size )
{
    address_node_t *cur_entry = malloc( sizeof( address_node_t ) );

    if( cur_entry == NULL )
        return -1;

    cur_entry->address = address;
    cur_entry->region_size = region_size;

    if( head == NULL )
    {
        head = cur_entry;
    }
    else
    {
        last_entry->next = cur_entry;
    }

    last_entry = cur_entry;

    return 0;
}

void address_list_iterate( void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) )
{
    address_node_t *cur_entry = head;

    while( cur_entry != NULL )
    {
        (*function)( cur_entry->address, cur_entry->region_size );

        cur_entry = cur_entry->next;
    }
}

void address_list_cleanup( )
{
    address_node_t *prev_entry;
    address_node_t *cur_entry;

    while( cur_entry != NULL )
    {
        if( prev_entry != NULL )
        {
            free( prev_entry );
        }
        prev_entry = cur_entry;

        cur_entry = cur_entry->next;
    }
}