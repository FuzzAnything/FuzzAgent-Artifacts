// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int parse_fuzz_data(const uint8_t *Data, size_t Size,
                           char **out_db, char **out_journal, char **out_wal,
                           int *out_n_param, char ***out_params) {
    if (Size < 1) return 0;

    size_t offset = 0;
    char *db = NULL, *journal = NULL, *wal = NULL;
    int n_param = 0;
    char **params = NULL;

    // Helper to extract null-terminated string
    #define EXTRACT_STRING(var) \
        do { \
            if (offset >= Size) goto cleanup; \
            size_t len = 0; \
            while (offset + len < Size && Data[offset + len]) len++; \
            if (len == 0) { var = strdup(""); } \
            else { \
                var = (char *)malloc(len + 1); \
                if (!var) goto cleanup; \
                memcpy(var, Data + offset, len); \
                var[len] = '\0'; \
            } \
            offset += len + 1; \
        } while(0)

    EXTRACT_STRING(db);
    EXTRACT_STRING(journal);
    EXTRACT_STRING(wal);

    // Read number of parameters
    if (offset >= Size) goto cleanup;
    n_param = (int)Data[offset++];
    if (n_param < 0) n_param = 0;

    // Limit parameters to prevent excessive allocation
    if (n_param > 100) n_param = 100;

    // Read parameters (pairs of key/value)
    if (n_param > 0) {
        params = (char **)malloc(sizeof(char *) * (size_t)n_param * 2);
        if (!params) goto cleanup;
        
        // Initialize array to NULL to ensure safe cleanup even if allocation fails mid-loop
        memset(params, 0, sizeof(char *) * (size_t)n_param * 2);

        int filled_count = 0;
        for (int i = 0; i < n_param * 2; i++) {
            if (offset >= Size) {
                // If we run out of data, break. We will only return the count we successfully filled.
                break;
            }
            EXTRACT_STRING(params[i]);
            if (!params[i]) {
                // If strdup/malloc failed, break
                break;
            }
            filled_count++;
        }
        
        // If we couldn't fill all expected parameters, adjust n_param to match what we have
        // n_param must be consistent with the number of pairs we actually have.
        // filled_count is the number of strings. We need pairs.
        int actual_pairs = filled_count / 2;
        if (actual_pairs != n_param) {
            n_param = actual_pairs;
            // If we have an odd number of strings, the last one is useless, ignore it.
            // The cleanup loop will handle freeing based on n_param * 2.
        }
    }

    *out_db = db;
    *out_journal = journal;
    *out_wal = wal;
    *out_n_param = n_param;
    *out_params = params;
    return 1;

cleanup:
    if (db) free(db);
    if (journal) free(journal);
    if (wal) free(wal);
    if (params) {
        // We don't know the exact count here if we failed mid-loop, 
        // but we can iterate up to the allocated size (n_param * 2) safely because we memset to 0.
        // However, n_param might be uninitialized if we failed before reading it? 
        // No, n_param is read before params allocation.
        // If we are in cleanup, n_param holds the value read from input.
        int limit = n_param * 2;
        for (int i = 0; i < limit; i++) {
            if (params[i]) free(params[i]);
        }
        free(params);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    char *db = NULL, *journal = NULL, *wal = NULL;
    int n_param = 0;
    char **params = NULL;

    if (!parse_fuzz_data(Data, Size, &db, &journal, &wal, &n_param, &params)) {
        return 0;
    }

    // Ensure non-NULL pointers for sqlite3_create_filename
    // If parsing failed to allocate, strdup might fail, check pointers
    if (!db) { db = strdup("dummy.db"); if (!db) return 0; }
    if (!journal) { journal = strdup("dummy.db-journal"); if (!journal) { free(db); return 0; } }
    if (!wal) { wal = strdup("dummy.db-wal"); if (!wal) { free(db); free(journal); return 0; } }

    // Verify that all parameter strings are non-NULL before passing to sqlite3_create_filename
    // The API requires: "None of the 2*N pointers in the P array may be NULL pointers"
    // If our parsing resulted in any NULLs (e.g. due to truncated input), we must skip the call or fix it.
    // Since we adjusted n_param in parse_fuzz_data to only include complete pairs, 
    // params should be valid for i < n_param * 2.
    // But let's double check just in case.
    int valid_params = 1;
    if (n_param > 0 && params) {
        for (int i = 0; i < n_param * 2; i++) {
            if (params[i] == NULL) {
                valid_params = 0;
                break;
            }
        }
    }

    sqlite3_filename fn = NULL;
    if (valid_params) {
        fn = sqlite3_create_filename(db, journal, wal, n_param, (const char **)params);
    }

    if (fn) {
        // Test sqlite3_filename_database
        const char *db_name = sqlite3_filename_database(fn);
        (void)db_name;

        // Test sqlite3_filename_journal
        const char *j_name = sqlite3_filename_journal(fn);
        (void)j_name;

        // Test sqlite3_filename_wal
        const char *w_name = sqlite3_filename_wal(fn);
        (void)w_name;

        // Test sqlite3_uri_parameter with various parameter names
        if (n_param > 0 && params) {
            for (int i = 0; i < n_param; i++) {
                const char *key = params[i * 2];
                if (key) {
                    const char *val = sqlite3_uri_parameter(fn, key);
                    (void)val;
                }
            }
        }

        // Test with a fixed parameter name to ensure coverage
        const char *val_fixed = sqlite3_uri_parameter(fn, "mode");
        (void)val_fixed;

        // Test with a parameter that likely doesn't exist
        const char *val_missing = sqlite3_uri_parameter(fn, "nonexistent_param_xyz");
        (void)val_missing;

        // Free the filename structure
        sqlite3_free_filename(fn);
    }

    // Cleanup allocated strings
    free(db);
    free(journal);
    free(wal);
    if (params) {
        int limit = n_param * 2;
        for (int i = 0; i < limit; i++) {
            if (params[i]) free(params[i]);
        }
        free(params);
    }

    return 0;
}