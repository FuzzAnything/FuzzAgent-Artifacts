// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_trace(void* arg, const char* sql) {
    (void)arg;
    (void)sql;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    FILE *fp;
    
    if (Size < 1) return 0;
    
    fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        remove(filename);
        return 0;
    }
    
    int op1 = Data[0] % 10;
    int val1 = (int)Data[0];
    rc = sqlite3_db_config(db, op1, val1);
    
    int op2 = Data[0] % 256;
    rc = sqlite3_file_control(db, "main", op2, NULL);
    
    int op3 = (Data[0] + 1) % 10;
    int val3 = (int)Data[0] * 2;
    rc = sqlite3_db_config(db, op3, val3);
    
    char func_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(func_name, Data, name_len);
    func_name[name_len] = '\0';
    
    int nArg = (int)Data[0] % 10;
    if (nArg < 0) nArg = 0;
    int eTextRep = SQLITE_UTF8;
    if (Data[0] & 1) eTextRep |= SQLITE_DETERMINISTIC;
    
    rc = sqlite3_create_function(db, func_name, nArg, eTextRep, NULL,
                                 dummy_func, NULL, NULL);
    
    void *pTraceArg = NULL;
    if (Size > 1) pTraceArg = (void*)(uintptr_t)Data[1];
    sqlite3_trace(db, dummy_trace, pTraceArg);
    
    sqlite3_close(db);
    remove(filename);
    
    return 0;
}