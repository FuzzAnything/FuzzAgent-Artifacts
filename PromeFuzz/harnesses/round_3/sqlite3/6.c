// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
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

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql = "CREATE TABLE t1(a TEXT, b INTEGER, c BLOB);";
    const char *sql2 = "INSERT INTO t1 VALUES(?, ?, ?);";
    const char *sql3 = "SELECT * FROM t1;";
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create table
    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare insert statement
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind some values using fuzz data
    size_t offset = 0;
    
    // Bind text parameter
    if (offset + 1 < Size) {
        int text_len = Data[offset] % 256;
        offset++;
        if (offset + text_len <= Size && text_len > 0) {
            sqlite3_bind_text(stmt, 1, (const char*)(Data + offset), text_len, SQLITE_TRANSIENT);
            offset += text_len;
        } else {
            sqlite3_bind_text(stmt, 1, "test", 4, SQLITE_TRANSIENT);
        }
    }
    
    // Bind integer parameter
    if (offset < Size) {
        int int_val = Data[offset];
        offset++;
        sqlite3_bind_int(stmt, 2, int_val);
    }
    
    // Bind blob parameter
    if (offset < Size) {
        int blob_len = Data[offset] % 256;
        offset++;
        if (offset + blob_len <= Size && blob_len > 0) {
            sqlite3_bind_blob(stmt, 3, Data + offset, blob_len, SQLITE_TRANSIENT);
            offset += blob_len;
        } else {
            const uint8_t dummy_blob[] = {0x01, 0x02, 0x03};
            sqlite3_bind_blob(stmt, 3, dummy_blob, sizeof(dummy_blob), SQLITE_TRANSIENT);
        }
    }
    
    // Execute insert
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    // Insert another row with different values
    sqlite3_bind_text(stmt, 1, "another", 7, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, 42);
    const uint8_t blob2[] = {0xAA, 0xBB, 0xCC, 0xDD};
    sqlite3_bind_blob(stmt, 3, blob2, sizeof(blob2), SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Prepare select statement
    if (sqlite3_prepare_v2(db, sql3, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare another statement for bind_value testing
    const char *sql4 = "SELECT ?;";
    if (sqlite3_prepare_v2(db, sql4, -1, &stmt2, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Execute select to get rows
    int step_res = sqlite3_step(stmt);
    if (step_res == SQLITE_ROW) {
        // 1. sqlite3_column_text
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        (void)text; // Use to avoid unused variable warning
        
        // 2. sqlite3_bind_value (using value from column 0)
        sqlite3_value *val = sqlite3_column_value(stmt, 0);
        sqlite3_bind_value(stmt2, 1, val);
        
        // 3. sqlite3_column_value
        sqlite3_value *val2 = sqlite3_column_value(stmt, 1);
        (void)val2;
        
        // 4. sqlite3_bind_value (using value from column 1)
        sqlite3_bind_value(stmt2, 1, sqlite3_column_value(stmt, 1));
        
        // 5. sqlite3_column_value
        sqlite3_value *val3 = sqlite3_column_value(stmt, 2);
        (void)val3;
        
        // 6. sqlite3_step (on second statement)
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
        
        // 7. sqlite3_column_int
        int int_col = sqlite3_column_int(stmt, 1);
        (void)int_col;
        
        // 8. sqlite3_reset
        sqlite3_reset(stmt);
        
        // Step again to get row for remaining functions
        step_res = sqlite3_step(stmt);
        if (step_res == SQLITE_ROW) {
            // 9. sqlite3_column_bytes16
            int bytes16 = sqlite3_column_bytes16(stmt, 0);
            (void)bytes16;
            
            // 10. sqlite3_column_blob
            const void *blob = sqlite3_column_blob(stmt, 2);
            (void)blob;
            
            // 11. sqlite3_column_bytes16
            bytes16 = sqlite3_column_bytes16(stmt, 2);
            (void)bytes16;
            
            // 12. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 0);
            (void)blob;
            
            // 13. sqlite3_column_bytes
            int bytes = sqlite3_column_bytes(stmt, 2);
            (void)bytes;
            
            // 14. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 1);
            (void)blob;
            
            // 15. sqlite3_column_bytes
            bytes = sqlite3_column_bytes(stmt, 0);
            (void)bytes;
            
            // 16. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 2);
            (void)blob;
        }
        
        // Try to get more rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Exercise functions with different column indices
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count && i < 3; i++) {
                sqlite3_column_text(stmt, i);
                sqlite3_column_int(stmt, i);
                sqlite3_column_blob(stmt, i);
                sqlite3_column_bytes(stmt, i);
                sqlite3_column_bytes16(stmt, i);
                sqlite3_column_value(stmt, i);
            }
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}