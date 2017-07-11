typedef struct options {
    int pid;
    mach_port_t task;
    int show_map;
    uint32_t read_value;
} options_t;