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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"sqlite3_drop_modules":[[153793,9,153793,16,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_initialize","sqlite3_compileoption_get","sqlite3_compileoption_get","sqlite3_open","sqlite3_initialize","sqlite3_prepare_v2","sqlite3_close_v2","sqlite3_initialize","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_close_v2","sqlite3_initialize","sqlite3_bind_zeroblob64","sqlite3_finalize","sqlite3_close_v2","sqlite3_initialize","sqlite3_step","sqlite3_finalize","sqlite3_autovacuum_pages","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close_v2","sqlite3_initialize"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_compileoption_get","sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_autovacuum_pages","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close_v2","sqlite3_initialize"],"visited":0}
/**/


// Assume sqlite3.h provides these types and function declarations
// We'll use the provided prototypes
extern "C" {
    struct sqlite3;
    struct sqlite3_stmt;
    struct sqlite3_context;
    typedef unsigned long long sqlite3_uint64;
    
    // Required APIs from the list
    int sqlite3_bind_zeroblob64(sqlite3_stmt *, int, sqlite3_uint64);
    int sqlite3_initialize();
    const char *sqlite3_compileoption_get(int N);
    void sqlite3_result_blob64(sqlite3_context *, const void *, sqlite3_uint64, void (*)(void *));
    int sqlite3_autovacuum_pages(sqlite3 *db, unsigned int (*)(void *, const char *, unsigned int, unsigned int, unsigned int), void *, void (*)(void *));
    int sqlite3_drop_modules(sqlite3 *db, const char **azKeep);
    const char *sqlite3_libversion();
    int sqlite3_errcode(sqlite3 *db);
    
    // Helper APIs we'll need (from the provided list)
    int sqlite3_open(const char *filename, sqlite3 **ppDb);
    int sqlite3_close_v2(sqlite3 *);
    int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail);
    int sqlite3_finalize(sqlite3_stmt *pStmt);
    int sqlite3_step(sqlite3_stmt *pStmt);
    void sqlite3_free(void *);
}

// Custom callback for autovacuum_pages
static unsigned int  id_000029_dummy_autovacuum_callback(void *pUser, const char *zSchema, unsigned int nPage, unsigned int nFree, unsigned int nSize) {
    return 100; // Suggest 100 pages per autovacuum step
}

// Custom destructor for blob data
static void  id_000029_dummy_blob_destructor(void *p) {
    // No-op destructor for our allocated blob
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite library
    if (sqlite3_initialize() != 0) {
        return 0;
    }
    
    // Get library version (API usage)
    const char *version = sqlite3_libversion();
    if (!version) {
        sqlite3_initialize(); // Reinitialize before return
        return 0;
    }
    
    // Get some compile options (API usage)
    const char *opt0 = sqlite3_compileoption_get(0);
    const char *opt1 = sqlite3_compileoption_get(1);
    
    // Create an in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != 0 || !db) {
        sqlite3_initialize(); // Reinitialize before return
        return 0;
    }
    
    // Create a table for testing
    const char *create_table_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_stmt *create_stmt = nullptr;
    if (sqlite3_prepare_v2(db, create_table_sql, -1, &create_stmt, nullptr) != 0) {
        sqlite3_close_v2(db);
        sqlite3_initialize(); // Reinitialize before return
        return 0;
    }
    sqlite3_step(create_stmt);
    sqlite3_finalize(create_stmt);
    
    // Prepare INSERT statement
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    sqlite3_stmt *insert_stmt = nullptr;
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr) != 0) {
        sqlite3_close_v2(db);
        sqlite3_initialize(); // Reinitialize before return
        return 0;
    }
    
    // Use input data to determine zeroblob size
    sqlite3_uint64 blob_size = 0;
    if (size >= sizeof(blob_size)) {
        // Use first 8 bytes as blob size
        memcpy(&blob_size, data, sizeof(blob_size));
        // Limit to reasonable size for fuzzing
        if (blob_size > 1024 * 1024) blob_size = 1024 * 1024;
    } else if (size > 0) {
        // Use size as blob size if less than 8 bytes
        blob_size = size;
    } else {
        blob_size = 1; // Minimum size
    }
    
    // Bind zeroblob using the API (API usage)
    if (sqlite3_bind_zeroblob64(insert_stmt, 1, blob_size) != 0) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close_v2(db);
        sqlite3_initialize(); // Reinitialize before return
        return 0;
    }
    
    // Execute INSERT
    sqlite3_step(insert_stmt);
    sqlite3_finalize(insert_stmt);
    
    // Set up autovacuum pages callback (API usage)
    // We use a dummy callback since we don't have actual page info
    if (sqlite3_autovacuum_pages(db,  id_000029_dummy_autovacuum_callback, nullptr, nullptr) != 0) {
        // Continue anyway, this might fail on in-memory DB
    }
    
    // Drop modules (API usage)
    // Try to drop all modules by passing null for azKeep
    if (sqlite3_drop_modules(db, nullptr) != 0) {
        // Continue anyway, this might fail if no modules loaded
    }
    
    // Create a dummy context for testing sqlite3_result_blob64
    // We'll simulate what a custom function might do
    if (size > 0) {
        // Allocate memory for blob data
        void *blob_data = malloc(size);
        if (blob_data) {
            memcpy(blob_data, data, size);
            
            // We can't actually call sqlite3_result_blob64 without a real context
            // from a custom function, but we demonstrate the pattern
            // In a real custom function, we would call:
            // sqlite3_result_blob64(context, blob_data, size,  id_000029_dummy_blob_destructor);
            
            // For fuzzing purposes, we'll just free the memory
            free(blob_data);
        }
    }
    
    // Check error code (API usage)
    int err = sqlite3_errcode(db);
    
    // Clean up
    sqlite3_close_v2(db);
    
    // Reinitialize library for next fuzzing iteration
    sqlite3_initialize();
    
    return 0;
}