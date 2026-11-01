// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:186782:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to safely extract an integer from the fuzz input.
 * Returns a default value if the size is insufficient.
 */
static int read_int(const uint8_t *data, size_t size, size_t *offset, int default_val) {
    if (*offset + sizeof(int) > size) {
        return default_val;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/*
 * Helper function to safely extract a 64-bit integer from the fuzz input.
 */
static sqlite3_int64 read_int64(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(sqlite3_int64) > size) {
        return 0;
    }
    sqlite3_int64 val;
    memcpy(&val, data + *offset, sizeof(sqlite3_int64));
    *offset += sizeof(sqlite3_int64);
    return val;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    size_t offset = 0;

    /*
     * 1. Prepare the environment
     * We need a valid database connection to test the target APIs.
     * We use ":memory:" to avoid file system dependencies, but we also
     * write to "./dummy_file" if the input suggests it (step 5).
     */
    const char *db_path = ":memory:";
    
    /* Use first byte to decide database type */
    if (Size > 0 && Data[0] == 1) {
        db_path = "./dummy_file";
        /* Ensure dummy_file exists and is writable */
        FILE *f = fopen(db_path, "w");
        if (f) {
            fclose(f);
        }
    }

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If we can't open the DB, we can't test the APIs properly.
           We will not test APIs that require a valid DB pointer with NULL
           to avoid crashes in internal implementations that might not
           handle NULL gracefully in all builds (e.g., API armor checks
           might differ or internal dereferences might occur).
           Instead, we focus on valid DB paths or skip NULL tests if they crash.
        */
    }

    /*
     * Create a table to ensure we have something to modify for changes() tests.
     * We use a simple INSERT to trigger change counts.
     */
    if (db != NULL) {
        const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, val TEXT);";
        rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
        (void)rc; /* Ignore errors, just try */

        /* Prepare a statement to modify rows */
        const char *insert_sql = "INSERT INTO test_table (val) VALUES ('fuzz_data');";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            /* Execute the statement to generate changes */
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /*
     * 2. & 3. Conduct tests and explore program states
     * We will fuzz the inputs for the target functions using the Data buffer.
     * Note: We avoid passing NULL to functions that dereference the pointer
     * immediately without a check, as seen in the ASAN error.
     */

    /* --- Test sqlite3_db_status64 --- */
    if (Size >= 2) {
        int op_code = read_int(Data, Size, &offset, 0);
        int reset_flag = read_int(Data, Size, &offset, 0);
        
        sqlite3_int64 current_val = 0;
        sqlite3_int64 high_water = 0;
        
        /* Test with valid db */
        if (db != NULL) {
            sqlite3_db_status64(db, op_code, &current_val, &high_water, reset_flag);
        }
        
        /* 
         * Test with NULL db. 
         * Note: Some builds might crash here if they don't check for NULL first.
         * However, the API documentation says it returns SQLITE_MISUSE.
         * If the previous ASAN error was specific to changes64, we might skip NULL
         * for functions that are known to be unsafe with NULL in this specific build.
         * But generally, sqlite3_db_status64 should handle NULL. 
         * If it crashes, we would need to guard it. 
         * Let's try to call it, but if the build is strict, we might need to skip.
         * Given the previous error was on changes64, let's be cautious.
         * We will only call with NULL if we are sure, or skip to be safe.
         * The error log showed sqlite3_changes64(NULL) crashing.
         * Let's assume similar behavior for others if they dereference.
         * We will skip NULL tests for now to prevent crashes, focusing on valid DB.
         */
        // sqlite3_db_status64(NULL, op_code, &current_val, &high_water, reset_flag);
    }

    /* --- Test sqlite3_changes64 --- */
    /* This function takes only a db pointer. We test with valid db only. */
    if (db != NULL) {
        sqlite3_int64 changes64_val = sqlite3_changes64(db);
        (void)changes64_val;
    }
    /* Skip NULL test as it caused ASAN SEGV in the previous run */
    // sqlite3_int64 changes64_null = sqlite3_changes64(NULL);
    // (void)changes64_null;

    /* --- Test sqlite3_total_changes --- */
    if (db != NULL) {
        int total_changes_val = sqlite3_total_changes(db);
        (void)total_changes_val;
    }
    /* Skip NULL test to be safe */
    // int total_changes_null = sqlite3_total_changes(NULL);
    // (void)total_changes_null;

    /* --- Test sqlite3_errcode --- */
    /* First, try to induce an error to see if errcode captures it */
    if (db != NULL) {
        /* Execute a bad SQL to set an error code */
        sqlite3_exec(db, "SELECT * FROM nonexistent_table_xyz;", NULL, NULL, NULL);
        int err = sqlite3_errcode(db);
        (void)err;
    }
    /* Skip NULL test */
    // int err_null = sqlite3_errcode(NULL);
    // (void)err_null;

    /* --- Test sqlite3_changes --- */
    if (db != NULL) {
        int changes_val = sqlite3_changes(db);
        (void)changes_val;
    }
    /* Skip NULL test */
    // int changes_null = sqlite3_changes(NULL);
    // (void)changes_null;

    /* --- Test sqlite3_total_changes64 --- */
    if (db != NULL) {
        sqlite3_int64 total64_val = sqlite3_total_changes64(db);
        (void)total64_val;
    }
    /* Skip NULL test */
    // sqlite3_int64 total64_null = sqlite3_total_changes64(NULL);
    // (void)total64_null;

    /*
     * 2. Cleanup
     * Close the database connection and finalize any statements.
     */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    /* Clean up dummy file if created */
    if (Size > 0 && Data[0] == 1) {
        remove("./dummy_file");
    }

    return 0;
}