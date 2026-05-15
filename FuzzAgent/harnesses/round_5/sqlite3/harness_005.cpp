#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_005 {
/*
** SQLite database serialization/deserialization fuzzing harness
**
** This harness specifically targets SQLite serialization/deserialization APIs 
** identified as high-priority coverage gaps:
** - sqlite3_serialize: Serialize database to a contiguous memory buffer
** - sqlite3_deserialize: Create in-memory database from serialized data
** - sqlite3_wal_checkpoint: Perform WAL checkpoint operation (legacy)
** - sqlite3_wal_checkpoint_v2: Perform WAL checkpoint with extended options
** - sqlite3_get_table: Retrieve query results as table
** - sqlite3_free_table: Free memory allocated by sqlite3_get_table
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step)
** - harness_001: Advanced operations with progress handlers and authorizers
** - harness_002: BLOB-specific incremental I/O operations
** - harness_003: Virtual table creation, configuration, and operations
** - harness_004: Database backup operations between source and destination databases
** - harness_005: Database serialization/deserialization with WAL checkpoints (this harness)
**
** Operation lifecycle: 
**   Database setup → Data population → Serialization → Deserialization → 
**   WAL checkpoint operations → Table retrieval operations → Cleanup
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Operation types for the serialization/deserialization harness
enum SerializationOperation {
    SERIALIZE_OP_BASIC = 0,
    SERIALIZE_OP_WITH_CHECKPOINT = 1,
    SERIALIZE_OP_WITH_TABLE = 2,
    SERIALIZE_OP_FULL_CYCLE = 3
};

// Available database schemas for serialization
static const char* const db_schemas[] = {
    "main",
    "temp",
    // Note: "aux" schemas are not supported for deserialization
};

// WAL checkpoint modes for sqlite3_wal_checkpoint_v2
static const int checkpoint_modes[] = {
    SQLITE_CHECKPOINT_PASSIVE,
    SQLITE_CHECKPOINT_FULL,
    SQLITE_CHECKPOINT_RESTART,
    SQLITE_CHECKPOINT_TRUNCATE
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and basic operations
    // sizeof(int) for config + sizeof(sqlite3_int64) for sizes + at least some data bytes
    const size_t MIN_SIZE = sizeof(int) * 4 + sizeof(sqlite3_int64) * 2 + 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;  // Second database for deserialization
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
    
    // Create tables and insert some data to make serialization meaningful
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_data ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "blob_data BLOB);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        // Continue anyway - database might still be usable
    }
    
    // Insert some test data using fuzzer input
    int num_rows = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_rows; i++) {
        if (fdp.remaining_bytes() < 32) break;
        
        std::string name = fdp.ConsumeRandomLengthString(64);
        double value = fdp.ConsumeFloatingPoint<double>();
        std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(0, 256));
        
        // Build and execute INSERT statement
        char insert_sql[512];
        if (blob_data.empty()) {
            snprintf(insert_sql, sizeof(insert_sql),
                    "INSERT INTO test_data (name, value) VALUES ('%s', %f)",
                    name.c_str(), value);
        } else {
            // For BLOB data, we'd need to use prepared statement with binding
            // For simplicity in this harness, we'll skip BLOB insertion
            snprintf(insert_sql, sizeof(insert_sql),
                    "INSERT INTO test_data (name, value) VALUES ('%s', %f)",
                    name.c_str(), value);
        }
        
        sqlite3_exec(db, insert_sql, nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Determine operation type from fuzzer input
    SerializationOperation op_type = static_cast<SerializationOperation>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 3));
    
    // Select schema for serialization
    const char* schema = db_schemas[fdp.ConsumeIntegralInRange<size_t>(0, 
        sizeof(db_schemas)/sizeof(db_schemas[0]) - 1)];
    
    // PHASE 1: Serialization
    sqlite3_int64 serialized_size = 0;
    unsigned int serialize_flags = 0;
    
    // Set serialization flags based on fuzzer input
    if (fdp.ConsumeBool()) {
        serialize_flags |= SQLITE_SERIALIZE_NOCOPY;
    }
    
    unsigned char* serialized_data = sqlite3_serialize(
        db, schema, &serialized_size, serialize_flags);
    
    // Track if memory is owned by SQLite (for FREEONCLOSE flag)
    bool memory_owned_by_sqlite = false;
    
    // PHASE 2: Deserialization (if we got serialized data)
    if (serialized_data != nullptr && serialized_size > 0 && 
        op_type != SERIALIZE_OP_BASIC) {
        
        // Open second database for deserialization
        int db2_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
        if (fdp.ConsumeBool()) {
            db2_flags |= SQLITE_OPEN_NOMUTEX;
        }
        
        rc = sqlite3_open_v2(":memory:", &db2, db2_flags, nullptr);
        if (rc == SQLITE_OK && db2 != nullptr) {
            unsigned int deserialize_flags = 0;
            
            // Set deserialization flags based on fuzzer input
            if (fdp.ConsumeBool()) {
                deserialize_flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
                memory_owned_by_sqlite = true;
            }
            if (fdp.ConsumeBool()) {
                deserialize_flags |= SQLITE_DESERIALIZE_RESIZEABLE;
            }
            if (fdp.ConsumeBool()) {
                deserialize_flags |= SQLITE_DESERIALIZE_READONLY;
            }
            
            // Determine buffer size (may be larger than actual data)
            sqlite3_int64 buffer_size = serialized_size;
            if (fdp.ConsumeBool() && serialized_size < 1024 * 1024) {
                buffer_size = serialized_size + fdp.ConsumeIntegralInRange<sqlite3_int64>(
                    0, 1024 * 1024);
            }
            
            // Perform deserialization
            rc = sqlite3_deserialize(db2, schema, serialized_data, 
                                    serialized_size, buffer_size, deserialize_flags);
            // If deserialization succeeded and we have the right operation type,
            // perform additional operations on the deserialized database
            if (rc == SQLITE_OK && db2 != nullptr) {
                // PHASE 3: WAL Checkpoint operations
                if (op_type == SERIALIZE_OP_WITH_CHECKPOINT || 
                    op_type == SERIALIZE_OP_FULL_CYCLE) {
                    
                    // Try both checkpoint APIs
                    int checkpoint_rc = sqlite3_wal_checkpoint(db2, schema);
                    
                    // Also try the v2 version with different modes
                    int checkpoint_mode = checkpoint_modes[
                        fdp.ConsumeIntegralInRange<size_t>(0, 
                            sizeof(checkpoint_modes)/sizeof(checkpoint_modes[0]) - 1)];
                    
                    int nLog = 0;
                    int nCkpt = 0;
                    checkpoint_rc = sqlite3_wal_checkpoint_v2(
                        db2, schema, checkpoint_mode, &nLog, &nCkpt);
                }
                
                // PHASE 4: Table retrieval operations
                if (op_type == SERIALIZE_OP_WITH_TABLE || 
                    op_type == SERIALIZE_OP_FULL_CYCLE) {
                    
                    char** pazResult = nullptr;
                    int nRow = 0;
                    int nColumn = 0;
                    char* pzErrmsg = nullptr;
                    
                    // Consume SQL query from fuzzer input
                    std::string query = fdp.ConsumeRandomLengthString(256);
                    if (query.empty()) {
                        query = "SELECT * FROM test_data";
                    }
                    
                    // Use sqlite3_get_table to retrieve results
                    int table_rc = sqlite3_get_table(db2, query.c_str(), &pazResult, 
                                                    &nRow, &nColumn, &pzErrmsg);
                    
                    // Free the table if we got one
                    if (pazResult != nullptr) {
                        sqlite3_free_table(pazResult);
                    }
                    
                    if (pzErrmsg != nullptr) {
                        sqlite3_free(pzErrmsg);
                    }
                }
            }
            
            // Close second database
            if (db2 != nullptr) {
                sqlite3_close(db2);
            }
        }
        
        // Free serialized data if SQLITE_SERIALIZE_NOCOPY wasn't set
        // and SQLITE_DESERIALIZE_FREEONCLOSE wasn't set
        if (!(serialize_flags & SQLITE_SERIALIZE_NOCOPY) && !memory_owned_by_sqlite) {
            sqlite3_free(serialized_data);
        }
    } else if (serialized_data != nullptr && 
               !(serialize_flags & SQLITE_SERIALIZE_NOCOPY)) {
        // Free data if we didn't use it for deserialization
        sqlite3_free(serialized_data);
    }
    
    // Also perform checkpoint operations on original database for certain operation types
    if (op_type == SERIALIZE_OP_WITH_CHECKPOINT || op_type == SERIALIZE_OP_FULL_CYCLE) {
        sqlite3_wal_checkpoint(db, schema);
        
        int checkpoint_mode = checkpoint_modes[
            fdp.ConsumeIntegralInRange<size_t>(0, 
                sizeof(checkpoint_modes)/sizeof(checkpoint_modes[0]) - 1)];
        
        int nLog = 0;
        int nCkpt = 0;
        sqlite3_wal_checkpoint_v2(db, schema, checkpoint_mode, &nLog, &nCkpt);
    }
    
    // Also test table retrieval on original database
    if (op_type == SERIALIZE_OP_WITH_TABLE || op_type == SERIALIZE_OP_FULL_CYCLE) {
        char** pazResult = nullptr;
        int nRow = 0;
        int nColumn = 0;
        char* pzErrmsg = nullptr;
        
        std::string query = fdp.ConsumeRandomLengthString(256);
        if (query.empty()) {
            query = "SELECT * FROM test_data";
        }
        
        int table_rc = sqlite3_get_table(db, query.c_str(), &pazResult, 
                                        &nRow, &nColumn, &pzErrmsg);
        
        if (pazResult != nullptr) {
            sqlite3_free_table(pazResult);
        }
        
        if (pzErrmsg != nullptr) {
            sqlite3_free(pzErrmsg);
        }
    }
    
    // Cleanup
    if (db != nullptr) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_005
