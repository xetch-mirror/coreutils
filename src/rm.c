#include "sys_io.h"
#include "vftypes.h"
#include "vfs.h"

int cmd_rm(int argc, char **argv)
{
    if (argc < 2) {
        sys_write_str("usage: rm <path>/n");
        return -1;
    }

    if (!vfs_exists(argv[1])) {
        sys_write_str("rm: no such file: ");
        sys_write_str(argv[1]);
        sys_write_str("/n");
        return -1;
    }

    int r = vfs_unlink(argv[1]);
    if (r != VFS_OK) {
        sys_write_str("rm: could not remove: ");
        sys_write_str(argv[1]);
        sys_write_str("/n");
        return -1;
    }

    return 0;
}