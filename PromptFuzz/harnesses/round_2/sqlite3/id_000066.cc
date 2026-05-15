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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 6
//<Quality> {"density":33,"unique_branches":{"sqlite3VdbeMemFromBtree":[[83215,7,83215,49,0,0,4,0]],"sqlite3_extended_errcode":[[180325,7,180325,9,0,0,4,0],[180325,13,180325,44,0,0,4,1],[180328,7,180328,10,0,0,4,1],[180328,14,180328,30,0,0,4,1]],"sqlite3ValueBytes":[[83959,34,83959,50,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_extended_errcode","sqlite3_system_errno","sqlite3_value_text16","sqlite3_value_bytes","sqlite3_result_text16","sqlite3_result_blob64","sqlite3_value_text16","sqlite3_value_bytes","sqlite3_result_text16","sqlite3_result_blob64","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_free","sqlite3_finalize","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_extended_errcode","sqlite3_system_errno","sqlite3_result_blob64","sqlite3_result_text16","sqlite3_value_bytes","sqlite3_value_text16","sqlite3_result_blob64","sqlite3_result_text16","sqlite3_value_bytes","sqlite3_value_text16","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_free","sqlite3_finalize","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Create a temporary database file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Write input data to file (simulating SQLite database file)
    fwrite(data, 1, size, out_file);
    assert_file_closed(&out_file);;

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    char *errmsg = nullptr;
    int rc;

    // Open database from the file we just wrote
    rc = sqlite3_open("output_file", &db);
    if (rc != SQLITE_OK || !db) {
        sqlite3_shutdown();
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
    }

    // Use extended error code
    int extended_rc = sqlite3_extended_errcode(db);
    (void)extended_rc;

    // Use system errno
    int sys_errno = sqlite3_system_errno(db);
    (void)sys_errno;

    // Custom function implementation
    auto custom_function = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        if (argc == 1) {
            const void *text = sqlite3_value_text16(argv[0]);
            int len = sqlite3_value_bytes(argv[0]);
            sqlite3_result_text16(ctx, text, len, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_blob64(ctx, nullptr, 0, SQLITE_TRANSIENT);
        }
    };

    // Register custom function
    rc = sqlite3_create_function16(db, u"custom_func", 1, SQLITE_UTF16, 
                                   nullptr, custom_function, nullptr, nullptr);

    // Execute SQL using sqlite3_exec
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare a statement
    const char *insert_sql = 
        "INSERT INTO test_table (id, name, data) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (stmt) {
        // Bind parameters
        int id = data[0] % 100;
        rc = sqlite3_bind_int(stmt, 1, id);
        
        const char *name = "test_name";
        rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
        
        // Use input data as blob
        rc = sqlite3_bind_blob(stmt, 3, data, std::min(size, (size_t)1000), SQLITE_TRANSIENT);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Prepare and execute a SELECT statement
    const char *select_sql = "SELECT id, name, data FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (stmt) {
        int col_count = sqlite3_column_count(stmt);
        
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                
                switch (col_type) {
                    case SQLITE_INTEGER:
                        break;
                    case SQLITE_TEXT:
                        break;
                    case SQLITE_BLOB: {
                        const void *blob = sqlite3_column_blob(stmt, i);
                        int blob_size = sqlite3_column_bytes(stmt, i);
                        (void)blob;
                        (void)blob_size;
                        break;
                    }
                }
            }
        }
        sqlite3_finalize(stmt);
    }

    // Use sqlite3_str for string building
    sqlite3_str *str_builder = sqlite3_str_new(db);
    if (str_builder) {
        sqlite3_str_append(str_builder, "Test string ", 12);
        sqlite3_str_append(str_builder, "appended", 8);
        char *built_str = sqlite3_str_finish(str_builder);
        if (built_str) {
            sqlite3_free(built_str);
        }
    }

    // Work with sqlite3_value
    const char *select_value_sql = "SELECT custom_func(name) FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_value_sql, -1, &stmt, nullptr);
    if (stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            sqlite3_value *value = sqlite3_column_value(stmt, 0);
            sqlite3_value *dup_value = sqlite3_value_dup(value);
            if (dup_value) {
                sqlite3_value_free(dup_value);
            }
        }
        sqlite3_finalize(stmt);
    }

    // Use sqlite3_filename_database
    const char *filename = "output_file";
    const char *db_name = sqlite3_filename_database((sqlite3_filename)filename);
    (void)db_name;

    // Use string comparison
    int cmp_result = sqlite3_stricmp("test1", "test2");
    (void)cmp_result;

    // Get total changes
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    (void)total_changes;

    // Get memory usage
    sqlite3_int64 memory_used = sqlite3_memory_used();
    (void)memory_used;

    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    // Remove temporary file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}