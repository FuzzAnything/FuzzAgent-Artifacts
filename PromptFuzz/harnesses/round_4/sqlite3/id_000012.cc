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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["sqlite3_shutdown","sqlite3_initialize","sqlite3_open","sqlite3_open_v2","sqlite3_profile","sqlite3_create_function","sqlite3_create_function_v2","sqlite3_create_module","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_free","sqlite3_close","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_shutdown","sqlite3_initialize","sqlite3_open","sqlite3_open_v2","sqlite3_profile","sqlite3_create_function","sqlite3_create_function_v2","sqlite3_create_module","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_free","sqlite3_close","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Profile callback function
static void  id_000012_profile_callback(void *pCtx, const char *zSql, sqlite3_uint64 nTime) {
    // Do nothing in fuzzing context
    (void)pCtx;
    (void)zSql;
    (void)nTime;
}

// Custom function implementation
static void  id_000012_custom_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(context, 42);
}

// Step function for custom aggregate
static void  id_000012_custom_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    (void)context;
}

// Final function for custom aggregate
static void  id_000012_custom_final(sqlite3_context *context) {
    sqlite3_result_int(context, 100);
}

// SQLite module methods (minimal implementation)
static int  id_000012_module_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}

static int  id_000012_module_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return  id_000012_module_create(db, pAux, argc, argv, ppVTab, pzErr);
}

static int  id_000012_module_best_index(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    (void)pVTab; (void)pIdxInfo;
    return SQLITE_OK;
}

static int  id_000012_module_disconnect(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int  id_000012_module_destroy(sqlite3_vtab *pVTab) {
    return  id_000012_module_disconnect(pVTab);
}

static int  id_000012_module_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab; (void)ppCursor;
    return SQLITE_OK;
}

static int  id_000012_module_close(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int  id_000012_module_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int  id_000012_module_next(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int  id_000012_module_eof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int  id_000012_module_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)ctx; (void)i;
    return SQLITE_OK;
}

static int  id_000012_module_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor; (void)pRowid;
    return SQLITE_OK;
}

// SQLite module structure
static sqlite3_module  id_000012_custom_module = {
    0,                           // iVersion
     id_000012_module_create,               // xCreate
     id_000012_module_connect,              // xConnect
     id_000012_module_best_index,           // xBestIndex
     id_000012_module_disconnect,           // xDisconnect
     id_000012_module_destroy,              // xDestroy
     id_000012_module_open,                 // xOpen
     id_000012_module_close,                // xClose
     id_000012_module_filter,               // xFilter
     id_000012_module_next,                 // xNext
     id_000012_module_eof,                  // xEof
     id_000012_module_column,               // xColumn
     id_000012_module_rowid,                // xRowid
    nullptr,                     // xUpdate
    nullptr,                     // xBegin
    nullptr,                     // xSync
    nullptr,                     // xCommit
    nullptr,                     // xRollback
    nullptr,                     // xFindFunction
    nullptr,                     // xRename
    nullptr,                     // xSavepoint
    nullptr,                     // xRelease
    nullptr                      // xRollbackTo
};

// Exec callback function
static int  id_000012_exec_callback(void *data, int argc, char **argv, char **colNames) {
    (void)data; (void)argc; (void)argv; (void)colNames;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<153) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning
    
    // Use input data as database file content
    const char *db_filename = "input_file";
    FILE *db_file = fopen(db_filename, "wb");
    if (db_file) {
        // Copy input data to database file
        fwrite(data, 1, size, db_file);
        assert_file_closed(&db_file);;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Shutdown SQLite (for cleanup)
    sqlite3_shutdown();
    
    // Initialize SQLite
    sqlite3_initialize();
    
    // Open database using sqlite3_open
    sqlite3 *db1 = nullptr;
    int rc1 = sqlite3_open(fuzz_str_1, &db1);
    
    // Open database using sqlite3_open_v2
    sqlite3 *db2 = nullptr;
    int rc2 = sqlite3_open_v2(fuzz_str_2, &db2, 
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                             nullptr);
    
    // Use db2 for further operations if opened successfully
    sqlite3 *db = (rc2 == SQLITE_OK && db2) ? db2 : db1;
    
    if (db) {
        // Set up profile callback
        sqlite3_profile(db,  id_000012_profile_callback, nullptr);
        
        // Create custom function
        sqlite3_create_function(db, fuzz_str_3, fuzz_int32_t_4, 
                               fuzz_int32_t_5, nullptr,
                                id_000012_custom_func, nullptr, nullptr);
        
        // Create custom function with v2
        sqlite3_create_function_v2(db, fuzz_str_6, fuzz_int32_t_7,
                                  fuzz_int32_t_8, nullptr,
                                   id_000012_custom_func,  id_000012_custom_step,  id_000012_custom_final,
                                  nullptr);
        
        // Create module
        sqlite3_create_module(db, fuzz_str_9, & id_000012_custom_module, nullptr);
        
        // Execute SQL statements
        char *errmsg = nullptr;
        const char *sql = "SELECT * FROM sqlite_master;";
        sqlite3_exec(db, fuzz_str_10,  id_000012_exec_callback, nullptr, &errmsg);
        
        // Prepare and execute a statement
        sqlite3_stmt *stmt = nullptr;
        const char *tail = nullptr;
        const char *select_sql = "SELECT 1, 2, 3;";
        
        if (sqlite3_prepare_v2(db, fuzz_str_11, -1, &stmt, &tail) == SQLITE_OK) {
            // Step through results
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Do nothing with results
            }
            
            // Reset statement
            sqlite3_reset(stmt);
            
            // Finalize statement
            sqlite3_finalize(stmt);
        }
        
        // Check transaction state
        sqlite3_txn_state(db, fuzz_str_12);
        
        // Get status information
        int current = 0, highwater = 0;
        sqlite3_status(fuzz_int32_t_13, &current, &highwater, fuzz_int32_t_14);
        
        // Free error message if any
        if (errmsg) {
            sqlite3_free(errmsg);
        }
        
        // Close databases
        if (db1) sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
    }
    
    // Final shutdown
    sqlite3_shutdown();
    
    // Cleanup
    assert_file_closed(&in_file);;
    remove(db_filename);
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}