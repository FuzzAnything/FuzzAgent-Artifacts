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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 11
//<Quality> {"density":22,"unique_branches":{"sqlite3_snapshot_recover":[[182572,7,182572,32,0,0,4,1],[182579,7,182579,13,0,0,4,0],[182581,9,182581,51,0,0,4,0],[182583,11,182583,24,0,0,4,0]],"sqlite3VdbeMemTranslate":[[34054,12,34054,34,9,0,4,0]],"sqlite3PagerSnapshotRecover":[[64171,7,64171,19,0,0,4,1]],"sqlite3VdbeExec":[[95632,44,95632,67,0,0,4,0],[95740,13,95740,38,0,0,4,0],[14098,27,14098,28,383,0,4,1],[14098,27,14098,28,384,0,4,0]],"sqlite3_set_last_insert_rowid":[[178699,7,178699,32,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_autovacuum_pages","sqlite3_create_collation","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_sleep","sqlite3_snapshot_recover","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_value_text16","sqlite3_value_bytes16","sqlite3_result_int","sqlite3_result_null","sqlite3_value_text16","sqlite3_value_bytes16","sqlite3_result_int","sqlite3_result_null","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_text16","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_autovacuum_pages","sqlite3_create_collation","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_sleep","sqlite3_snapshot_recover","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_result_null","sqlite3_result_int","sqlite3_value_bytes16","sqlite3_value_text16","sqlite3_result_null","sqlite3_result_int","sqlite3_value_bytes16","sqlite3_value_text16","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_text16","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Autovacuum callback function
static unsigned int  id_000054_autovacuum_callback(void *pArg, const char *zSchema,
                                        unsigned int nPage, unsigned int nFree,
                                        unsigned int iDummy) {
    // Simple callback that returns a fixed value
    (void)pArg; (void)zSchema; (void)nPage; (void)nFree; (void)iDummy;
    return 0;
}

// Collation callback function
static int  id_000054_collation_callback(void *pArg, int n1, const void *p1,
                              int n2, const void *p2) {
    // Simple binary collation
    (void)pArg;
    int result = memcmp(p1, p2, n1 < n2 ? n1 : n2);
    if (result == 0) {
        result = n1 - n2;
    }
    return result;
}

// Cleanup callback for collation
static void  id_000054_cleanup_callback(void *pArg) {
    // Nothing to cleanup
    (void)pArg;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    FILE *in_file = nullptr;
    int rc = 0;
    
    // Create a temporary file with the input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open an in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        if (in_file) assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set up autovacuum pages callback
    rc = sqlite3_autovacuum_pages(db,  id_000054_autovacuum_callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Continue anyway
    }
    
    // 2. Create a collation using the input data
    // Use first 4 bytes as collation name (ensure null termination)
    char collation_name[5] = {0};
    size_t name_len = size < 4 ? size : 4;
    memcpy(collation_name, data, name_len);
    collation_name[4] = '\0';
    
    // Note: sqlite3_create_collation16 is not in the provided list, using create_collation instead
    rc = sqlite3_create_collation(db, collation_name, SQLITE_UTF8, 
                                  nullptr,  id_000054_collation_callback);
    if (rc != SQLITE_OK) {
        // Continue anyway
    }
    
    // 3. Create a table and insert some data
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, value REAL, text TEXT);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Continue anyway
    }
    
    // Insert some data using the input
    const char *insert_sql = "INSERT INTO test (value, text) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind a double value from the input
        double input_double = 0.0;
        if (size >= sizeof(double)) {
            memcpy(&input_double, data, sizeof(double));
        } else {
            input_double = static_cast<double>(size);
        }
        sqlite3_bind_double(stmt, 1, input_double);
        
        // Bind text from input (skip first 8 bytes for safety)
        const char *text_data = reinterpret_cast<const char*>(data);
        int text_len = static_cast<int>(size > 8 ? size - 8 : 0);
        sqlite3_bind_text(stmt, 2, text_data + 8, text_len, SQLITE_STATIC);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 4. Set last insert rowid from input data
    sqlite3_int64 rowid = 0;
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&rowid, data, sizeof(sqlite3_int64));
    } else {
        rowid = static_cast<sqlite3_int64>(size);
    }
    sqlite3_set_last_insert_rowid(db, rowid);
    
    // 5. Sleep for a short time (use input to determine duration)
    int sleep_ms = 1; // Minimum sleep
    if (size > 0) {
        sleep_ms = data[0] % 10 + 1; // Sleep 1-10 ms
    }
    sqlite3_sleep(sleep_ms);
    
    // 6. Attempt snapshot recovery (on main database)
    // This may fail since we're using in-memory database
    rc = sqlite3_snapshot_recover(db, "main");
    // Ignore result - it's expected to fail for in-memory db
    
    // 7. Query the data and use sqlite3_column_double
    const char *select_sql = "SELECT value FROM test WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_bind_int64(stmt, 1, rowid);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Use sqlite3_column_double
            double column_value = sqlite3_column_double(stmt, 0);
            (void)column_value; // Use the value to avoid compiler warning
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 8. Also demonstrate sqlite3_value_text16 by creating a function
    // Create a scalar function that uses sqlite3_value_text16
    struct FuncContext {
        const uint8_t *data;
        size_t size;
    };
    
    static auto scalar_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        if (argc > 0) {
            const void *text = sqlite3_value_text16(argv[0]);
            if (text) {
                // Return the length of the text
                int len = sqlite3_value_bytes16(argv[0]);
                sqlite3_result_int(ctx, len);
                return;
            }
        }
        sqlite3_result_null(ctx);
    };
    
    // Register the function
    sqlite3_create_function(db, "myfunc", 1, SQLITE_UTF16, nullptr, 
                           scalar_func, nullptr, nullptr);
    
    // Test the function
    const char *func_test_sql = "SELECT myfunc(?)";
    rc = sqlite3_prepare_v2(db, func_test_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind some text from input
        const void *input_text = reinterpret_cast<const void*>(data);
        int bind_size = static_cast<int>(size > 100 ? 100 : size);
        sqlite3_bind_text16(stmt, 1, input_text, bind_size, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Function executed
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Cleanup
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);
	return 0;
}