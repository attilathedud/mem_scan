typedef struct options {
    int pid;
    mach_port_t task;
    int show_map;
    uint32_t read_value;
    uint32_t write_value;
    mach_vm_address_t write_address;
} options_t;