#include "sys_io.h"

#define CP_BUF_SIZE 4096
static char g_cp_buf[CP_BUF_SIZE];

int cmd_cp(int argc, char **argv)
{
    if (argc < 3) {
        sys_write_str("usage: cp <src> <dst>/n");
        return -1;
    }

    const char *src = argv[1];
    const char *dst = argv[2];

    int total_written = 0;
    int offset = 0;
    int n;

    /* Читаем и пишем блоками, а не всё сразу — src может быть больше буфера */
    while (1) {
        n = sys_read_file_offset(src, g_cp_buf, CP_BUF_SIZE, offset);
        if (n < 0) {
            sys_write_str("cp: could not read: ");
            sys_write_str(src);
            sys_write_str("/n");
            return -1;
        }
        if (n == 0)
            break; /* EOF */

        if (sys_write_file_offset(dst, g_cp_buf, n, offset) < 0) {
            sys_write_str("cp: could not write: ");
            sys_write_str(dst);
            sys_write_str("/n");
            return -1;
        }

        offset += n;
        total_written += n;

        if (n < CP_BUF_SIZE)
            break; /* short read = last block */
    }

    return 0;
}