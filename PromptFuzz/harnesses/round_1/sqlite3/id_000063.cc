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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"sqlite3AffinityType":[[121847,9,121847,14,0,0,4,0]],"sqlite3ExprCheckHeight":[[108724,7,108724,23,0,0,4,0]]},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"visited":0}
/**/


// Dummy module structure since we need it for sqlite3_create_module_v2
static sqlite3_module  id_000063_dummy_module = {
    0, // iVersion
    nullptr, // xCreate
    nullptr, // xConnect
    nullptr, // xBestIndex
    nullptr, // xDisconnect
    nullptr, // xDestroy
    nullptr, // xOpen
    nullptr, // xClose
    nullptr, // xFilter
    nullptr, // xNext
    nullptr, // xEof
    nullptr, // xColumn
    nullptr, // xRowid
    nullptr, // xUpdate
    nullptr, // xBegin
    nullptr, // xSync
    nullptr, // xCommit
    nullptr, // xRollback
    nullptr, // xFindFunction
    nullptr, // xRename
    nullptr, // xSavepoint
    nullptr, // xRelease
    nullptr  // xRollbackTo
};

// Profile callback function
static void  id_000063_profile_callback(void *user_data, const char *sql, sqlite3_uint64 elapsed) {
    // Empty callback for fuzzing
    (void)user_data;
    (void)sql;
    (void)elapsed;
}

// Custom function implementation
static void  id_000063_custom_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Return a static value
    sqlite3_result_text(ctx, "custom_function_result", -1, SQLITE_STATIC);
}

// Step function for aggregate
static void  id_000063_step_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Empty step function
    (void)ctx;
    (void)argc;
    (void)argv;
}

// Final function for aggregate
static void  id_000063_final_function(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 42);
}

// Callback for sqlite3_exec
static int  id_000063_exec_callback(void *data, int argc, char **argv, char **col_names) {
    // Just count the rows
    int *count = static_cast<int*>(data);
    (*count)++;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 1) {
        return 0;
    }

    // Create output file as per instruction 4
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // Create input file using fmemopen as per instruction 4
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Get file descriptor as per instruction 5
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    // Call sqlite3_libversion_number()
    int version = sqlite3_libversion_number();
    (void)version; // Mark as used

    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set profile callback
    sqlite3_profile(db,  id_000063_profile_callback, nullptr);

    // Create a module
    rc = sqlite3_create_module_v2(db, " id_000063_dummy_module", & id_000063_dummy_module, nullptr, nullptr);
    (void)rc; // Mark as used

    // Create a custom function
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, nullptr, 
                                  id_000063_custom_function,  id_000063_step_function,  id_000063_final_function);

    // Prepare and use the input data as SQL commands
    // Copy data to a null-terminated string
    std::vector<char> sql_data(data, data + size);
    if (sql_data.empty()) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    sql_data.push_back('\0');

    // Execute the input as SQL (first sqlite3_exec call)
    char *errmsg1 = nullptr;
    int row_count1 = 0;
    rc = sqlite3_exec(db, sql_data.data(),  id_000063_exec_callback, &row_count1, &errmsg1);
    if (errmsg1) {
        sqlite3_free(errmsg1);
    }

    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    rc = sqlite3_prepare(db, "SELECT ?, ?, ?", -1, &stmt, &tail);
    
    if (stmt) {
        // Bind blob data to the prepared statement
        if (size > 0) {
            // Use first part of data for blob binding
            size_t bind_size = size > 100 ? 100 : size;
            rc = sqlite3_bind_blob(stmt, 1, data, (int)bind_size, SQLITE_STATIC);
            
            // Bind null and zero blob
            sqlite3_bind_null(stmt, 2);
            sqlite3_bind_zeroblob(stmt, 3, 10);
        }

        // Step through the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get column count
            int col_count = sqlite3_column_count(stmt);
            
            // Get column text if available
            for (int i = 0; i < col_count; i++) {
                const unsigned char *text = sqlite3_column_text(stmt, i);
                (void)text; // Mark as used
                
                // Get column value
                sqlite3_value *val = sqlite3_column_value(stmt, i);
                (void)val; // Mark as used
            }
        }

        // Reset scan status
        sqlite3_stmt_scanstatus_reset(stmt);

        // Finalize the statement (first finalize)
        sqlite3_finalize(stmt);
    }

    // Second sqlite3_exec call with a different SQL
    char *errmsg2 = nullptr;
    int row_count2 = 0;
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);", 
                       id_000063_exec_callback, &row_count2, &errmsg2);
    if (errmsg2) {
        sqlite3_free(errmsg2);
    }

    // Create another statement and finalize it (second finalize)
    sqlite3_stmt *stmt2 = nullptr;
    rc = sqlite3_prepare(db, "SELECT 1", -1, &stmt2, &tail);
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }

    // Drop modules
    rc = sqlite3_drop_modules(db, nullptr);

    // Close the database
    rc = sqlite3_close(db);

    // Write something to output file to demonstrate file usage
    if (size > 0) {
        fwrite(data, 1, size > 1000 ? 1000 : size, out_file);
    }

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}