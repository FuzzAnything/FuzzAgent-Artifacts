#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_009 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for BLOB operations
    // We need enough data for: operation selection + blob size + some data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which BLOB operation scenario to test
    bool use_zero_length_blob = fdp.ConsumeBool();           // Test zero-length BLOB
    bool use_large_blob = fdp.ConsumeBool();                 // Test larger BLOB
    bool test_invalid_rowid = fdp.ConsumeBool();             // Test invalid row ID scenario
    bool read_only_mode = fdp.ConsumeBool();                 // Open BLOB in read-only mode
    
    // Consume blob size (within reasonable limits)
    int blob_size = 1024;  // Default size
    if (use_zero_length_blob) {
        blob_size = 0;
    } else if (use_large_blob) {
        blob_size = fdp.ConsumeIntegralInRange<int>(1, 65536);  // Up to 64KB
    } else {
        blob_size = fdp.ConsumeIntegralInRange<int>(1, 4096);   // Up to 4KB
    }
    
    // Consume row IDs for blob operations
    sqlite3_int64 rowid1 = fdp.ConsumeIntegral<sqlite3_int64>();
    sqlite3_int64 rowid2 = fdp.ConsumeIntegral<sqlite3_int64>();
    
    // Consume remaining data for BLOB content
    std::vector<uint8_t> blob_data;
    if (blob_size > 0) {
        // Take at least blob_size bytes or remaining bytes
        size_t data_to_consume = std::min<size_t>(fdp.remaining_bytes(), blob_size);
        blob_data = fdp.ConsumeBytes<uint8_t>(data_to_consume);
        
        // If we didn't get enough data, pad with zeros
        if (blob_data.size() < (size_t)blob_size) {
            blob_data.resize(blob_size, 0);
        }
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    int rc;
    
    // 1. Call sqlite3_open_v2 to create/open a database - PRIMARY TARGET: sqlite3_open_v2
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        // Clean up and return early if we can't open database
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable extended result codes for better error information
    sqlite3_extended_result_codes(db, 1);
    
    // 2. Prepare and execute SQL to create a table with BLOB column
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS test_blob (id INTEGER PRIMARY KEY, data BLOB)";
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    // 3. Insert row(s) with zeroblob
    // Insert first row
    const char* insert_sql = "INSERT INTO test_blob (data) VALUES (zeroblob(?))";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, blob_size);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    // Get the rowid of the inserted row
    sqlite3_int64 inserted_rowid = sqlite3_last_insert_rowid(db);
    
    // Insert a second row for testing sqlite3_blob_reopen
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    int second_blob_size = fdp.ConsumeIntegralInRange<int>(0, 2048);
    sqlite3_bind_int(stmt, 1, second_blob_size);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    sqlite3_int64 second_rowid = sqlite3_last_insert_rowid(db);
    
    // 4. Call sqlite3_blob_open to obtain a BLOB handle for the inserted row - PRIMARY TARGET: sqlite3_blob_open
    int flags = read_only_mode ? 0 : 1;  // 0 = read-only, 1 = read-write
    
    // Choose which row to open based on test scenario
    sqlite3_int64 target_rowid = test_invalid_rowid ? rowid1 : inserted_rowid;
    
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", target_rowid, flags, &blob);
    
    // Handle error cases gracefully
    if (rc != SQLITE_OK) {
        // Expected error for invalid rowid or other issues
        // Clean up and continue with other operations if possible
        if (blob) {
            sqlite3_blob_close(blob);
            blob = nullptr;
        }
        
        // Try with valid rowid instead
        if (test_invalid_rowid) {
            rc = sqlite3_blob_open(db, "main", "test_blob", "data", inserted_rowid, flags, &blob);
            if (rc != SQLITE_OK) {
                sqlite3_close(db);
                return 0;
            }
        } else {
            sqlite3_close(db);
            return 0;
        }
    }
    
    // 5. Call sqlite3_blob_bytes to get the BLOB size - PRIMARY TARGET: sqlite3_blob_bytes
    int blob_bytes = sqlite3_blob_bytes(blob);
    // blob_bytes should match blob_size (or second_blob_size for second row)
    
    // Only proceed with read/write if we have a valid blob handle and blob has content
    if (blob && blob_bytes > 0) {
        // Create buffers for reading/writing
        std::vector<uint8_t> read_buffer(blob_bytes);
        std::vector<uint8_t> write_buffer;
        
        if (!blob_data.empty() && blob_data.size() >= (size_t)blob_bytes) {
            write_buffer.assign(blob_data.begin(), blob_data.begin() + blob_bytes);
        } else {
            write_buffer.resize(blob_bytes, 0xAA);  // Default pattern
        }
        
        // 6. Call sqlite3_blob_read to read data from the BLOB - PRIMARY TARGET: sqlite3_blob_read
        // Test reading from different offsets
        for (int offset = 0; offset < blob_bytes && offset < 256; offset += 64) {
            int bytes_to_read = std::min(64, blob_bytes - offset);
            if (bytes_to_read > 0) {
                rc = sqlite3_blob_read(blob, read_buffer.data() + offset, bytes_to_read, offset);
                // Don't fail on read errors - they're part of the test
            }
        }
        
        // 7. Call sqlite3_blob_write to write data to the BLOB - PRIMARY TARGET: sqlite3_blob_write
        // Only write if blob was opened for writing
        if (!read_only_mode && blob_bytes > 0) {
            // Test writing at different offsets
            for (int offset = 0; offset < blob_bytes && offset < 256; offset += 64) {
                int bytes_to_write = std::min(64, blob_bytes - offset);
                if (bytes_to_write > 0 && offset < (int)write_buffer.size()) {
                    rc = sqlite3_blob_write(blob, write_buffer.data() + offset, bytes_to_write, offset);
                    // Don't fail on write errors - they're part of the test
                }
            }
        }
    }
    
    // 8. Call sqlite3_blob_reopen to reopen the BLOB handle on a different row - PRIMARY TARGET: sqlite3_blob_reopen
    if (blob) {
        // Try to reopen on the second row
        rc = sqlite3_blob_reopen(blob, second_rowid);
        
        // If reopen succeeded, test operations on the new blob
        if (rc == SQLITE_OK) {
            int new_blob_bytes = sqlite3_blob_bytes(blob);
            
            if (new_blob_bytes > 0) {
                std::vector<uint8_t> temp_buffer(new_blob_bytes);
                
                // Try reading from the reopened blob
                if (new_blob_bytes > 0) {
                    rc = sqlite3_blob_read(blob, temp_buffer.data(), std::min(64, new_blob_bytes), 0);
                }
                
                // Try writing if in read-write mode
                if (!read_only_mode && new_blob_bytes > 0) {
                    uint8_t test_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
                    int write_size = std::min(4, new_blob_bytes);
                    if (write_size > 0) {
                        rc = sqlite3_blob_write(blob, test_data, write_size, 0);
                    }
                }
            }
        }
    }
    
    // 9. Call sqlite3_blob_close to close the BLOB handle - PRIMARY TARGET: sqlite3_blob_close
    if (blob) {
        sqlite3_blob_close(blob);
        blob = nullptr;
    }
    
    // 10. Call sqlite3_close to clean up the database connection - PRIMARY TARGET: sqlite3_close
    sqlite3_close(db);
    
    return 0;
}

} // namespace harness_009
