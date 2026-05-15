// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:132460:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:80193:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Prepare a statement using sqlite3_prepare (legacy)
    if (Size > 0) {
        const char *sql = (const char *)Data;
        int nByte = (Size < 1024) ? (int)Size : 1024;
        
        rc = sqlite3_prepare(db, sql, nByte, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test sqlite3_bind_parameter_count
            int param_count = sqlite3_bind_parameter_count(stmt);
            
            // Test sqlite3_bind_parameter_name for valid indices
            for (int i = 1; i <= param_count && i <= 10; i++) {
                const char *name = sqlite3_bind_parameter_name(stmt, i);
                if (name != NULL) {
                    // Test sqlite3_bind_parameter_index
                    int idx = sqlite3_bind_parameter_index(stmt, name);
                    if (idx > 0) {
                        // Test sqlite3_bind_int with fuzzed data
                        int value = (i < (int)Size) ? Data[i] : 42;
                        sqlite3_bind_int(stmt, idx, value);
                    }
                }
            }
            
            // Also test sqlite3_bind_int with some arbitrary indices
            if (param_count > 0) {
                int idx = (Data[0] % param_count) + 1;
                int value = (Size > 1) ? Data[1] : 100;
                sqlite3_bind_int(stmt, idx, value);
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    // Test sqlite3_prepare_v3 with various flags
    if (Size > 0) {
        const char *sql = (const char *)Data;
        int nByte = (Size < 512) ? (int)Size : 512;
        unsigned int prepFlags = 0;
        
        // Use some fuzzed data to determine flags
        if (Size > 1) {
            prepFlags = Data[0] & 0x0F; // Use lower 4 bits for flags
        }
        
        rc = sqlite3_prepare_v3(db, sql, nByte, prepFlags, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != NULL) {
            // Test parameter functions again
            int param_count = sqlite3_bind_parameter_count(stmt);
            
            // Try binding with some fuzzed parameter names
            if (Size > 10) {
                char param_name[32];
                size_t name_len = (Size - 10 < 30) ? Size - 10 : 30;
                memcpy(param_name, Data + 10, name_len);
                param_name[name_len] = '\0';
                
                // Add a prefix to make it a valid parameter name
                char full_name[40];
                snprintf(full_name, sizeof(full_name), ":%s", param_name);
                
                int idx = sqlite3_bind_parameter_index(stmt, full_name);
                if (idx > 0) {
                    int value = (Size > 20) ? Data[20] : 99;
                    sqlite3_bind_int(stmt, idx, value);
                }
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif