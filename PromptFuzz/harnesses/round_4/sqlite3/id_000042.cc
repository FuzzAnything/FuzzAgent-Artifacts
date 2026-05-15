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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"patternCompare":[[128264,10,128264,21,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_stmt_busy","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_stmt_busy","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_free","sqlite3_close"],"visited":1}
/**/


// Custom function implementation for sqlite3_create_function
static void  id_000042_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        sqlite3_int64 val = sqlite3_value_int64(argv[0]);
        sqlite3_result_int64(context, val * 2);
    } else {
        sqlite3_result_int64(context, 0);
    }
}

// Module implementation for sqlite3_create_module
static int  id_000042_module_create(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int  id_000042_module_connect(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int  id_000042_module_best_index(sqlite3_vtab* pVtab, sqlite3_index_info* info) {
    return SQLITE_OK;
}

static int  id_000042_module_disconnect(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static int  id_000042_module_destroy(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static int  id_000042_module_open(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
    return SQLITE_OK;
}

static int  id_000042_module_close(sqlite3_vtab_cursor* cursor) {
    return SQLITE_OK;
}

static int  id_000042_module_filter(sqlite3_vtab_cursor* cursor, int idxNum, const char* idxStr, int argc, sqlite3_value** argv) {
    return SQLITE_OK;
}

static int  id_000042_module_next(sqlite3_vtab_cursor* cursor) {
    return SQLITE_OK;
}

static int  id_000042_module_eof(sqlite3_vtab_cursor* cursor) {
    return 1;
}

static int  id_000042_module_column(sqlite3_vtab_cursor* cursor, sqlite3_context* ctx, int col) {
    return SQLITE_OK;
}

static int  id_000042_module_rowid(sqlite3_vtab_cursor* cursor, sqlite3_int64* pRowid) {
    return SQLITE_OK;
}

static sqlite3_module  id_000042_module = {
    0,                           // iVersion
     id_000042_module_create,              // xCreate
     id_000042_module_connect,             // xConnect
     id_000042_module_best_index,          // xBestIndex
     id_000042_module_disconnect,          // xDisconnect
     id_000042_module_destroy,             // xDestroy
     id_000042_module_open,                // xOpen
     id_000042_module_close,               // xClose
     id_000042_module_filter,              // xFilter
     id_000042_module_next,                // xNext
     id_000042_module_eof,                 // xEof
     id_000042_module_column,              // xColumn
     id_000042_module_rowid,               // xRowid
    nullptr,                    // xUpdate
    nullptr,                    // xBegin
    nullptr,                    // xSync
    nullptr,                    // xCommit
    nullptr,                    // xRollback
    nullptr,                    // xFindFunction
    nullptr,                    // xRename
    nullptr,                    // xSavepoint
    nullptr,                    // xRelease
    nullptr                     // xRollbackTo
};

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<123) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
	//fuzzer shim end}




    // Open input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_str* str_obj = nullptr;
    char* str_result = nullptr;
    char* err_msg = nullptr;
    int rc;
    
    // 1. Open database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create custom function
    rc = sqlite3_create_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr, 
                                  id_000042_custom_function, nullptr, nullptr);
    
    // 3. Create  id_000042_module
    rc = sqlite3_create_module(db, fuzz_str_4, & id_000042_module, nullptr);
    
    // 4. Create string object
    str_obj = sqlite3_str_new(db);
    
    // Read from input file and append to string
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        sqlite3_str_append(str_obj, buffer, bytes_read);
    }
    
    // 5. Get string result
    str_result = sqlite3_str_finish(str_obj);
    
    // 6. Execute SQL using the string we built
    if (str_result) {
        rc = sqlite3_exec(db, str_result, nullptr, nullptr, &err_msg);
        sqlite3_free(str_result);
    }
    
    // 7. Prepare a statement
    const char* sql = "SELECT ?1, ?2, ?3";
    rc = sqlite3_prepare_v2(db, fuzz_str_5, -1, &stmt, nullptr);
    
    if (stmt) {
        // 8. Bind integer
        sqlite3_bind_int(stmt, fuzz_int32_t_6, fuzz_int32_t_7);
        
        // 9. Bind text (using first 100 bytes of input if available)
        int text_len = size > 100 ? 100 : size;
        sqlite3_bind_text(stmt, fuzz_int32_t_8, (const char*)data, text_len, SQLITE_TRANSIENT);
        
        // 10. Bind pointer (using output file descriptor)
        int fd = fuzz_fileno(out_file);
        sqlite3_bind_pointer(stmt, fuzz_int32_t_10, &fd, fuzz_str_9, nullptr);
        
        // 11. Check if statement is busy
        int busy = sqlite3_stmt_busy(stmt);
        
        // Execute statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Process row if needed
        }
        
        // 12. Reset statement
        sqlite3_reset(stmt);
        
        // 13. Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 14. WAL checkpoint
    int pnLog, pnCkpt;
    rc = sqlite3_wal_checkpoint_v2(db, fuzz_str_11, fuzz_int32_t_12, &pnLog, &pnCkpt);
    
    // 15. Set client data
    int client_data = 12345;
    rc = sqlite3_set_clientdata(db, fuzz_str_13, &client_data, nullptr);
    
    // 16. Get client data
    void* retrieved_data = sqlite3_get_clientdata(db, fuzz_str_14);
    
    // Clean up
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // 17. Close database
    sqlite3_close(db);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}