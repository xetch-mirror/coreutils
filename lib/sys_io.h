#ifndef SYS_IO_H
#define SYS_IO_H

/* Абстрактный слой ввода-вывода — coreutils не знает о FAT32-драйвере
   Zythos напрямую, только об этих примитивах. */

int sys_read_file(const char *path, void *buf, unsigned int max_len);
void sys_write_str(const char *str);

/* Читает один сектор диска по LBA-адресу.
   Возвращает 0 при успехе, отрицательное значение при ошибке. */
int sys_read_sector(unsigned int lba, void *buf);

/* Геометрия смонтированной файловой системы —
   достаточно для вычисления LBA по номеру кластера без fat32.h */
struct sys_fs_geometry {
    unsigned short bytes_per_sector;
    unsigned char  sectors_per_cluster;
    unsigned int   fat_begin_lba;
    unsigned int   cluster_begin_lba;
    unsigned int   root_dir_first_cluster;
};

/* Возвращает 0 при успехе (fs смонтирована), отрицательное — иначе */
int sys_get_fs_geometry(struct sys_fs_geometry *out);

#endif // SYS_IO_H