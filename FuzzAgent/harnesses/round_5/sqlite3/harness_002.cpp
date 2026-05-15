#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_002 {
/*
** SQLite BLOB manipulation fuzzing harness
**
** This harness specifically targets SQLite BLOB APIs identified as high-priority coverage gaps:
** - sqlite3_blob_open: Open a BLOB for incremental I/O
** - sqlite3_blob_read: Read data from an open BLOB handle
** - sqlite3_blob_write: Write data to an open BLOB handle
** - sqlite3_blob_bytes: Get the size of an open BLOB
** - sqlite3_blob_close: Close a BLOB handle
** - sqlite3_blob_reopen: Reopen a BLOB handle to a different row
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step)
** - harness_001: Advanced operations with progress handlers and authorizers
** - harness_002: BLOB-specific incremental I/O operations (this harness)
**
** BLOB operation lifecycle: Database setup → BLOB creation → Open → Read/Write → Close
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Operation types for the BLOB harness
enum BlobOperation {
    BLOB_OP_READ = 0,
    BLOB_OP_WRITE = 1,
    BLOB_OP_READ_WRITE = 2,
    BLOB_OP_REOPEN = 3
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for basic configuration
    // sizeof(int) for config + sizeof(int64_t) for rowid + at least 1 byte for data
    const size_t MIN_SIZE = sizeof(int) * 4 + sizeof(sqlite3_int64) + 1;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_blob* pBlob = nullptr;
    char* err_msg = nullptr;
    
    // Consume database configuration from fuzzer input
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    // Add optional flags based on fuzzer input
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    
    // Open in-memory database
    int rc = sqlite3_open_v2(":memory:", &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    
    // Create a table with a BLOB column
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS blob_test ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "description TEXT);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Insert initial rows with BLOB data
    // We need at least one row for BLOB operations to work
    const char* insert_sql = 
        "INSERT INTO blob_test (id, data, description) VALUES "
        "(1, zeroblob(1024), 'test blob 1'), "
        "(2, zeroblob(512), 'test blob 2'), "
        "(3, zeroblob(256), 'test blob 3'), "
        "(4, zeroblob(128), 'test blob 4');";
    
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        // Continue anyway - some rows might have been inserted
    }
    // Consume BLOB operation parameters from fuzzer input
    BlobOperation op_type = static_cast<BlobOperation>(fdp.ConsumeIntegralInRange<int>(0, 3));
    sqlite3_int64 rowid = fdp.ConsumeIntegralInRange<sqlite3_int64>(1, 4); // Use existing rows
    int blob_flags = 0; // Default to read-only
    
    // Determine flags based on operation type
    if (op_type == BLOB_OP_WRITE || op_type == BLOB_OP_READ_WRITE) {
        blob_flags = 1; // SQLITE_OPEN_READWRITE for BLOB
    }
    
    // Open a BLOB handle
    rc = sqlite3_blob_open(db, "main", "blob_test", "data", rowid, blob_flags, &pBlob);
    
    if (rc == SQLITE_OK && pBlob != nullptr) {
        // Get BLOB size
        int blob_size = sqlite3_blob_bytes(pBlob);
        
        // Consume operation-specific parameters
        int offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, blob_size - 1));
        int data_size = fdp.ConsumeIntegralInRange<int>(1, std::min(256, blob_size - offset));
        
        // Ensure we have enough data for the operation
        if (data_size > 0 && offset + data_size <= blob_size) {
            switch (op_type) {
                case BLOB_OP_READ: {
                    // Read data from BLOB
                    std::vector<uint8_t> read_buffer(data_size);
                    rc = sqlite3_blob_read(pBlob, read_buffer.data(), data_size, offset);
                    
                    // Read result can be SQLITE_OK, SQLITE_ERROR, or SQLITE_ABORT
                    // All are valid outcomes for fuzzing
                    break;
                }
                
                case BLOB_OP_WRITE: {
                    // Write data to BLOB
                    if (blob_flags == 1) { // Only if opened for writing
                        std::vector<uint8_t> write_buffer = fdp.ConsumeBytes<uint8_t>(data_size);
                        if (write_buffer.size() == static_cast<size_t>(data_size)) {
                            rc = sqlite3_blob_write(pBlob, write_buffer.data(), data_size, offset);
                        }
                    }
                    break;
                }
                
                case BLOB_OP_READ_WRITE: {
                    // Combined read-then-write operation
                    if (blob_flags == 1) {
                        // First read
                        std::vector<uint8_t> read_buffer(data_size);
                        int read_rc = sqlite3_blob_read(pBlob, read_buffer.data(), data_size, offset);
                        
                        // Then write (different data)
                        if (fdp.remaining_bytes() >= static_cast<size_t>(data_size)) {
                            std::vector<uint8_t> write_buffer = fdp.ConsumeBytes<uint8_t>(data_size);
                            if (write_buffer.size() == static_cast<size_t>(data_size)) {
                                rc = sqlite3_blob_write(pBlob, write_buffer.data(), data_size, offset);
                            }
                        }
                    }
                    break;
                }
                
                case BLOB_OP_REOPEN: {
                    // Reopen the BLOB handle to a different row
                    sqlite3_int64 new_rowid = fdp.ConsumeIntegralInRange<sqlite3_int64>(1, 4);
                    if (new_rowid != rowid) {
                        rc = sqlite3_blob_reopen(pBlob, new_rowid);
                        
                        // If reopen succeeded, try a small read
                        if (rc == SQLITE_OK) {
                            int new_blob_size = sqlite3_blob_bytes(pBlob);
                            int new_offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, new_blob_size - 1));
                            int new_size = fdp.ConsumeIntegralInRange<int>(1, std::min(64, new_blob_size - new_offset));
                            
                            if (new_size > 0 && new_offset + new_size <= new_blob_size) {
                                std::vector<uint8_t> buffer(new_size);
                                sqlite3_blob_read(pBlob, buffer.data(), new_size, new_offset);
                            }
                        }
                    }
                    break;
                }
            }
        }
        
        // Close the BLOB handle
        sqlite3_blob_close(pBlob);
    }
    
    // Test error cases: Try to open BLOB with invalid parameters
    if (fdp.remaining_bytes() > 10) {
        // Try invalid table/column names
        std::string invalid_table = fdp.ConsumeRandomLengthString(20);
        std::string invalid_column = fdp.ConsumeRandomLengthString(20);
        sqlite3_int64 invalid_rowid = fdp.ConsumeIntegral<sqlite3_int64>();
        
        sqlite3_blob* invalid_blob = nullptr;
        sqlite3_blob_open(db, "main", invalid_table.c_str(), invalid_column.c_str(), 
                         invalid_rowid, 0, &invalid_blob);
        
        // If somehow it succeeded, close it
        if (invalid_blob != nullptr) {
            sqlite3_blob_close(invalid_blob);
        }
    }
    
    // Clean up
    if (err_msg) sqlite3_free(err_msg);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_002
