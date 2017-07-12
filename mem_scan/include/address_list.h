#include <stdlib.h>

#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#endif

typedef struct address_node {
    mach_vm_address_t address;
    mach_vm_size_t region_size;

    struct address_node *next;
} address_node_t;

typedef struct address_list {
    address_node_t *head;
    address_node_t *last_entry;
} address_list_t;

mem_return_t add_address_to_list( address_list_t *list, mach_vm_address_t address, mach_vm_size_t region_size );
void address_list_iterate( address_list_t *list, void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) );
void address_list_cleanup( address_list_t *list );
