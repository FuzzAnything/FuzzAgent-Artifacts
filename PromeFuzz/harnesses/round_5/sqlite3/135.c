// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:17178:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static sqlite3* create_test_db(void) {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    
    // Create a simple table to ensure the database has some structure
    const char* create_table_sql = "CREATE TABLE test(id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert some data to potentially allocate memory
    const char* insert_sql = "INSERT INTO test(data) VALUES('test data');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    return db;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API calls
    // We'll use the first few bytes to determine which functions to call and with what parameters
    
    if (Size < 1) {
        return 0;
    }
    
    // Create a database connection for testing
    sqlite3* db = create_test_db();
    if (!db) {
        return 0;
    }
    
    // Use input data to generate parameters
    int param1 = 0;
    int param2 = 0;
    int reset_flag = 0;
    int status_op = 0;
    
    if (Size >= 4) {
        param1 = (int)((Data[0] << 8) | Data[1]);
        param2 = (int)((Data[2] << 8) | Data[3]);
    } else if (Size >= 2) {
        param1 = (int)Data[0];
        param2 = (int)Data[1];
    } else {
        param1 = (int)Data[0];
    }
    
    if (Size >= 5) {
        reset_flag = Data[4] & 1;
        status_op = Data[4] % 10; // There are 10 status parameters (0-9)
    }
    
    // Call sqlite3_release_memory with parameter derived from input
    sqlite3_release_memory(param1);
    
    // Call sqlite3_db_release_memory on our database connection
    sqlite3_db_release_memory(db);
    
    // Call sqlite3_soft_heap_limit (deprecated but still callable)
    sqlite3_soft_heap_limit(param2);
    
    // Call sqlite3_memory_highwater with reset flag
    sqlite3_memory_highwater(reset_flag);
    
    // Call sqlite3_status with various operations
    int current = 0;
    int highwater = 0;
    
    // Try different status operations based on input
    for (int i = 0; i < 3 && i < Size; i++) {
        int op = (Data[i] % 10);
        int local_reset = (Data[i] >> 1) & 1;
        sqlite3_status(op, &current, &highwater, local_reset);
    }
    
    // Also try the specific status operation derived from input
    sqlite3_status(status_op, &current, &highwater, reset_flag);
    
    // Call sqlite3_global_recover
    sqlite3_global_recover();
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}