// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:171994:25 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:90957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:90950:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
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

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_int(context, 42);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    static const char* dummy_file = "./dummy_file";
    
    // Create a dummy file with fuzzer data
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a table with BLOB column
    char *err_msg = NULL;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT OR IGNORE INTO test (data) VALUES (zeroblob(1024));";
    
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Get the last inserted rowid
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    
    // Open BLOB handle for writing
    sqlite3_blob *blob = NULL;
    if (sqlite3_blob_open(db, "main", "test", "data", rowid, 1, &blob) == SQLITE_OK) {
        // 1. sqlite3_blob_write
        int write_offset = 0;
        int write_size = (int)(Size % 1024); // Ensure we don't exceed blob size
        if (write_size > 0) {
            sqlite3_blob_write(blob, Data, write_size, write_offset);
        }
        
        // 2. sqlite3_errmsg (first call)
        const char *err1 = sqlite3_errmsg(db);
        (void)err1; // Use variable to avoid unused warning
        
        // 3. sqlite3_blob_read
        void *read_buffer = malloc(1024);
        if (read_buffer) {
            int read_offset = 0;
            int read_size = 512; // Read half the blob
            sqlite3_blob_read(blob, read_buffer, read_size, read_offset);
            free(read_buffer);
        }
        
        // 4. sqlite3_errmsg (second call)
        const char *err2 = sqlite3_errmsg(db);
        (void)err2; // Use variable to avoid unused warning
        
        sqlite3_blob_close(blob);
    }
    
    // 5. sqlite3_create_function
    // Use fuzzer data to determine function name and parameters
    const char *func_name = "fuzz_func";
    int nArg = (int)(Size % 8); // 0-7 arguments
    int eTextRep = SQLITE_UTF8;
    if (Size > 0) {
        eTextRep |= (Data[0] & 1) ? SQLITE_DETERMINISTIC : 0;
    }
    
    sqlite3_create_function(
        db,
        func_name,
        nArg,
        eTextRep,
        NULL,
        dummy_func,
        NULL,
        NULL
    );
    
    // 6. sqlite3_prepare_v2
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Create SQL statement using fuzzer data
    char *sql = NULL;
    if (Size > 0) {
        sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            sqlite3_prepare_v2(
                db,
                sql,
                (int)Size,
                &stmt,
                &tail
            );
            
            free(sql);
        }
    } else {
        // Default SQL if no data
        sqlite3_prepare_v2(
            db,
            "SELECT 1",
            -1,
            &stmt,
            &tail
        );
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    // Remove dummy file
    remove(dummy_file);
    
    return 0;
}