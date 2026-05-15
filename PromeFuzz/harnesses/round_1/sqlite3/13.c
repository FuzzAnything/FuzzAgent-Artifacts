// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifndef SQLITE_N_LIMIT
#define SQLITE_N_LIMIT 100
#endif

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Initialize a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Use the fuzzer input to generate parameters for the API calls
    // We'll use the input data to create various parameter values
    
    // For sqlite3_limit calls - call 4 times as specified
    // We'll use different limit IDs and values derived from the input
    if (Size >= 16) {
        // Use first 4 ints for limit IDs and next 4 ints for new values
        const int *int_data = (const int*)Data;
        
        // First sqlite3_limit call
        int limit_id1 = int_data[0] % SQLITE_N_LIMIT;  // Ensure valid limit ID
        int new_val1 = int_data[1];
        sqlite3_limit(db, limit_id1, new_val1);
        
        // Second sqlite3_limit call
        int limit_id2 = int_data[2] % SQLITE_N_LIMIT;
        int new_val2 = int_data[3];
        sqlite3_limit(db, limit_id2, new_val2);
        
        // Third sqlite3_limit call
        int limit_id3 = int_data[4] % SQLITE_N_LIMIT;
        int new_val3 = int_data[5];
        sqlite3_limit(db, limit_id3, new_val3);
        
        // Fourth sqlite3_limit call
        int limit_id4 = int_data[6] % SQLITE_N_LIMIT;
        int new_val4 = int_data[7];
        sqlite3_limit(db, limit_id4, new_val4);
    } else {
        // Not enough data, use some defaults
        sqlite3_limit(db, SQLITE_LIMIT_LENGTH, -1);
        sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, -1);
        sqlite3_limit(db, SQLITE_LIMIT_COLUMN, -1);
        sqlite3_limit(db, SQLITE_LIMIT_EXPR_DEPTH, -1);
    }

    // sqlite3_hard_heap_limit64
    sqlite3_int64 heap_limit = 0;
    if (Size >= 24) {
        const sqlite3_int64 *int64_data = (const sqlite3_int64*)Data;
        heap_limit = int64_data[0];
    }
    sqlite3_int64 prev_limit = sqlite3_hard_heap_limit64(heap_limit);
    (void)prev_limit; // Use variable to avoid unused warning

    // sqlite3_test_control
    // We'll use a few different opcodes based on input
    if (Size >= 28) {
        const int *test_data = (const int*)Data;
        int opcode = test_data[8] % 100; // Use a reasonable range for opcodes
        
        // Try different numbers of arguments based on opcode
        if (opcode % 3 == 0) {
            sqlite3_test_control(opcode, test_data[9]);
        } else if (opcode % 3 == 1) {
            sqlite3_test_control(opcode, test_data[9], test_data[10]);
        } else {
            sqlite3_test_control(opcode, test_data[9], test_data[10], test_data[11]);
        }
    }

    // sqlite3_deserialize
    // Create a dummy file for deserialization if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal valid SQLite header
            unsigned char header[] = "SQLite format 3\0";
            fwrite(header, 1, 16, fp);
        }
        fclose(fp);
    }
    
    // Prepare data for deserialization
    unsigned char *pData = NULL;
    sqlite3_int64 szDb = 0;
    sqlite3_int64 szBuf = 0;
    unsigned mFlags = 0;
    
    if (Size > 0) {
        pData = (unsigned char*)malloc(Size);
        if (pData) {
            memcpy(pData, Data, Size);
            szDb = Size;
            szBuf = Size;
            
            // Try deserialization with different schemas
            const char *schemas[] = {"main", "temp", NULL};
            for (int i = 0; schemas[i] != NULL; i++) {
                // Try with different flags
                for (unsigned flags = 0; flags < 4; flags++) {
                    rc = sqlite3_deserialize(db, schemas[i], pData, szDb, szBuf, flags);
                    (void)rc; // Ignore return code
                }
            }
            free(pData);
        }
    }

    // sqlite3_file_control
    // Try with different database names and opcodes
    const char *db_names[] = {"main", "temp", NULL};
    if (Size >= 32) {
        const int *control_data = (const int*)Data;
        int op = control_data[12];
        
        for (int i = 0; db_names[i] != NULL; i++) {
            void *arg = NULL;
            // Use some data as argument if available
            if (Size >= 36) {
                arg = (void*)(Data + 32);
            }
            rc = sqlite3_file_control(db, db_names[i], op, arg);
            (void)rc; // Ignore return code
        }
    } else {
        // Default file_control call
        sqlite3_file_control(db, "main", SQLITE_FCNTL_LOCKSTATE, NULL);
    }

    // Clean up
    sqlite3_close(db);
    
    // Remove dummy file if it exists
    remove(dummy_file);
    
    return 0;
}