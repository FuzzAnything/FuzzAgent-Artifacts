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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_stmt_status","sqlite3_finalize","sqlite3_exec","sqlite3_error_offset","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_memory_highwater","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_stmt_status","sqlite3_finalize","sqlite3_exec","sqlite3_error_offset","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_memory_highwater","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_close_v2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create database from input file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data from input file into database
    char buffer[4096];
    size_t bytes_read;
    sqlite3_stmt *stmt = nullptr;
    
    // Create a table
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT);";
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Prepare INSERT statement
    const char *insert_sql = "INSERT INTO test VALUES(?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Use sqlite3_bind_int
        int int_value = size % 1000;  // Derive from input size
        sqlite3_bind_int(stmt, 1, int_value);
        
        // Read string from input for binding
        fseek(in_file, 0, SEEK_SET);
        bytes_read = fread(buffer, 1, sizeof(buffer) - 1, in_file);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            // Note: sqlite3_bind_text is in provided APIs, using it instead
            sqlite3_bind_text(stmt, 2, buffer, -1, SQLITE_TRANSIENT);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Prepare SELECT statement
    const char *select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Execute and get statement status
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Use sqlite3_stmt_status - get current status
            int status_value = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 0);
            (void)status_value; // Use to avoid unused warning
        }
        
        // Reset statement for status with reset
        sqlite3_reset(stmt);
        int status_reset = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        (void)status_reset; // Use to avoid unused warning
        
        sqlite3_finalize(stmt);
    }

    // Use sqlite3_error_offset when there's an error
    // Force an error by executing invalid SQL
    const char *error_sql = "INVALID SQL STATEMENT;";
    rc = sqlite3_exec(db, error_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Get error offset if available
        int error_offset = sqlite3_error_offset(db);
        (void)error_offset; // Use to avoid unused warning
    }

    // Use sqlite3_strnicmp for string comparison
    // Compare parts of input data
    if (size >= 10) {
        const char *str1 = reinterpret_cast<const char*>(data);
        const char *str2 = reinterpret_cast<const char*>(data) + (size / 2);
        int compare_len = size / 4;
        if (compare_len > 100) compare_len = 100;
        
        int strcmp_result = sqlite3_strnicmp(str1, str2, compare_len);
        (void)strcmp_result; // Use to avoid unused warning
    }

    // Use sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(0); // Get without reset
    (void)highwater; // Use to avoid unused warning
    
    // Get with reset
    sqlite3_int64 highwater_reset = sqlite3_memory_highwater(1);
    (void)highwater_reset; // Use to avoid unused warning

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write database info to output
        sqlite3_stmt *backup_stmt = nullptr;
        const char *count_sql = "SELECT COUNT(*) FROM test;";
        if (sqlite3_prepare_v2(db, count_sql, -1, &backup_stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(backup_stmt) == SQLITE_ROW) {
                int count = sqlite3_column_int(backup_stmt, 0);
                fprintf(out_file, "Records in database: %d\n", count);
            }
            sqlite3_finalize(backup_stmt);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    assert_file_closed(&in_file);;
    sqlite3_close_v2(db);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}