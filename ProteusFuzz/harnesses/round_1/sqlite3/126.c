// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

static int fuzzer_prepare_statement(const uint8_t *data, size_t size, sqlite3_stmt **out_stmt) {
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (size == 0) {
        return -1;
    }

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    // Ensure null-termination for the SQL string
    const char *sql = (const char *)data;
    size_t sql_len = size;
    
    // Create a buffer with null termination
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return -1;
    }
    memcpy(sql_buf, sql, sql_len);
    sql_buf[sql_len] = '\0';

    // Try to prepare the statement
    rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
    free(sql_buf);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    // Store db pointer in a static variable for cleanup later
    static sqlite3 *g_db = NULL;
    if (g_db) {
        sqlite3_close(g_db);
    }
    g_db = db;

    *out_stmt = stmt;
    return 0;
}

static void cleanup_db() {
    static sqlite3 *g_db = NULL;
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    sqlite3_stmt *stmt = NULL;
    
    // Cleanup any previous DB
    cleanup_db();

    if (fuzzer_prepare_statement(Data, Size, &stmt) != 0) {
        // Invalid SQL, try to generate a valid one if possible or just return
        // To explore more states, let's try a default valid SQL if input is invalid
        sqlite3 *db;
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
            static sqlite3 *g_db = NULL;
            if (g_db) sqlite3_close(g_db);
            g_db = db;
        }
    }

    if (!stmt) {
        return 0;
    }

    // 1. Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 2. Test sqlite3_bind_parameter_name
    // Iterate through possible indices (1 to param_count + 5 to test out of bounds)
    for (int i = 1; i <= param_count + 5; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Use the result
    }

    // 3. Test sqlite3_bind_parameter_index
    // Try some common names and random names from input
    const char *test_names[] = {":name", ":id", "@val", "$num", "?1", NULL};
    for (int i = 0; test_names[i]; i++) {
        int idx = sqlite3_bind_parameter_index(stmt, test_names[i]);
        (void)idx;
    }
    
    // Try names from the input data if they look like strings
    if (Size > 4) {
        char name_buf[64];
        size_t copy_len = Size < 63 ? Size : 63;
        memcpy(name_buf, Data, copy_len);
        name_buf[copy_len] = '\0';
        sqlite3_bind_parameter_index(stmt, name_buf);
    }

    // 4. Test sqlite3_bind_int
    // Bind to indices 1 to param_count + 5
    // Ensure we have enough data before accessing
    if (Size >= 2) {
        for (int i = 1; i <= param_count + 5; i++) {
            int val = (int)(Data[0] * 256 + Data[1]);
            sqlite3_bind_int(stmt, i, val);
        }
    }

    // 5. Test sqlite3_bind_int64
    // Ensure we have enough data before accessing
    if (Size >= 4) {
        for (int i = 1; i <= param_count + 5; i++) {
            sqlite3_int64 val64 = (sqlite3_int64)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
            sqlite3_bind_int64(stmt, i, val64);
        }
    }

    // 6. Test sqlite3_bind_text64
    // Create a text buffer from input
    if (Size > 0) {
        for (int i = 1; i <= param_count + 5; i++) {
            // Use a portion of the input as text
            size_t text_len = Size;
            const char *text = (const char *)Data;
            
            // Try different encodings
            unsigned char encodings[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
            for (int e = 0; e < 4; e++) {
                sqlite3_bind_text64(stmt, i, text, text_len, SQLITE_STATIC, encodings[e]);
            }
        }
    }

    // Cleanup
    cleanup_stmt(stmt);
    cleanup_db();

    return 0;
}