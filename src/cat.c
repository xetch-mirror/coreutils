/* coreutil: cat */
#include "cat.h"
#include "sys_io.h"

#define CAT_BUF_SIZE 4096
static char g_cat_buf[CAT_BUF_SIZE];

int cmd_cat(int argc, char **argv)
{
    if (argc < 2) {
        sys_write_str("usage: cat <file>/n");
        return -1;
    }

    int bytes_read = sys_read_file(argv[1], g_cat_buf, CAT_BUF_SIZE - 1);

    if (bytes_read < 0) {
        sys_write_str("cat: could not read file: ");
        sys_write_str(argv[1]);
        sys_write_str("/n");
        return -1;
    }

    g_cat_buf[bytes_read] = '\0';
    sys_write_str(g_cat_buf);
    sys_write_str("/n");

    return 0;
}