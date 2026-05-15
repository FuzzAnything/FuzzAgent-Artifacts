// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
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

static int dummy_callback(void* not_used, int argc, char** argv, char** col_names) {
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for database operations
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzer data to file (use first byte to determine how much to write)
    size_t write_size = (Data[0] % Size) + 1;
    if (write_size > Size) write_size = Size;
    fwrite(Data, 1, write_size, fp);
    fclose(fp);

    // Step 1: sqlite3_blob_close with NULL (safe no-op)
    sqlite3_blob_close(NULL);

    // Step 2: sqlite3_exec - needs a database connection
    sqlite3* db1 = NULL;
    char* errmsg = NULL;
    
    // Open a temporary in-memory database
    int rc = sqlite3_open(":memory:", &db1);
    if (rc == SQLITE_OK) {
        // Use fuzzer data for SQL command (ensure null termination)
        char* sql = malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            sqlite3_exec(db1, sql, dummy_callback, NULL, &errmsg);
            free(sql);
        }
        
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }

    // Step 3: sqlite3_close on db1
    if (db1) {
        sqlite3_close(db1);
    }

    // Step 4: sqlite3_close with NULL (safe no-op)
    sqlite3_close(NULL);

    // Step 5: sqlite3_libversion
    const char* version = sqlite3_libversion();
    (void)version; // Use version to avoid unused variable warning

    // Step 6: sqlite3_open using the dummy file
    sqlite3* db2 = NULL;
    rc = sqlite3_open(filename, &db2);
    
    if (rc == SQLITE_OK && db2) {
        // Step 7: sqlite3_prepare_v2
        sqlite3_stmt* stmt = NULL;
        const char* tail = NULL;
        
        // Create SQL from fuzzer data
        char* sql2 = malloc(Size + 1);
        if (sql2) {
            memcpy(sql2, Data, Size);
            sql2[Size] = '\0';
            
            // Try to prepare a statement
            rc = sqlite3_prepare_v2(db2, sql2, -1, &stmt, &tail);
            
            if (rc == SQLITE_OK && stmt) {
                // Step 8: sqlite3_step
                rc = sqlite3_step(stmt);
                
                // Step 9: sqlite3_column_text (only if we got a row)
                if (rc == SQLITE_ROW) {
                    const unsigned char* text = sqlite3_column_text(stmt, 0);
                    (void)text; // Use text to avoid unused variable warning
                }
                
                // Step 10: sqlite3_finalize
                sqlite3_finalize(stmt);
            }
            
            free(sql2);
        }
        
        // Step 11: sqlite3_close
        sqlite3_close(db2);
    }

    // Clean up dummy file
    remove(filename);
    
    return 0;
}