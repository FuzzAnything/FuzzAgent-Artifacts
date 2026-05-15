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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 94.5, nr_unique_branch: 15
//<Quality> {"density":27,"unique_branches":{"sqlite3Prepare":[[140981,7,140981,16,0,0,4,0],[140981,21,140981,30,0,0,4,1],[140981,34,140981,51,0,0,4,0],[140981,34,140981,51,0,0,4,1],[140986,9,140986,21,0,0,4,1],[140992,9,140992,17,0,0,4,0],[14098,27,14098,28,11,0,4,1],[14098,27,14098,28,12,0,4,1]],"cellSizePtrTableLeaf":[[71277,7,71277,22,0,0,4,1],[71278,7,71278,22,0,0,4,1],[71279,7,71279,22,0,0,4,1]],"sqlite3VdbeMemSetDouble":[[82921,7,82921,25,0,0,4,1]],"sqlite3VdbeExec":[[95740,13,95740,38,0,0,4,0]],"sqlite3VdbeRealValue":[[82613,13,82613,45,0,0,4,1]],"sqlite3GetVarint":[[35677,7,35677,16,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_stmt_status","sqlite3_prepare_v2","sqlite3_error_offset","sqlite3_finalize","sqlite3_reset","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_stmt_status","sqlite3_prepare_v2","sqlite3_error_offset","sqlite3_finalize","sqlite3_reset","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_close"],"visited":1}
/*To create a comprehensive fuzz driver that uses the specified SQLite3 APIs, I'll design a scenario that involves preparing SQL statements, binding parameters, executing queries, and handling potential errors. Here's the implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 20) {
	return 0;
	}
	

    // Create in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        return 0;
    }

    // Create a table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

    // Prepare a statement for binding
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test_table (id, name, value) VALUES (?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Extract integer from data for binding
        int bind_value = 0;
        if (size >= sizeof(int)) {
            memcpy(&bind_value, data, sizeof(int));
            bind_value = abs(bind_value); // Ensure positive for primary key
        }
        
        // Use sqlite3_bind_int (Note: This is actually in the standard SQLite API)
        sqlite3_bind_int(stmt, 1, bind_value);
        
        // Bind text from data (use middle portion)
        const char *text_data = reinterpret_cast<const char*>(data + sizeof(int));
        size_t text_len = size - sizeof(int) > 100 ? 100 : size - sizeof(int);
        sqlite3_bind_text(stmt, 2, text_data, text_len, SQLITE_TRANSIENT);
        
        // Bind a double value (use a portion of data)
        double double_value = 0.0;
        if (size >= sizeof(int) + sizeof(double)) {
            memcpy(&double_value, data + sizeof(int), sizeof(double));
        }
        sqlite3_bind_double(stmt, 3, double_value);
        
        // Execute the statement
        sqlite3_step(stmt);
        
        // Use sqlite3_stmt_status to get statement statistics
        // SQLITE_STMTSTATUS_FULLSCAN_STEP is 1, SQLITE_STMTSTATUS_SORT is 2
        int fullscan_steps = sqlite3_stmt_status(stmt, 1, 1); // Reset after get
        int sort_ops = sqlite3_stmt_status(stmt, 2, 1);
        
        // Prepare another statement for error testing
        sqlite3_stmt *error_stmt = nullptr;
        // Create invalid SQL using input data to potentially trigger error
        const char *error_sql = reinterpret_cast<const char*>(data);
        size_t sql_len = size > 50 ? 50 : size;
        
        if (sqlite3_prepare_v2(db, error_sql, sql_len, &error_stmt, nullptr) != SQLITE_OK) {
            // Use sqlite3_error_offset to get error position
            int error_offset = sqlite3_error_offset(db);
            // We can use this offset for debugging if needed
            (void)error_offset; // Prevent unused variable warning
        }
        if (error_stmt) sqlite3_finalize(error_stmt);
        
        // Reset statement for reuse
        sqlite3_reset(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Use sqlite3_strnicmp for string comparison
    // Compare first 10 bytes with next 10 bytes (if available)
    if (size >= 20) {
        const char *str1 = reinterpret_cast<const char*>(data);
        const char *str2 = reinterpret_cast<const char*>(data + 10);
        int comparison = sqlite3_strnicmp(str1, str2, 10);
        (void)comparison; // Prevent unused variable warning
    }

    // Use sqlite3_memory_highwater to check memory usage
    sqlite3_int64 highwater = sqlite3_memory_highwater(1); // Reset after get
    
    // Create and test a file-based database using FILE* interface
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Read from input file and write to output file
            uint8_t buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                fwrite(buffer, 1, bytes_read, out_file);
            }
            
            // Get file descriptors
            int in_fd = fuzz_fileno(in_file);
            int out_fd = fuzz_fileno(out_file);
            (void)in_fd; // Prevent unused variable warnings
            (void)out_fd;
            
            assert_file_closed(&out_file);;
            remove("output_file"); // Clean up output file
        }
        assert_file_closed(&in_file);;
    }

    // Create another database on disk for additional testing
    sqlite3 *file_db = nullptr;
    if (sqlite3_open_v2("test.db", &file_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) == SQLITE_OK) {
        // Prepare and execute a simple query
        sqlite3_stmt *file_stmt = nullptr;
        const char *select_sql = "SELECT 1;";
        
        if (sqlite3_prepare_v2(file_db, select_sql, -1, &file_stmt, nullptr) == SQLITE_OK) {
            sqlite3_step(file_stmt);
            sqlite3_finalize(file_stmt);
        }
        
        // Test backup functionality
        sqlite3_backup *backup = sqlite3_backup_init(file_db, "main", db, "main");
        if (backup) {
            sqlite3_backup_step(backup, 1);
            sqlite3_backup_finish(backup);
        }
        
        sqlite3_close(file_db);
        remove("test.db"); // Clean up test database file
    }

    // Additional API usage to meet requirements
    // Use sqlite3_column_double (from provided APIs)
    sqlite3_stmt *query_stmt = nullptr;
    const char *query_sql = "SELECT value FROM test_table WHERE id = 1;";
    if (sqlite3_prepare_v2(db, query_sql, -1, &query_stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(query_stmt) == SQLITE_ROW) {
            double col_value = sqlite3_column_double(query_stmt, 0);
            (void)col_value;
        }
        sqlite3_finalize(query_stmt);
    }

    // Clean up
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}