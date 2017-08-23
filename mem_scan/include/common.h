#ifndef MACH_H
#define MACH_H

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_region.h>

#endif

/*!
*   Return types for the mach_vm operations.
*/
typedef enum mem_return {
    RETURN_SUCCESS = 0, 
    RETURN_GERROR = -1, 
    RETURN_INVALID_PID = -2, 
    RETURN_KERN_ERROR = -3, 
    RETURN_VM_PROTECT_ERROR = -4, 
    RETURN_VM_WRITE_ERROR = -5
} mem_return_t;
