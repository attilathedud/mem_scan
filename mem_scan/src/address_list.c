#include "../include/address_list.h"

/*!
*   Given an address and the associated region size, add it to the provided list.
*/
mem_return_t add_address_to_list( address_list_t *list, mach_vm_address_t address, mach_vm_size_t region_size )
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

    return RETURN_SUCCESS;
}

/*!
*   Given a list and a function pointer, apply that function to each item in the list.
*
*   Function prototype must be in the form of void function( mach_vm_address_t, mach_vm_size_t ).
*/
void address_list_iterate( address_list_t *list, void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) )
{
    address_node_t *cur_entry = list->head;

    while( cur_entry != NULL )
    {
        (*function)( cur_entry->address, cur_entry->region_size );

        cur_entry = cur_entry->next;
    }
}

/*!
*   Given a list, free all memory for each node.
*/
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