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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 207, nr_unique_branch: 37
//<Quality> {"density":23,"unique_branches":{"sqlite3_set_authorizer":[[119943,7,119943,32,0,0,4,1],[119948,7,119948,16,0,0,4,0]],"sqlite3AuthCheck":[[120078,7,120078,19,0,0,4,1],[120078,23,120078,36,0,0,4,0],[120078,23,120078,36,0,0,4,1],[120098,7,120098,22,0,0,4,1],[120101,13,120101,26,0,0,4,1],[19767,28,19767,67,1,0,4,1],[14098,27,14098,28,3,0,4,0],[14098,27,14098,28,3,0,4,1],[14098,27,14098,28,4,0,4,0],[14098,27,14098,28,4,0,4,1],[14098,27,14098,28,5,0,4,0],[14098,27,14098,28,5,0,4,1],[14098,27,14098,28,6,0,4,0]],"sqlite3_total_changes64":[[178730,7,178730,32,0,0,4,1]],"sqlite3AuthRead":[[120023,7,120023,12,0,0,4,1],[120029,7,120029,28,0,0,4,1],[120033,17,120033,36,0,0,4,0],[120034,11,120034,51,0,0,4,0],[120041,7,120041,14,0,0,4,1],[120043,7,120043,14,0,0,4,0],[120043,7,120043,14,0,0,4,1],[120046,13,120046,27,0,0,4,1],[120053,7,120053,72,0,0,4,1]],"sqlite3AuthReadCol":[[119981,7,119981,20,0,0,4,0],[119981,7,119981,20,0,0,4,1],[119987,7,119987,22,0,0,4,1],[119992,13,119992,30,0,0,4,0],[119992,34,119992,47,0,0,4,1]],"sqlite3_create_module_v2":[[153776,7,153776,32,0,0,4,1],[153776,36,153776,44,0,0,4,1]],"lookupName":[[106532,9,106532,26,0,0,4,0],[106533,10,106533,30,0,0,4,0]],"sqlite3_clear_bindings":[[89629,12,89629,21,0,0,4,0],[89629,12,89629,21,0,0,4,1],[89634,7,89634,17,0,0,4,1]]},"library_calls":["sqlite3_malloc","sqlite3_malloc64","sqlite3_free","sqlite3_open","sqlite3_free","sqlite3_set_authorizer","sqlite3_create_module_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_clear_bindings","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_frombind","sqlite3_finalize","sqlite3_finalize","sqlite3_total_changes64","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_malloc","sqlite3_malloc64","sqlite3_free","sqlite3_open","sqlite3_set_authorizer","sqlite3_create_module_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_clear_bindings","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_frombind","sqlite3_finalize","sqlite3_finalize","sqlite3_total_changes64","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Define dummy sqlite3_module for testing
static sqlite3_module  id_000016_dummy_module = {
    0,                        /* iVersion */
    nullptr,                  /* xCreate */
    nullptr,                  /* xConnect */
    nullptr,                  /* xBestIndex */
    nullptr,                  /* xDisconnect */
    nullptr,                  /* xDestroy */
    nullptr,                  /* xOpen */
    nullptr,                  /* xClose */
    nullptr,                  /* xFilter */
    nullptr,                  /* xNext */
    nullptr,                  /* xEof */
    nullptr,                  /* xColumn */
    nullptr,                  /* xRowid */
    nullptr,                  /* xUpdate */
    nullptr,                  /* xBegin */
    nullptr,                  /* xSync */
    nullptr,                  /* xCommit */
    nullptr,                  /* xRollback */
    nullptr,                  /* xFindFunction */
    nullptr,                  /* xRename */
    nullptr,                  /* xSavepoint */
    nullptr,                  /* xRelease */
    nullptr                   /* xRollbackTo */
};

// Simple authorizer callback that allows everything
static int  id_000016_test_authorizer(void* pUserData, int action, 
                          const char* arg1, const char* arg2,
                          const char* arg3, const char* arg4) {
    return SQLITE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Need minimum data
    }

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_value* value = nullptr;
    const char* db_name = nullptr;
    
    // 1. Allocate memory using sqlite3_malloc
    int* int_ptr = (int*)sqlite3_malloc(sizeof(int));
    if (!int_ptr) {
        return 0;
    }
    *int_ptr = 42;
    
    // 2. Allocate more memory using sqlite3_malloc64
    char* char_ptr = (char*)sqlite3_malloc64(size * 2);
    if (char_ptr) {
        memcpy(char_ptr, data, size > 1024 ? 1024 : size);
        sqlite3_free(char_ptr);
    }
    
    // 3. Open in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        sqlite3_free(int_ptr);
        return 0;
    }
    
    // 4. Set authorizer
    rc = sqlite3_set_authorizer(db,  id_000016_test_authorizer, nullptr);
    
    // 5. Create a virtual table module
    rc = sqlite3_create_module_v2(db, " id_000016_dummy_module", & id_000016_dummy_module, 
                                  nullptr, nullptr);
    
    // 6. Create a simple table and prepare a statement
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, data BLOB)", 
                      nullptr, nullptr, nullptr);
    
    rc = sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?, ?)", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        if (size > 0) {
            sqlite3_bind_blob(stmt, 2, data, (int)(size > 1000 ? 1000 : size), 
                             SQLITE_STATIC);
        }
        
        // 7. Clear bindings
        sqlite3_clear_bindings(stmt);
        
        // Rebind
        sqlite3_bind_int(stmt, 1, 2);
        if (size > 0) {
            sqlite3_bind_blob(stmt, 2, data, (int)(size > 1000 ? 1000 : size), 
                             SQLITE_STATIC);
        }
        
        // Execute
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // 8. Create another statement to test sqlite3_value_frombind
        sqlite3_stmt* stmt2 = nullptr;
        rc = sqlite3_prepare_v2(db, "SELECT data FROM test WHERE id = ?", -1, 
                                &stmt2, nullptr);
        if (rc == SQLITE_OK && stmt2) {
            sqlite3_bind_int(stmt2, 1, 2);
            
            if (sqlite3_step(stmt2) == SQLITE_ROW) {
                // Get value from column
                value = sqlite3_column_value(stmt2, 0);
                
                // 9. Check if value is from bind (it shouldn't be for column values)
                int from_bind = sqlite3_value_frombind(value);
                // Result intentionally not used - just calling the API
            }
            
            sqlite3_finalize(stmt2);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // 10. Get total changes
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    
    // Cleanup
    if (int_ptr) {
        sqlite3_free(int_ptr);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}