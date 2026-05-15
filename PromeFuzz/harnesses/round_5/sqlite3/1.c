// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        remove_dummy_file();
        return 0;
    }
    
    // Prepare statement using fuzzer input as SQL
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    rc = sqlite3_prepare_v2(db, (const char *)Data, Size, &stmt, &tail);
    
    // Get error message (even if prepare succeeded)
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use variable to avoid unused warning
    
    // First step execution
    if (stmt) {
        int step_rc = sqlite3_step(stmt);
        
        // Second step execution
        if (step_rc == SQLITE_ROW) {
            step_rc = sqlite3_step(stmt);
        }
        
        // Get column count
        int col_count = sqlite3_column_count(stmt);
        
        // Process columns if available
        if (col_count > 0) {
            // Use first column (index 0) for testing
            int col_type = sqlite3_column_type(stmt, 0);
            const char *col_name = sqlite3_column_name(stmt, 0);
            (void)col_name; // Use variable
            
            // Call column_text three times as specified
            const unsigned char *text1 = sqlite3_column_text(stmt, 0);
            const unsigned char *text2 = sqlite3_column_text(stmt, 0);
            const unsigned char *text3 = sqlite3_column_text(stmt, 0);
            (void)text1; (void)text2; (void)text3; // Use variables
            
            // Get column bytes
            int bytes = sqlite3_column_bytes(stmt, 0);
            (void)bytes; // Use variable
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    remove_dummy_file();
    
    return 0;
}