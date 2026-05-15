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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 221, nr_unique_branch: 32
//<Quality> {"density":13,"unique_branches":{"whereRangeAdjust":[[162034,7,162034,12,0,0,4,1]],"sqlite3DbStrNDup":[[30596,10,30596,11,0,0,4,1]],"btreeParseCellPtrIndex":[[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,0]],"vdbeSorterCompareInt":[[102689,13,102689,17,0,0,4,1],[102692,9,102692,13,0,0,4,0],[102701,9,102701,14,0,0,4,0],[102702,11,102702,21,0,0,4,0]],"sqlite3VdbeMultiLoad":[[84372,28,84372,32,0,0,4,0]],"whereLikeOptimizationStringFixup":[[156565,7,156565,36,0,0,4,1]],"sqlite3PreferredTableName":[[120653,7,120653,46,0,0,4,0],[120654,9,120654,60,0,0,4,0]],"balance_nonroot":[[77961,9,77961,27,0,0,4,0],[77961,31,77961,58,0,0,4,0]],"cellSizePtrIdxLeaf":[[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,4,0,4,0]],"compileoptionusedFunc":[[128464,7,128464,63,0,0,4,1]],"cellSizePtr":[[14098,27,14098,28,1,0,4,0]],"whereLoopAddBtreeIndex":[[163229,7,163229,38,0,0,4,0],[163396,13,163396,42,0,0,4,1],[163535,10,163535,43,0,0,4,0],[14098,27,14098,28,46,0,4,0],[14098,27,14098,28,47,0,4,1]],"sqlite3VdbeRecordCompareWithSkip":[[88738,15,88738,33,0,0,4,0]],"sqlite3WhereCodeOneLoopStart":[[14098,27,14098,28,139,0,4,1],[14098,27,14098,28,146,0,4,0]],"whereRangeScanEst":[[162354,40,162354,46,0,0,4,1]],"sqlite3Pragma":[[138577,3,138577,21,0,0,4,0],[138582,52,138582,53,0,0,4,0],[138582,52,138582,53,0,0,4,1],[138590,30,138590,34,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_autovacuum_pages","sqlite3_cancel_auto_extension","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_autovacuum_pages","sqlite3_cancel_auto_extension","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom function to demonstrate sqlite3_create_function
static void  id_000052_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc == 1) {
        int val = sqlite3_value_int(argv[0]);
        sqlite3_result_int(context, val * val);  // Square the input
    } else {
        // If too many arguments, report error
        sqlite3_result_error_toobig(context);
    }
}

// Callback for sqlite3_exec
static int  id_000052_exec_callback(void* data, int argc, char** argv, char** colNames) {
    // Do nothing for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize SQLite
    sqlite3_initialize();
    
    // 2. Get library version and compile options (use for information)
    const char* version = sqlite3_libversion();
    const char* compile_option = sqlite3_compileoption_get(0);
    
    // 3. Open database (in-memory for fuzzing)
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 4. Create a string object and append input data
    // First, we need to create a sqlite3_str object
    // Since sqlite3_str_new isn't in the API list, we'll use a workaround
    // by creating a temporary string with the input data
    char* sql_cstr = nullptr;
    if (size > 0) {
        sql_cstr = (char*)malloc(size + 1);
        if (sql_cstr) {
            memcpy(sql_cstr, data, size);
            sql_cstr[size] = '\0';
        }
    }
    
    // 5. Execute SQL commands from input
    char* errmsg = nullptr;
    if (sql_cstr) {
        sqlite3_exec(db, sql_cstr,  id_000052_exec_callback, nullptr, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
        }
    }
    
    // 6. Prepare a statement and bind zeroblob
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    const char* simple_sql = "SELECT ?";
    
    if (sqlite3_prepare_v2(db, simple_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Bind zeroblob using input size (capped at reasonable value)
        sqlite3_uint64 blob_size = size > 1024 ? 1024 : size;
        sqlite3_bind_zeroblob64(stmt, 1, blob_size);
        
        // Step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if needed
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // 7. Create custom function
    sqlite3_create_function(db, "square", 1, SQLITE_UTF8, nullptr, 
                            id_000052_custom_function, nullptr, nullptr);
    
    // 8. Set autovacuum pages callback (dummy implementation)
    sqlite3_autovacuum_pages(db, nullptr, nullptr, nullptr);
    
    // 9. Cancel auto extension (try to cancel a non-existent one)
    sqlite3_cancel_auto_extension(nullptr);
    
    // 10. Drop modules (keep none)
    const char* keep_modules[] = {nullptr};
    sqlite3_drop_modules(db, keep_modules);
    
    // 11. Get error code
    int errcode = sqlite3_errcode(db);
    
    // 12. Clean up
    free(sql_cstr);
    
    // 13. Close database
    sqlite3_close(db);
    
    // 14. Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}