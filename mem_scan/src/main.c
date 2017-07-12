#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#ifndef COMMON_H
#define COMMON_H
#include "../include/common.h"
#endif

#ifndef ADDRESS_H
#define ADDRESS_H
#include "../include/address_list.h"
#endif

#include "../include/mem_functions.h"

/*! 
*	Helper function to safety extract a numerical value from a passed character array.
*
*	Sets errno with the result of strtol.
*
*	Returns the value on success, -1 on failure.
*/
unsigned long get_long_value_from_optarg( char* optarg, int base )
{
	char *end_ptr = NULL;

	errno = 0;

	unsigned long temp_value = strtol( optarg, &end_ptr, base );
	if( end_ptr != optarg && errno != ERANGE && (temp_value >= LONG_MIN || temp_value <= LONG_MAX))
	{
		return temp_value;
	}

	return -1;
}

void print_memory_map( mach_vm_address_t address, mach_vm_size_t region_size )
{
    printf( "0x%llx \t\t %llu\n", address, region_size );
}

int main( int argc, char** argv )
{
    typedef struct options {
        int                 pid;
        mach_port_t         task;
        int                 show_map;
        mach_vm_address_t   upper_limit;
        uint32_t            read_value;
        char                *filter_file;
        uint32_t            write_value;
        mach_vm_address_t   write_address;
    } options_t;

    address_list_t memory_regions = { 0 };

    kern_return_t kern_return = 0;

    options_t passed_options = { 0 };

    int cur_arg = 0;

    while( ( cur_arg = getopt( argc, argv, "p:mu:r:f:w:v:h" ) ) != -1 )
    {
        switch( cur_arg )
        {
            case 'p':
                passed_options.pid = get_long_value_from_optarg( optarg, 10 );
                break;
            case 'm':
                passed_options.show_map = 1;
                break;
            case 'u':
                passed_options.upper_limit = get_long_value_from_optarg( optarg, 16 );
                break;
            case 'r':
                passed_options.read_value = get_long_value_from_optarg( optarg, 10 );
                break;
            case 'f':
                passed_options.filter_file = optarg;
                break;
            case 'w':
                passed_options.write_address = get_long_value_from_optarg( optarg, 16 );
                break;
            case 'v':
                passed_options.write_value = get_long_value_from_optarg( optarg, 10 );
                break;
            case 'h':
                printf( "Help text.\n" );
                break;
            case '?':
                switch( optopt )
                {
                    case 'p':
                        fprintf( stderr, "Option %c requires a valid pid of a user-land process.\n", optopt );
                        break;
                    case 'r':
                        fprintf( stderr, "Option %c requires a valid int value to search for.\n", optopt );
                        break;
                    case 'f':
                        fprintf( stderr, "Option %c requires a valid path to a file.\n", optopt );
                        break;
                    case 'w':
                        fprintf( stderr, "Option %c requires a valid hex address to write to.\n", optopt );
                        break;
                    default:
						if ( isprint( optopt ) )
						{
							fprintf(stderr, "Unknown option '-%c'.\n", optopt);
						}
						else
						{
							fprintf(stderr, "Unknown option character '%x'.\n", optopt);
						}
						
						break;
                }

                return RETURN_GERROR;
            default:
                abort( );
        }
    }

    if( passed_options.pid == 0 )
    {
        return 0;
    }

    passed_options.task = get_task_for_pid( passed_options.pid, &kern_return );
    if( passed_options.task == RETURN_INVALID_PID || kern_return != KERN_SUCCESS )
    {
        printf( "task_for_pid failed: %s\n", mach_error_string( kern_return ) );
        return RETURN_GERROR;
    }

    if( passed_options.filter_file == NULL )
    {
        fill_active_memory_regions( &memory_regions, passed_options.task, passed_options.upper_limit );
    }
    else
    {
        FILE *filter_file = fopen( passed_options.filter_file, "r" );
        if( filter_file != NULL )
        {
            size_t cur_len = 0;
            char *cur_line = NULL;

            mach_vm_address_t address = 0;
            mach_vm_size_t region_size = 0;

            while( getline( &cur_line, &cur_len, filter_file ) != -1 )
            {
                sscanf( cur_line, "0x%llx \t\t %llu\n", &address, &region_size );
                add_address_to_list( &memory_regions, address, region_size );
            }

            if( cur_line != NULL )
            {
                free( cur_line );
            }

            fclose( filter_file );
        }
    }

    if( passed_options.show_map )
    {
        address_list_iterate( &memory_regions, &print_memory_map );
    }

    if( passed_options.read_value ) 
    {
        address_list_t results = { 0 };

        scan_memory_regions( &memory_regions, passed_options.task, passed_options.read_value, &results );
        address_list_iterate( &results, &print_memory_map );

        address_list_cleanup( &results );
    }

    if( passed_options.write_address && passed_options.write_value )
    {
        mem_return_t result = write_memory( passed_options.task, passed_options.write_address, passed_options.write_value, &kern_return );
        if( result == RETURN_VM_PROTECT_ERROR )
        {
            printf( "vm_protect failed: %s\n", mach_error_string( kern_return ) );
        }
        else if( result == RETURN_VM_WRITE_ERROR )
        {
            printf( "vm_write failed: %s\n", mach_error_string( kern_return ) );
        }
    }

    mach_port_deallocate( mach_task_self(), passed_options.task );

    address_list_cleanup( &memory_regions );

    return RETURN_SUCCESS;
}