// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy extension entry point function.
 * Matches the signature expected by sqlite3_auto_extension:
 * int xEntryPoint(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pThunk);
 * But declared as void(*)(void) for registration.
 */
static int dummy_extension_entry_point(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pThunk) {
    (void)db;
    (void)pzErrMsg;
    (void)pThunk;
    return SQLITE_OK;
}

/*
 * Another dummy extension to test multiple registrations.
 */
static int dummy_extension_entry_point_2(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pThunk) {
    (void)db;
    (void)pzErrMsg;
    (void)pThunk;
    return SQLITE_OK;
}

/*
 * Helper to open a database connection safely.
 */
static sqlite3 *open_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_free(sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Helper to close a database connection safely.
 */
static void close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Use the input data to drive non-deterministic behavior where possible.
     * We will use the first few bytes to decide which operations to perform.
     * Ensure we check Size before accessing Data indices.
     */
    uint8_t op = Data[0] % 10;
    int enable_load_ext = (Size > 0) ? (Data[0] % 2) : 0;
    int config_op = (Size > 1) ? (Data[1] % 10) : 0;
    int cancel_idx = (Size > 2) ? (Data[2] % 3) : 0;

    /*
     * Reset auto extensions at the start of each test to ensure a clean state.
     * This is a global function, so it affects the entire process.
     */
    sqlite3_reset_auto_extension();

    /*
     * Register some dummy extensions to test the auto-extension mechanism.
     * We register them conditionally based on the input.
     */
    if (op < 5) {
        sqlite3_auto_extension((void (*)(void))dummy_extension_entry_point);
    }
    if (op < 8) {
        sqlite3_auto_extension((void (*)(void))dummy_extension_entry_point_2);
    }

    /*
     * Test sqlite3_cancel_auto_extension.
     * We try to cancel one of the registered extensions or a NULL pointer.
     */
    void (*ext_funcs[])(void) = {
        (void (*)(void))dummy_extension_entry_point,
        (void (*)(void))dummy_extension_entry_point_2,
        NULL
    };
    int cancel_result = sqlite3_cancel_auto_extension(ext_funcs[cancel_idx]);
    (void)cancel_result; /* Suppress unused variable warning */

    /*
     * Test sqlite3_config.
     * We use a dummy configuration operation. Note that sqlite3_config is not threadsafe
     * and should be called before sqlite3_initialize() or after sqlite3_shutdown().
     * Since we are in a fuzzing environment, we assume sqlite3_initialize() might have
     * been called implicitly. We'll try a few safe config options.
     * SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, SQLITE_CONFIG_SERIALIZED
     * are safe to call anytime (though they might return SQLITE_MISUSE if already initialized).
     * We'll use a random value to select a config option.
     */
    int config_result = SQLITE_OK;
    int dummy_int = 0; /* Buffer for options requiring an output pointer */
    switch (config_op) {
        case 0:
            config_result = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            config_result = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            config_result = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
        case 3:
            config_result = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
            break;
        case 4:
            config_result = sqlite3_config(SQLITE_CONFIG_GETMALLOC, NULL);
            break;
        case 5:
            config_result = sqlite3_config(SQLITE_CONFIG_GETMALLOC, NULL);
            break;
        case 6:
            config_result = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, NULL);
            break;
        case 7:
            config_result = sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
            break;
        case 8:
            /* Fixed: Pass address of dummy_int instead of NULL to avoid dereferencing NULL */
            config_result = sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &dummy_int);
            break;
        case 9:
            config_result = sqlite3_config(SQLITE_CONFIG_SQLLOG, NULL, NULL);
            break;
        default:
            break;
    }
    (void)config_result;

    /*
     * Create a dummy database file for testing load_extension.
     */
    const char *db_filename = "./dummy_file";
    sqlite3 *db = open_db(db_filename);

    if (db) {
        /*
         * Test sqlite3_enable_load_extension.
         */
        int enable_result = sqlite3_enable_load_extension(db, enable_load_ext);
        (void)enable_result;

        /*
         * Test sqlite3_load_extension.
         * We try to load a non-existent extension to trigger an error path.
         * The filename is derived from the input data to add variability.
         */
        char *zErrMsg = NULL;
        char ext_filename[256];
        
        /* Ensure we have enough data to copy */
        size_t ext_len = (Size > 1) ? (Size - 1) : 0;
        if (ext_len > 255) {
            ext_len = 255;
        }
        
        if (ext_len > 0) {
            memcpy(ext_filename, Data + 1, ext_len);
            ext_filename[ext_len] = '\0';
        } else {
            strcpy(ext_filename, "nonexistent_extension.so");
        }

        /* Ensure the filename is not NULL and has a valid extension */
        if (ext_len == 0 || ext_filename[0] == '\0') {
            strcpy(ext_filename, "nonexistent_extension.so");
        }

        int load_result = sqlite3_load_extension(db, ext_filename, NULL, &zErrMsg);
        (void)load_result;

        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }

        /*
         * Try loading with a specific entry point name.
         */
        const char *zProc = "sqlite3_extension_init";
        if (Size > 11) {
            /* Create a temporary buffer for the entry point name to ensure null termination */
            char proc_buf[256];
            size_t proc_len = (Size - 11 > 255) ? 255 : (Size - 11);
            memcpy(proc_buf, Data + 11, proc_len);
            proc_buf[proc_len] = '\0';
            zProc = proc_buf;
        }
        
        load_result = sqlite3_load_extension(db, ext_filename, zProc, &zErrMsg);
        (void)load_result;

        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }

        /*
         * Clean up the database connection.
         */
        close_db(db);
    }

    /*
     * Reset auto extensions again to ensure a clean state for the next test.
     */
    sqlite3_reset_auto_extension();

    return 0;
}