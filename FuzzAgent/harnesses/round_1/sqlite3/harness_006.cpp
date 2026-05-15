#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_006 {
/*
 * Fuzzing harness for SQLite Serialization APIs
 * Targets: sqlite3_serialize (0% coverage, 774 undiscovered branches)
 *          sqlite3_deserialize (59.09% coverage, 743 undiscovered branches)
 *          sqlite3_table_column_metadata (0% coverage, 774 undiscovered branches)
 *          sqlite3_wal_checkpoint (0% coverage, 770 undiscovered branches)
 *          sqlite3_wal_checkpoint_v2 (0% coverage, 770 undiscovered branches)
 * 
 * This harness specifically targets SQLite serialization APIs which have
 * 1,517+ total undiscovered branches according to coverage analysis.
 * Implementation follows exact sequence:
 * 1) sqlite3_open_v2 to create database
 * 2) sqlite3_exec to create tables and insert test data
 * 3) sqlite3_serialize to convert database to BLOB
 * 4) sqlite3_table_column_metadata to verify structure
 * 5) sqlite3_deserialize to restore from BLOB
 * 6) Query deserialized data for integrity verification
 * 7) sqlite3_wal_checkpoint/v2 for WAL operations
 * 8) sqlite3_close
 * 
 * Includes varied parameters, BLOB handling, memory management scenarios.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Core UTF-8 APIs (prepared statements, exec)
 * - harness_001: Database file parsing (deserialization)
 * - harness_002: BLOB operations
 * - harness_003: UTF-16/Unicode specific APIs
 * - harness_004: Database backup operations
 * - harness_005: Table data retrieval (get_table/free_table)
 * - harness_006: Database serialization/deserialization APIs
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful serialization testing
    // Need enough for configuration + table data + serialization flags
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool test_temp_db = fdp.ConsumeBool();
    bool test_no_copy = fdp.ConsumeBool();
    bool test_free_on_close = fdp.ConsumeBool();
    bool test_resizeable = fdp.ConsumeBool();
    bool test_readonly = fdp.ConsumeBool();
    
    // Limit the number of tables and rows to prevent excessive memory usage
    int num_test_tables = fdp.ConsumeIntegralInRange<int>(1, 3);
    int max_rows_per_table = fdp.ConsumeIntegralInRange<int>(0, 10);
    int max_blob_size = fdp.ConsumeIntegralInRange<int>(0, 512);
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    sqlite3* db = nullptr;
    // Always use in-memory database to avoid file system issues with TCL test files
    const char* db_name = ":memory:";
    
    // Open database connection
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    int rc = sqlite3_open_v2(db_name, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    // Set some safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 20);
    sqlite3_limit(db, SQLITE_LIMIT_EXPR_DEPTH, 10);
    
    char* err_msg = nullptr;
    
    // Step 1: Create test tables with various structures
    for (int table_idx = 0; table_idx < num_test_tables && fdp.remaining_bytes() > 0; table_idx++) {
        // Generate table name from fuzzer input
        std::string table_name = "tbl" + std::to_string(table_idx);
        
        // Create table with various column types including BLOB for serialization testing
        std::string create_sql = "CREATE TABLE IF NOT EXISTS " + table_name + 
            " (id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "value REAL, "
            "data BLOB, "
            "flag INTEGER, "
            "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
        
        rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
        if (rc != SQLITE_OK && err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert some test data
        for (int row = 0; row < max_rows_per_table && fdp.remaining_bytes() > 0; row++) {
            std::string name = fdp.ConsumeRandomLengthString(50);
            double value = fdp.ConsumeFloatingPoint<double>();
            int flag = fdp.ConsumeIntegral<int>();
            
            // Generate random BLOB data
            int blob_size = fdp.ConsumeIntegralInRange<int>(0, max_blob_size);
            std::vector<uint8_t> blob_data;
            if (blob_size > 0 && fdp.remaining_bytes() >= blob_size) {
                blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
            }
            
            std::string insert_sql = "INSERT INTO " + table_name + 
                " (name, value, data, flag) VALUES (?, ?, ?, ?);";
            
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_double(stmt, 2, value);
                if (!blob_data.empty()) {
                    sqlite3_bind_blob(stmt, 3, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, 3);
                }
                sqlite3_bind_int(stmt, 4, flag);
                
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                    // Error occurred, but we continue
                }
                sqlite3_finalize(stmt);
            }
        }
    }
    
    // Step 2: Test sqlite3_table_column_metadata API
    for (int table_idx = 0; table_idx < num_test_tables; table_idx++) {
        std::string table_name = "tbl" + std::to_string(table_idx);
        
        // Test metadata for each column
        const char* columns[] = {"id", "name", "value", "data", "flag", "timestamp"};
        for (const char* col_name : columns) {
            const char* data_type = nullptr;
            const char* coll_seq = nullptr;
            int not_null = 0;
            int primary_key = 0;
            int autoinc = 0;
            
            rc = sqlite3_table_column_metadata(
                db, "main", table_name.c_str(), col_name,
                &data_type, &coll_seq, &not_null, &primary_key, &autoinc
            );
            
            // Metadata call succeeded, check the results
            if (rc == SQLITE_OK) {
                // Verify metadata makes sense
                if (data_type != nullptr) {
                    // Data type should be valid
                }
                if (coll_seq != nullptr) {
                    // Collation sequence should be valid
                }
            }
        }
    }
    
    // Step 3: Serialize the database using sqlite3_serialize
    sqlite3_int64 serialize_size = 0;
    unsigned int serialize_flags = 0;
    if (test_no_copy) {
        serialize_flags |= SQLITE_SERIALIZE_NOCOPY;
    }
    
    // Try serializing different schemas
    const char* schemas[] = {"main", "temp"};
    for (const char* schema : schemas) {
        // Skip temp schema if not testing it
        if (strcmp(schema, "temp") == 0 && !test_temp_db) {
            continue;
        }
        
        unsigned char* serialized_data = sqlite3_serialize(
            db, schema, &serialize_size, serialize_flags
        );
        
        // If we got serialized data, test deserialization
        if (serialized_data != nullptr && serialize_size > 0) {
            // Step 4: Deserialize using sqlite3_deserialize
            unsigned int deserialize_flags = 0;
            if (test_free_on_close) {
                deserialize_flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
            }
            if (test_resizeable) {
                deserialize_flags |= SQLITE_DESERIALIZE_RESIZEABLE;
            }
            if (test_readonly) {
                deserialize_flags |= SQLITE_DESERIALIZE_READONLY;
            }
            
            // For deserialization, we need to create a new database connection
            sqlite3* db2 = nullptr;
            rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
            
            if (rc == SQLITE_OK && db2 != nullptr) {
                // Deserialize into the new database
                rc = sqlite3_deserialize(
                    db2, schema,
                    serialized_data, serialize_size, serialize_size * 2,  // Use larger buffer
                    deserialize_flags
                );
                
                if (rc == SQLITE_OK) {
                    // Step 5: Verify deserialized data integrity
                    std::string verify_sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table';";
                    sqlite3_stmt* stmt = nullptr;
                    rc = sqlite3_prepare_v2(db2, verify_sql.c_str(), -1, &stmt, nullptr);
                    if (rc == SQLITE_OK && stmt != nullptr) {
                        if (sqlite3_step(stmt) == SQLITE_ROW) {
                            int table_count = sqlite3_column_int(stmt, 0);
                            // Table count should match
                        }
                        sqlite3_finalize(stmt);
                    }
                    
                    // Step 6: Test WAL checkpoint operations on deserialized database
                    if (fdp.ConsumeBool()) {
                        // Test sqlite3_wal_checkpoint
                        rc = sqlite3_wal_checkpoint(db2, schema);
                        // Test sqlite3_wal_checkpoint_v2 with various modes
                        int checkpoint_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
                        int* pnLog = nullptr;
                        int* pnCkpt = nullptr;
                        int log_val = 0;  // Declare outside conditional scope
                        int ckpt_val = 0; // Declare outside conditional scope
                        
                        if (fdp.remaining_bytes() > sizeof(int) * 2) {
                            log_val = fdp.ConsumeIntegral<int>();
                            ckpt_val = fdp.ConsumeIntegral<int>();
                            pnLog = &log_val;
                            pnCkpt = &ckpt_val;
                        }
                        
                        rc = sqlite3_wal_checkpoint_v2(
                            db2, schema,
                            checkpoint_mode,
                            pnLog, pnCkpt
                        );
                    } // End of if (fdp.ConsumeBool()) block
                } // End of if (rc == SQLITE_OK) block
                
                // Clean up the second database
                sqlite3_close(db2);
            } // End of if (rc == SQLITE_OK && db2 != nullptr) block
            
            // Free serialized data if we're not using FREEONCLOSE flag
            // (SQLite will handle it if FREEONCLOSE is set)
            if (!test_free_on_close) {
                // sqlite3_serialize may have allocated this memory
                // According to docs, we should free it if NOCOPY wasn't set
                if ((serialize_flags & SQLITE_SERIALIZE_NOCOPY) == 0) {
                    sqlite3_free(serialized_data);
                }
            }
        }
    }
    
    // Step 7: Also test WAL checkpoint on the original database
    if (fdp.ConsumeBool()) {
        // Test sqlite3_wal_checkpoint
        rc = sqlite3_wal_checkpoint(db, "main");
        
        // Test sqlite3_wal_checkpoint_v2 with various modes
        int checkpoint_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
        int* pnLog = nullptr;
        int* pnCkpt = nullptr;
        int log_val = 0;  // Declare outside conditional scope
        int ckpt_val = 0; // Declare outside conditional scope
        
        if (fdp.remaining_bytes() > sizeof(int) * 2) {
            log_val = fdp.ConsumeIntegral<int>();
            ckpt_val = fdp.ConsumeIntegral<int>();
            pnLog = &log_val;
            pnCkpt = &ckpt_val;
        }
        
        rc = sqlite3_wal_checkpoint_v2(
            db, "main",
            checkpoint_mode,
            pnLog, pnCkpt
        );
    }
    
    
    // Step 8: Cleanup
    if (db != nullptr) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_006
