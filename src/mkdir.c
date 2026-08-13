#include "sys_io.h"
#include "vftypes.h"
#include "vfs.h"

int cmd_mkdir(int argc, char **argv)
{
    if (argc < 2) {
        sys_write_str("usage: mkdir <path>/n");
        return -1;
    }

    if (vfs_exists(argv[1])) {
        sys_write_str("mkdir: already exists: ");
        sys_write_str(argv[1]);
        sys_write_str("/n");
        return -1;
    }

    if (vfs_mkdir(argv[1]) != VFS_OK) {
        sys_write_str("mkdir: could not create: ");
        sys_write_str(argv[1]);
        sys_write_str("/n");
        return -1;
    }

    return 0;
}