#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_004 {
/*
 * Fuzzing harness for SQLite library - focusing on serialization/deserialization and table metadata APIs
 * FINAL CORRECTED VERSION - Based on lessons from working harness_000.cpp
 * Targets: sqlite3_table_column_metadata, sqlite3_serialize, sqlite3_deserialize, sqlite3_get_table
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite (let it auto-initialize if needed)
    // sqlite3_initialize() is optional for many use cases
    
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Create a simple table for testing
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name TEXT);", 
                      nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Insert some data if we have input
    if (rc == SQLITE_OK && fdp.remaining_bytes() > 4) {
        rc = sqlite3_exec(db, "INSERT INTO test_table VALUES (1, 'test');", 
                          nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
    }
    
    // Test 1: sqlite3_table_column_metadata
    if (rc == SQLITE_OK && fdp.ConsumeBool()) {
        const char* data_type = nullptr;
        const char* coll_seq = nullptr;
        int not_null = 0;
        int primary_key = 0;
        int autoinc = 0;
        
        rc = sqlite3_table_column_metadata(
            db,
            "main",
            "test_table",
            "id",
            &data_type,
            &coll_seq,
            &not_null,
            &primary_key,
            &autoinc
        );
        // Don't use the pointers - they're only valid if rc == SQLITE_OK
        (void)data_type;
        (void)coll_seq;
    }
    
    // Test 2: sqlite3_get_table
    char** result = nullptr;
    int nrow = 0;
    int ncol = 0;
    if (rc == SQLITE_OK && fdp.ConsumeBool()) {
        rc = sqlite3_get_table(db, "SELECT * FROM test_table", &result, &nrow, &ncol, &errmsg);
        if (rc != SQLITE_OK && errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
    }
    
    // Test 3: sqlite3_serialize
    sqlite3_int64 serialized_size = 0;
    unsigned char* serialized_data = nullptr;
    if (rc == SQLITE_OK && fdp.ConsumeBool()) {
        serialized_data = sqlite3_serialize(
            db,
            "main",
            &serialized_size,
            0  // No flags
        );
    }
    
    // Test 4: sqlite3_deserialize (only if we have serialized data)
    if (serialized_data != nullptr && serialized_size > 0 && fdp.ConsumeBool()) {
        sqlite3* db2 = nullptr;
        rc = sqlite3_open_v2(":memory:", &db2,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
        
        if (rc == SQLITE_OK && db2 != nullptr) {
            // Use FREEONCLOSE so SQLite manages memory
            rc = sqlite3_deserialize(
                db2,
                "main",
                serialized_data,
                serialized_size,
                serialized_size,
                SQLITE_DESERIALIZE_FREEONCLOSE
            );
            
            // Don't free serialized_data - SQLite will free it with FREEONCLOSE
            
            if (db2 != nullptr) {
                sqlite3_close(db2);
            }
        } else {
            // Failed to open db2, free serialized_data ourselves
            if (serialized_data != nullptr) {
                sqlite3_free(serialized_data);
                serialized_data = nullptr;
            }
            if (db2 != nullptr) {
                sqlite3_close(db2);
            }
        }
    } else {
        // We didn't use deserialize, free serialized_data if we have it
        if (serialized_data != nullptr) {
            sqlite3_free(serialized_data);
            serialized_data = nullptr;
        }
    }
    
    // Cleanup
    if (result != nullptr) {
        sqlite3_free_table(result);
        result = nullptr;
    }
    
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
    
    // Don't call sqlite3_shutdown() - let SQLite manage its own cleanup
    
    return 0;
}

} // namespace harness_004
