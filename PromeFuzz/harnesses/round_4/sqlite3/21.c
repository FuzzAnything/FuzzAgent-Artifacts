// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void dummy_vfs_register(void) {
    // No-op for compatibility
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Initialize SQLite library
    sqlite3_initialize();
    
    // Create in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Create dynamic string object
    sqlite3_str *str_obj = sqlite3_str_new(db);
    if (!str_obj) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Prepare data for string operations
    // Use Data for string content, ensure we don't exceed bounds
    size_t usable_size = Size > 0 ? Size : 1;
    const uint8_t *data_ptr = Data;
    if (Size == 0) {
        // Use dummy data if input is empty
        static const uint8_t dummy = 'A';
        data_ptr = &dummy;
        usable_size = 1;
    }
    
    // Convert to string for format operations
    char *temp_str = (char*)malloc(usable_size + 1);
    if (!temp_str) {
        sqlite3_str_finish(str_obj);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    memcpy(temp_str, data_ptr, usable_size);
    temp_str[usable_size] = '\0';
    
    // 1. Call sqlite3_str_appendf
    sqlite3_str_appendf(str_obj, "Test format: %s", temp_str);
    
    // 2. First call to sqlite3_str_append
    // Use first half of data (or all if small)
    size_t append1_len = usable_size / 2;
    if (append1_len > 0) {
        sqlite3_str_append(str_obj, temp_str, (int)append1_len);
    } else {
        sqlite3_str_append(str_obj, "default", 7);
    }
    
    // 3. Second call to sqlite3_str_append
    // Use remaining data
    size_t append2_len = usable_size - append1_len;
    if (append2_len > 0) {
        sqlite3_str_append(str_obj, temp_str + append1_len, (int)append2_len);
    } else {
        sqlite3_str_append(str_obj, "data", 4);
    }
    
    // Get the constructed string for SQL preparation
    char *constructed_sql = sqlite3_str_value(str_obj);
    
    // 4. Call sqlite3_prepare_v2 with the constructed string
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    if (constructed_sql) {
        sqlite3_prepare_v2(db, constructed_sql, -1, &stmt, &tail);
    } else {
        // Fallback SQL if string construction failed
        sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, &tail);
    }
    
    // 5. Call sqlite3_str_value again (maybe after preparation)
    char *str_value = sqlite3_str_value(str_obj);
    (void)str_value; // Mark as used
    
    // 6. Call sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(temp_str);
    sqlite3_str_finish(str_obj);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}