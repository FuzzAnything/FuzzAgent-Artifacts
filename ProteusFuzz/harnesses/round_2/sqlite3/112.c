// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to generate values
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT, blob_data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement to insert data derived from fuzz input
    const char *insert_sql = "INSERT INTO test (data, blob_data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind fuzz data as text and blob
    if (Size > 0) {
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, (const void *)Data, Size, SQLITE_STATIC);
    } else {
        sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, "", 0, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_finalize(stmt);

    // Prepare a statement to retrieve the values
    const char *select_sql = "SELECT data, blob_data FROM test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get unprotected sqlite3_value objects from columns
        // Note: sqlite3_column_value returns an UNPROTECTED value.
        // Accessor functions like sqlite3_value_text16be require a PROTECTED value.
        // We must use sqlite3_value_dup to create a protected copy before accessing.
        
        sqlite3_value *val_text_raw = sqlite3_column_value(stmt, 0);
        sqlite3_value *val_blob_raw = sqlite3_column_value(stmt, 1);

        // Create protected copies for val_text
        sqlite3_value *val_text = sqlite3_value_dup(val_text_raw);
        if (val_text) {
            // 1. Test sqlite3_value_text16be
            const void *text16be = sqlite3_value_text16be(val_text);
            (void)text16be;

            // 2. Test sqlite3_value_text16
            const void *text16 = sqlite3_value_text16(val_text);
            (void)text16;

            // 3. Test sqlite3_value_text16le
            const void *text16le = sqlite3_value_text16le(val_text);
            (void)text16le;

            // 4. Test sqlite3_value_blob on the duplicated text (might convert)
            const void *blob_from_text = sqlite3_value_blob(val_text);
            (void)blob_from_text;

            // Free the protected copy
            sqlite3_value_free(val_text);
        }

        // Create protected copies for val_blob
        sqlite3_value *val_blob = sqlite3_value_dup(val_blob_raw);
        if (val_blob) {
            // 1. Test sqlite3_value_blob
            const void *blob_data = sqlite3_value_blob(val_blob);
            (void)blob_data;

            // 2. Test text16 variants on the duplicate blob (might convert)
            (void)sqlite3_value_text16be(val_blob);
            (void)sqlite3_value_text16(val_blob);
            (void)sqlite3_value_text16le(val_blob);

            // Free the protected copy
            sqlite3_value_free(val_blob);
        }

        // Test with NULL input for sqlite3_value_dup (should return NULL)
        sqlite3_value *dup_null = sqlite3_value_dup(NULL);
        (void)dup_null;

        // Test sqlite3_value_free with NULL (should be safe no-op)
        sqlite3_value_free(NULL);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}