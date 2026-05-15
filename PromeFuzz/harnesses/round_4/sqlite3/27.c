// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_trace(void *pArg, const char *zSql) {
    /* Empty trace callback */
    (void)pArg;
    (void)zSql;
}

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Empty scalar function */
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    int flags;
    const char *zVfs = NULL;
    int db_config_op1, db_config_op2;
    int file_control_op;
    int create_function_nArg;
    int create_function_eTextRep;
    const char *function_name = "fuzz_func";
    void *pApp = NULL;
    char *dbname = NULL;
    void *file_control_arg = NULL;
    
    /* Use fuzz input to derive parameters */
    if (Size < 10) {
        /* Not enough data, use defaults */
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        db_config_op1 = SQLITE_DBCONFIG_LOOKASIDE;
        db_config_op2 = SQLITE_DBCONFIG_ENABLE_FKEY;
        file_control_op = SQLITE_FCNTL_LOCKSTATE;
        create_function_nArg = 1;
        create_function_eTextRep = SQLITE_UTF8;
        dbname = "main";
    } else {
        /* Use first bytes for various parameters */
        flags = Data[0] | (Data[1] << 8);
        db_config_op1 = Data[2];
        db_config_op2 = Data[3];
        file_control_op = Data[4];
        create_function_nArg = (int)Data[5];
        create_function_eTextRep = Data[6];
        
        /* Choose database name based on input */
        if (Data[7] & 1) {
            dbname = "main";
        } else {
            dbname = "temp";
        }
        
        /* Use remaining data for file_control argument if needed */
        if (Size > 8) {
            file_control_arg = (void *)(Data + 8);
        }
    }
    
    /* Open database */
    rc = sqlite3_open_v2(filename, &db, flags, zVfs);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    /* First sqlite3_db_config call */
    if (db_config_op1 == SQLITE_DBCONFIG_LOOKASIDE) {
        /* Configure lookaside buffer */
        void *pBuf = NULL;
        int sz = 128;
        int cnt = 8;
        sqlite3_db_config(db, db_config_op1, pBuf, sz, cnt);
    } else if (db_config_op1 == SQLITE_DBCONFIG_ENABLE_FKEY) {
        /* Enable/disable foreign keys */
        int bEnable = (Data[2] & 1) ? 1 : 0;
        sqlite3_db_config(db, db_config_op1, bEnable);
    } else {
        /* Try with just the opcode */
        sqlite3_db_config(db, db_config_op1);
    }
    
    /* sqlite3_file_control call */
    sqlite3_file_control(db, dbname, file_control_op, file_control_arg);
    
    /* Second sqlite3_db_config call */
    if (db_config_op2 == SQLITE_DBCONFIG_ENABLE_TRIGGER) {
        int bEnable = (Data[3] & 1) ? 1 : 0;
        sqlite3_db_config(db, db_config_op2, bEnable);
    } else if (db_config_op2 == SQLITE_DBCONFIG_ENABLE_VIEW) {
        int bEnable = (Data[3] & 1) ? 1 : 0;
        sqlite3_db_config(db, db_config_op2, bEnable);
    } else {
        sqlite3_db_config(db, db_config_op2);
    }
    
    /* sqlite3_create_function call */
    /* Limit nArg to reasonable range */
    if (create_function_nArg < -1) create_function_nArg = -1;
    if (create_function_nArg > 127) create_function_nArg = 127;
    
    /* Use fuzz data for function name if available */
    if (Size > 20) {
        /* Create a temporary function name from input */
        char temp_name[256];
        size_t name_len = Size - 20;
        if (name_len > 255) name_len = 255;
        memcpy(temp_name, Data + 20, name_len);
        temp_name[name_len] = '\0';
        sqlite3_create_function(db, temp_name, create_function_nArg,
                               create_function_eTextRep, pApp,
                               dummy_func, NULL, NULL);
    } else {
        sqlite3_create_function(db, function_name, create_function_nArg,
                               create_function_eTextRep, pApp,
                               dummy_func, NULL, NULL);
    }
    
    /* sqlite3_trace call */
    sqlite3_trace(db, dummy_trace, NULL);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}