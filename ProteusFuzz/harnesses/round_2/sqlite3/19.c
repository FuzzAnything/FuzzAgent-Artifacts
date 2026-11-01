// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper to safely finalize statements */
static void safe_finalize(sqlite3_stmt *pStmt) {
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
}

/* Helper to safely free memory */
static void safe_free(void *p) {
    if (p) {
        sqlite3_free(p);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *tail = NULL;
    char *expanded = NULL;
    int rc;

    /* Ensure we have enough data to work with */
    if (Size < 1) {
        return 0;
    }

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a dummy file if needed (though we use :memory:) */
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Prepare a valid SQL statement first to ensure db is usable */
    const char *valid_sql = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, valid_sql, -1, &stmt1, &tail);
    
    /* Sequence: sqlite3_step -> sqlite3_finalize -> sqlite3_finalize -> sqlite3_prepare_v2 ... */
    
    /* 1. sqlite3_step */
    if (stmt1) {
        sqlite3_step(stmt1);
    }

    /* 2. sqlite3_finalize */
    safe_finalize(stmt1);
    stmt1 = NULL;

    /* 3. sqlite3_finalize (on NULL is safe) */
    safe_finalize(stmt1);

    /* 4. sqlite3_prepare_v2 with fuzzed input */
    /* Ensure null termination for safety, though nByte handles length */
    char *fuzzed_sql = (char *)malloc(Size + 1);
    if (!fuzzed_sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(fuzzed_sql, Data, Size);
    fuzzed_sql[Size] = '\0';

    rc = sqlite3_prepare_v2(db, fuzzed_sql, Size, &stmt2, &tail);

    /* 5. sqlite3_expanded_sql */
    if (stmt2) {
        expanded = sqlite3_expanded_sql(stmt2);
    }

    /* 6. sqlite3_free */
    safe_free(expanded);

    /* 7. sqlite3_bind_pointer */
    if (stmt2) {
        /* Bind a dummy pointer to parameter 1 if it exists */
        void *dummy_ptr = (void *)Data;
        sqlite3_bind_pointer(stmt2, 1, dummy_ptr, "fuzz_type", NULL);
    }

    /* 8. sqlite3_step */
    if (stmt2) {
        sqlite3_step(stmt2);
    }

    /* 9. sqlite3_finalize */
    safe_finalize(stmt2);
    stmt2 = NULL;

    /* 10. sqlite3_finalize (on NULL is safe) */
    safe_finalize(stmt2);

    /* Cleanup */
    free(fuzzed_sql);
    sqlite3_close(db);

    return 0;
}