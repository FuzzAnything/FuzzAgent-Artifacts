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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 4
//<Quality> {"density":20,"unique_branches":{"sqlite3_stmt_status":[[91543,9,91543,18,0,0,4,1]],"sqlite3_error_offset":[[180260,7,180260,9,0,0,4,0],[180260,13,180260,43,0,0,4,0],[180260,47,180260,58,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_stmt_status","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_stmt_status","sqlite3_finalize","sqlite3_error_offset","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_stmt_status","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_error_offset","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_stmt_status","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_stmt_status","sqlite3_finalize","sqlite3_error_offset","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_stmt_status","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_error_offset","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open a database file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a prepared statement
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "CREATE TABLE test (id INTEGER, name TEXT);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    // Use sqlite3_stmt_status API
    if (stmt) {
        int status_value = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 0);
        (void)status_value; // Use value to avoid unused warning
    }
    
    // Finalize the first statement
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Create another statement with bind parameters
    sqlite3_stmt *stmt2 = nullptr;
    const char *sql2 = "INSERT INTO test (id, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr);
    
    // Use sqlite3_bind_int API
    if (stmt2) {
        // Read an integer from the input file
        int bind_value = 0;
        if (fread(&bind_value, sizeof(bind_value), 1, in_file) == 1) {
            sqlite3_bind_int(stmt2, 1, bind_value);
        } else {
            sqlite3_bind_int(stmt2, 1, 42); // Default value
        }
        
        // Bind a text parameter (using part of input data)
        if (size > sizeof(int)) {
            size_t text_len = size - sizeof(int);
            const char *text_data = reinterpret_cast<const char*>(data + sizeof(int));
            sqlite3_bind_text(stmt2, 2, text_data, text_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt2, 2, "default", 7, SQLITE_STATIC);
        }
        
        // Execute the statement
        rc = sqlite3_step(stmt2);
        
        // Use sqlite3_stmt_status again
        int status2 = sqlite3_stmt_status(stmt2, SQLITE_STMTSTATUS_VM_STEP, 1);
        (void)status2;
    }
    
    // Finalize second statement
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    
    // Use sqlite3_error_offset API
    int error_offset = sqlite3_error_offset(db);
    (void)error_offset;
    
    // Use sqlite3_strnicmp API
    // Compare two strings from the input data
    if (size >= 20) {
        const char *str1 = reinterpret_cast<const char*>(data);
        const char *str2 = reinterpret_cast<const char*>(data + 10);
        int cmp_result = sqlite3_strnicmp(str1, str2, 10);
        (void)cmp_result;
    }
    
    // Use sqlite3_memory_highwater API
    sqlite3_int64 highwater = sqlite3_memory_highwater(1); // Reset the high-water mark
    (void)highwater;
    
    // Create a third statement to test more
    sqlite3_stmt *stmt3 = nullptr;
    const char *sql3 = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, sql3, -1, &stmt3, nullptr);
    
    if (stmt3) {
        // Execute and step through results
        while ((rc = sqlite3_step(stmt3)) == SQLITE_ROW) {
            // Read some data
            int col_count = sqlite3_column_count(stmt3);
            (void)col_count;
        }
        
        // Use sqlite3_stmt_status with different op
        int status3 = sqlite3_stmt_status(stmt3, SQLITE_STMTSTATUS_AUTOINDEX, 0);
        (void)status3;
        
        sqlite3_finalize(stmt3);
    }
    
    // Try to create another error condition to test error_offset
    sqlite3_stmt *bad_stmt = nullptr;
    const char *bad_sql = "INVALID SQL SYNTAX;";
    rc = sqlite3_prepare_v2(db, bad_sql, -1, &bad_stmt, nullptr);
    if (bad_stmt) {
        sqlite3_finalize(bad_stmt);
    }
    
    // Get error offset again
    error_offset = sqlite3_error_offset(db);
    (void)error_offset;
    
    // Clean up
    assert_file_closed(&in_file);;
    if (db) {
        sqlite3_close_v2(db);
    }
    
    assert_file_closed(&in_file);
	return 0;
}