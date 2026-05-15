// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_soft_heap_limit64(const uint8_t *data, size_t size) {
    sqlite3_int64 limit = 0;
    
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&limit, data, sizeof(sqlite3_int64));
    } else if (size > 0) {
        limit = (sqlite3_int64)data[0];
    }
    
    sqlite3_int64 previous = sqlite3_soft_heap_limit64(limit);
    
    if (limit == -1 || limit == 0 || limit > 0) {
        sqlite3_soft_heap_limit64(previous);
    }
}

static void fuzz_hard_heap_limit64(const uint8_t *data, size_t size) {
    sqlite3_int64 limit = 0;
    
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&limit, data, sizeof(sqlite3_int64));
    } else if (size > 0) {
        limit = (sqlite3_int64)data[0];
    }
    
    sqlite3_int64 previous = sqlite3_hard_heap_limit64(limit);
    
    if (limit == -1 || limit == 0 || limit > 0) {
        sqlite3_hard_heap_limit64(previous);
    }
}

static void fuzz_memory_functions(const uint8_t *data, size_t size) {
    int reset_flag = 0;
    
    if (size > 0) {
        reset_flag = data[0] & 1;
    }
    
    sqlite3_int64 current_usage = sqlite3_memory_used();
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    if (reset_flag) {
        sqlite3_memory_highwater(0);
    }
}

static void fuzz_status_functions(const uint8_t *data, size_t size) {
    int op = SQLITE_STATUS_MEMORY_USED;
    int reset_flag = 0;
    
    if (size >= 2) {
        op = data[0] % 10;
        reset_flag = data[1] & 1;
    } else if (size == 1) {
        op = data[0] % 10;
    }
    
    sqlite3_int64 current64 = 0;
    sqlite3_int64 highwater64 = 0;
    int current = 0;
    int highwater = 0;
    
    sqlite3_status64(op, &current64, &highwater64, reset_flag);
    sqlite3_status(op, &current, &highwater, reset_flag);
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_soft_heap_limit64(Data, Size);
    fuzz_hard_heap_limit64(Data, Size);
    fuzz_memory_functions(Data, Size);
    fuzz_status_functions(Data, Size);
    
    return 0;
}