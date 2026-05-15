#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 414, nr_unique_branch: 70
//<Quality> {"density":18,"unique_branches":{"memdbLock":[[52569,7,52569,26,0,0,4,1],[52576,7,52576,31,0,0,4,1],[52580,7,52580,30,0,0,4,0],[52582,13,52582,25,0,0,4,1],[52590,7,52590,32,0,0,4,1],[52591,7,52591,31,0,0,4,1],[52603,7,52603,14,0,0,4,1],[52615,7,52615,20,0,0,4,0]],"sqlite3_vfs_find":[[26014,9,26014,37,0,0,4,1]],"memdbFromDbSchema":[[52936,7,52936,42,0,0,4,1],[52939,7,52939,24,0,0,4,1]],"sqlite3BtreeOpen":[[72502,11,72502,37,0,0,4,0],[72502,41,72502,55,0,0,4,1]],"attachFunc":[[119357,9,119357,16,0,0,4,1],[119359,9,119359,22,0,0,4,0],[119361,11,119361,21,0,0,4,0],[119374,9,119374,11,0,0,4,1],[119471,9,119471,25,0,0,4,1],[119346,31,119346,53,2,0,4,0],[14155,25,14155,46,5,0,4,0],[14155,26,14155,29,5,0,4,0]],"sqlite3BtreeSchema":[[80975,7,80975,20,0,0,4,1]],"sqlite3PagerOpen":[[61476,22,61476,27,0,0,4,0]],"sqlite3_db_cacheflush":[[178509,7,178509,32,0,0,4,1],[178513,12,178513,25,0,0,4,0],[178513,29,178513,38,0,0,4,0],[178513,29,178513,38,0,0,4,1],[178515,9,178515,12,0,0,4,0],[178515,9,178515,12,0,0,4,1],[178515,16,178515,59,0,0,4,1],[178526,12,178526,25,0,0,4,0],[178526,29,178526,38,0,0,4,1]],"memdbUnlock":[[52626,7,52626,26,0,0,4,0],[52626,7,52626,26,0,0,4,1],[52630,7,52630,32,0,0,4,1],[52635,9,52635,40,0,0,4,1]],"sqlite3_system_errno":[[180334,10,180334,12,0,0,4,0]],"memdbOpen":[[52753,7,52753,15,0,0,4,1],[52802,9,52802,13,0,0,4,1],[52810,7,52810,19,0,0,4,0]],"sqlite3PagerWalSupported":[[63957,61,63957,78,0,0,4,1]],"sqlite3PagerLockingMode":[[63740,19,63740,36,0,0,4,0],[63740,40,63740,75,0,0,4,0]],"sqlite3PagerFilename":[[63495,40,63495,68,0,0,4,0]],"sqlite3_free_filename":[[182315,7,182315,11,0,0,4,0]],"databaseIsUnmoved":[[60558,7,60558,26,0,0,4,0]],"sqlite3_deserialize":[[53043,7,53043,32,0,0,4,1],[53046,7,53046,13,0,0,4,1],[53047,7,53047,14,0,0,4,1],[53051,7,53051,17,0,0,4,1],[53054,7,53054,12,0,0,4,0],[53054,16,53054,22,0,0,4,1],[53059,7,53059,14,0,0,4,1],[53065,7,53065,9,0,0,4,1],[53070,7,53070,22,0,0,4,1],[53075,7,53075,11,0,0,4,1],[53084,9,53084,54,0,0,4,0],[53093,7,53093,12,0,0,4,1],[14098,27,14098,28,4,0,4,1]],"memdbRead":[[52457,7,52457,23,0,0,4,0],[52457,7,52457,23,0,0,4,1],[52459,9,52459,20,0,0,4,0],[52459,9,52459,20,0,0,4,1]],"memdbFileControl":[[52665,7,52665,31,0,0,4,1],[52669,7,52669,34,0,0,4,1]],"memdbClose":[[52409,7,52409,16,0,0,4,1],[52433,7,52433,17,0,0,4,0],[52434,9,52434,51,0,0,4,0]],"sqlite3WalHeapMemory":[[68622,11,68622,15,0,0,4,1]],"sqlite3PagerOpenWal":[[64044,9,64044,42,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_shutdown","sqlite3_close","sqlite3_shutdown","sqlite3_deserialize","sqlite3_exec","sqlite3_prepare_v3","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_bytes16","sqlite3_column_text16","sqlite3_finalize","sqlite3_db_cacheflush","sqlite3_sleep","sqlite3_system_errno","sqlite3_mprintf","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_deserialize","sqlite3_exec","sqlite3_prepare_v3","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_bytes16","sqlite3_column_text16","sqlite3_finalize","sqlite3_db_cacheflush","sqlite3_sleep","sqlite3_system_errno","sqlite3_mprintf","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 64) {
	return 0;
	}
	
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_str *str_builder = nullptr;
    int rc;
    
    // 1. Create an in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_shutdown();
        return 0;
    }
    
    // 2. Deserialize database from input data
    // We'll use the first part of input as serialized database
    size_t deserialize_size = size > 1024 ? 1024 : size / 2;
    unsigned char *db_data = (unsigned char*)malloc(deserialize_size);
    if (!db_data) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    memcpy(db_data, data, deserialize_size);
    
    // Try to deserialize the database
    rc = sqlite3_deserialize(db, "main", db_data, deserialize_size, 
                             deserialize_size * 2, SQLITE_DESERIALIZE_FREEONCLOSE);
    if (rc != SQLITE_OK) {
        // If deserialize fails, free the buffer ourselves
        free(db_data);
    }
    // Note: If deserialize succeeds, SQLite takes ownership of db_data
    
    // 3. Create a simple table and insert some data if deserialization failed
    if (rc != SQLITE_OK) {
        const char *create_table = 
            "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);"
            "INSERT INTO test (data) VALUES ('sample1'), ('sample2');";
        sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    }
    
    // 4. Prepare a statement
    const char *query = "SELECT * FROM test";
    rc = sqlite3_prepare_v3(db, query, -1, 0, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 5. Get expanded SQL
        char *expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql) {
            // Do something with expanded SQL
            sqlite3_free(expanded_sql);
        }
        
        // 6. Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // 7. Use sqlite3_column_bytes16
                int bytes16 = sqlite3_column_bytes16(stmt, i);
                
                // Create a sqlite3_value from column data for encoding check
                if (i == 1) { // Assuming second column has text data
                    const void *text_data = sqlite3_column_text16(stmt, i);
                    if (text_data && bytes16 > 0) {
                        // Note: sqlite3_value_encoding is not in provided APIs list
                        // We'll demonstrate usage with a different approach
                    }
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // 8. Create and use sqlite3_str
    // First we need to allocate a sqlite3_str object
    // Since sqlite3_str_new is not in the provided APIs, we'll use alternative approach
    // Create a string using SQL functions instead
    
    // 9. Flush database cache
    sqlite3_db_cacheflush(db);
    
    // 10. Sleep for a short time (use part of input to determine duration)
    int sleep_ms = data[0] % 100; // 0-99 ms
    sqlite3_sleep(sleep_ms);
    
    // 11. Get system errno
    int sys_errno = sqlite3_system_errno(db);
    
    // 12. Create a string builder simulation using available APIs
    // Since we don't have sqlite3_str_new, we'll simulate string building
    char *dynamic_str = sqlite3_mprintf("Test %d %d", sleep_ms, sys_errno);
    if (dynamic_str) {
        sqlite3_free(dynamic_str);
    }
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}