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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"vdbeUnbind":[[91095,7,91095,38,0,0,4,0]],"sqlite3_value_pointer":[[89680,7,89681,49,0,0,4,1]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_mprintf","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_vtab_config","sqlite3_get_clientdata","sqlite3_create_function","sqlite3_create_module","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_bind_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_journal","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_mprintf","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_vtab_config","sqlite3_get_clientdata","sqlite3_create_function","sqlite3_create_module","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_bind_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_journal","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/**/


// Custom function implementation for sqlite3_create_function
static void  id_000047_custom_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Return a zero blob of size 64
    sqlite3_result_zeroblob64(context, 64);
}

// Custom function for pointer binding test
static void  id_000047_pointer_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        const char* type = "test_pointer";
        void* ptr = sqlite3_value_pointer(argv[0], type);
        if (ptr) {
            sqlite3_result_text(context, "valid_pointer", -1, SQLITE_TRANSIENT);
        }
    }
}

// Virtual table module implementation
static int  id_000047_vtab_create(sqlite3*, void*, int, const char* const*, sqlite3_vtab**, char**) { return SQLITE_OK; }
static int  id_000047_vtab_connect(sqlite3*, void*, int, const char* const*, sqlite3_vtab**, char**) { return SQLITE_OK; }
static int  id_000047_vtab_best_index(sqlite3_vtab*, sqlite3_index_info*) { return SQLITE_OK; }
static int  id_000047_vtab_disconnect(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_destroy(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_open(sqlite3_vtab*, sqlite3_vtab_cursor**) { return SQLITE_OK; }
static int  id_000047_vtab_close(sqlite3_vtab_cursor*) { return SQLITE_OK; }
static int  id_000047_vtab_filter(sqlite3_vtab_cursor*, int, const char*, int, sqlite3_value**) { return SQLITE_OK; }
static int  id_000047_vtab_next(sqlite3_vtab_cursor*) { return SQLITE_OK; }
static int  id_000047_vtab_eof(sqlite3_vtab_cursor*) { return 1; }
static int  id_000047_vtab_column(sqlite3_vtab_cursor*, sqlite3_context*, int) { return SQLITE_OK; }
static int  id_000047_vtab_rowid(sqlite3_vtab_cursor*, sqlite3_int64*) { return SQLITE_OK; }
static int  id_000047_vtab_update(sqlite3_vtab*, int, sqlite3_value**, sqlite3_int64*) { return SQLITE_OK; }
static int  id_000047_vtab_begin(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_sync(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_commit(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_rollback(sqlite3_vtab*) { return SQLITE_OK; }
static int  id_000047_vtab_find_function(sqlite3_vtab*, int, const char*, void (**)(sqlite3_context*,int,sqlite3_value**), void**) { return 0; }
static int  id_000047_vtab_rename(sqlite3_vtab*, const char*) { return SQLITE_OK; }

static sqlite3_module  id_000047_vtab_module = {
    0,                           // iVersion
     id_000047_vtab_create,                // xCreate
     id_000047_vtab_connect,               // xConnect
     id_000047_vtab_best_index,            // xBestIndex
     id_000047_vtab_disconnect,            // xDisconnect
     id_000047_vtab_destroy,               // xDestroy
     id_000047_vtab_open,                  // xOpen
     id_000047_vtab_close,                 // xClose
     id_000047_vtab_filter,                // xFilter
     id_000047_vtab_next,                  // xNext
     id_000047_vtab_eof,                   // xEof
     id_000047_vtab_column,                // xColumn
     id_000047_vtab_rowid,                 // xRowid
     id_000047_vtab_update,                // xUpdate
     id_000047_vtab_begin,                 // xBegin
     id_000047_vtab_sync,                  // xSync
     id_000047_vtab_commit,                // xCommit
     id_000047_vtab_rollback,              // xRollback
     id_000047_vtab_find_function,         // xFindFunction
     id_000047_vtab_rename,                // xRename
};

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<354) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_20, fuzz_str_sz_20, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_21, fuzz_str_sz_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_23, fuzz_str_sz_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_25, fuzz_str_sz_25, fdp);
	//fuzzer shim end}




    // Set heap limit
    sqlite3_hard_heap_limit64(1024 * 1024); // 1MB limit
    
    // Create memory buffer from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read input as database content
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char* db_content = (char*)malloc(file_size);
    if (!db_content) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(db_content, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(db_content);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Open database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(db_content);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Execute SQL to create table
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, 
        fuzz_str_1,
        nullptr, nullptr, &errmsg);
    
    if (errmsg) sqlite3_free(errmsg);
    
    // Use sqlite3_mprintf
    char* sql = sqlite3_mprintf("INSERT INTO test_table (data) VALUES (zeroblob(%d))", 100);
    if (sql) {
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
        sqlite3_free(sql);
        if (errmsg) sqlite3_free(errmsg);
    }
    
    // Get last insert rowid
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    
    // Open blob for writing
    sqlite3_blob* blob = nullptr;
    rc = sqlite3_blob_open(db, fuzz_str_2, fuzz_str_3, fuzz_str_4, rowid, fuzz_int32_t_5, &blob);
    
    if (rc == SQLITE_OK && blob) {
        // Write data to blob
        int write_size = size > 100 ? 100 : size;
        rc = sqlite3_blob_write(blob, data, write_size, fuzz_int32_t_6);
        
        // Close blob
        sqlite3_blob_close(blob);
    }
    
    // Configure virtual table
    sqlite3_vtab_config(db, fuzz_int32_t_7);
    
    // Get client data
    void* client_data = sqlite3_get_clientdata(db, fuzz_str_8);
    
    // Create custom function
    rc = sqlite3_create_function(db, fuzz_str_9, fuzz_int32_t_10, fuzz_int32_t_11, nullptr, 
                                  id_000047_custom_func, nullptr, nullptr);
    
    // Create virtual table module
    rc = sqlite3_create_module(db, fuzz_str_12, & id_000047_vtab_module, nullptr);
    
    // Create another function for pointer test
    rc = sqlite3_create_function(db, fuzz_str_13, fuzz_int32_t_14, fuzz_int32_t_15, nullptr,
                                  id_000047_pointer_func, nullptr, nullptr);
    
    // Prepare and execute statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(db, fuzz_str_16, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        
        // Bind pointer to statement
        int bind_rc = sqlite3_bind_pointer(stmt, fuzz_int32_t_18, &db, fuzz_str_17, nullptr);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Get database filename
    sqlite3_filename db_filename = sqlite3_db_filename(db, fuzz_str_19);
    if (db_filename) {
        const char* journal = sqlite3_filename_journal(db_filename);
        if (journal) {
            fwrite(journal, 1, strlen(journal), out_file);
        }
    }
    
    // Create backup
    sqlite3* backup_db = nullptr;
    rc = sqlite3_open(":memory:", &backup_db);
    if (rc == SQLITE_OK && backup_db) {
        sqlite3_backup* backup = sqlite3_backup_init(backup_db, fuzz_str_20, db, fuzz_str_21);
        if (backup) {
            rc = sqlite3_backup_step(backup, fuzz_int32_t_22); // Backup 5 pages
            sqlite3_backup_finish(backup);
        }
        sqlite3_close_v2(backup_db);
    }
    
    // Create and free sqlite3_value
    // Note: sqlite3_value objects are typically created by SQLite internally
    // We'll use value from a prepared statement if available
    sqlite3_stmt* val_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, fuzz_str_23, -1, &val_stmt, nullptr);
    if (rc == SQLITE_OK && val_stmt) {
        if (sqlite3_step(val_stmt) == SQLITE_ROW) {
            sqlite3_value* value = sqlite3_column_value(val_stmt, fuzz_int32_t_24);
            // Get pointer from value
            void* ptr = sqlite3_value_pointer(value, fuzz_str_25);
            // Note: sqlite3_value_free is for values created by sqlite3_value_new, not column values
        }
        sqlite3_finalize(val_stmt);
    }
    
    // Create sqlite3_str object (requires sqlite3_str_new which isn't in list)
    // We'll skip this as the required API isn't available
    
    // Close database
    sqlite3_close_v2(db);
    
    // Clean up
    free(db_content);
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}