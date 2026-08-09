#ifndef SYS_IO_H
#define SYS_IO_H

/* Абстрактный слой ввода-вывода — coreutils ничего не знает
   о FAT32 или serial напрямую. Реализация предоставляется
   основным репозиторием Zythos при линковке. */

/* Читает файл целиком (или до max_len байт) в buf.
   Возвращает количество прочитанных байт, или отрицательное значение при ошибке. */
int sys_read_file(const char *path, void *buf, unsigned int max_len);

/* Выводит null-terminated строку куда-либо (serial/VGA/что угодно) */
void sys_write_str(const char *str);

#endif // SYS_IO_H