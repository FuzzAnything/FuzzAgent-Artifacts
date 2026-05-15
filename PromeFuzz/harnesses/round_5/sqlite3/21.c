// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create dynamic string object
    sqlite3_str *str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare input data for string operations
    // Ensure we have at least some data for string operations
    size_t usable_size = Size > 0 ? Size : 1;
    const uint8_t *str_data = Data;
    size_t str_size = usable_size;
    
    // Create a null-terminated string for format string
    char *format_str = malloc(str_size + 1);
    if (!format_str) {
        sqlite3_str_finish(str);
        sqlite3_close(db);
        return 0;
    }
    memcpy(format_str, str_data, str_size);
    format_str[str_size] = '\0';
    
    // 1. sqlite3_str_appendf
    sqlite3_str_appendf(str, "%s", format_str);
    
    // 2. First sqlite3_str_append
    sqlite3_str_append(str, (const char*)str_data, (int)(str_size % 1024));
    
    // 3. Second sqlite3_str_append
    sqlite3_str_append(str, (const char*)str_data, (int)(str_size % 512));
    
    // Prepare SQL statement using the string value
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // 4. sqlite3_prepare_v2
    // Use the string value from the dynamic string as SQL
    char *sql_str = sqlite3_str_value(str);
    if (sql_str) {
        sqlite3_prepare_v2(db, sql_str, -1, &stmt, &tail);
    }
    
    // 5. sqlite3_str_value (again, as per order)
    char *value = sqlite3_str_value(str);
    (void)value; // Use value to avoid unused variable warning
    
    // 6. sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(format_str);
    sqlite3_str_finish(str);
    sqlite3_close(db);
    
    // Also test with file-based database
    if (Size > 0) {
        dummy_file_write(Data, Size);
        sqlite3 *file_db = NULL;
        if (sqlite3_open("./dummy_file", &file_db) == SQLITE_OK) {
            sqlite3_str *str2 = sqlite3_str_new(file_db);
            if (str2) {
                sqlite3_str_appendf(str2, "test %d", 123);
                sqlite3_str_append(str2, "append1", 7);
                sqlite3_str_append(str2, "append2", 7);
                
                sqlite3_stmt *stmt2 = NULL;
                sqlite3_prepare_v2(file_db, "SELECT 1", -1, &stmt2, &tail);
                char *val2 = sqlite3_str_value(str2);
                (void)val2;
                if (stmt2) {
                    sqlite3_finalize(stmt2);
                }
                sqlite3_str_finish(str2);
            }
            sqlite3_close(file_db);
        }
    }
    
    return 0;
}