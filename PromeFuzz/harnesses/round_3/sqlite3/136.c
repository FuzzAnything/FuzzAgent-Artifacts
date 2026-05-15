// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_template = "SELECT ?, ?2, :param, $param, @param;";
    int rc;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    /* Prepare a statement with various parameter types */
    rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Test sqlite3_bind_parameter_count */
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    /* Use fuzzer data to determine binding indices and values */
    size_t data_idx = 0;
    
    /* Test sqlite3_bind_parameter_name for valid indices */
    for (int i = 1; i <= param_count && i <= 5; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        if (name) {
            /* Test sqlite3_bind_parameter_index for named parameters */
            int idx = sqlite3_bind_parameter_index(stmt, name);
        }
    }
    
    /* Bind parameters using fuzzer data */
    for (int i = 1; i <= param_count; i++) {
        if (data_idx >= Size) data_idx = 0;
        
        /* Use fuzzer data to decide binding type */
        uint8_t bind_type = Data[data_idx++ % Size];
        
        switch (bind_type % 4) {
            case 0:
                sqlite3_bind_int(stmt, i, (int)Data[data_idx++ % Size]);
                break;
            case 1:
                sqlite3_bind_double(stmt, i, (double)Data[data_idx++ % Size]);
                break;
            case 2:
                sqlite3_bind_int64(stmt, i, (sqlite3_int64)Data[data_idx++ % Size]);
                break;
            case 3:
                /* Bind with a value derived from multiple bytes */
                if (data_idx + 3 < Size) {
                    int value = (Data[data_idx] << 24) | 
                               (Data[(data_idx + 1) % Size] << 16) |
                               (Data[(data_idx + 2) % Size] << 8) |
                               Data[(data_idx + 3) % Size];
                    sqlite3_bind_int(stmt, i, value);
                    data_idx += 4;
                }
                break;
        }
    }
    
    /* Test binding with out-of-range indices */
    if (param_count > 0) {
        sqlite3_bind_int(stmt, 0, 42);  /* Invalid index 0 */
        sqlite3_bind_int(stmt, param_count + 1, 42);  /* Out of range */
        sqlite3_bind_int(stmt, -1, 42);  /* Negative index */
    }
    
    /* Test sqlite3_bind_parameter_index with random names from fuzzer data */
    if (Size > 0) {
        /* Create a null-terminated name from fuzzer data */
        char param_name[256];
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(param_name, Data, name_len);
        param_name[name_len] = '\0';
        
        int idx = sqlite3_bind_parameter_index(stmt, param_name);
        
        /* Also test with common parameter prefixes */
        idx = sqlite3_bind_parameter_index(stmt, ":param");
        idx = sqlite3_bind_parameter_index(stmt, "$param");
        idx = sqlite3_bind_parameter_index(stmt, "@param");
        idx = sqlite3_bind_parameter_index(stmt, "?2");
    }
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif