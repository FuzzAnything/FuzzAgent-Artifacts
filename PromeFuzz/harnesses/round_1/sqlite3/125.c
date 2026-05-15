// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callback functions for testing */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Do nothing */
}

static void dummy_xFinal(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_xDestroy(void *p) {
    /* Do nothing */
}

static void dummy_xValue(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

static void dummy_xInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Do nothing */
}

static int dummy_xCompare(void *pArg, int len1, const void *str1, int len2, const void *str2) {
    return 0;
}

/* Helper to extract function name from fuzzer input */
static const char* extract_string(const uint8_t *data, size_t size, size_t *offset, char *buffer, size_t buf_size) {
    if (*offset >= size) return NULL;
    
    size_t i = 0;
    while (*offset < size && i < buf_size - 1 && data[*offset] != 0) {
        buffer[i++] = data[(*offset)++];
    }
    buffer[i] = '\0';
    
    if (*offset < size && data[*offset] == 0) {
        (*offset)++; /* Skip null terminator */
    }
    
    return buffer;
}

/* Helper to extract integer from fuzzer input */
static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) <= size) {
        int value;
        memcpy(&value, data + *offset, sizeof(int));
        *offset += sizeof(int);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;
    char func_name[256];
    char clientdata_name[256];
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Extract parameters from fuzzer input */
    const char *zFunctionName = extract_string(Data, Size, &offset, func_name, sizeof(func_name));
    if (zFunctionName == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    int nArg = extract_int(Data, Size, &offset);
    int eTextRep = extract_int(Data, Size, &offset);
    
    /* Test sqlite3_create_function_v2 */
    void *pApp = (void*)(intptr_t)extract_int(Data, Size, &offset);
    sqlite3_create_function_v2(
        db,
        zFunctionName,
        nArg,
        eTextRep,
        pApp,
        dummy_xFunc,
        dummy_xStep,
        dummy_xFinal,
        dummy_xDestroy
    );
    
    /* Test sqlite3_set_clientdata */
    const char *zClientDataName = extract_string(Data, Size, &offset, clientdata_name, sizeof(clientdata_name));
    if (zClientDataName != NULL) {
        void *pClientData = (void*)(intptr_t)extract_int(Data, Size, &offset);
        sqlite3_set_clientdata(db, zClientDataName, pClientData, dummy_xDestroy);
    }
    
    /* Test sqlite3_create_function */
    sqlite3_create_function(
        db,
        zFunctionName,
        nArg,
        eTextRep,
        pApp,
        dummy_xFunc,
        dummy_xStep,
        dummy_xFinal
    );
    
    /* Test sqlite3_create_window_function */
    sqlite3_create_window_function(
        db,
        zFunctionName,
        nArg,
        eTextRep,
        pApp,
        dummy_xStep,
        dummy_xFinal,
        dummy_xValue,
        dummy_xInverse,
        dummy_xDestroy
    );
    
    /* Test sqlite3_create_collation_v2 */
    sqlite3_create_collation_v2(
        db,
        zFunctionName,
        eTextRep,
        pApp,
        dummy_xCompare,
        dummy_xDestroy
    );
    
    /* Test sqlite3_overload_function */
    sqlite3_overload_function(db, zFunctionName, nArg);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}