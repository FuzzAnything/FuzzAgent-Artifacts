// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
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
 * Fuzz driver for sqlite3 API functions.
 * Targets: sqlite3_open, sqlite3_errcode, sqlite3_db_status,
 *          sqlite3_db_config, sqlite3_error_offset, sqlite3_errmsg.
 */

/* Helper to safely read an integer from fuzz input */
static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    int val = 0;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

/* Helper to safely read a 64-bit integer from fuzz input */
static sqlite3_int64 read_int64(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 8 > size) return 0;
    sqlite3_int64 val = 0;
    memcpy(&val, data + *offset, 8);
    *offset += 8;
    return val;
}

/* Helper to safely read a string from fuzz input (null-terminated or max length) */
static char *read_string(const uint8_t *data, size_t size, size_t *offset, int max_len) {
    if (*offset >= size) return NULL;
    int len = 0;
    while (*offset + len < size && len < max_len - 1) {
        if (data[*offset + len] == '\0') break;
        len++;
    }
    if (len == 0 && data[*offset] == '\0') {
        *offset += 1;
        return NULL;
    }
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len;
    return str;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    size_t offset = 0;
    int op = read_int(Data, Size, &offset);
    int resetFlag = read_int(Data, Size, &offset);
    
    /* Ensure we have enough data for basic operations */
    if (Size < 8) return 0;

    /* Create a dummy file for testing file-based operations */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Test sqlite3_open with various filenames derived from input */
    char filename[256];
    int filename_len = (Size > 255) ? 255 : Size;
    memcpy(filename, Data, filename_len);
    filename[filename_len] = '\0';

    /* Try opening with different modes by manipulating the filename */
    int rc = sqlite3_open(filename, &db);

    /* Test sqlite3_errcode immediately after open */
    if (db) {
        int errcode = sqlite3_errcode(db);
        (void)errcode; /* Use the value to avoid unused warning */
        
        /* Test sqlite3_errmsg */
        const char *errmsg = sqlite3_errmsg(db);
        if (errmsg) {
            /* Just read the string to ensure it's valid */
            (void)strlen(errmsg);
        }

        /* Test sqlite3_error_offset */
        int error_offset = sqlite3_error_offset(db);
        (void)error_offset;

        /* Test sqlite3_db_status with various operations */
        int pCur = 0, pHiwtr = 0;
        /* Use op from input, but ensure it's a valid range for testing */
        int status_op = op % 100; 
        rc = sqlite3_db_status(db, status_op, &pCur, &pHiwtr, resetFlag % 2);
        (void)rc;

        /* Test sqlite3_db_config with various operations */
        int config_op = (op + 1) % 100;
        int arg1 = read_int(Data, Size, &offset);
        int arg2 = read_int(Data, Size, &offset);
        int arg3 = read_int(Data, Size, &offset);
        
        /* Only pass arguments if the operation expects them */
        if (config_op == 1) { /* SQLITE_DBCONFIG_LOOKASIDE */
            rc = sqlite3_db_config(db, config_op, (void*)arg1, arg2, arg3);
        } else if (config_op == 2) { /* SQLITE_DBCONFIG_ENABLE_FKEY */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 3) { /* SQLITE_DBCONFIG_ENABLE_TRIGGER */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 4) { /* SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 5) { /* SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 6) { /* SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 7) { /* SQLITE_DBCONFIG_ENABLE_QPSG */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 8) { /* SQLITE_DBCONFIG_TRIGGER_EQP */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 9) { /* SQLITE_DBCONFIG_RESET_DATABASE */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 10) { /* SQLITE_DBCONFIG_DEFENSIVE */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 11) { /* SQLITE_DBCONFIG_WRITABLE_SCHEMA */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 12) { /* SQLITE_DBCONFIG_LEGACY_ALTER_TABLE */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 13) { /* SQLITE_DBCONFIG_DQS_DML */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 14) { /* SQLITE_DBCONFIG_DQS_DDL */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 15) { /* SQLITE_DBCONFIG_ENABLE_VIEW */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else if (config_op == 16) { /* SQLITE_DBCONFIG_ENABLE_TRIGGER */
            rc = sqlite3_db_config(db, config_op, arg1);
        } else {
            /* Default case: pass no extra args */
            rc = sqlite3_db_config(db, config_op);
        }
        (void)rc;

        /* Try to open another database to test error handling */
        sqlite3 *db2 = NULL;
        rc = sqlite3_open("./dummy_file", &db2);
        if (db2) {
            int errcode2 = sqlite3_errcode(db2);
            (void)errcode2;
            const char *errmsg2 = sqlite3_errmsg(db2);
            if (errmsg2) (void)strlen(errmsg2);
            int error_offset2 = sqlite3_error_offset(db2);
            (void)error_offset2;
            int pCur2 = 0, pHiwtr2 = 0;
            rc = sqlite3_db_status(db2, status_op, &pCur2, &pHiwtr2, resetFlag % 2);
            (void)rc;
            sqlite3_close(db2);
        }

        sqlite3_close(db);
    } else {
        /* If db is NULL, test error functions with NULL pointer */
        int errcode = sqlite3_errcode(NULL);
        (void)errcode;
        const char *errmsg = sqlite3_errmsg(NULL);
        if (errmsg) (void)strlen(errmsg);
        int error_offset = sqlite3_error_offset(NULL);
        (void)error_offset;
    }

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}