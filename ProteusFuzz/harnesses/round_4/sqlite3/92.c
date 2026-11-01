// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_* functions.
 *
 * This driver attempts to exercise:
 * - sqlite3_value_text16be
 * - sqlite3_value_dup
 * - sqlite3_value_free
 * - sqlite3_value_text16
 * - sqlite3_value_pointer
 * - sqlite3_value_text16le
 *
 * It creates a temporary in-memory database, prepares a simple statement,
 * binds various types of values (text, blob, integer, real, null, pointer),
 * steps through the statement, and then calls the target APIs on the
 * resulting sqlite3_value objects.
 */

static void cleanup_statement(sqlite3_stmt **stmt) {
    if (stmt && *stmt) {
        sqlite3_finalize(*stmt);
        *stmt = NULL;
    }
}

static void cleanup_db(sqlite3 **db) {
    if (db && *db) {
        sqlite3_close(*db);
        *db = NULL;
    }
}

static void cleanup_value(sqlite3_value **val) {
    if (val && *val) {
        sqlite3_value_free(*val);
        *val = NULL;
    }
}

/*
 * Helper to create a simple statement that returns a single value.
 * The value type depends on the bound parameter.
 */
static int prepare_test_statement(sqlite3 *db, sqlite3_stmt **stmt_out, const char *sql) {
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt_out, NULL);
    if (rc != SQLITE_OK) {
        return -1;
    }
    return 0;
}

/*
 * Helper to bind a value to the first parameter of a prepared statement.
 * The type of value is determined by the 'type' byte from the input data.
 */
static int bind_value_from_input(sqlite3_stmt *stmt, const uint8_t *data, size_t size, int type_byte) {
    if (!stmt || !data) return -1;

    switch (type_byte % 7) {
        case 0: { /* NULL */
            return sqlite3_bind_null(stmt, 1);
        }
        case 1: { /* Integer */
            int64_t val = 0;
            if (size >= sizeof(val)) {
                memcpy(&val, data, sizeof(val));
            }
            return sqlite3_bind_int64(stmt, 1, val);
        }
        case 2: { /* Real */
            double val = 0.0;
            if (size >= sizeof(val)) {
                memcpy(&val, data, sizeof(val));
            }
            return sqlite3_bind_double(stmt, 1, val);
        }
        case 3: { /* Text (UTF-8) */
            /* Ensure null termination */
            int len = (size > 1000) ? 1000 : size;
            char *str = (char *)malloc(len + 1);
            if (!str) return SQLITE_NOMEM;
            memcpy(str, data, len);
            str[len] = '\0';
            int rc = sqlite3_bind_text(stmt, 1, str, len, SQLITE_TRANSIENT);
            free(str);
            return rc;
        }
        case 4: { /* Blob */
            int len = (size > 1000) ? 1000 : size;
            return sqlite3_bind_blob(stmt, 1, data, len, SQLITE_TRANSIENT);
        }
        case 5: { /* Pointer (using sqlite3_bind_pointer) */
            /* We need a dummy pointer and a type string */
            static char dummy_type[] = "fuzz_type";
            void *ptr = (void *)0xdeadbeef;
            return sqlite3_bind_pointer(stmt, 1, ptr, dummy_type, NULL);
        }
        case 6: { /* Text (UTF-16BE) - manually constructed */
            /* Construct a simple UTF-16BE string: "A" (0x00 0x41) */
            uint8_t utf16be[] = {0x00, 0x41, 0x00, 0x00};
            return sqlite3_bind_text16(stmt, 1, (const char *)utf16be, 4, SQLITE_TRANSIENT);
        }
        default:
            return sqlite3_bind_null(stmt, 1);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to ensure we have a schema */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, val TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    /* Prepare a statement that selects the bound value */
    const char *select_sql = "SELECT ?1;";
    if (prepare_test_statement(db, &stmt, select_sql) != 0) {
        cleanup_db(&db);
        return 0;
    }

    /* Use the first byte to determine the type of value to bind */
    int type_byte = Data[0];

    /* Bind the value */
    rc = bind_value_from_input(stmt, Data, Size, type_byte);
    if (rc != SQLITE_OK) {
        cleanup_statement(&stmt);
        cleanup_db(&db);
        return 0;
    }

    /* Step to execute the statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        cleanup_statement(&stmt);
        cleanup_db(&db);
        return 0;
    }

    /* Get the value from the result column */
    sqlite3_value *val = sqlite3_column_value(stmt, 0);
    if (!val) {
        cleanup_statement(&stmt);
        cleanup_db(&db);
        return 0;
    }

    /*
     * Fix: sqlite3_column_value returns an UNPROTECTED value.
     * The text accessors (text16, text16le, text16be) and pointer accessors
     * require a PROTECTED value (mutex held).
     * We must duplicate the value to make it protected before calling these APIs.
     * We remove the direct calls on the unprotected 'val' to prevent assertion failures.
     */

    /* Create a protected copy using sqlite3_value_dup */
    sqlite3_value *val_dup = sqlite3_value_dup(val);
    if (val_dup) {
        /* Now call the text accessors on the protected copy */
        const void *text16 = sqlite3_value_text16(val_dup);
        (void)text16;

        const void *text16le = sqlite3_value_text16le(val_dup);
        (void)text16le;

        const void *text16be = sqlite3_value_text16be(val_dup);
        (void)text16be;

        void *ptr = sqlite3_value_pointer(val_dup, "fuzz_type");
        (void)ptr;

        /* Free the duplicated value */
        sqlite3_value_free(val_dup);
    }

    /* Cleanup */
    cleanup_statement(&stmt);
    cleanup_db(&db);

    return 0;
}