#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_002 {
/*
 * Fuzzing harness for SQLite BLOB incremental I/O APIs
 * Targets: sqlite3_blob_open, sqlite3_blob_read, sqlite3_blob_write, 
 *          sqlite3_blob_reopen, sqlite3_blob_close, sqlite3_blob_bytes
 * 
 * This harness specifically targets BLOB incremental I/O APIs which have
 * ~3,000 undiscovered branch points according to coverage analysis.
 * Implementation follows exact sequence: 1) Create database, 2) CREATE TABLE 
 * with BLOB column, 3) INSERT row with zeroblob(), 4) sqlite3_blob_open, 
 * 5) sqlite3_blob_read, 6) sqlite3_blob_write, 7) sqlite3_blob_reopen, 
 * 8) sqlite3_blob_bytes, 9) sqlite3_blob_close, 10) cleanup.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful BLOB testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool use_memory_db = fdp.ConsumeBool();
    bool open_read_write = fdp.ConsumeBool();
    int blob_flags = open_read_write ? 1 : 0;  // 0 = read-only, 1 = read-write
    int blob_size = fdp.ConsumeIntegralInRange<int>(1, 4096);  // Size of zeroblob
    int rowid = fdp.ConsumeIntegral<int>();  // Row ID for blob operations
    int reopen_rowid = fdp.ConsumeIntegral<int>();  // New row ID for reopen
    bool perform_read = fdp.ConsumeBool();
    bool perform_write = fdp.ConsumeBool();
    bool perform_reopen = fdp.ConsumeBool();
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    const char* db_name = use_memory_db ? ":memory:" : "fuzz_blob.db";
    
    // Open database connection with appropriate flags
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (use_memory_db) {
        open_flags |= SQLITE_OPEN_MEMORY;
    }
    
    int rc = sqlite3_open_v2(db_name, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 50);
    
    // Step 1: Create table with BLOB column
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS blob_test ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB);";
    
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
        }
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 2: Insert row with zeroblob() 
    // First, delete any existing row to avoid conflicts
    sqlite3_exec(db, "DELETE FROM blob_test;", nullptr, nullptr, nullptr);
    
    // Prepare INSERT statement with zeroblob
    sqlite3_stmt* stmt_insert = nullptr;
    const char* insert_sql = "INSERT INTO blob_test (id, data) VALUES (?, zeroblob(?));";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, nullptr);
    if (rc != SQLITE_OK || stmt_insert == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Bind parameters
    sqlite3_bind_int(stmt_insert, 1, rowid);
    sqlite3_bind_int(stmt_insert, 2, blob_size);
    
    // Execute INSERT
    rc = sqlite3_step(stmt_insert);
    sqlite3_finalize(stmt_insert);
    
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Now we have a BLOB in the database, open it for incremental I/O
    sqlite3_blob* blob_handle = nullptr;
    
    // Step 3: Open BLOB handle
    rc = sqlite3_blob_open(db, "main", "blob_test", "data", rowid, blob_flags, &blob_handle);
    if (rc != SQLITE_OK || blob_handle == nullptr) {
        // BLOB opening failed, clean up
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 4: Get BLOB size
    int actual_blob_size = sqlite3_blob_bytes(blob_handle);
    
    // Step 5: Perform read operation if requested and blob has data
    if (perform_read && actual_blob_size > 0) {
        // Read from the beginning of the BLOB
        size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, actual_blob_size);
        if (read_size > 0) {
            std::vector<uint8_t> read_buffer(read_size, 0);
            rc = sqlite3_blob_read(blob_handle, read_buffer.data(), read_size, 0);
            // Read may fail if offset is out of bounds, which is OK for fuzzing
        }
    }
    
    // Step 6: Perform write operation if requested and blob is writable
    if (perform_write && blob_flags == 1 && actual_blob_size > 0) {
        // Write to the BLOB
        size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, actual_blob_size);
        if (write_size > 0) {
            // Consume data to write from fuzzer input
            std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
            if (write_data.size() == write_size) {
                int write_offset = fdp.ConsumeIntegralInRange<int>(0, actual_blob_size - write_size);
                rc = sqlite3_blob_write(blob_handle, write_data.data(), write_size, write_offset);
                // Write may fail if constraints violated, which is OK for fuzzing
            }
        }
    }
    
    // Step 7: Reopen BLOB to different row if requested
    if (perform_reopen) {
        // Insert another row with zeroblob for reopening target
        sqlite3_stmt* stmt_insert2 = nullptr;
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert2, nullptr);
        if (rc == SQLITE_OK && stmt_insert2 != nullptr) {
            sqlite3_bind_int(stmt_insert2, 1, reopen_rowid);
            sqlite3_bind_int(stmt_insert2, 2, blob_size);
            rc = sqlite3_step(stmt_insert2);
            sqlite3_finalize(stmt_insert2);
            
            if (rc == SQLITE_DONE) {
                // Try to reopen the blob handle to the new row
                rc = sqlite3_blob_reopen(blob_handle, reopen_rowid);
                // Reopen may fail if row doesn't exist or other conditions, OK for fuzzing
                
                if (rc == SQLITE_OK) {
                    // If reopen succeeded, we can test operations on the new blob
                    int new_blob_size = sqlite3_blob_bytes(blob_handle);
                    
                    // Try a read on the reopened blob
                    if (new_blob_size > 0 && fdp.remaining_bytes() > 0) {
                        size_t test_read = fdp.ConsumeIntegralInRange<size_t>(1, new_blob_size);
                        if (test_read > 0) {
                            std::vector<uint8_t> test_buffer(test_read, 0);
                            sqlite3_blob_read(blob_handle, test_buffer.data(), test_read, 0);
                        }
                    }
                }
            }
        }
    }
    
    // Step 8: Get BLOB size again (after possible operations)
    int final_blob_size = sqlite3_blob_bytes(blob_handle);
    
    // Step 9: Close BLOB handle
    rc = sqlite3_blob_close(blob_handle);
    
    // Step 10: Cleanup - drop table and close database
    sqlite3_exec(db, "DROP TABLE IF EXISTS blob_test;", nullptr, nullptr, nullptr);
    
    rc = sqlite3_close(db);
    
    // Final shutdown
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_002
