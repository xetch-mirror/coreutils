#include "list.h"
#include "sys_io.h"
#include "fs4ls.h"

#define FAT32_ATTR_DIRECTORY 0x10
#define FAT32_ATTR_LFN       0x0F

static uint8_t g_sector_buf[512]; /* TODO: предполагается 512 байт/сектор */

static uint32_t cluster_to_lba(const struct sys_fs_geometry *geo, uint32_t cluster)
{
    return geo->cluster_begin_lba + (cluster - 2) * geo->sectors_per_cluster;
}

static uint32_t fat_next_cluster(const struct sys_fs_geometry *geo, uint32_t cluster)
{
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = geo->fat_begin_lba + (fat_offset / geo->bytes_per_sector);
    uint32_t entry_offset = fat_offset % geo->bytes_per_sector;
    uint32_t value;

    if (sys_read_sector(fat_sector, g_sector_buf) != 0) {
        return 0x0FFFFFFF;
    }

    value = *(uint32_t *)(g_sector_buf + entry_offset);
    return value & 0x0FFFFFFF;
}

static int is_end_of_chain(uint32_t cluster)
{
    return cluster >= 0x0FFFFFF8;
}

static void format_83_name(const uint8_t *raw, char *out)
{
    int j = 0;

    for (int i = 0; i < 8 && raw[i] != ' '; i++) {
        out[j++] = (char)raw[i];
    }

    if (raw[8] != ' ') {
        out[j++] = '.';
        for (int i = 8; i < 11 && raw[i] != ' '; i++) {
            out[j++] = (char)raw[i];
        }
    }

    out[j] = '\0';
}

static void print_entry(const char *name, unsigned char attr)
{
    sys_write_str(name);

    if (attr & FAT32_ATTR_DIRECTORY) {
        sys_write_str("/  <DIR>/n");
    } else {
        sys_write_str("/n");
    }
}

int cmd_ls(int argc, char **argv)
{
    (void)argc;
    (void)argv; /* TODO: путь игнорируется — поддерживается только корень,
                   т.к. здесь нет обхода подкаталогов */

    struct sys_fs_geometry geo;

    if (sys_get_fs_geometry(&geo) != 0) {
        sys_write_str("ls: filesystem not mounted/n");
        return -1;
    }

    uint32_t cluster = geo.root_dir_first_cluster;
    char name_buf[13];

    while (!is_end_of_chain(cluster)) {
        uint32_t lba = cluster_to_lba(&geo, cluster);

        for (uint8_t s = 0; s < geo.sectors_per_cluster; s++) {
            if (sys_read_sector(lba + s, g_sector_buf) != 0) {
                sys_write_str("ls: read error/n");
                return -1;
            }

            struct FAT32DirectoryEntry *entries =
                (struct FAT32DirectoryEntry *)g_sector_buf;
            uint32_t count = geo.bytes_per_sector / sizeof(struct FAT32DirectoryEntry);

            for (uint32_t e = 0; e < count; e++) {
                if (entries[e].filename[0] == 0x00) {
                    return 0; /* конец списка записей */
                }
                if (entries[e].filename[0] == 0xE5) {
                    continue;
                }
                if (entries[e].attributes == FAT32_ATTR_LFN) {
                    continue; /* TODO: LFN не поддерживается */
                }

                format_83_name(entries[e].filename, name_buf);
                print_entry(name_buf, entries[e].attributes);
            }
        }

        cluster = fat_next_cluster(&geo, cluster);
    }

    return 0;
}