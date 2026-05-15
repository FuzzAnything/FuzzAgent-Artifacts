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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 288, nr_unique_branch: 54
//<Quality> {"density":24,"unique_branches":{"sqlite3_vtab_config":[[154960,7,154960,32,0,0,4,1],[154964,7,154964,9,0,0,4,0],[154994,7,154994,20,0,0,4,0]],"sqlite3Prepare16":[[141211,7,141211,16,0,0,4,1],[141214,7,141214,32,0,0,4,1],[141214,34,141214,41,0,0,4,1],[141217,7,141217,16,0,0,4,1],[141225,7,141225,12,0,0,4,0],[141229,7,141229,13,0,0,4,0],[141229,17,141229,23,0,0,4,0]],"sqlite3FkRequired":[[131319,7,131319,43,0,0,4,0],[131320,9,131320,17,0,0,4,0],[131320,9,131320,17,0,0,4,1],[131324,18,131324,43,0,0,4,1],[131324,47,131324,64,0,0,4,1],[131331,32,131331,33,0,0,4,1],[131339,40,131339,41,0,0,4,1],[18323,29,18323,57,2,0,4,0]],"sqlite3Insert":[[133003,11,133003,41,0,0,4,0]],"sqlite3GenerateConstraintChecks":[[133642,7,133642,63,0,0,4,1],[133648,9,133648,37,0,0,4,1],[133655,9,133655,19,0,0,4,1]],"sqlite3ColumnColl":[[120897,7,120897,44,0,0,4,1],[120899,10,120899,12,0,0,4,0],[120899,10,120899,12,0,0,4,1],[120900,7,120900,39,0,0,4,1]],"sqlite3Utf16ByteLen":[[34406,10,34406,17,0,0,4,0],[34406,10,34406,17,0,0,4,1],[34409,9,34409,16,0,0,4,1]],"sqlite3Utf8CharLen":[[34329,7,34329,15,0,0,4,0],[34335,10,34335,15,0,0,4,0],[34335,10,34335,15,0,0,4,1],[34335,19,34335,26,0,0,4,0],[34335,19,34335,26,0,0,4,1],[20329,7,20329,23,1,0,4,0],[20329,7,20329,23,1,0,4,1],[20330,12,20330,31,1,0,4,0],[20330,12,20330,31,1,0,4,1]],"sqlite3_db_config":[[178537,7,178537,32,0,0,4,1],[178542,5,178542,36,0,0,4,1],[178549,5,178549,35,0,0,4,1],[178556,5,178556,12,0,0,4,0],[178583,16,178583,27,0,0,4,0],[178584,13,178584,30,0,0,4,0],[178588,15,178588,22,0,0,4,0],[178593,15,178593,34,0,0,4,0],[178596,15,178596,19,0,0,4,0]],"sqlite3VdbeMemHandleBom":[[34292,7,34292,16,0,0,4,1],[34295,9,34295,17,0,0,4,0],[34295,21,34295,29,0,0,4,1]],"sqlite3FkCheck":[[131074,7,131074,40,0,0,4,1],[131075,7,131075,23,0,0,4,1],[131082,32,131082,37,0,0,4,1],[131185,42,131185,47,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_mutex_alloc","sqlite3_close","sqlite3_db_config","sqlite3_mutex_free","sqlite3_close","sqlite3_create_collation","sqlite3_mutex_free","sqlite3_close","sqlite3_vtab_config","sqlite3_exec","sqlite3_mutex_free","sqlite3_close","sqlite3_exec","sqlite3_mutex_free","sqlite3_close","sqlite3_create_function","sqlite3_mutex_free","sqlite3_close","sqlite3_mutex_free","sqlite3_close","sqlite3_prepare16","sqlite3_step","sqlite3_column_count","sqlite3_column_text16","sqlite3_reset","sqlite3_stmt_scanstatus","sqlite3_finalize","sqlite3_strnicmp","sqlite3_exec","sqlite3_mutex_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_mutex_alloc","sqlite3_db_config","sqlite3_create_collation","sqlite3_vtab_config","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_prepare16","sqlite3_step","sqlite3_column_count","sqlite3_column_text16","sqlite3_reset","sqlite3_stmt_scanstatus","sqlite3_finalize","sqlite3_strnicmp","sqlite3_exec","sqlite3_mutex_free","sqlite3_close"],"visited":0}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs to achieve an event involving database operations, collation creation, and statement analysis:

*/


// Callback function for collation
static int  id_000031_collation_callback(void *pArg, int n1, const void *s1, int n2, const void *s2) {
    // Use sqlite3_strnicmp for case-insensitive comparison
    return sqlite3_strnicmp(static_cast<const char*>(s1), static_cast<const char*>(s2), 
                           (n1 < n2) ? n1 : n2);
}

// Aggregate function step callback
static void  id_000031_aggregate_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Do nothing in step - we'll count in final
}

// Aggregate function final callback
static void  id_000031_aggregate_final(sqlite3_context *context) {
    // Get aggregate count
    int count = sqlite3_aggregate_count(context);
    sqlite3_result_int(context, count);
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_mutex *mutex = nullptr;
    const void *zTail = nullptr;
    int rc;
    
    // Create in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Allocate a mutex
    mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    if (!mutex) {
        sqlite3_close(db);
        return 0;
    }
    
    // Configure database
    rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);
    if (rc != SQLITE_OK) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Create a custom collation
    rc = sqlite3_create_collation(db, "fuzz_collation", SQLITE_UTF8, 
                                  nullptr,  id_000031_collation_callback);
    if (rc != SQLITE_OK) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Configure virtual table (even though we don't create one)
    rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    // Continue even if this fails - it's expected without actual vtab
    
    // Create a simple table
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data TEXT COLLATE fuzz_collation);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data
    const char *insert_data = 
        "INSERT INTO fuzz_table (data) VALUES ('test1'), ('Test2'), ('TEST3'), ('test4');";
    rc = sqlite3_exec(db, insert_data, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Create aggregate function
    rc = sqlite3_create_function(db, "fuzz_agg", 1, SQLITE_UTF8, nullptr, 
                                 nullptr,  id_000031_aggregate_step,  id_000031_aggregate_final);
    if (rc != SQLITE_OK) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare SQL statement using input data
    // Convert input to UTF-16 for prepare16
    size_t utf16_size = size * 2 + 2;
    uint16_t *utf16_data = static_cast<uint16_t*>(malloc(utf16_size));
    if (!utf16_data) {
        sqlite3_mutex_free(mutex);
        sqlite3_close(db);
        return 0;
    }
    
    // Simple conversion: copy ASCII/UTF-8 as is (not proper UTF-8 to UTF-16 conversion)
    for (size_t i = 0; i < size && i < utf16_size/2 - 1; i++) {
        utf16_data[i] = static_cast<uint16_t>(data[i]);
    }
    utf16_data[(size < utf16_size/2 - 1) ? size : utf16_size/2 - 1] = 0;
    
    // Prepare statement with the input data as SQL
    rc = sqlite3_prepare16(db, utf16_data, -1, &stmt, &zTail);
    free(utf16_data);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Process rows if any
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // Access column data (even if we don't use it)
                const void *text = sqlite3_column_text16(stmt, i);
                (void)text; // Suppress unused warning
            }
        }
        
        // Reset statement for scanstatus
        sqlite3_reset(stmt);
        
        // Use scanstatus to get information about the statement
        // We'll try to get scan status for the first operation (idx=0)
        int64_t scan_info;
        rc = sqlite3_stmt_scanstatus(stmt, 0, SQLITE_SCANSTAT_NLOOP, &scan_info);
        (void)rc; // Suppress unused warning
        (void)scan_info; // Suppress unused warning
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Test sqlite3_strnicmp with some strings
    int cmp_result = sqlite3_strnicmp("TEST", "test", 4);
    (void)cmp_result; // Suppress unused warning
    
    // Execute a query using our aggregate function
    const char *agg_query = "SELECT fuzz_agg(data) FROM fuzz_table WHERE data LIKE 'test%';";
    rc = sqlite3_exec(db, agg_query, nullptr, nullptr, nullptr);
    (void)rc; // Suppress unused warning
    
    // Clean up mutex
    sqlite3_mutex_free(mutex);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}