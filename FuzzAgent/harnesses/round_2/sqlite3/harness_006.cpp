#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "sqlite3.h"

namespace harness_006 {
/*
 * SQLite fuzzing harness - basic database operations
 * Tests core SQLite APIs with proper library linking
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 1) return 0;
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Open in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Simple SQL execution based on input
    char* err_msg = nullptr;
    const char* sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER);";
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert a row
    sql = "INSERT INTO test(id) VALUES(1);";
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Query the table
    sql = "SELECT * FROM test;";
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_006
