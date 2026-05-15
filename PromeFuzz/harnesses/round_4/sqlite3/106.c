// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Initialize variables for the API calls
    sqlite3 *db = NULL;
    sqlite3_int64 current_val = 0;
    sqlite3_int64 highwater_val = 0;
    int int_current = 0;
    int int_highwater = 0;
    int reset_flag = 0;
    int op = 0;
    sqlite3_int64 limit_val = 0;
    
    // Try to open an in-memory database for testing
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        // If we can't open a database, we can still test some functions
        // but we need to handle db being NULL
    }
    
    // Use the input data to generate parameters for the API calls
    // We'll use the first few bytes to determine which functions to call
    // and what parameters to use
    
    if (Size > 0) {
        // Determine which operation to perform based on first byte
        uint8_t operation = Data[0] % 6;  // 6 different functions to test
        
        // Use subsequent bytes for parameters if available
        size_t data_index = 1;
        
        switch (operation) {
            case 0:  // sqlite3_db_status64
                if (db != NULL) {
                    if (data_index < Size) {
                        op = Data[data_index++] % 20;  // Reasonable range for op codes
                    }
                    if (data_index < Size) {
                        reset_flag = Data[data_index++] % 2;
                    }
                    sqlite3_db_status64(db, op, &current_val, &highwater_val, reset_flag);
                }
                break;
                
            case 1:  // sqlite3_memory_highwater
                if (data_index < Size) {
                    reset_flag = Data[data_index++] % 2;
                }
                sqlite3_memory_highwater(reset_flag);
                break;
                
            case 2:  // sqlite3_status64
                if (data_index < Size) {
                    op = Data[data_index++] % 20;  // Reasonable range for op codes
                }
                if (data_index < Size) {
                    reset_flag = Data[data_index++] % 2;
                }
                sqlite3_status64(op, &current_val, &highwater_val, reset_flag);
                break;
                
            case 3:  // sqlite3_status
                if (data_index < Size) {
                    op = Data[data_index++] % 20;  // Reasonable range for op codes
                }
                if (data_index < Size) {
                    reset_flag = Data[data_index++] % 2;
                }
                sqlite3_status(op, &int_current, &int_highwater, reset_flag);
                break;
                
            case 4:  // sqlite3_memory_used
                sqlite3_memory_used();
                break;
                
            case 5:  // sqlite3_soft_heap_limit64
                // Build a limit value from available data
                limit_val = 0;
                for (int i = 0; i < 8 && data_index < Size; i++) {
                    limit_val = (limit_val << 8) | Data[data_index++];
                }
                sqlite3_soft_heap_limit64(limit_val);
                break;
        }
        
        // Try to call all functions with various parameters to explore more states
        // Use different combinations of parameters from the input data
        
        // Test sqlite3_db_status64 with different ops if we have a database
        if (db != NULL && Size > 2) {
            for (int i = 0; i < 5 && (data_index + 1) < Size; i++) {
                int test_op = Data[data_index++] % 20;
                int test_reset = Data[data_index++] % 2;
                sqlite3_db_status64(db, test_op, &current_val, &highwater_val, test_reset);
            }
        }
        
        // Test sqlite3_status64 with different ops
        if (Size > 3) {
            for (int i = 0; i < 3 && (data_index + 1) < Size; i++) {
                int test_op = Data[data_index++] % 20;
                int test_reset = Data[data_index++] % 2;
                sqlite3_status64(test_op, &current_val, &highwater_val, test_reset);
            }
        }
        
        // Test sqlite3_status with different ops
        if (Size > 3) {
            for (int i = 0; i < 3 && (data_index + 1) < Size; i++) {
                int test_op = Data[data_index++] % 20;
                int test_reset = Data[data_index++] % 2;
                sqlite3_status(test_op, &int_current, &int_highwater, test_reset);
            }
        }
        
        // Test sqlite3_memory_highwater with different reset flags
        if (data_index < Size) {
            sqlite3_memory_highwater(Data[data_index++] % 2);
        }
        
        // Test sqlite3_soft_heap_limit64 with different values
        if (data_index < Size) {
            sqlite3_int64 test_limit = 0;
            for (int i = 0; i < 4 && data_index < Size; i++) {
                test_limit = (test_limit << 8) | Data[data_index++];
            }
            sqlite3_soft_heap_limit64(test_limit);
            
            // Also test with negative value (query current limit)
            sqlite3_soft_heap_limit64(-1);
            
            // Test with zero (disable limit)
            sqlite3_soft_heap_limit64(0);
        }
        
        // Always call sqlite3_memory_used at least once
        sqlite3_memory_used();
    }
    
    // Clean up
    if (db != NULL) {
        sqlite3_close(db);
    }
    
    return 0;
}