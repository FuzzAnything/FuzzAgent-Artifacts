// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare a simple statement to get sqlite3_value objects
    const char *sql = "SELECT ?, ?, ?, ?";
    sqlite3_stmt *stmt = NULL;
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind values from input data to create sqlite3_value objects
    // Bind 1: Integer
    if (Size >= 8) {
        int64_t int_val = 0;
        memcpy(&int_val, Data, 8);
        sqlite3_bind_int64(stmt, 1, int_val);
    } else {
        sqlite3_bind_int(stmt, 1, 0);
    }

    // Bind 2: Text (UTF-8)
    if (Size > 8) {
        size_t text_len = Size - 8;
        if (text_len > 1024) text_len = 1024;
        sqlite3_bind_text(stmt, 2, (const char*)(Data + 8), text_len, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 2, "", 0, SQLITE_TRANSIENT);
    }

    // Bind 3: Blob
    if (Size > 16) {
        size_t blob_len = Size - 16;
        if (blob_len > 1024) blob_len = 1024;
        sqlite3_bind_blob(stmt, 3, (const void*)(Data + 16), blob_len, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_blob(stmt, 3, "", 0, SQLITE_TRANSIENT);
    }

    // Bind 4: NULL
    sqlite3_bind_null(stmt, 4);

    // Step to execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Get values from columns
    // These are UNPROTECTED values. They cannot be used directly with 
    // text16be, text16le, text16, or pointer functions which require PROTECTED values.
    // We must duplicate them first to make them protected.
    sqlite3_value *val1_raw = sqlite3_column_value(stmt, 0);
    sqlite3_value *val2_raw = sqlite3_column_value(stmt, 1);
    sqlite3_value *val3_raw = sqlite3_column_value(stmt, 2);
    sqlite3_value *val4_raw = sqlite3_column_value(stmt, 3);

    // Create protected copies
    sqlite3_value *val1 = sqlite3_value_dup(val1_raw);
    sqlite3_value *val2 = sqlite3_value_dup(val2_raw);
    sqlite3_value *val3 = sqlite3_value_dup(val3_raw);
    sqlite3_value *val4 = sqlite3_value_dup(val4_raw);

    // Test sqlite3_value_dup on NULL
    sqlite3_value *dup_null = sqlite3_value_dup(NULL);

    // Test sqlite3_value_text16be (Requires protected value)
    // Only call on non-NULL pointers to avoid internal dereference issues
    const void *text16be1 = (val1) ? sqlite3_value_text16be(val1) : NULL;
    const void *text16be2 = (val2) ? sqlite3_value_text16be(val2) : NULL;
    const void *text16be3 = (val3) ? sqlite3_value_text16be(val3) : NULL;
    const void *text16be4 = (val4) ? sqlite3_value_text16be(val4) : NULL;
    // Do not call with NULL directly as it may cause internal dereference in some builds
    const void *text16be_null = NULL;
    
    // Test on duplicates too (val1 is already a duplicate)
    const void *text16be_dup1 = (val1) ? sqlite3_value_text16be(val1) : NULL;
    const void *text16be_dup2 = (val2) ? sqlite3_value_text16be(val2) : NULL;

    // Test sqlite3_value_text16le
    const void *text16le1 = (val1) ? sqlite3_value_text16le(val1) : NULL;
    const void *text16le2 = (val2) ? sqlite3_value_text16le(val2) : NULL;
    const void *text16le3 = (val3) ? sqlite3_value_text16le(val3) : NULL;
    const void *text16le4 = (val4) ? sqlite3_value_text16le(val4) : NULL;
    const void *text16le_null = NULL;

    // Test sqlite3_value_text16 (native)
    const void *text16_1 = (val1) ? sqlite3_value_text16(val1) : NULL;
    const void *text16_2 = (val2) ? sqlite3_value_text16(val2) : NULL;
    const void *text16_3 = (val3) ? sqlite3_value_text16(val3) : NULL;
    const void *text16_4 = (val4) ? sqlite3_value_text16(val4) : NULL;
    const void *text16_null = NULL;

    // Test sqlite3_value_pointer
    // Avoid passing NULL to prevent internal dereference
    void *ptr1 = (val1) ? sqlite3_value_pointer(val1, "test_type") : NULL;
    void *ptr2 = (val2) ? sqlite3_value_pointer(val2, "test_type") : NULL;
    void *ptr_null = NULL;

    // Suppress unused variable warnings
    (void)text16be1; (void)text16be2; (void)text16be3; (void)text16be4; (void)text16be_null;
    (void)text16be_dup1; (void)text16be_dup2;
    (void)text16le1; (void)text16le2; (void)text16le3; (void)text16le4; (void)text16le_null;
    (void)text16_1; (void)text16_2; (void)text16_3; (void)text16_4; (void)text16_null;
    (void)ptr1; (void)ptr2; (void)ptr_null;
    (void)dup_null;

    // Cleanup: Free duplicated values
    // We only free the duplicates we created (val1, val2, val3, val4)
    // The raw values from sqlite3_column_value are managed by the statement.
    if (val1) sqlite3_value_free(val1);
    if (val2) sqlite3_value_free(val2);
    if (val3) sqlite3_value_free(val3);
    if (val4) sqlite3_value_free(val4);
    
    // NULL is safe to free
    sqlite3_value_free(NULL);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}