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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 594, nr_unique_branch: 61
//<Quality> {"density":18,"unique_branches":{"defragmentPage":[[14098,27,14098,28,18,0,4,0]],"kahanBabuskaNeumaierInit":[[129212,7,129212,32,0,0,4,1],[129212,36,129212,61,0,0,4,1]],"btreeComputeFreeSpace":[[71877,11,71877,26,0,0,4,1]],"sqlite3VdbeExpandSql":[[92171,17,92171,39,0,0,4,0]],"destroyTable":[[123462,29,123462,46,0,0,4,0],[123462,51,123462,64,0,0,4,1]],"xferOptimization":[[134713,15,134713,31,0,0,4,0],[134713,35,134713,70,0,0,4,0]],"yy_reduce":[[174572,7,174572,15,0,0,4,0],[14098,27,14098,28,289,0,4,1]],"resolveSelectStep":[[107611,9,107611,19,0,0,4,0],[107612,11,107612,42,0,0,4,0]],"isAuxiliaryVtabOperator":[[18333,5,18333,23,6,0,4,1]],"sqlite3BtreeIndexMoveto":[[75715,9,75715,62,0,0,4,0],[75716,9,75716,36,0,0,4,0],[75719,11,75719,31,0,0,4,1]],"pager_truncate":[[59085,8,59085,42,0,0,4,0]],"kahanBabuskaNeumaierStep":[[129182,7,129182,24,0,0,4,1]],"lookupName":[[106146,13,106146,17,0,0,4,0],[106147,15,106147,18,0,0,4,1],[106151,15,106151,31,0,0,4,1],[106155,21,106155,57,0,0,4,0],[106156,17,106156,30,0,0,4,0],[106362,9,106362,16,0,0,4,1]],"pager_end_transaction":[[58569,41,58569,74,0,0,4,0]],"sqlite3ExprCodeTarget":[[14098,27,14098,28,100,0,4,0]],"sqlite3VdbeExec":[[96381,7,96381,9,0,0,4,0]],"sqlite3ExprIfFalse":[[14098,27,14098,28,41,0,4,0]],"sumFinalize":[[129302,9,129302,18,0,0,4,0],[129303,11,129303,19,0,0,4,1],[129305,17,129305,39,0,0,4,0]],"sqlite3IsLikeFunction":[[129700,25,129700,64,0,0,4,0]],"sumStep":[[129241,9,129241,21,0,0,4,1],[129242,11,129242,31,0,0,4,0],[129258,11,129258,31,0,0,4,1]],"balance_quick":[[77698,47,77698,58,0,0,4,1]],"jsonAppendString":[[203025,9,203025,20,0,0,4,1],[203027,15,203027,21,0,0,4,1],[203027,25,203027,32,0,0,4,1],[203032,15,203032,22,0,0,4,0]],"lengthFunc":[[127548,13,127548,20,0,0,4,0],[127549,18,127549,35,0,0,4,1]],"indexCellCompare":[[75612,8,75612,63,0,0,4,0],[14098,27,14098,28,3,0,4,0],[14098,27,14098,28,3,0,4,1]],"sqlite3ColumnSetExpr":[[120842,7,120842,21,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]],"unixFileControl":[[41907,11,41907,46,0,0,4,0]],"sqlite3_strglob":[[128272,7,128272,17,0,0,4,1],[128274,13,128274,28,0,0,4,1]],"balance":[[78785,28,78785,44,0,0,4,0]],"unixMapfile":[[43127,7,43127,26,0,0,4,1]],"codeAllEqualityTerms":[[156505,9,156505,22,0,0,4,0],[156506,11,156506,18,0,0,4,0],[156523,44,156523,72,0,0,4,0]],"isLikeOrGlob":[[158627,7,158627,60,0,0,4,0]],"sqlite3BtreeUpdateMeta":[[80084,7,80084,20,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_errcode","sqlite3_reset","sqlite3_strglob","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_errcode","sqlite3_reset","sqlite3_strglob","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/

// Custom function implementation
static void  id_000052_myCustomFunc(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc == 1) {
        int val = sqlite3_value_int(argv[0]);
        int type = sqlite3_value_type(argv[0]);
        
        if (type == SQLITE_INTEGER) {
            // Set auxdata (example: store an integer pointer)
            int* aux = new int(val * 2);
            sqlite3_set_auxdata(context, 0, aux, [](void* ptr) { delete static_cast<int*>(ptr); });
            
            sqlite3_result_int(context, val);
        } else if (type == SQLITE_BLOB) {
            sqlite3_result_zeroblob(context, 10);
        } else {
            sqlite3_result_null(context);
        }
    } else {
        sqlite3_result_null(context);
    }
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create temporary file with input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output database file
    const char* db_filename = "fuzz_db.sqlite";
    
    // 2. Open database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set rollback hook
    sqlite3_rollback_hook(db, [](void* data) {
        // Rollback callback
    }, nullptr);
    
    // 4. Create custom function
    rc = sqlite3_create_function(db, "myfunc", 1, SQLITE_UTF8, nullptr, 
                                   id_000052_myCustomFunc, nullptr, nullptr);
    
    // 5. Use sqlite3_exec with SQL from input (if there's enough data)
    char* exec_err = nullptr;
    if (size > 0) {
        // Use first part of data as SQL string (null-terminated copy)
        size_t sql_len = size < 1000 ? size : 1000;
        char* sql = new char[sql_len + 1];
        memcpy(sql, data, sql_len);
        sql[sql_len] = '\0';
        
        sqlite3_exec(db, sql, nullptr, nullptr, &exec_err);
        
        if (exec_err) {
            sqlite3_free(exec_err);
        }
        delete[] sql;
    }
    
    // 6. Prepare and execute a statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    
    const char* test_sql = "SELECT myfunc(?)";
    rc = sqlite3_prepare_v2(db, test_sql, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // 7. Bind zeroblob
        sqlite3_bind_zeroblob(stmt, 1, 100);
        
        // 8. Get expanded SQL
        char* expanded = sqlite3_expanded_sql(stmt);
        if (expanded) {
            sqlite3_free(expanded);
        }
        
        // 9. Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 10. Get column data
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                int int_val = sqlite3_column_int(stmt, i);
                // Just use the value to avoid unused variable warning
                (void)int_val;
                
                // Try to get blob data if available
                const void* blob = sqlite3_column_blob(stmt, i);
                if (blob) {
                    int blob_size = sqlite3_column_bytes(stmt, i);
                    (void)blob_size;
                }
            }
        }
        
        // 11. Check error code
        int err = sqlite3_errcode(db);
        (void)err;
        
        // 12. Reset statement
        sqlite3_reset(stmt);
        
        // 13. Test vtab collation (simulated)
        // Note: sqlite3_vtab_collation requires valid sqlite3_index_info
        // We'll just declare the function pointer type
        const char* (*vtab_collation_func)(sqlite3_index_info*, int) = sqlite3_vtab_collation;
        (void)vtab_collation_func;
        
        // 14. Test string glob matching
        const char* pattern = "test*";
        const char* test_str = "test123";
        int glob_result = sqlite3_strglob(pattern, test_str);
        (void)glob_result;
        
        // 15. Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 16. Close database
    sqlite3_close(db);
    
    // 17. Clean up temporary file
    assert_file_closed(&in_file);;
    
    // Remove the database file
    remove(db_filename);
    
    assert_file_closed(&in_file);
	return 0;
}