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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 135, nr_unique_branch: 22
//<Quality> {"density":15,"unique_branches":{"sqlite3_blob_bytes":[[101766,11,101766,12,0,0,4,0],[101766,16,101766,24,0,0,4,0]],"copyPayload":[[74758,7,74758,10,0,0,4,0],[74761,9,74761,22,0,0,4,1]],"sqlite3VdbeExec":[[95763,13,95763,18,0,0,4,0],[95764,15,95764,45,0,0,4,1],[97603,7,97603,47,0,0,4,0],[97611,9,97611,31,0,0,4,0],[14098,27,14098,28,567,0,4,1]],"sqlite3_blob_open":[[101501,9,101501,15,0,0,4,0],[101505,11,101505,39,0,0,4,1],[101522,30,101522,34,0,0,4,1],[101531,11,101531,17,0,0,4,1],[101599,13,101599,19,0,0,4,0]],"saveAllCursors":[[70555,7,70555,14,0,0,4,0]],"blobReadWrite":[[101705,9,101705,35,0,0,4,0],[101705,39,101705,61,0,0,4,1]],"sqlite3_value_subtype":[[89676,11,89676,38,0,0,4,1]],"sqlite3BtreePutData":[[81042,7,81042,20,0,0,4,1],[81046,7,81046,33,0,0,4,1],[81068,7,81068,43,0,0,4,1],[70664,4,70664,33,1,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_wal_hook","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_close","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_bytes","sqlite3_blob_write","sqlite3_blob_bytes","sqlite3_declare_vtab","sqlite3_release_memory","sqlite3_reset","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_blob_close","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_wal_hook","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_close","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_bytes","sqlite3_blob_write","sqlite3_blob_bytes","sqlite3_declare_vtab","sqlite3_release_memory","sqlite3_reset","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_blob_close","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input stream from fuzzing data
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

    // Initialize database (in-memory)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set WAL hook
    static int wal_hook_calls = 0;
    auto wal_hook_callback = [](void *, sqlite3 *, const char *dbname, int pages) -> int {
        wal_hook_calls++;
        return SQLITE_OK;
    };
    sqlite3_wal_hook(db, wal_hook_callback, nullptr);

    // Set hard heap limit
    sqlite3_hard_heap_limit64(1024 * 1024); // 1MB limit

    // Create a table and insert some data
    const char *create_table = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB, value REAL);"
        "INSERT INTO test (data, value) VALUES (zeroblob(100), 3.14);"
        "INSERT INTO test (data, value) VALUES (x'010203', 2.71);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Cleanup and continue
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Prepare a statement
    const char *select_sql = "SELECT * FROM test WHERE id = ?";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind NULL to parameter (id = NULL)
        sqlite3_bind_null(stmt, 1);
        
        // Execute statement (won't return rows due to NULL binding)
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if any
        }
        
        // Get scan status if available (using v2 version)
        void *scan_status_out = nullptr;
        int idx = 0;
        int iScanStatusOp = 0; // SQLITE_SCANSTAT_NLOOP
        int flags = 0;
        // Try to get scan status - may fail if not supported
        sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, &scan_status_out);
    }

    // Open blob to read data
    rc = sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    if (rc == SQLITE_OK && blob) {
        // Get blob size
        int blob_size = sqlite3_blob_bytes(blob);
        
        // Try to read data from input file into blob (if blob is writable)
        char buffer[4096];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        if (bytes_read > 0) {
            // Note: This requires a writable blob, so we need to reopen as writable
            sqlite3_blob_close(blob);
            rc = sqlite3_blob_open(db, "main", "test", "data", 1, 1, &blob);
            if (rc == SQLITE_OK) {
                // Write data to blob (limited by blob size)
                int write_size = (bytes_read < sqlite3_blob_bytes(blob)) ? 
                                 bytes_read : sqlite3_blob_bytes(blob);
                if (write_size > 0) {
                    sqlite3_blob_write(blob, buffer, write_size, 0);
                }
            }
        }
        
        // Get blob size again
        if (blob) {
            int new_blob_size = sqlite3_blob_bytes(blob);
            // Write blob size to output file
            fwrite(&new_blob_size, sizeof(new_blob_size), 1, out_file);
        }
    }

    // Create virtual table (declare vtab)
    // This is typically called from within a virtual table implementation
    // We'll create a simple schema
    const char *vtab_schema = "CREATE TABLE x(a, b, c);";
    rc = sqlite3_declare_vtab(db, vtab_schema);

    // Release memory
    sqlite3_release_memory(100); // Try to release 100 bytes

    // Try to get subtype from a value
    // We need to create a value first - using sqlite3_value from a column
    if (stmt) {
        // Reset and execute with valid parameter
        sqlite3_reset(stmt);
        sqlite3_bind_int64(stmt, 1, 1);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get value from column
            sqlite3_value *value = sqlite3_column_value(stmt, 0);
            if (value) {
                // Get subtype (returns 0 for non-subtyped values)
                unsigned int subtype = sqlite3_value_subtype(value);
                // Write subtype to output
                fwrite(&subtype, sizeof(subtype), 1, out_file);
            }
        }
    }

    // Cleanup
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}