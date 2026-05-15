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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 52
//<Quality> {"density":23,"unique_branches":{"patternCompare":[[128155,14,128155,48,0,0,4,0],[128156,18,128156,31,0,0,4,0],[128156,35,128156,46,0,0,4,0],[128157,13,128157,24,0,0,4,0],[128157,13,128157,24,0,0,4,1],[128157,28,128157,56,0,0,4,0],[128157,28,128157,56,0,0,4,1],[128163,17,128163,30,0,0,4,0],[128164,13,128164,31,0,0,4,1],[128171,18,128171,26,0,0,4,0],[128171,18,128171,26,0,0,4,1],[128173,17,128173,39,0,0,4,0],[128173,17,128173,39,0,0,4,1],[128189,11,128189,17,0,0,4,1],[128209,16,128209,43,0,0,4,0],[128209,16,128209,43,0,0,4,1],[128210,15,128210,20,0,0,4,0],[128218,11,128218,29,0,0,4,1],[128227,13,128227,17,0,0,4,0],[128227,13,128227,17,0,0,4,1],[128229,13,128229,20,0,0,4,0],[128229,13,128229,20,0,0,4,1],[128233,13,128233,20,0,0,4,0],[128233,13,128233,20,0,0,4,1],[128234,15,128234,21,0,0,4,1],[128237,16,128237,18,0,0,4,0],[128237,16,128237,18,0,0,4,1],[128237,22,128237,29,0,0,4,0],[128237,22,128237,29,0,0,4,1],[128238,15,128238,22,0,0,4,0],[128238,15,128238,22,0,0,4,1],[128238,26,128238,42,0,0,4,0],[128238,26,128238,42,0,0,4,1],[128238,46,128238,60,0,0,4,0],[128238,46,128238,60,0,0,4,1],[128238,64,128238,73,0,0,4,0],[128238,64,128238,73,0,0,4,1],[128240,17,128240,27,0,0,4,0],[128240,17,128240,27,0,0,4,1],[128240,31,128240,36,0,0,4,0],[128240,31,128240,36,0,0,4,1],[128243,17,128243,22,0,0,4,0],[128243,17,128243,22,0,0,4,1],[128250,13,128250,18,0,0,4,0],[128250,13,128250,18,0,0,4,1],[128250,22,128250,40,0,0,4,0],[128250,22,128250,40,0,0,4,1],[128261,46,128261,51,0,0,4,1],[128264,10,128264,21,0,0,4,1],[128082,37,128082,46,2,0,4,1],[20329,7,20329,23,6,0,4,1],[128082,37,128082,46,12,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_mprintf","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_explain","sqlite3_step","sqlite3_column_count","sqlite3_column_name","sqlite3_strglob","sqlite3_column_type","sqlite3_column_bytes16","sqlite3_column_origin_name16","sqlite3_finalize","sqlite3_get_clientdata","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_mprintf","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_explain","sqlite3_step","sqlite3_column_count","sqlite3_column_name","sqlite3_strglob","sqlite3_column_type","sqlite3_column_bytes16","sqlite3_column_origin_name16","sqlite3_finalize","sqlite3_get_clientdata","sqlite3_free","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite APIs in a coordinated event:

*/


extern "C" {
}

// Custom comparison function for collation
static int  id_000031_customCollation(void* pArg, int n1, const void* v1, int n2, const void* v2) {
    // Simple comparison that doesn't depend on data
    return 0;
}

// Cleanup function for pointer binding
static void  id_000031_pointerCleanup(void* p) {
    // No-op cleanup
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimum data
    }

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    
    // 1. Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    // 2. Set client data first (so we can retrieve it later)
    char* clientKey = sqlite3_mprintf("fuzz_key_%d", data[0] % 100);
    int* clientData = new int(42);
    sqlite3_set_clientdata(db, clientKey, clientData, [](void* p) { delete static_cast<int*>(p); });
    
    // 3. Create a custom collation
    const char* collationName = "custom_fuzz_collation";
    rc = sqlite3_create_collation(
        db, 
        collationName,
        SQLITE_UTF8,
        nullptr,  // pArg
         id_000031_customCollation
    );
    
    // 4. Create a table with the custom collation
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT COLLATE custom_fuzz_collation, "
        "data BLOB)";
    
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);
    
    // 5. Prepare an INSERT statement
    const char* insertSQL = "INSERT INTO fuzz_table (name, data) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 6. Bind pointer parameter (using data address as a dummy pointer)
        void* dummyPtr = const_cast<void*>(static_cast<const void*>(data));
        rc = sqlite3_bind_pointer(
            stmt,
            1,  // parameter index
            dummyPtr,
            "fuzz_pointer_type",
             id_000031_pointerCleanup
        );
        
        // 7. Bind blob parameter with actual fuzz data
        if (size > 0) {
            // Use part of fuzz data for blob
            size_t blobSize = size > 100 ? 100 : size;
            rc = sqlite3_bind_blob(
                stmt,
                2,  // parameter index
                data,
                static_cast<int>(blobSize),
                SQLITE_STATIC
            );
        }
        
        // 8. Execute the statement
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 9. Prepare a SELECT statement to retrieve data
    const char* selectSQL = "SELECT * FROM fuzz_table";
    rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 10. Get EXPLAIN output
        rc = sqlite3_stmt_explain(stmt, 1);  // 1 = EXPLAIN QUERY PLAN
        
        // 11. Execute and process results
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int colCount = sqlite3_column_count(stmt);
            
            for (int i = 0; i < colCount; i++) {
                // 12. Use strglob to check column names
                const char* colName = sqlite3_column_name(stmt, i);
                if (colName) {
                    // Use part of fuzz data as pattern
                    char pattern[32];
                    size_t patternLen = size > 31 ? 31 : size;
                    memcpy(pattern, data, patternLen);
                    pattern[patternLen] = '\0';
                    
                    int globResult = sqlite3_strglob(pattern, colName);
                    // globResult is -1 for pattern error, 0 for match, 1 for no match
                }
                
                // 13. Get column bytes for BLOB/text columns
                int colType = sqlite3_column_type(stmt, i);
                if (colType == SQLITE_BLOB || colType == SQLITE_TEXT) {
                    int byteCount = sqlite3_column_bytes16(stmt, i);
                    // byteCount contains the size
                }
                
                // 14. Get origin name (returns NULL for non-column expressions)
                const void* originName = sqlite3_column_origin_name16(stmt, i);
                // originName might be NULL
            }
        }
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 15. Retrieve client data we set earlier
    void* retrievedData = sqlite3_get_clientdata(db, clientKey);
    // retrievedData should be our pointer
    
    // 16. Use the file APIs as specified in requirements
    FILE* in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen(const_cast<void*>(static_cast<const void*>(data)), size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            // Could use fd for something, but sqlite3 doesn't directly use file descriptors
            assert_file_closed(&in_file);;
        }
    }
    
    // Clean up
    if (clientKey) {
        sqlite3_free(clientKey);
    }
    
    // Close database
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}