// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT * FROM table WHERE id = ?1 AND val = ?2 AND real_val = ?3 AND text_val = ?4 AND id2 = ?5;";
    
    // Initialize database in memory
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table to ensure the statement can be prepared
    const char *create_sql = "CREATE TABLE IF NOT EXISTS table (id INTEGER, val INTEGER, real_val REAL, text_val TEXT, id2 INTEGER);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, create_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Parse input data to generate test values
    int idx = 0;
    int int_val1 = 0;
    sqlite3_int64 int64_val = 0;
    double double_val = 0.0;
    const char *text_val = "";
    sqlite3_uint64 text_len = 0;
    int int_val2 = 0;
    
    if (Size > 0) {
        int_val1 = Data[idx++];
        if (Size > 1) {
            int64_val = (sqlite3_int64)Data[idx++];
            if (Size > 2) {
                // Create a double from bytes
                double d_temp = 0.0;
                if (Size > idx + 7) {
                    memcpy(&d_temp, &Data[idx], 8);
                    double_val = d_temp;
                    idx += 8;
                } else {
                    double_val = (double)Data[idx];
                    idx++;
                }
                
                // Extract text
                if (Size > idx) {
                    text_len = (sqlite3_uint64)Data[idx++];
                    // Ensure text_len doesn't exceed remaining data
                    if (text_len > Size - idx) {
                        text_len = Size - idx;
                    }
                    if (text_len > 0) {
                        text_val = (const char *)&Data[idx];
                        idx += text_len;
                    } else {
                        text_val = "";
                        text_len = 0;
                    }
                } else {
                    text_val = "";
                    text_len = 0;
                }
                
                if (Size > idx) {
                    int_val2 = Data[idx++];
                }
            }
        }
    }
    
    // Invoke the target functions in the specified order
    sqlite3_bind_int(stmt, 1, int_val1);
    sqlite3_bind_int64(stmt, 2, int64_val);
    sqlite3_bind_double(stmt, 3, double_val);
    sqlite3_bind_text64(stmt, 4, text_val, text_len, SQLITE_STATIC, SQLITE_UTF8);
    sqlite3_bind_int(stmt, 5, int_val2);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}