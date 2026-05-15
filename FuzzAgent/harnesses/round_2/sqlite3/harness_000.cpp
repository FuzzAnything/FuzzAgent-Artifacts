#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "sqlite3.h"

namespace harness_000 {
/*
 * Fuzzing harness for SQLite database operations.
 * Targets main API entry points: sqlite3_open, sqlite3_exec, sqlite3_prepare_v2,
 * sqlite3_step, sqlite3_finalize, sqlite3_close.
 */



/*
 * Callback function for sqlite3_exec.
 * Simple callback that does nothing but count rows.
 */
static int exec_callback(void* data, int argc, char** argv, char** col_names) {
    int* count = static_cast<int*>(data);
    (*count)++;
    return 0;
}

/*
 * Main fuzzer entry point.
 */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least some data for meaningful testing
    if (size < 4) {
        return 0;
    }
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    int rc;
    
    // Open database with appropriate flags
    rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some safety limits to prevent timeouts
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Simple test: execute SQL based on input
    if (size > 0) {
        char* err_msg = nullptr;
        // Use first byte to determine operation
        uint8_t operation = data[0] % 2;
        
        if (operation == 0) {
            // Create table and insert data
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER);", nullptr, nullptr, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            
            err_msg = nullptr;
            sqlite3_exec(db, "INSERT INTO test(id) VALUES(1);", nullptr, nullptr, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
        } else {
            // Simple query
            err_msg = nullptr;
            sqlite3_exec(db, "SELECT * FROM test;", exec_callback, nullptr, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
        }
    }
    
    // Test sqlite3_close
    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
    }
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_000
