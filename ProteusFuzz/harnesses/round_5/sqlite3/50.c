// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 aggregate and result functions.
 * This driver creates a custom aggregate function to test:
 * - sqlite3_aggregate_count
 * - sqlite3_result_text16be
 * - sqlite3_result_error
 * - sqlite3_result_blob
 * - sqlite3_result_int
 * - sqlite3_get_auxdata
 */

/* Global database handle for the fuzzing session */
static sqlite3 *g_db = NULL;

/* Custom aggregate state structure */
typedef struct {
    int count;
    int error_triggered;
    int auxdata_triggered;
} AggState;

/* Destructor for auxiliary data if needed */
static void auxdata_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Step function for the custom aggregate.
 * Tests: sqlite3_get_auxdata, sqlite3_result_int (indirectly via step logic)
 */
static void fuzz_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    AggState *state = (AggState *)sqlite3_aggregate_context(ctx, sizeof(AggState));
    if (!state) {
        sqlite3_result_error_nomem(ctx);
        return;
    }

    if (!state->count) {
        state->count = 0;
        state->error_triggered = 0;
        state->auxdata_triggered = 0;
    }

    state->count++;

    /* Test sqlite3_get_auxdata on the first argument if available */
    if (argc > 0) {
        void *aux = sqlite3_get_auxdata(ctx, 0);
        if (!aux && argc > 0) {
            /* Create dummy aux data to store */
            void *new_aux = malloc(1);
            if (new_aux) {
                *(char*)new_aux = 1;
                sqlite3_set_auxdata(ctx, 0, new_aux, auxdata_destructor);
                state->auxdata_triggered = 1;
            }
        }
    }
}

/*
 * Finalize function for the custom aggregate.
 * Tests: sqlite3_aggregate_count, sqlite3_result_text16be,
 *        sqlite3_result_error, sqlite3_result_blob, sqlite3_result_int
 */
static void fuzz_agg_finalize(sqlite3_context *ctx) {
    AggState *state = (AggState *)sqlite3_aggregate_context(ctx, sizeof(AggState));
    
    /* Test sqlite3_aggregate_count */
    int cnt = sqlite3_aggregate_count(ctx);
    
    if (cnt < 0) {
        /* Should not happen, but handle gracefully */
        sqlite3_result_error(ctx, "Invalid aggregate count", -1);
        return;
    }

    /* Use cnt to decide which result type to return, exploring different paths */
    if (cnt == 0) {
        /* Path 1: Return NULL or Int 0 */
        sqlite3_result_int(ctx, 0);
    } else if (cnt == 1) {
        /* Path 2: Return Text16BE */
        /* Ensure even length for UTF-16BE */
        const char utf16be[] = {0x00, 0x41, 0x00, 0x42}; /* "AB" in UTF-16BE */
        sqlite3_result_text16be(ctx, utf16be, 4, SQLITE_STATIC);
    } else if (cnt == 2) {
        /* Path 3: Return Error */
        sqlite3_result_error(ctx, "Fuzz error triggered", -1);
    } else if (cnt == 3) {
        /* Path 4: Return BLOB */
        const unsigned char blob_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
        sqlite3_result_blob(ctx, blob_data, 4, SQLITE_STATIC);
    } else {
        /* Path 5: Return Int based on count */
        sqlite3_result_int(ctx, cnt);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    /* Initialize database if not already done */
    if (!g_db) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }
        
        /* Register the custom aggregate function */
        sqlite3_create_function(
            g_db,
            "fuzz_agg",
            -1, /* Variable arguments */
            SQLITE_UTF8 | SQLITE_INNOCUOUS,
            NULL, /* pUserData */
            fuzz_agg_step,
            NULL, /* xInverse not needed */
            fuzz_agg_finalize
        );
    }

    /* Prepare a dummy file if needed (not strictly needed for :memory: but per instructions) */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    /* Construct a SQL query based on input data to trigger diverse states */
    /* We use the input data to generate random-looking SQL that calls our aggregate */
    
    char *sql = NULL;
    size_t sql_len = Size + 100;
    sql = (char *)malloc(sql_len);
    if (!sql) {
        return 0;
    }

    /* Create a safe SQL string that calls the aggregate */
    /* We limit the SQL generation to avoid buffer overflows and ensure valid syntax */
    int sql_idx = 0;
    
    /* Base query: SELECT fuzz_agg(...) FROM ... */
    const char *base = "SELECT fuzz_agg(";
    int base_len = strlen(base);
    
    if (sql_len < base_len + 1) {
        free(sql);
        return 0;
    }

    memcpy(sql, base, base_len);
    sql_idx += base_len;

    /* Add arguments based on input data */
    /* We will add up to 5 arguments to test argc variations */
    int arg_count = 0;
    if (Size > 0) {
        /* Use first byte to determine number of arguments (1 to 5) */
        arg_count = (Data[0] % 5) + 1;
        if (arg_count > (int)Size) arg_count = (int)Size;
    } else {
        arg_count = 1;
    }

    for (int i = 0; i < arg_count; i++) {
        if (sql_idx + 20 >= sql_len) break;
        
        if (i > 0) {
            sql[sql_idx++] = ',';
            sql[sql_idx++] = ' ';
        }
        
        /* Add a string literal or integer based on input */
        if (i < Size) {
            /* Try to make a safe string literal */
            char c = Data[i];
            if (c == '\'' || c == 0) c = 'x'; /* Escape single quote or null */
            
            sql[sql_idx++] = '\'';
            sql[sql_idx++] = c;
            sql[sql_idx++] = '\'';
        } else {
            /* Fallback to integer */
            snprintf(sql + sql_idx, sql_len - sql_idx, "%d", i);
            sql_idx += strlen(sql + sql_idx);
        }
    }

    if (sql_idx + 50 >= sql_len) {
        free(sql);
        return 0;
    }

    /* Close the function call and add FROM clause */
    const char *suffix = ") FROM (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3)";
    int suffix_len = strlen(suffix);
    memcpy(sql + sql_idx, suffix, suffix_len);
    sql_idx += suffix_len;
    sql[sql_idx] = '\0';

    /* Execute the query */
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        /* Invalid SQL is fine for fuzzing, just clean up */
        free(sql);
        return 0;
    }

    /* Step through the statement to trigger the aggregate */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Process row if needed, but mainly we want the aggregate to run */
        (void)rc;
    }

    /* Finalize statement */
    sqlite3_finalize(stmt);
    free(sql);

    return 0;
}