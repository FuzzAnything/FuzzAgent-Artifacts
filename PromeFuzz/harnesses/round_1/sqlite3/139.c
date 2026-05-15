// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Define a minimal context structure that matches what we need for testing
typedef struct {
    void *pOut;
    void *pFunc;
    void *pMem;
    void *pVdbe;
    int iOp;
    int isError;
    unsigned char enc;
    unsigned char skipFlag;
    unsigned short argc;
    void *argv[1];
} test_context;

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function is never called directly
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy function to get a valid context
    static int user_data = 42;
    sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, &user_data, 
                           dummy_func, NULL, NULL);
    
    // Prepare a dummy statement
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a test context structure
    test_context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Cast to sqlite3_context pointer for API calls
    sqlite3_context *ctx_ptr = (sqlite3_context*)&ctx;
    
    // Use the fuzz data to determine which functions to call
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: { // sqlite3_result_double
            double value = 0.0;
            if (Size - offset >= sizeof(double)) {
                memcpy(&value, Data + offset, sizeof(double));
                offset += sizeof(double);
            } else if (Size - offset > 0) {
                // Use available bytes
                char temp[sizeof(double)] = {0};
                size_t copy_size = Size - offset < sizeof(double) ? Size - offset : sizeof(double);
                memcpy(temp, Data + offset, copy_size);
                memcpy(&value, temp, sizeof(double));
            }
            sqlite3_result_double(ctx_ptr, value);
            break;
        }
        
        case 1: { // sqlite3_result_null
            sqlite3_result_null(ctx_ptr);
            break;
        }
        
        case 2: { // sqlite3_result_subtype
            unsigned int subtype = 0;
            if (Size - offset >= sizeof(unsigned int)) {
                memcpy(&subtype, Data + offset, sizeof(unsigned int));
                offset += sizeof(unsigned int);
            } else if (Size - offset > 0) {
                subtype = Data[offset];
            }
            sqlite3_result_subtype(ctx_ptr, subtype);
            break;
        }
        
        case 3: { // sqlite3_result_int64
            sqlite3_int64 value = 0;
            if (Size - offset >= sizeof(sqlite3_int64)) {
                memcpy(&value, Data + offset, sizeof(sqlite3_int64));
                offset += sizeof(sqlite3_int64);
            } else if (Size - offset > 0) {
                // Use available bytes
                char temp[sizeof(sqlite3_int64)] = {0};
                size_t copy_size = Size - offset < sizeof(sqlite3_int64) ? Size - offset : sizeof(sqlite3_int64);
                memcpy(temp, Data + offset, copy_size);
                memcpy(&value, temp, sizeof(sqlite3_int64));
            }
            sqlite3_result_int64(ctx_ptr, value);
            break;
        }
        
        case 4: { // sqlite3_aggregate_context
            int nBytes = 0;
            if (Size - offset >= sizeof(int)) {
                memcpy(&nBytes, Data + offset, sizeof(int));
                offset += sizeof(int);
            } else if (Size - offset > 0) {
                nBytes = Data[offset];
            }
            
            // Test with various sizes, including negative and zero
            void *agg_ctx = sqlite3_aggregate_context(ctx_ptr, nBytes);
            if (agg_ctx && nBytes > 0) {
                // Write some data to the aggregate context if allocated
                size_t write_size = Size - offset < (size_t)nBytes ? Size - offset : (size_t)nBytes;
                if (write_size > 0) {
                    memcpy(agg_ctx, Data + offset, write_size);
                }
            }
            break;
        }
        
        case 5: { // sqlite3_user_data
            void *user_data_ptr = sqlite3_user_data(ctx_ptr);
            // The context doesn't have real user data, but we can still call the function
            (void)user_data_ptr; // Suppress unused warning
            break;
        }
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}