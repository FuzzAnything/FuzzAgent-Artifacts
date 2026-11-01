// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 value accessor functions.
 * Target APIs:
 * 1. sqlite3_value_text16be
 * 2. sqlite3_value_text16
 * 3. sqlite3_value_text
 * 4. sqlite3_value_text16le
 * 5. sqlite3_value_subtype
 * 6. sqlite3_value_blob
 *
 * Strategy:
 * - Create an in-memory database.
 * - Register a custom SQL function that receives arguments.
 * - Inside the function, cast arguments to protected sqlite3_value*.
 * - Call the target APIs on these values.
 * - Use fuzz input to construct SQL statements with various parameter types.
 */

static sqlite3 *g_db = NULL;

/* Custom function implementation to exercise value accessors */
static void fuzz_value_accessor_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    for (i = 0; i < argc; i++) {
        sqlite3_value *val = argv[i];
        if (!val) continue;

        /* 1. sqlite3_value_text16be */
        const void *text16be = sqlite3_value_text16be(val);
        (void)text16be; /* Suppress unused warning */

        /* 2. sqlite3_value_text16 */
        const void *text16 = sqlite3_value_text16(val);
        (void)text16;

        /* 3. sqlite3_value_text */
        const unsigned char *text = sqlite3_value_text(val);
        (void)text;

        /* 4. sqlite3_value_text16le */
        const void *text16le = sqlite3_value_text16le(val);
        (void)text16le;

        /* 5. sqlite3_value_subtype */
        unsigned int subtype = sqlite3_value_subtype(val);
        (void)subtype;

        /* 6. sqlite3_value_blob */
        const void *blob = sqlite3_value_blob(val);
        (void)blob;
    }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize database if not already done */
    if (g_db == NULL) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }
        
        /* Register the custom function with SQLITE_SUBTYPE flag to enable subtype testing */
        if (sqlite3_create_function(g_db, "fuzz_accessor", -1, SQLITE_SUBTYPE, NULL, fuzz_value_accessor_func, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    }

    /* Prepare a statement that uses our custom function with fuzzed data */
    /* We construct a SQL string from the input data, ensuring it's null-terminated */
    char *sql = (char *)malloc(Size + 100);
    if (!sql) return 0;

    /* Create a safe SQL query that calls our function with various inputs derived from Data */
    /* We use a simple SELECT with the function and some literals/parameters */
    snprintf(sql, Size + 100, 
        "SELECT fuzz_accessor(%s);", 
        Size > 0 ? "1" : "0"
    );
    
    /* Add more complexity by trying to bind values if possible */
    if (Size > 10) {
        /* Try to create a statement with parameters */
        sqlite3_stmt *stmt;
        const char *param_sql = "SELECT fuzz_accessor(?1, ?2, ?3);";
        
        if (sqlite3_prepare_v2(g_db, param_sql, -1, &stmt, NULL) == SQLITE_OK) {
            /* Bind integer from data */
            int val1 = Data[0] | (Data[1] << 8);
            sqlite3_bind_int(stmt, 1, val1);
            
            /* Bind text from data */
            sqlite3_bind_text(stmt, 2, (const char *)Data + 2, Size - 2, SQLITE_STATIC);
            
            /* Bind blob from data */
            sqlite3_bind_blob(stmt, 3, Data, Size, SQLITE_STATIC);
            
            /* Execute the statement */
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    } else {
        /* Simple execution for small inputs */
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    free(sql);
    return 0;
}