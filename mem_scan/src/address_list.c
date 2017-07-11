#include "../include/address_list.h"

int add_address_to_list( address_list_t *list, mach_vm_address_t address, mach_vm_size_t region_size )
{
    address_node_t *cur_entry = malloc( sizeof( address_node_t ) );

    if( cur_entry == NULL )
        return -1;

    cur_entry->address = address;
    cur_entry->region_size = region_size;

    if( list->head == NULL )
    {
        list->head = cur_entry;
    }
    else
    {
        list->last_entry->next = cur_entry;
    }

    list->last_entry = cur_entry;

    return 0;
}

void address_list_iterate( address_list_t *list, void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) )
{
    address_node_t *cur_entry = list->head;

    while( cur_entry != NULL )
    {
        (*function)( cur_entry->address, cur_entry->region_size );

        cur_entry = cur_entry->next;
    }
}

void address_list_cleanup( address_list_t *list )
{
    address_node_t *prev_entry = NULL;
    address_node_t *cur_entry = list->head;

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