#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "../include/address_list.h"
#include "../include/mem_functions.h"
#include "../include/common.h"

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
	if( end_ptr != optarg && errno != ERANGE && (temp_value >= INT_MIN || temp_value <= INT_MAX))
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
    kern_return_t kern_return;

    options_t passed_options;

    int cur_arg = 0;

    while( ( cur_arg = getopt( argc, argv, "p:m" ) ) != -1 )
    {
        switch( cur_arg )
        {
            case 'p':
                passed_options.pid = get_long_value_from_optarg( optarg, 10 );
                break;
            case 'm':
                passed_options.show_map = 1;
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

                return 1;
            default:
                abort( );
        }
    }

    if( passed_options.pid != 0 )
    {
        passed_options.task = get_task_for_pid( passed_options.pid, &kern_return );
        if( passed_options.task == -1 || kern_return != KERN_SUCCESS )
        {
            printf( "task_for_pid failed: %s\n", mach_error_string( kern_return ) );
            return 0;
        }

        fill_active_memory_regions( passed_options.task );
    }

    if( passed_options.show_map )
    {
        printf( "Address \t\t Size (Bytes)\n" );
        printf( "======= \t\t ============\n" );
        address_list_iterate( &print_memory_map );
    }

    address_list_cleanup( );

    return 0;
}