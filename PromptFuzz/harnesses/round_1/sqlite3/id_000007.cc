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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 162.5, nr_unique_branch: 38
//<Quality> {"density":25,"unique_branches":{"pager_write":[[62500,11,62500,24,0,0,4,0]],"blobSeekToRow":[[101378,9,101378,16,0,0,4,0],[101380,11,101380,18,0,0,4,0],[101395,13,101395,21,0,0,4,1]],"balance_quick":[[77641,7,77641,20,0,0,4,1],[77698,12,77698,43,0,0,4,0],[77698,47,77698,58,0,0,4,0]],"memjrnlWrite":[[105262,15,105262,20,0,0,4,0]],"sqlite3_stmt_scanstatus_v2":[[91842,7,91842,11,0,0,4,1],[91842,15,91842,22,0,0,4,1],[91843,10,91843,45,0,0,4,1],[91844,10,91844,46,0,0,4,1],[91850,7,91850,16,0,0,4,1],[91857,7,91857,14,0,0,4,1],[91869,7,91869,38,0,0,4,1],[91875,16,91875,28,0,0,4,0],[91877,11,91877,23,0,0,4,0],[91879,13,91879,20,0,0,4,0],[91883,7,91883,20,0,0,4,1],[91886,5,91886,31,0,0,4,0],[91887,11,91887,28,0,0,4,0],[91894,5,91894,32,0,0,4,1],[91902,5,91902,29,0,0,4,1],[91912,5,91912,30,0,0,4,1],[91916,5,91916,33,0,0,4,1],[91924,5,91924,34,0,0,4,1],[91932,5,91932,34,0,0,4,1],[91940,5,91940,32,0,0,4,1],[91971,5,91971,12,0,0,4,1]],"readJournalHdr":[[58041,16,58041,43,0,0,4,0],[58046,9,58046,57,0,0,4,0]],"sqlite3IoerrnomemError":[[14098,27,14098,28,1,0,4,1]],"balance":[[78858,11,78858,24,0,0,4,1]],"pagerAddPageToRollbackJournal":[[62424,7,62424,20,0,0,4,0]],"sqlite3VdbeExec":[[101215,24,101215,46,0,0,4,1],[101224,7,101224,29,0,0,4,0]],"sqlite3_declare_vtab":[[154468,7,154468,12,0,0,4,0]],"sqlite3SystemError":[[34591,7,34591,29,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_wal_hook","sqlite3_exec","sqlite3_declare_vtab","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_hard_heap_limit64","sqlite3_release_memory","sqlite3_prepare_v2","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_reset","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_wal_hook","sqlite3_exec","sqlite3_declare_vtab","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_hard_heap_limit64","sqlite3_release_memory","sqlite3_prepare_v2","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_reset","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_blob *blob = nullptr;
    int rc = 0;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use sqlite3_wal_hook
    auto wal_hook_callback = [](void *user_data, sqlite3 *db, const char *db_name, int pages) -> int {
        // Simple callback that does nothing
        return SQLITE_OK;
    };
    sqlite3_wal_hook(db, wal_hook_callback, nullptr);

    // 2. Create a table and declare virtual table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "value REAL);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Use sqlite3_declare_vtab (typically used in virtual table implementations)
    // We'll create a simple virtual table declaration
    const char *vtab_sql = "CREATE TABLE x(a, b);";
    sqlite3_declare_vtab(db, vtab_sql);

    // 3. Prepare a statement and bind null
    const char *insert_sql = "INSERT INTO test_table (data, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind null to first parameter
        sqlite3_bind_null(stmt, 1);
        
        // Bind a value to second parameter
        sqlite3_bind_double(stmt, 2, 3.14);
        
        // Execute and reset
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    // 4. Create a blob
    rc = sqlite3_blob_open(db, "main", "test_table", "data", 
                          sqlite3_last_insert_rowid(db), 0, &blob);
    if (rc == SQLITE_OK && blob) {
        // Use sqlite3_blob_bytes
        int blob_size = sqlite3_blob_bytes(blob);
        
        // Read blob data into buffer
        char *buffer = new char[blob_size + 1];
        if (buffer) {
            sqlite3_blob_read(blob, buffer, blob_size, 0);
            buffer[blob_size] = '\0';
            
            // Write blob data to output file
            fwrite(buffer, 1, blob_size, out_file);
            delete[] buffer;
        }
        
        sqlite3_blob_close(blob);
    }

    // 5. Set hard heap limit
    sqlite3_int64 heap_limit = 1024 * 1024; // 1MB limit
    sqlite3_hard_heap_limit64(heap_limit);

    // 6. Release memory
    sqlite3_release_memory(100); // Try to release 100 bytes

    // 7. Prepare another statement for scanstatus
    const char *select_sql = "SELECT * FROM test_table;";
    sqlite3_stmt *stmt2 = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
    if (rc == SQLITE_OK && stmt2) {
        // Execute once to populate scan status
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            // Just iterate through results
        }
        
        // Use sqlite3_stmt_scanstatus_v2
        int scan_status_op = 0; // SQLITE_SCANSTAT_NLOOP
        void *pOut = nullptr;
        sqlite3_stmt_scanstatus_v2(stmt2, 0, scan_status_op, 0, &pOut);
        
        sqlite3_finalize(stmt2);
    }

    // 8. Get value subtype (requires a value from a query)
    if (stmt) {
        // Reuse the prepared statement
        sqlite3_reset(stmt);
        sqlite3_step(stmt);
        
        // Get value from column (if any)
        int col_count = sqlite3_column_count(stmt);
        if (col_count > 0) {
            sqlite3_value *value = sqlite3_column_value(stmt, 0);
            if (value) {
                // Use sqlite3_value_subtype
                unsigned int subtype = sqlite3_value_subtype(value);
                (void)subtype; // Use variable to avoid warning
            }
        }
    }

    // Read from input file and write to database
    fseek(in_file, 0, SEEK_SET);
    char buffer[4096];
    size_t bytes_read;
    
    // Create another table for input data
    const char *create_data_table = 
        "CREATE TABLE IF NOT EXISTS input_data ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "content BLOB);";
    sqlite3_exec(db, create_data_table, nullptr, nullptr, nullptr);
    
    // Prepare insert statement for input data
    sqlite3_stmt *insert_data_stmt = nullptr;
    const char *insert_data_sql = "INSERT INTO input_data (content) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_data_sql, -1, &insert_data_stmt, nullptr);
    
    if (rc == SQLITE_OK && insert_data_stmt) {
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            sqlite3_bind_blob(insert_data_stmt, 1, buffer, bytes_read, SQLITE_STATIC);
            sqlite3_step(insert_data_stmt);
            sqlite3_reset(insert_data_stmt);
        }
        sqlite3_finalize(insert_data_stmt);
    }

    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}