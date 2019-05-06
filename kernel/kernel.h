
#ifndef KERNEL_H   
# define KERNEL_H  

# define KEYBOARD_STATUS_PORT 0x64
# define KEYBOARD_DATA_PORT 0x60

typedef uint16_t u16;

void		write_port(u16, u16);
u16		read_port(u16);

#endif
