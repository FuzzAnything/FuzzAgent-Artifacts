// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for any potential file operations
    dummy_file_write(Data, Size);
    
    // 1. sqlite3_complete - requires null-terminated string
    char *sql_str = NULL;
    if (Size > 0) {
        sql_str = (char *)malloc(Size + 1);
        if (sql_str) {
            memcpy(sql_str, Data, Size);
            sql_str[Size] = '\0';
            int complete_result = sqlite3_complete(sql_str);
            (void)complete_result; // Use result to avoid unused variable warning
        }
    } else {
        // Test with empty string
        int complete_result = sqlite3_complete("");
        (void)complete_result;
    }
    
    // 2. sqlite3_free - free the allocated string if it exists
    if (sql_str) {
        sqlite3_free(sql_str);
    }
    
    // 3. sqlite3_close - test with NULL (safe) and with a real connection
    sqlite3 *db = NULL;
    // Try to open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        // Close the database connection
        sqlite3_close(db);
        db = NULL;
    }
    // Also test with NULL
    sqlite3_close(NULL);
    
    // 4. sqlite3_memory_highwater - test with reset flag 0 and 1
    sqlite3_int64 highwater1 = sqlite3_memory_highwater(0);
    (void)highwater1;
    sqlite3_int64 highwater2 = sqlite3_memory_highwater(1);
    (void)highwater2;
    
    // 5. sqlite3_memory_used - get current memory usage
    sqlite3_int64 mem_used1 = sqlite3_memory_used();
    (void)mem_used1;
    
    // 6. sqlite3_status - test with different op codes
    int current = 0, highwater = 0;
    // Use fuzzer data to select status op code (modulo number of status types)
    int status_op = Size > 0 ? Data[0] % 10 : 0; // SQLITE_STATUS_MEMORY_USED = 0, etc.
    int reset_flag = Size > 1 ? Data[1] % 2 : 0;
    sqlite3_status(status_op, &current, &highwater, reset_flag);
    
    // 7. sqlite3_memory_used - again after status call
    sqlite3_int64 mem_used2 = sqlite3_memory_used();
    (void)mem_used2;
    
    // 8. sqlite3_hard_heap_limit64 - set based on fuzzer data
    sqlite3_int64 hard_limit = 0;
    if (Size >= 8) {
        // Use first 8 bytes as int64
        memcpy(&hard_limit, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        // Use available bytes
        hard_limit = Data[0];
        if (Size > 1) hard_limit |= ((sqlite3_int64)Data[1] << 8);
        if (Size > 2) hard_limit |= ((sqlite3_int64)Data[2] << 16);
        if (Size > 3) hard_limit |= ((sqlite3_int64)Data[3] << 24);
    }
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(hard_limit);
    (void)prev_hard_limit;
    
    // 9. sqlite3_soft_heap_limit64 - set based on fuzzer data
    sqlite3_int64 soft_limit = 0;
    if (Size >= 16) {
        // Use next 8 bytes as int64
        memcpy(&soft_limit, Data + 8, sizeof(sqlite3_int64));
    } else if (Size > 4) {
        // Use remaining bytes
        size_t offset = Size > 8 ? 8 : 4;
        soft_limit = Data[offset % Size];
        if (Size > offset + 1) soft_limit |= ((sqlite3_int64)Data[(offset + 1) % Size] << 8);
        if (Size > offset + 2) soft_limit |= ((sqlite3_int64)Data[(offset + 2) % Size] << 16);
        if (Size > offset + 3) soft_limit |= ((sqlite3_int64)Data[(offset + 3) % Size] << 24);
    }
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(soft_limit);
    (void)prev_soft_limit;
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}