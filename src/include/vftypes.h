#ifndef VFTYPES_H
#define VFTYPES_H

#include <stdint.h>

/* Общие лимиты, используемые всеми утилитами файловой системы */
#define VFS_NAME_MAX   64
#define VFS_MAX_PATH   256
#define VFS_MAX_NODES  128
#define VFS_MAX_FILES  128

/* Коды возврата */
#define VFS_OK          0
#define VFS_ERROR      -1
#define VFS_ENOENT     -2
#define VFS_ENOMEM     -3

/* Флаги типа узла */
#define VFS_FILE       (1 << 0)  /* 0x01 */
#define VFS_DIRECTORY  (1 << 1)  /* 0x02 */
#define VFS_BLOCKDEV   (1 << 2)  /* 0x04 */

/* Предварительные объявления — сама структура и её операции
   остаются в vfs.h, здесь только типы, на которые все ссылаются */
struct vfs_node;
struct vfs_file;

typedef struct {
    int (*read)(struct vfs_node *node, uint32_t offset, void *buffer, uint32_t size);
    int (*write)(struct vfs_node *node, uint32_t offset, const void *buffer, uint32_t size);
} vfs_ops_t;

typedef struct vfs_node {
    char name[VFS_MAX_PATH];
    uint32_t size;
    uint32_t flags;
    uint32_t inode_id;
    void *priv_data;
    vfs_ops_t *ops;
} vfs_node_t;

typedef struct vfs_file {
    vfs_node_t *node;
    uint32_t offset;
    uint32_t flags;
} vfs_file_t;

#endif