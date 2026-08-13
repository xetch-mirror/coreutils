#include "sys_io.h"
#include "vftypes.h"
#include "vfs.h"

int cmd_ln(int argc, char **argv)
{
    if (argc < 3) {
        sys_write_str("usage: ln <target> <linkname>/n");
        return -1;
    }

    const char *target = argv[1];
    const char *linkname = argv[2];

    if (!vfs_exists(target)) {
        sys_write_str("ln: no such target: ");
        sys_write_str(target);
        sys_write_str("/n");
        return -1;
    }

    if (vfs_exists(linkname)) {
        sys_write_str("ln: linkname already exists: ");
        sys_write_str(linkname);
        sys_write_str("/n");
        return -1;
    }

    if (vfs_link(target, linkname) != VFS_OK) {
        sys_write_str("ln: could not create link/n");
        return -1;
    }

    return 0;
}