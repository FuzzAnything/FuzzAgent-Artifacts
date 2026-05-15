#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_001 {
/*
 * Fuzzing harness for SQLite library - focusing on BLOB (Binary Large Object) manipulation APIs
 * This harness targets sqlite3_blob_open, sqlite3_blob_close, sqlite3_blob_read, 
 * sqlite3_blob_write, sqlite3_blob_reopen, and sqlite3_blob_bytes
 * following the coverage guidance to target 130+ uncovered branches
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough for: operation type, blob size, blob data, rowid, flags, etc.
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Step 1: Create a table with BLOB column
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        // Continue anyway - table might already exist
    }
    
    // Step 2: Prepare INSERT statement
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test_blob (data) VALUES (?1);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 3: Bind BLOB data to statement
    // Consume blob size from fuzzer input (limit to reasonable size)
    size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)4096));
    std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
    
    rc = sqlite3_bind_blob(stmt, 1, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 4: Execute INSERT statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Get the rowid of the inserted row
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    
    // Finalize the statement
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    // Now we have a BLOB in the database, proceed with BLOB operations
    
    // Step 5: Open BLOB handle
    sqlite3_blob* pBlob = nullptr;
    // Use default database "main", table "test_blob", column "data"
    // Flags: 0 for read-only, 1 for read-write
    int flags = fdp.ConsumeBool() ? 1 : 0;
    
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", rowid, flags, &pBlob);
    if (rc != SQLITE_OK || pBlob == nullptr) {
        // If blob open fails, clean up and exit
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 6: Get BLOB size
    int blob_bytes = sqlite3_blob_bytes(pBlob);
    
    // Step 7: Read from BLOB (if we have a blob handle)
    if (blob_bytes > 0) {
        // Consume offset and read size from fuzzer input
        int read_offset = fdp.ConsumeIntegralInRange<int>(0, blob_bytes - 1);
        int read_size = fdp.ConsumeIntegralInRange<int>(1, blob_bytes - read_offset);
        
        if (read_size > 0 && read_offset >= 0 && read_offset + read_size <= blob_bytes) {
            std::vector<uint8_t> read_buffer(read_size);
            rc = sqlite3_blob_read(pBlob, read_buffer.data(), read_size, read_offset);
            // Don't check rc - we want to test error paths too
        }
    }
    
    // Step 8: Write to BLOB (if opened for writing)
    if (flags == 1 && blob_bytes > 0) {  // Read-write mode
        // Consume offset and write size from fuzzer input
        int write_offset = fdp.ConsumeIntegralInRange<int>(0, blob_bytes - 1);
        int write_size = fdp.ConsumeIntegralInRange<int>(1, blob_bytes - write_offset);
        
        if (write_size > 0 && fdp.remaining_bytes() >= write_size) {
            std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
            if (write_offset >= 0 && write_offset + write_size <= blob_bytes) {
                rc = sqlite3_blob_write(pBlob, write_data.data(), write_size, write_offset);
                // Don't check rc - we want to test error paths too
            }
        }
    }
    
    // Step 9: Reopen BLOB handle to a different row (if we have another row)
    // First, insert another row to have a target for reopening
    stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt != nullptr) {
        // Use remaining input for second blob
        size_t blob2_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)4096));
        std::vector<uint8_t> blob2_data = fdp.ConsumeBytes<uint8_t>(blob2_size);
        
        if (blob2_size > 0) {
            rc = sqlite3_bind_blob(stmt, 1, blob2_data.data(), blob2_data.size(), SQLITE_TRANSIENT);
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
                    sqlite3_int64 rowid2 = sqlite3_last_insert_rowid(db);
                    
                    // Try to reopen the blob handle to the new row
                    rc = sqlite3_blob_reopen(pBlob, rowid2);
                    // Don't check rc - test both success and failure paths
                    
                    // After reopening, we could read/write to the new blob
                    if (rc == SQLITE_OK) {
                        int new_blob_bytes = sqlite3_blob_bytes(pBlob);
                        if (new_blob_bytes > 0 && fdp.remaining_bytes() > 0) {
                            // Try a small read from the reopened blob
                            int test_read_size = fdp.ConsumeIntegralInRange<int>(1, std::min(new_blob_bytes, 100));
                            int test_offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, new_blob_bytes - test_read_size));
                            if (test_read_size > 0) {
                                std::vector<uint8_t> test_buffer(test_read_size);
                                sqlite3_blob_read(pBlob, test_buffer.data(), test_read_size, test_offset);
                            }
                        }
                    }
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Step 10: Close BLOB handle
    rc = sqlite3_blob_close(pBlob);
    // Don't check rc - test both success and failure paths
    
    // Step 11: Clean up database
    // Delete test table to clean up
    sqlite3_exec(db, "DROP TABLE IF EXISTS test_blob;", nullptr, nullptr, nullptr);
    
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_001
