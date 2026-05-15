// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string for sqlite3_complete
    char *sql_str = NULL;
    if (Size > 0) {
        sql_str = (char *)malloc(Size + 1);
        if (!sql_str) return 0;
        memcpy(sql_str, Data, Size);
        sql_str[Size] = '\0';
    } else {
        sql_str = (char *)malloc(1);
        if (!sql_str) return 0;
        sql_str[0] = '\0';
    }
    
    // 1. sqlite3_complete
    int complete_result = sqlite3_complete(sql_str);
    
    // 2. sqlite3_free - allocate some memory first
    void *mem_to_free = sqlite3_malloc(100);
    sqlite3_free(mem_to_free);
    
    // Also free the string we allocated
    sqlite3_free(sql_str);
    
    // 3. sqlite3_close - need a database connection first
    sqlite3 *db = NULL;
    write_dummy_file(Data, Size);
    int open_result = sqlite3_open("./dummy_file", &db);
    if (open_result == SQLITE_OK) {
        sqlite3_close(db);
    } else {
        // Close can handle NULL safely
        sqlite3_close(NULL);
    }
    
    // 4. sqlite3_memory_highwater
    int reset_flag = (Size > 0) ? (Data[0] & 1) : 0;
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    // 5. sqlite3_memory_used
    sqlite3_int64 memory_used1 = sqlite3_memory_used();
    
    // 6. sqlite3_status
    int current = 0;
    int highwater_status = 0;
    int status_op = (Size > 1) ? (Data[1] % 10) : 0; // Use modulo to get valid opcode range
    int status_reset = (Size > 2) ? (Data[2] & 1) : 0;
    int status_result = sqlite3_status(status_op, &current, &highwater_status, status_reset);
    
    // 7. sqlite3_memory_used (again)
    sqlite3_int64 memory_used2 = sqlite3_memory_used();
    
    // 8. sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit = 0;
    if (Size >= 8) {
        memcpy(&hard_limit, Data, sizeof(sqlite3_int64));
    }
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(hard_limit);
    
    // 9. sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit = 0;
    if (Size >= 16) {
        memcpy(&soft_limit, Data + 8, sizeof(sqlite3_int64));
    }
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(soft_limit);
    
    return 0;
}