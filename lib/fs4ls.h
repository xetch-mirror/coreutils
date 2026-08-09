// fs4ls.h

#ifndef _LS_H
#define _LS_H

#include <stdint.h>

struct __attribute__((__packed__)) FAT32DirectoryEntry {
    uint8_t  filename[11];      // 8 bytes name + 3 bytes extension
    uint8_t  attributes;       // File flags (Read-only, Hidden, Directory, etc.)
    uint8_t  reserved_win_nt;  // Reserved byte
    uint8_t  creation_time_ms; // Creation time in milliseconds
    uint16_t creation_time;    // Hour/minute/second
    uint16_t creation_date;    // Year/month/day
    uint16_t last_access_date; // Last time the file was opened
    uint16_t cluster_high;     // Top 16 bits of the file data location
    uint16_t modified_time;    // Last modification time
    uint16_t modified_date;    // Last modification date
    uint16_t cluster_low;      // Bottom 16 bits of the file data location
    uint32_t file_size;        // Size of the file in bytes
};