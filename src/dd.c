#include "sys_io.h"

#define DD_DEFAULT_BS 512
#define DD_MAX_BS     4096
static char g_dd_buf[DD_MAX_BS];

/* Простой парсер аргументов вида if=SRC of=DST bs=N count=N */
static int parse_kv(const char *arg, const char *key, const char **out)
{
    int klen = 0;
    while (key[klen]) klen++;

    for (int i = 0; i < klen; i++)
        if (arg[i] != key[i]) return 0;

    if (arg[klen] != '=') return 0;
    *out = arg + klen + 1;
    return 1;
}

static int my_atoi(const char *s)
{
    int n = 0;
    while (*s >= '0' && *s <= '9') { n = n * 10 + (*s - '0'); s++; }
    return n;
}

int cmd_dd(int argc, char **argv)
{
    const char *src = 0;
    const char *dst = 0;
    int bs = DD_DEFAULT_BS;
    int count = -1; /* -1 = без ограничения, копировать до EOF */

    for (int i = 1; i < argc; i++) {
        const char *val;
        if (parse_kv(argv[i], "if", &val)) src = val;
        else if (parse_kv(argv[i], "of", &val)) dst = val;
        else if (parse_kv(argv[i], "bs", &val)) bs = my_atoi(val);
        else if (parse_kv(argv[i], "count", &val)) count = my_atoi(val);
    }

    if (!src || !dst) {
        sys_write_str("usage: dd if=<src> of=<dst> [bs=N] [count=N]/n");
        return -1;
    }

    if (bs <= 0 || bs > DD_MAX_BS) {
        sys_write_str("dd: bs out of range (1..4096)/n");
        return -1;
    }

    int offset = 0;
    int blocks_done = 0;
    int n;

    while (count < 0 || blocks_done < count) {
        n = sys_read_file_offset(src, g_dd_buf, bs, offset);
        if (n < 0) {
            sys_write_str("dd: read error: ");
            sys_write_str(src);
            sys_write_str("/n");
            return -1;
        }
        if (n == 0)
            break; /* EOF */

        if (sys_write_file_offset(dst, g_dd_buf, n, offset) < 0) {
            sys_write_str("dd: write error: ");
            sys_write_str(dst);
            sys_write_str("/n");
            return -1;
        }

        offset += n;
        blocks_done++;

        if (n < bs)
            break; /* короткое чтение = EOF */
    }

    sys_write_str("dd: ");
    /* TODO: вывод числа blocks_done требует itoa — пока не реализован в этом файле */
    sys_write_str(" blocks copied/n");

    return 0;
}