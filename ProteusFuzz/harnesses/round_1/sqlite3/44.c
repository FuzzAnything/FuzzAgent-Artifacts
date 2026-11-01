// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_value_dup, sqlite3_value_blob, 
 * sqlite3_value_text16, sqlite3_value_text16le, 
 * sqlite3_value_text16be, and sqlite3_value_free.
 * 
 * Strategy:
 * 1. Open a database connection.
 * 2. Prepare a statement with bind parameters to generate sqlite3_value objects.
 * 3. Extract values using sqlite3_column_value.
 * 4. Use the target API functions on these values.
 * 5. Clean up resources.
 * 
 * Note: We avoid passing NULL to sqlite3_value_blob/text functions directly
 * as they may dereference the pointer before checking for NULL in some builds.
 * Instead, we test with valid values and handle NULL returns from dup.
 */

static int execute_fuzz_logic(sqlite3 *db, const uint8_t *Data, size_t Size) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5;";
    
    /* Prepare statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Bind parameters based on input data */
    size_t idx = 0;
    
    /* Bind 1: Integer */
    if (idx < Size) {
        int val = Data[idx];
        sqlite3_bind_int(stmt, 1, val);
        idx++;
    } else {
        sqlite3_bind_int(stmt, 1, 0);
    }

    /* Bind 2: Text */
    if (idx < Size) {
        size_t len = (Size - idx) < 100 ? (Size - idx) : 100;
        sqlite3_bind_text(stmt, 2, (const char*)&Data[idx], len, SQLITE_STATIC);
        idx += len;
    } else {
        sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
    }

    /* Bind 3: Blob */
    if (idx < Size) {
        size_t len = (Size - idx) < 100 ? (Size - idx) : 100;
        sqlite3_bind_blob(stmt, 3, &Data[idx], len, SQLITE_STATIC);
        idx += len;
    } else {
        sqlite3_bind_blob(stmt, 3, "", 0, SQLITE_STATIC);
    }

    /* Bind 4: Float */
    if (idx < Size) {
        double val = (double)Data[idx];
        sqlite3_bind_double(stmt, 4, val);
        idx++;
    } else {
        sqlite3_bind_double(stmt, 4, 0.0);
    }

    /* Bind 5: NULL */
    sqlite3_bind_null(stmt, 5);

    /* Step to execute */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }

    /* Extract values */
    sqlite3_value *v1 = sqlite3_column_value(stmt, 0);
    sqlite3_value *v2 = sqlite3_column_value(stmt, 1);
    sqlite3_value *v3 = sqlite3_column_value(stmt, 2);
    sqlite3_value *v4 = sqlite3_column_value(stmt, 3);
    sqlite3_value *v5 = sqlite3_column_value(stmt, 4);

    /* Test sqlite3_value_dup */
    sqlite3_value *dup1 = sqlite3_value_dup(v1);
    sqlite3_value *dup2 = sqlite3_value_dup(v2);
    sqlite3_value *dup3 = sqlite3_value_dup(v3);
    sqlite3_value *dup4 = sqlite3_value_dup(v4);
    sqlite3_value *dup5 = sqlite3_value_dup(v5);
    sqlite3_value *dup_null = sqlite3_value_dup(NULL);

    /* Test sqlite3_value_blob only on non-NULL duplicates */
    if (dup1) { const void *b1 = sqlite3_value_blob(dup1); (void)b1; }
    if (dup2) { const void *b2 = sqlite3_value_blob(dup2); (void)b2; }
    if (dup3) { const void *b3 = sqlite3_value_blob(dup3); (void)b3; }
    if (dup4) { const void *b4 = sqlite3_value_blob(dup4); (void)b4; }
    if (dup5) { const void *b5 = sqlite3_value_blob(dup5); (void)b5; }
    /* Do not call sqlite3_value_blob(NULL) directly to avoid ASAN issues */

    /* Test sqlite3_value_text16 only on non-NULL duplicates */
    if (dup1) { const void *t16_1 = sqlite3_value_text16(dup1); (void)t16_1; }
    if (dup2) { const void *t16_2 = sqlite3_value_text16(dup2); (void)t16_2; }
    if (dup3) { const void *t16_3 = sqlite3_value_text16(dup3); (void)t16_3; }
    if (dup4) { const void *t16_4 = sqlite3_value_text16(dup4); (void)t16_4; }
    if (dup5) { const void *t16_5 = sqlite3_value_text16(dup5); (void)t16_5; }

    /* Test sqlite3_value_text16le only on non-NULL duplicates */
    if (dup1) { const void *t16le_1 = sqlite3_value_text16le(dup1); (void)t16le_1; }
    if (dup2) { const void *t16le_2 = sqlite3_value_text16le(dup2); (void)t16le_2; }
    if (dup3) { const void *t16le_3 = sqlite3_value_text16le(dup3); (void)t16le_3; }
    if (dup4) { const void *t16le_4 = sqlite3_value_text16le(dup4); (void)t16le_4; }
    if (dup5) { const void *t16le_5 = sqlite3_value_text16le(dup5); (void)t16le_5; }

    /* Test sqlite3_value_text16be only on non-NULL duplicates */
    if (dup1) { const void *t16be_1 = sqlite3_value_text16be(dup1); (void)t16be_1; }
    if (dup2) { const void *t16be_2 = sqlite3_value_text16be(dup2); (void)t16be_2; }
    if (dup3) { const void *t16be_3 = sqlite3_value_text16be(dup3); (void)t16be_3; }
    if (dup4) { const void *t16be_4 = sqlite3_value_text16be(dup4); (void)t16be_4; }
    if (dup5) { const void *t16be_5 = sqlite3_value_text16be(dup5); (void)t16be_5; }

    /* Test sqlite3_value_free */
    sqlite3_value_free(dup1);
    sqlite3_value_free(dup2);
    sqlite3_value_free(dup3);
    sqlite3_value_free(dup4);
    sqlite3_value_free(dup5);
    sqlite3_value_free(dup_null);
    sqlite3_value_free(NULL);

    sqlite3_finalize(stmt);
    return 0;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute fuzz logic */
    execute_fuzz_logic(db, Data, Size);

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}