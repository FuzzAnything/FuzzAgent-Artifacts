// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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

static int fuzz_sqlite3_extended_errcode(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Trigger an error to have something to query
    const char *sql = (const char *)Data;
    size_t len = Size;
    if (len > 0 && Data[len-1] != 0) {
        // Ensure null termination for SQL string
        char *safe_sql = malloc(len + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, len);
            safe_sql[len] = '\0';
            sqlite3_exec(db, safe_sql, NULL, NULL, NULL);
            free(safe_sql);
        }
    } else if (len > 0) {
        sqlite3_exec(db, (const char *)Data, NULL, NULL, NULL);
    } else {
        // Empty input, try a known bad SQL
        sqlite3_exec(db, "SELECT FROM;", NULL, NULL, NULL);
    }

    // Call the target function
    int errcode = sqlite3_extended_errcode(db);
    (void)errcode; // Suppress unused warning

    sqlite3_close_v2(db);
    return 0;
}

static int fuzz_sqlite3_set_errmsg(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare error code and message from input
    int errcode = SQLITE_OK;
    const char *zErrMsg = NULL;

    if (Size >= 4) {
        errcode = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        if (Size > 4) {
            // Ensure null termination for message
            char *safe_msg = malloc(Size - 3);
            if (safe_msg) {
                memcpy(safe_msg, Data + 4, Size - 4);
                safe_msg[Size - 4] = '\0';
                zErrMsg = safe_msg;
            }
        }
    } else {
        // Default values if input is too small
        errcode = SQLITE_ERROR;
        zErrMsg = "Default error message";
    }

    // Call the target function
    int result = sqlite3_set_errmsg(db, errcode, zErrMsg);
    (void)result;

    // Cleanup allocated message
    if (Size > 4 && zErrMsg != "Default error message") {
        free((void *)zErrMsg);
    }

    sqlite3_close_v2(db);
    return 0;
}

static int fuzz_sqlite3_close_v2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Perform some operations to ensure state
    const char *sql = (const char *)Data;
    if (Size > 0 && Data[Size-1] != 0) {
        char *safe_sql = malloc(Size + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, Size);
            safe_sql[Size] = '\0';
            sqlite3_exec(db, safe_sql, NULL, NULL, NULL);
            free(safe_sql);
        }
    } else if (Size > 0) {
        sqlite3_exec(db, (const char *)Data, NULL, NULL, NULL);
    }

    // Call the target function
    int result = sqlite3_close_v2(db);
    (void)result;

    return 0;
}

static int fuzz_sqlite3_get_table(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare SQL from input
    const char *sql = (const char *)Data;
    char **azResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    char *pzErrmsg = NULL;

    if (Size > 0 && Data[Size-1] != 0) {
        char *safe_sql = malloc(Size + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, Size);
            safe_sql[Size] = '\0';
            sql = safe_sql;
        }
    } else if (Size > 0) {
        // Already null-terminated or empty
        sql = (const char *)Data;
    } else {
        sql = "SELECT 1;";
    }

    // Call the target function
    int result = sqlite3_get_table(db, sql, &azResult, &nRow, &nColumn, &pzErrmsg);
    (void)result;

    // Cleanup
    if (azResult) {
        sqlite3_free_table(azResult);
    }
    if (pzErrmsg) {
        sqlite3_free(pzErrmsg);
    }
    if (Size > 0 && Data[Size-1] != 0) {
        free((void *)sql);
    }

    sqlite3_close_v2(db);
    return 0;
}

static int fuzz_sqlite3_errcode(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Trigger an error
    const char *sql = (const char *)Data;
    if (Size > 0 && Data[Size-1] != 0) {
        char *safe_sql = malloc(Size + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, Size);
            safe_sql[Size] = '\0';
            sqlite3_exec(db, safe_sql, NULL, NULL, NULL);
            free(safe_sql);
        }
    } else if (Size > 0) {
        sqlite3_exec(db, (const char *)Data, NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "SELECT FROM;", NULL, NULL, NULL);
    }

    // Call the target function
    int errcode = sqlite3_errcode(db);
    (void)errcode;

    sqlite3_close_v2(db);
    return 0;
}

static int fuzz_sqlite3_extended_result_codes(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Determine onoff from input
    int onoff = 0;
    if (Size > 0) {
        onoff = Data[0] % 2; // 0 or 1
    }

    // Call the target function
    int result = sqlite3_extended_result_codes(db, onoff);
    (void)result;

    // Trigger an error to see the effect
    sqlite3_exec(db, "SELECT FROM;", NULL, NULL, NULL);

    sqlite3_close_v2(db);
    return 0;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Fuzz each target function
    fuzz_sqlite3_extended_errcode(Data, Size);
    fuzz_sqlite3_set_errmsg(Data, Size);
    fuzz_sqlite3_close_v2(Data, Size);
    fuzz_sqlite3_get_table(Data, Size);
    fuzz_sqlite3_errcode(Data, Size);
    fuzz_sqlite3_extended_result_codes(Data, Size);

    return 0;
}