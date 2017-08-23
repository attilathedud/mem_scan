#include <stdlib.h>

#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

/*!
*   Each node in our address list holds the address and the length of the memory stored there.
*
*   Nodes are used for both memory regions and when scanning memory for values.
*/
typedef struct address_node {
    mach_vm_address_t address;
    mach_vm_size_t region_size;

    struct address_node *next;
} address_node_t;

/*!
*   A modified linked-list implementation that holds the head and the last inserted member.
*/
typedef struct address_list {
    address_node_t *head;
    address_node_t *last_entry;
} address_list_t;

/*!
*   Given an address and the associated region size, add it to the provided list.
*/
mem_return_t add_address_to_list( address_list_t *list, mach_vm_address_t address, mach_vm_size_t region_size );

/*!
*   Given a list and a function pointer, apply that function to each item in the list.
*
*   Function prototype must be in the form of void function( mach_vm_address_t, mach_vm_size_t ).
*/
void address_list_iterate( address_list_t *list, void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) );

/*!
*   Given a list, free all memory for each node.
*/
void address_list_cleanup( address_list_t *list );
