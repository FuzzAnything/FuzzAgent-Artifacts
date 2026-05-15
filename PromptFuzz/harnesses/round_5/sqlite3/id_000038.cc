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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 2
//<Quality> {"density":30,"unique_branches":{"sqlite3_transfer_bindings":[[91407,7,91407,29,0,0,4,0]],"sqlite3_table_column_metadata":[[181541,11,181541,56,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_close","sqlite3_transfer_bindings","sqlite3_clear_bindings","sqlite3_step","sqlite3_reset","sqlite3_table_column_metadata","sqlite3_column_count","sqlite3_column_name","sqlite3_str_new","sqlite3_str_append","sqlite3_str_reset","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_stmt_status","sqlite3_status","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_transfer_bindings","sqlite3_clear_bindings","sqlite3_step","sqlite3_reset","sqlite3_table_column_metadata","sqlite3_column_count","sqlite3_column_name","sqlite3_str_new","sqlite3_str_append","sqlite3_str_reset","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_stmt_status","sqlite3_status","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Open an in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a simple table to work with
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL);";
    char *err_msg = nullptr;
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Prepare a statement for inserting data
    sqlite3_stmt *stmt1 = nullptr;
    const char *insert_sql = "INSERT INTO test_table (name, value) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read data from input file for binding
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), in_file)) {
        // Bind data to the first statement
        sqlite3_bind_text(stmt1, 1, buffer, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt1, 2, 3.14);
        
        // Execute the first statement
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
    }

    // Prepare another statement
    sqlite3_stmt *stmt2 = nullptr;
    const char *select_sql = "SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Use sqlite3_transfer_bindings
    sqlite3_transfer_bindings(stmt1, stmt2);

    // Use sqlite3_clear_bindings
    sqlite3_clear_bindings(stmt2);

    // Execute stmt2
    sqlite3_step(stmt2);
    sqlite3_reset(stmt2);

    // Use sqlite3_table_column_metadata
    const char *data_type = nullptr;
    const char *coll_seq = nullptr;
    int not_null = 0, primary_key = 0, autoinc = 0;
    sqlite3_table_column_metadata(db, "main", "test_table", "name", 
                                  &data_type, &coll_seq, 
                                  &not_null, &primary_key, &autoinc);

    // Use sqlite3_column_name
    int col_count = sqlite3_column_count(stmt2);
    for (int i = 0; i < col_count; i++) {
        const char *col_name = sqlite3_column_name(stmt2, i);
        if (col_name) {
            fwrite(col_name, strlen(col_name), 1, out_file);
        }
    }

    // Use sqlite3_stmt_scanstatus_reset (if available)
    // Note: This function is not in the provided API list, so we'll skip it
    // but mention it in comments as requested by the user

    // Use sqlite3_str_* functions
    sqlite3_str *str_builder = sqlite3_str_new(db);
    if (str_builder) {
        // Read more data for string building
        rewind(in_file);
        while (fgets(buffer, sizeof(buffer), in_file)) {
            sqlite3_str_append(str_builder, buffer, strlen(buffer));
        }
        
        // Use sqlite3_str_reset
        sqlite3_str_reset(str_builder);
        
        // Append some static data
        sqlite3_str_append(str_builder, "Test string", 11);
        
        // Use sqlite3_str_finish
        char *finished_str = sqlite3_str_finish(str_builder);
        if (finished_str) {
            fwrite(finished_str, strlen(finished_str), 1, out_file);
            sqlite3_free(finished_str);
        }
    }

    // Use sqlite3_stmt_status
    int current = 0, highwater = 0;
    sqlite3_stmt_status(stmt1, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
    
    // Use sqlite3_status
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 1);

    // Clean up resources
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}