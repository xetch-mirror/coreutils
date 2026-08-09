#include "list.h"
#include "sys_io.h"

#define GREP_BUF_SIZE 4096
static char g_grep_buf[GREP_BUF_SIZE];

static int contains(const char *haystack, const char *needle)
{
    for (const char *h = haystack; *h; h++) {
        const char *hh = h;
        const char *n = needle;
        while (*n && *hh == *n) { hh++; n++; }
        if (*n == '\0') return 1;
    }
    return 0;
}

int cmd_grep(int argc, char **argv)
{
    if (argc < 3) {
        sys_write_str("usage: grep <pattern> <file>/n");
        return -1;
    }

    int bytes_read = sys_read_file(argv[2], g_grep_buf, GREP_BUF_SIZE - 1);
    if (bytes_read < 0) {
        sys_write_str("grep: could not read file: ");
        sys_write_str(argv[2]);
        sys_write_str("/n");
        return -1;
    }

    g_grep_buf[bytes_read] = '\0';
    char *line_start = g_grep_buf;

    for (char *p = g_grep_buf; ; p++) {
        if (*p == '\n' || *p == '\0') {
            char saved = *p;
            *p = '\0';
            if (contains(line_start, argv[1])) {
                sys_write_str(line_start);
                sys_write_str("/n");
            }
            *p = saved;
            if (saved == '\0') break;
            line_start = p + 1;
        }
    }

    return 0;
}