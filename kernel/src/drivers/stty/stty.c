#include <stdint.h>
#include <kernel.h>
#include <cio.h>

#define SUCCESS 0
#define EOF -1
#define FAILURE -2

#define MAX_STTY 1

char* stty_names[] = { "stty0", "stty1", "stty2", "stty3" };

char* com_devices[] = { ":://com1", ":://com2", ":://com3", ":://com4" };

char* devices[MAX_STTY];

int stty_io_wrapper(uint32_t dev, uint64_t loc, int type, uint8_t payload) {
    if (type == 0) {
        int val = vfs_kread(devices[dev], 0);
        if (val == IO_NOT_READY) return IO_NOT_READY;
        switch (val) {
            case 0x0d:
                vfs_kwrite(devices[dev], 0, 0x0d);
                vfs_kwrite(devices[dev], 0, 0x0a);
                return 0x0a;
            case 0x08:
                vfs_kwrite(devices[dev], 0, 0x08);
                vfs_kwrite(devices[dev], 0, 0x20);
                vfs_kwrite(devices[dev], 0, 0x08);
                return 0x08;
        }
        vfs_kwrite(devices[dev], 0, val);
        return val;
    }
    else if (type == 1) {
        switch (payload) {
            case 0x0a:
                vfs_kwrite(devices[dev], 0, 0x0d);
                vfs_kwrite(devices[dev], 0, 0x0a);
                return SUCCESS;
            case 0x08:
                vfs_kwrite(devices[dev], 0, 0x08);
                vfs_kwrite(devices[dev], 0, 0x20);
                vfs_kwrite(devices[dev], 0, 0x08);
                return SUCCESS;
        }
        return vfs_kwrite(devices[dev], 0, payload);
    }
}

void init_stty(void) {

    kputs("\nInitialising serial ttys...");
    
    for (int i = 0; i < MAX_STTY; i++) {
        kstrcpy(devices[i], com_devices[i]);
        kernel_add_device(stty_names[i], i, 0, &stty_io_wrapper);
        kputs("\nInitialised "); kputs(stty_names[i]);
    }

}
