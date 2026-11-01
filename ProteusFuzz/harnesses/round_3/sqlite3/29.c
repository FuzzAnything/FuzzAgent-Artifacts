// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to safely read a null-terminated string from fuzz input.
 */
static const char *get_safe_string(const uint8_t *Data, size_t Size, size_t *outLen) {
    if (Size == 0) return NULL;
    size_t len = 0;
    while (len < Size && Data[len] != 0) {
        len++;
    }
    if (outLen) *outLen = len;
    return (const char *)Data;
}

/*
 * Static callback function for sqlite3_trace.
 */
static void trace_callback(void *pArg, const char *zSql) {
    (void)pArg;
    (void)zSql;
}

/*
 * Static callback function for sqlite3_create_function (scalar).
 */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

/*
 * Static callback function for sqlite3_create_function (aggregate step).
 */
static void agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/*
 * Static callback function for sqlite3_create_function (aggregate final).
 */
static void agg_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";

    /* 1. Prepare environment: Open database */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        flags = (Data[0] & 0x07) | SQLITE_OPEN_READWRITE;
        if ((flags & SQLITE_OPEN_CREATE) == 0 && (flags & SQLITE_OPEN_READONLY) == 0) {
             flags |= SQLITE_OPEN_CREATE;
        }
    }

    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_close(db);
        db = NULL;
        rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc != SQLITE_OK || db == NULL) {
            return 0;
        }
    }

    /* Ensure the database is fully initialized by executing a simple query */
    sqlite3_exec(db, "SELECT 1;", NULL, NULL, NULL);

    /* 2. Invoke sqlite3_db_config (First call) */
    int db_config_op = SQLITE_DBCONFIG_LOOKASIDE;
    if (Size > 1) {
        int op_val = Data[1] % 10;
        switch (op_val) {
            case 0: db_config_op = SQLITE_DBCONFIG_LOOKASIDE; break;
            case 1: db_config_op = SQLITE_DBCONFIG_ENABLE_FKEY; break;
            case 2: db_config_op = SQLITE_DBCONFIG_ENABLE_TRIGGER; break;
            case 3: db_config_op = SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER; break;
            case 4: db_config_op = SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION; break;
            default: db_config_op = SQLITE_DBCONFIG_LOOKASIDE; break;
        }
    }
    
    void *pArg = (void *)(size_t)(Size > 2 ? Data[2] : 0);
    int szArg = (Size > 3) ? Data[3] : 0;
    int nArg1 = (Size > 4) ? Data[4] : 0;
    
    if (db_config_op == SQLITE_DBCONFIG_LOOKASIDE) {
        rc = sqlite3_db_config(db, db_config_op, pArg, szArg, nArg1);
    } else {
        rc = sqlite3_db_config(db, db_config_op, (int)(size_t)pArg);
    }

    /* 3. Invoke sqlite3_file_control */
    const char *zDbName = NULL;
    if (Size > 5) {
        if (Data[5] % 2 == 0) {
            zDbName = "main";
        } else {
            zDbName = NULL;
        }
    }
    
    int file_control_op = SQLITE_FCNTL_FILE_POINTER;
    if (Size > 6) {
        int op_val = Data[6] % 10;
        switch (op_val) {
            case 0: file_control_op = SQLITE_FCNTL_FILE_POINTER; break;
            case 1: file_control_op = SQLITE_FCNTL_JOURNAL_POINTER; break;
            case 2: file_control_op = SQLITE_FCNTL_VFS_POINTER; break;
            case 3: file_control_op = SQLITE_FCNTL_DATA_VERSION; break;
            case 4: file_control_op = SQLITE_FCNTL_SIZE_HINT; break;
            case 5: file_control_op = SQLITE_FCNTL_CHUNK_SIZE; break;
            case 6: file_control_op = SQLITE_FCNTL_WIN32_AV_RETRY; break;
            case 7: file_control_op = SQLITE_FCNTL_PERSIST_WAL; break;
            case 8: file_control_op = SQLITE_FCNTL_OVERWRITE; break;
            case 9: file_control_op = SQLITE_FCNTL_VFSNAME; break;
            default: file_control_op = SQLITE_FCNTL_FILE_POINTER; break;
        }
    }

    void *fc_arg = NULL;
    /* 
     * Fix: For opcodes that write to the argument pointer (FILE_POINTER, JOURNAL_POINTER, VFS_POINTER),
     * we must provide a valid memory location to write to.
     */
    sqlite3_file *dummy_file_ptr = NULL;
    sqlite3_vfs *dummy_vfs_ptr = NULL;
    
    if (file_control_op == SQLITE_FCNTL_FILE_POINTER || 
        file_control_op == SQLITE_FCNTL_JOURNAL_POINTER) {
        fc_arg = (void *)&dummy_file_ptr;
    } else if (file_control_op == SQLITE_FCNTL_VFS_POINTER) {
        fc_arg = (void *)&dummy_vfs_ptr;
    } else {
        fc_arg = (void *)(size_t)(Size > 7 ? Data[7] : 0);
    }

    rc = sqlite3_file_control(db, zDbName, file_control_op, fc_arg);

    /* 4. Invoke sqlite3_db_config (Second call) */
    int db_config_op2 = (Size > 8) ? (Data[8] % 5) : 0;
    int arg2 = (Size > 9) ? Data[9] : 0;
    
    if (db_config_op2 == 0) {
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, arg2);
    } else if (db_config_op2 == 1) {
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, arg2);
    } else {
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, (void *)(size_t)arg2, 0, 0);
    }

    /* 5. Invoke sqlite3_create_function */
    const char *func_name = "fuzz_func";
    size_t name_len = 0;
    if (Size > 10) {
        func_name = get_safe_string(Data + 10, Size - 10, &name_len);
        if (name_len == 0) func_name = "fuzz_func";
    }
    
    int nArgFunc = (Size > 11) ? (Data[11] % 10) : 0;
    int eTextRep = SQLITE_UTF8;
    if (Size > 12) {
        int enc = Data[12] % 3;
        if (enc == 1) eTextRep = SQLITE_UTF16LE;
        else if (enc == 2) eTextRep = SQLITE_UTF16BE;
    }
    
    void *pApp = (void *)(size_t)(Size > 13 ? Data[13] : 0);

    rc = sqlite3_create_function(db, func_name, nArgFunc, eTextRep, pApp, scalar_func, NULL, NULL);
    rc = sqlite3_create_function(db, "fuzz_agg", nArgFunc, eTextRep, pApp, NULL, agg_step, agg_final);

    /* 6. Invoke sqlite3_trace */
    void *pTraceArg = (void *)(size_t)(Size > 14 ? Data[14] : 0);
    void *prevArg = sqlite3_trace(db, trace_callback, pTraceArg);
    (void)prevArg;

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}