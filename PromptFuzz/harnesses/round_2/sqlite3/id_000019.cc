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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 15
//<Quality> {"density":19,"unique_branches":{"yy_reduce":[[173990,7,173990,13,0,0,4,0],[14098,27,14098,28,247,0,4,0],[14098,27,14098,28,248,0,4,1]],"sqlite3VdbeExec":[[96231,7,96231,10,0,0,4,1]],"sqlite3_compileoption_used":[[182630,9,182630,57,0,0,4,0],[182631,9,182631,62,0,0,4,0]],"vdbeCommit":[[86915,22,86915,41,0,0,4,0],[86917,9,86917,11,0,0,4,1]],"sqlite3BeginTransaction":[[125278,7,125278,24,0,0,4,0],[125279,14,125279,23,0,0,4,0],[125279,14,125279,23,0,0,4,1],[125282,11,125282,14,0,0,4,0],[125282,11,125282,14,0,0,4,1],[125282,18,125282,45,0,0,4,1],[125284,17,125284,35,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_compileoption_used","sqlite3_open","sqlite3_close","sqlite3_shutdown","sqlite3_commit_hook","sqlite3_prepare_v3","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_deserialize","sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_unlock_notify","sqlite3_exec","sqlite3_exec","sqlite3_close","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_os_end","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_compileoption_used","sqlite3_open","sqlite3_commit_hook","sqlite3_prepare_v3","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_deserialize","sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_unlock_notify","sqlite3_exec","sqlite3_exec","sqlite3_close","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_os_end","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // 1. Check compilation options
    const char* test_options[] = {
        "ENABLE_COLUMN_METADATA",
        "ENABLE_FTS3",
        "ENABLE_FTS4",
        "ENABLE_FTS5",
        "THREADSAFE",
        nullptr
    };
    
    for (int i = 0; test_options[i] != nullptr; i++) {
        sqlite3_compileoption_used(test_options[i]);
    }

    // 2. Create database and prepare statements
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }

    // 3. Set commit hook
    static int commit_count = 0;
    auto commit_hook_callback = [](void* data) -> int {
        int* count = static_cast<int*>(data);
        (*count)++;
        return 0; // Return non-zero to abort commit
    };
    sqlite3_commit_hook(db, commit_hook_callback, &commit_count);

    // 4. Prepare a simple statement
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test (name) VALUES ('test1'), ('test2'), ('test3');"
        "SELECT * FROM test;";
    
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    
    rc = sqlite3_prepare_v3(db, create_table_sql, -1, 0, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != nullptr) {
        // 5. Get bind parameter names
        int param_count = sqlite3_bind_parameter_count(stmt);
        for (int i = 1; i <= param_count; i++) {
            const char* param_name = sqlite3_bind_parameter_name(stmt, i);
            (void)param_name; // Suppress unused warning
        }
        
        // 6. Execute statement and use scanstatus APIs
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get scanstatus information if available
            int idx = 0;
            int iScanStatusOp = 0;
            int flags = 0;
            void* pOut = nullptr;
            
            // Try different scanstatus operations
            for (int op = 0; op < 5; op++) {
                sqlite3_stmt_scanstatus_v2(stmt, idx, op, flags, pOut);
            }
        }
        
        // Reset scanstatus
        sqlite3_stmt_scanstatus_reset(stmt);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }

    // 7. Use deserialize API with input data
    if (size > 0) {
        // Create a copy of the data since deserialize might modify it
        unsigned char* db_data = static_cast<unsigned char*>(malloc(size));
        if (db_data != nullptr) {
            memcpy(db_data, data, size);
            
            // Try to deserialize the data as a database
            sqlite3_deserialize(db, "main", db_data, 
                               static_cast<sqlite3_int64>(size),
                               static_cast<sqlite3_int64>(size) * 2,
                               SQLITE_DESERIALIZE_FREEONCLOSE | 
                               SQLITE_DESERIALIZE_RESIZEABLE);
            
            // The buffer will be freed by SQLite due to FREEONCLOSE flag
        }
    }

    // 8. Set up unlock notify (requires a blocked scenario)
    static int notify_called = 0;
    auto unlock_notify_callback = [](void** apArg, int nArg) {
        notify_called = 1;
    };
    
    // Create a second connection to trigger locking scenario
    sqlite3* db2 = nullptr;
    if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
        // Begin transactions to create locking scenario
        sqlite3_exec(db, "BEGIN EXCLUSIVE", nullptr, nullptr, nullptr);
        sqlite3_exec(db2, "BEGIN EXCLUSIVE", nullptr, nullptr, nullptr);
        
        // Try to trigger unlock notify
        sqlite3_unlock_notify(db, unlock_notify_callback, nullptr);
        
        // Clean up
        sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
        sqlite3_exec(db2, "COMMIT", nullptr, nullptr, nullptr);
        sqlite3_close(db2);
    }

    // 9. Test vtab distinct (requires virtual table setup)
    // Create a simple virtual table for testing
    const char* vtab_sql = 
        "CREATE VIRTUAL TABLE temp.testvtab USING fts3(content);"
        "INSERT INTO temp.testvtab(content) VALUES ('test content');";
    sqlite3_exec(db, vtab_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement that might use the virtual table
    const char* vtab_select = "SELECT * FROM temp.testvtab WHERE content MATCH 'test'";
    sqlite3_stmt* vtab_stmt = nullptr;
    if (sqlite3_prepare_v2(db, vtab_select, -1, &vtab_stmt, nullptr) == SQLITE_OK) {
        // Execute to populate index info
        while (sqlite3_step(vtab_stmt) == SQLITE_ROW) {
            // vtab_distinct would be called from within the virtual table implementation
            // We can't directly call it here as it's for virtual table implementations
        }
        sqlite3_finalize(vtab_stmt);
    }

    // 10. Call os_end (cleanup for the OS layer)
    sqlite3_os_end();

    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}