#include <stdlib.h>

#ifndef MACH_H
#define MACH_H

#include <mach/mach.h>

#endif

typedef struct address_node {
    mach_vm_address_t address;
    mach_vm_size_t region_size;

    struct address_node *next;
} address_node_t;

int add_address_to_list( mach_vm_address_t address, mach_vm_size_t region_size );
void address_list_iterate( void ( *function )( mach_vm_address_t address, mach_vm_size_t region_size ) );
void address_list_cleanup( );
