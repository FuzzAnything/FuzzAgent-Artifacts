// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Dummy scalar function for sqlite3_create_function */
static void dummy_scalar_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    sqlite3_result_int(context, 42);
}

/* Dummy trace callback for sqlite3_trace */
static void dummy_trace_callback(void* user_data, const char* sql) {
    /* Do nothing */
}

/* Valid db_config operations that don't require complex arguments */
static const int valid_db_config_ops[] = {
    SQLITE_DBCONFIG_ENABLE_FKEY,
    SQLITE_DBCONFIG_ENABLE_TRIGGER,
    SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER,
    SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION,
    SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE,
    SQLITE_DBCONFIG_ENABLE_QPSG,
    SQLITE_DBCONFIG_TRIGGER_EQP,
    SQLITE_DBCONFIG_DEFENSIVE,
    SQLITE_DBCONFIG_WRITABLE_SCHEMA,
    SQLITE_DBCONFIG_LEGACY_ALTER_TABLE,
    SQLITE_DBCONFIG_DQS_DML,
    SQLITE_DBCONFIG_DQS_DDL,
    SQLITE_DBCONFIG_ENABLE_VIEW,
    SQLITE_DBCONFIG_LEGACY_FILE_FORMAT,
    SQLITE_DBCONFIG_TRUSTED_SCHEMA,
};

/* Valid file_control operations */
static const int valid_file_control_ops[] = {
    SQLITE_FCNTL_LOCKSTATE,
    SQLITE_FCNTL_GET_LOCKPROXYFILE,
    SQLITE_FCNTL_SET_LOCKPROXYFILE,
    SQLITE_FCNTL_LAST_ERRNO,
    SQLITE_FCNTL_SIZE_HINT,
    SQLITE_FCNTL_CHUNK_SIZE,
    SQLITE_FCNTL_FILE_POINTER,
    SQLITE_FCNTL_SYNC_OMITTED,
    SQLITE_FCNTL_WIN32_AV_RETRY,
    SQLITE_FCNTL_PERSIST_WAL,
    SQLITE_FCNTL_OVERWRITE,
};

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int flags;
    int db_config_op1, db_config_op2;
    int file_control_op;
    void *file_control_arg = NULL;
    const char *func_name = "fuzz_func";
    int n_arg;
    int e_text_rep;
    void *app_data = NULL;
    
    /* Use fuzz data to set parameters safely */
    if (Size < 4) {
        /* Not enough data, use defaults */
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
        db_config_op1 = SQLITE_DBCONFIG_ENABLE_FKEY;
        db_config_op2 = SQLITE_DBCONFIG_ENABLE_TRIGGER;
        file_control_op = SQLITE_FCNTL_LOCKSTATE;
        n_arg = 1;
        e_text_rep = SQLITE_UTF8;
    } else {
        /* Extract values from fuzz data with bounds checking */
        uint32_t seed = 0;
        if (Size >= 4) {
            memcpy(&seed, Data, 4);
        }
        
        /* Use memory database to avoid file I/O issues */
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
        
        /* Select valid db_config operations */
        size_t db_config_count = sizeof(valid_db_config_ops) / sizeof(valid_db_config_ops[0]);
        db_config_op1 = valid_db_config_ops[seed % db_config_count];
        db_config_op2 = valid_db_config_ops[(seed >> 8) % db_config_count];
        
        /* Select valid file_control operations */
        size_t file_control_count = sizeof(valid_file_control_ops) / sizeof(valid_file_control_ops[0]);
        file_control_op = valid_file_control_ops[(seed >> 16) % file_control_count];
        
        /* Valid n_arg range: -1 to 2 (simple scalar functions) */
        n_arg = (int)((seed >> 24) % 4) - 1;
        
        /* Valid text representations */
        int text_reps[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16};
        e_text_rep = text_reps[seed % 4];
        if (seed & 0x100) {
            e_text_rep |= SQLITE_DETERMINISTIC;
        }
    }
    
    /* 1. sqlite3_open_v2 - use memory database to avoid file issues */
    rc = sqlite3_open_v2(":memory:", &db, flags, NULL);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    /* 2. First sqlite3_db_config - only use valid operations */
    (void)sqlite3_db_config(db, db_config_op1, 0);
    
    /* 3. sqlite3_file_control - use valid operations */
    (void)sqlite3_file_control(db, "main", file_control_op, file_control_arg);
    
    /* 4. Second sqlite3_db_config - use valid operations */
    (void)sqlite3_db_config(db, db_config_op2, 1);
    
    /* 5. sqlite3_create_function - with safe parameters */
    (void)sqlite3_create_function(
        db,
        func_name,
        n_arg,
        e_text_rep,
        app_data,
        dummy_scalar_func,
        NULL,
        NULL
    );
    
    /* 6. sqlite3_trace */
    (void)sqlite3_trace(db, dummy_trace_callback, NULL);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}