// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_soft_heap_limit(const uint8_t *data, size_t size) {
    sqlite3_int64 limit_val = 0;
    
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_val, data, sizeof(sqlite3_int64));
    } else if (size > 0) {
        memcpy(&limit_val, data, size);
    }
    
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(limit_val);
    
    if (limit_val == -1) {
        sqlite3_soft_heap_limit64(prev_limit);
    }
}

static void fuzz_memory_functions(const uint8_t *data, size_t size) {
    int reset_flag = 0;
    if (size > 0) {
        reset_flag = data[0] & 1;
    }
    
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    sqlite3_int64 used = sqlite3_memory_used();
    
    if (reset_flag) {
        sqlite3_memory_highwater(0);
    }
}

static void fuzz_alloc_functions(const uint8_t *data, size_t size) {
    sqlite3_uint64 alloc_size = 0;
    
    if (size >= sizeof(sqlite3_uint64)) {
        memcpy(&alloc_size, data, sizeof(sqlite3_uint64));
        alloc_size %= 1024 * 1024;
    } else if (size > 0) {
        alloc_size = data[0] % 1024;
    }
    
    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr) {
        sqlite3_uint64 msize_val = sqlite3_msize(ptr);
        
        sqlite3_uint64 realloc_size = 0;
        if (size >= 2 * sizeof(sqlite3_uint64)) {
            memcpy(&realloc_size, data + sizeof(sqlite3_uint64), sizeof(sqlite3_uint64));
            realloc_size %= 1024 * 1024;
        } else if (size > sizeof(sqlite3_uint64)) {
            realloc_size = data[sizeof(sqlite3_uint64)] % 1024;
        }
        
        void *new_ptr = sqlite3_realloc64(ptr, realloc_size);
        if (new_ptr) {
            sqlite3_uint64 new_msize = sqlite3_msize(new_ptr);
            sqlite3_free(new_ptr);
        } else {
            sqlite3_free(ptr);
        }
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_soft_heap_limit(Data, Size);
    
    fuzz_memory_functions(Data, Size);
    
    fuzz_alloc_functions(Data, Size);
    
    return 0;
}