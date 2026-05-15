// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static sqlite3_int64 read_int64(const uint8_t *data, size_t size, size_t *pos) {
    sqlite3_int64 value = 0;
    if (*pos + sizeof(value) <= size) {
        memcpy(&value, data + *pos, sizeof(value));
        *pos += sizeof(value);
    } else {
        for (size_t i = 0; i < sizeof(value) && *pos < size; i++) {
            value = (value << 8) | data[(*pos)++];
        }
    }
    return value;
}

static int read_int(const uint8_t *data, size_t size, size_t *pos) {
    int value = 0;
    if (*pos < size) {
        value = data[(*pos)++];
    }
    return value;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }

    if (Size == 0) {
        return 0;
    }

    size_t pos = 0;
    uint8_t selector = read_int(Data, Size, &pos) % 6;

    switch (selector) {
        case 0: {
            int resetFlag = read_int(Data, Size, &pos) & 1;
            sqlite3_memory_highwater(resetFlag);
            break;
        }
        case 1: {
            int op = read_int(Data, Size, &pos);
            sqlite3_int64 current = 0;
            sqlite3_int64 highwater = 0;
            int resetFlag = read_int(Data, Size, &pos) & 1;
            sqlite3_status64(op, &current, &highwater, resetFlag);
            break;
        }
        case 2: {
            int op = read_int(Data, Size, &pos);
            int current = 0;
            int highwater = 0;
            int resetFlag = read_int(Data, Size, &pos) & 1;
            sqlite3_status(op, &current, &highwater, resetFlag);
            break;
        }
        case 3: {
            sqlite3_memory_used();
            break;
        }
        case 4: {
            sqlite3_int64 N = read_int64(Data, Size, &pos);
            sqlite3_soft_heap_limit64(N);
            break;
        }
        case 5: {
            sqlite3_int64 N = read_int64(Data, Size, &pos);
            sqlite3_hard_heap_limit64(N);
            break;
        }
    }

    return 0;
}