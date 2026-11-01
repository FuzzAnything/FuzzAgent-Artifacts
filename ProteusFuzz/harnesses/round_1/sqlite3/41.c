// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/*
 * Static helper to safely extract a null-terminated string from fuzz input.
 * Returns a newly allocated string or NULL if Size is 0.
 */
static char *fuzz_str_dup(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;
    /* Find the first null byte */
    size_t len = 0;
    while (len < Size && Data[len] != '\0') {
        len++;
    }
    if (len == 0) return NULL;
    
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, Data, len);
    str[len] = '\0';
    return str;
}

/*
 * Static helper to create a dummy file for extension loading tests.
 */
static int create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    /* Write some dummy binary data that looks like a shared library header */
    const char *dummy = "ELF"; 
    fwrite(dummy, 1, 3, f);
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errMsg = NULL;
    
    /* 
     * Step 1: Prepare environment.
     * We need a valid database connection for most APIs.
     * Use ":memory:" to avoid file system issues, or a dummy file.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Extract potential parameters from fuzz input if available.
     * We use a simple state machine approach based on the first few bytes.
     */
    int op = 0;
    int param1 = 0;
    int param2 = 0;
    char *str1 = NULL;
    char *str2 = NULL;

    if (Size > 0) {
        op = Data[0] % 6; /* 0-5 for the 6 target functions */
        if (Size > 1) param1 = Data[1];
        if (Size > 2) param2 = Data[2];
    }

    /* 
     * Step 2 & 3: Invoke target functions diversely.
     */
    switch (op) {
        case 0: {
            /* sqlite3_log(int iErrCode, const char *zFormat, ...) */
            /* Requires non-NULL format string. */
            const char *fmt = "Test log message";
            if (Size > 10) {
                str1 = fuzz_str_dup(Data + 3, Size - 3);
                if (str1) fmt = str1;
            }
            /* Ensure format is not NULL as per docs */
            if (fmt) {
                sqlite3_log(param1, fmt, param2);
            }
            free(str1);
            break;
        }

        case 1: {
            /* sqlite3_extended_result_codes(sqlite3*, int onoff) */
            /* Enable (1) or Disable (0) */
            int onoff = (param1 % 2);
            rc = sqlite3_extended_result_codes(db, onoff);
            (void)rc; /* Ignore return for fuzzing, just trigger path */
            break;
        }

        case 2: {
            /* sqlite3_errcode(sqlite3 *db) */
            /* Just call it. It returns the last error code. */
            rc = sqlite3_errcode(db);
            (void)rc;
            break;
        }

        case 3: {
            /* sqlite3_errstr(int) */
            /* Convert an error code to string */
            int errCode = param1;
            const char *str = sqlite3_errstr(errCode);
            (void)str;
            break;
        }

        case 4: {
            /* sqlite3_load_extension(sqlite3 *db, const char *zFile, const char *zProc, char **pzErrMsg) */
            /* 
             * This requires extension loading to be enabled.
             * We enable it first.
             */
            sqlite3_enable_load_extension(db, 1);
            
            /* Create a dummy file to load */
            create_dummy_file("./dummy_file");
            
            const char *zFile = "./dummy_file";
            const char *zProc = NULL;
            if (Size > 20) {
                str1 = fuzz_str_dup(Data + 3, Size - 3);
                if (str1) zProc = str1;
            }
            
            rc = sqlite3_load_extension(db, zFile, zProc, &errMsg);
            
            /* Cleanup */
            if (errMsg) {
                sqlite3_free(errMsg);
                errMsg = NULL;
            }
            free(str1);
            break;
        }

        case 5: {
            /* sqlite3_extended_errcode(sqlite3 *db) */
            /* Similar to sqlite3_errcode but returns extended code */
            rc = sqlite3_extended_errcode(db);
            (void)rc;
            break;
        }

        default:
            /* Fallback: try a random SQL execution to generate errors for errcode APIs */
            {
                const char *sql = "SELECT 1";
                if (Size > 5) {
                    str1 = fuzz_str_dup(Data + 3, Size - 3);
                    if (str1) sql = str1;
                }
                sqlite3_stmt *stmt;
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                free(str1);
            }
            break;
    }

    /* 
     * Step 4: Cleanup.
     * Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}