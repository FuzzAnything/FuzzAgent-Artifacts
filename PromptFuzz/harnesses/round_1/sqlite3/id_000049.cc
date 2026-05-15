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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 162, nr_unique_branch: 11
//<Quality> {"density":18,"unique_branches":{"jsonAppendString":[[203027,15,203027,21,0,0,4,0]],"jsonAppendValue":[[203216,5,203216,21,0,0,4,0],[203219,11,203219,54,0,0,4,1]],"jsonPrintf":[[202964,7,202964,34,0,0,4,1]],"sqlite3BtreeOpen":[[72289,27,72289,61,0,0,4,0]],"sqlite3PagerOpen":[[61176,22,61176,34,0,0,4,1]],"sqlite3VdbeExec":[[94096,13,94096,19,0,0,4,0]],"sqlite3_db_handle":[[91429,10,91429,15,0,0,4,0]],"sqlite3LoadExtension":[[136260,7,136260,15,0,0,4,0],[136269,7,136269,44,0,0,4,0],[136270,9,136270,17,0,0,4,0]]},"library_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_load_extension","sqlite3_free","sqlite3_bind_parameter_count","sqlite3_bind_text","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_db_handle","sqlite3_finalize","sqlite3_interrupt","sqlite3_total_changes","sqlite3_close","sqlite3_free_filename"],"critical_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_load_extension","sqlite3_free","sqlite3_bind_parameter_count","sqlite3_bind_text","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_db_handle","sqlite3_finalize","sqlite3_interrupt","sqlite3_total_changes","sqlite3_close","sqlite3_free_filename"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create input and output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create database filename
    const char* zDatabase = ":memory:";
    const char* zJournal = "";
    const char* zWal = "";
    const char* azParam[] = {nullptr};
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, 0, azParam);
    if (!filename) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Open database
    sqlite3* db = nullptr;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    int rc = sqlite3_open_v2(filename, &db, flags, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_free_filename(filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Set rollback hook
    void* hook_data = (void*)0x1234;
    sqlite3_rollback_hook(db, [](void* data) {
        // Simple callback that does nothing
    }, hook_data);

    // Step 4: Read SQL from input file and prepare statement
    fseek(in_file, 0, SEEK_END);
    long sql_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char* sql = nullptr;
    if (sql_size > 0) {
        sql = new char[sql_size + 1];
        fread(sql, 1, sql_size, in_file);
        sql[sql_size] = '\0';
    } else {
        sql = strdup("SELECT 1;");
    }

    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    if (sql) {
        delete[] sql;
    }

    // Step 5: Try to load extension (will likely fail but we call it anyway)
    char* err_msg = nullptr;
    sqlite3_load_extension(db, "test_extension.so", "test_func", &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    // Step 6: Bind parameters if statement was prepared successfully
    if (rc == SQLITE_OK && stmt) {
        int param_count = sqlite3_bind_parameter_count(stmt);
        if (param_count > 0) {
            // Use the input data for binding
            const char* bind_text = reinterpret_cast<const char*>(data);
            size_t text_len = size > 1024 ? 1024 : size;
            sqlite3_bind_text(stmt, 1, bind_text, text_len, SQLITE_TRANSIENT);
        }

        // Step 7: Execute statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            // Step 8: Process column types
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                // Write column type to output file
                fwrite(&col_type, sizeof(int), 1, out_file);
            }
        }

        // Step 9: Get database handle from statement
        sqlite3* db_from_stmt = sqlite3_db_handle(stmt);
        if (db_from_stmt) {
            // Verify it's the same database
            int fd = fuzz_fileno(out_file);
            fwrite(&fd, sizeof(int), 1, out_file);
        }

        // Step 10: Finalize statement
        sqlite3_finalize(stmt);
    }

    // Step 11: Interrupt the database
    sqlite3_interrupt(db);

    // Step 12: Create a string using sqlite3_str_vappendf
    // Note: We need to create a sqlite3_str object first
    // Since we don't have sqlite3_str_new in the provided APIs,
    // we'll skip this step as we cannot properly create the object
    
    // Step 13: Get total changes
    int total_changes = sqlite3_total_changes(db);
    fwrite(&total_changes, sizeof(int), 1, out_file);

    // Step 14: Close database
    sqlite3_close(db);

    // Step 15: Free filename
    sqlite3_free_filename(filename);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}