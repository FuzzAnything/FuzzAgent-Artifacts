#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_002 {
/*
 * Fuzzing harness for SQLite transaction control, schema manipulation, and blob I/O APIs.
 * Targets:
 * - Transaction control: BEGIN, COMMIT, ROLLBACK, SAVEPOINT, RELEASE SAVEPOINT via sqlite3_exec
 * - Schema manipulation: CREATE INDEX, ALTER TABLE, CREATE VIEW, CREATE TRIGGER, DROP statements
 * - Blob I/O APIs: sqlite3_blob_open, sqlite3_blob_read, sqlite3_blob_write, sqlite3_blob_bytes, sqlite3_blob_close
 * 
 * This harness complements harness_000.cpp (basic SQL) and harness_001.cpp (binding/column APIs)
 * by targeting uncovered areas: transaction control and schema manipulation.
 * Uses FuzzedDataProvider to consume fuzzer input for SQL statements and blob operations.
 */



/*
 * Simple callback for sqlite3_exec that does nothing.
 */
static int exec_callback(void* NotUsed, int argc, char** argv, char** col_names) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

/*
 * Main fuzzer entry point.
 * Tests transaction control, schema manipulation, and blob I/O APIs.
 */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Open in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Create a test table for blob operations and schema manipulation
    char* err_msg = nullptr;
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data TEXT, "
        "blob_data BLOB"
        ");";
    
    rc = sqlite3_exec(db, create_table_sql, exec_callback, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert some test data using fuzzer input
    std::string test_data = fdp.ConsumeRandomLengthString(256);
    if (test_data.empty()) {
        test_data = "test";
    }
    
    // Consume blob size from fuzzer input
    int blob_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
    
    // Construct INSERT SQL with consumed data
    std::string insert_sql = "INSERT INTO test_table(data, blob_data) VALUES('" + 
                             test_data + "', zeroblob(" + std::to_string(blob_size) + "));";
    rc = sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (operation) {
        case 0: {
            // Test transaction control statements
            uint8_t tx_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            // Consume SQL statements for transaction operations
            std::string sql1 = fdp.ConsumeRandomLengthString(512);
            std::string sql2 = fdp.ConsumeRandomLengthString(512);
            
            if (!sql1.empty()) {
                // Begin transaction based on tx_type
                switch (tx_type) {
                    case 0:
                        sqlite3_exec(db, "BEGIN;", exec_callback, nullptr, &err_msg);
                        break;
                    case 1:
                        sqlite3_exec(db, "BEGIN TRANSACTION;", exec_callback, nullptr, &err_msg);
                        break;
                    case 2:
                        sqlite3_exec(db, "BEGIN DEFERRED;", exec_callback, nullptr, &err_msg);
                        break;
                    case 3:
                        sqlite3_exec(db, "BEGIN IMMEDIATE;", exec_callback, nullptr, &err_msg);
                        break;
                    case 4:
                        sqlite3_exec(db, "BEGIN EXCLUSIVE;", exec_callback, nullptr, &err_msg);
                        break;
                }
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Execute first SQL statement
                rc = sqlite3_exec(db, sql1.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Execute second SQL statement
                if (!sql2.empty()) {
                    rc = sqlite3_exec(db, sql2.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test savepoints
                if (fdp.ConsumeBool()) {
                    sqlite3_exec(db, "SAVEPOINT sp1;", exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Consume another SQL for savepoint transaction
                    std::string sql3 = fdp.ConsumeRandomLengthString(256);
                    if (!sql3.empty()) {
                        sqlite3_exec(db, sql3.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                    
                    // Release or rollback savepoint based on fuzzer input
                    if (fdp.ConsumeBool()) {
                        sqlite3_exec(db, "RELEASE SAVEPOINT sp1;", exec_callback, nullptr, &err_msg);
                    } else {
                        sqlite3_exec(db, "ROLLBACK TO SAVEPOINT sp1;", exec_callback, nullptr, &err_msg);
                    }
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Commit or rollback based on fuzzer input
                if (fdp.ConsumeBool()) {
                    sqlite3_exec(db, "COMMIT;", exec_callback, nullptr, &err_msg);
                } else {
                    sqlite3_exec(db, "ROLLBACK;", exec_callback, nullptr, &err_msg);
                }
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            break;
        }
        
        case 1: {
            // Test schema manipulation statements
            uint8_t schema_op = fdp.ConsumeIntegral<uint8_t>() % 6;
            
            // Consume names for schema objects
            std::string obj_name = fdp.ConsumeRandomLengthString(64);
            if (obj_name.empty()) {
                obj_name = "test_obj";
            }
            
            std::string col_name = fdp.ConsumeRandomLengthString(32);
            if (col_name.empty()) {
                col_name = "test_col";
            }
            
            switch (schema_op) {
                case 0: {
                    // CREATE INDEX
                    std::string index_sql = "CREATE INDEX IF NOT EXISTS " + obj_name + 
                                           "_idx ON test_table(data);";
                    rc = sqlite3_exec(db, index_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
                case 1: {
                    // ALTER TABLE (RENAME COLUMN - SQLite 3.25.0+)
                    std::string alter_sql = "ALTER TABLE test_table RENAME COLUMN data TO " + 
                                           col_name + "_renamed;";
                    rc = sqlite3_exec(db, alter_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
                case 2: {
                    // CREATE VIEW
                    std::string view_sql = "CREATE VIEW IF NOT EXISTS " + obj_name + 
                                          "_view AS SELECT id, data FROM test_table;";
                    rc = sqlite3_exec(db, view_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
                case 3: {
                    // CREATE TRIGGER
                    std::string trigger_value = fdp.ConsumeRandomLengthString(32);
                    if (trigger_value.empty()) {
                        trigger_value = "triggered";
                    }
                    std::string trigger_sql = 
                        "CREATE TRIGGER IF NOT EXISTS " + obj_name + "_trigger "
                        "AFTER INSERT ON test_table "
                        "BEGIN "
                        "  UPDATE test_table SET data = '" + trigger_value + "' WHERE id = NEW.id; "
                        "END;";
                    rc = sqlite3_exec(db, trigger_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
                case 4: {
                    // DROP INDEX (if exists from case 0)
                    std::string drop_sql = "DROP INDEX IF EXISTS " + obj_name + "_idx;";
                    rc = sqlite3_exec(db, drop_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
                case 5: {
                    // DROP VIEW (if exists from case 2)
                    std::string drop_sql = "DROP VIEW IF EXISTS " + obj_name + "_view;";
                    rc = sqlite3_exec(db, drop_sql.c_str(), exec_callback, nullptr, &err_msg);
                    break;
                }
            }
            
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            break;
        }
        
        case 2: {
            // Test blob I/O APIs
            sqlite3_blob* blob = nullptr;
            
            // Open blob handle
            rc = sqlite3_blob_open(db, "main", "test_table", "blob_data", 1, 1, &blob);
            
            if (rc == SQLITE_OK && blob != nullptr) {
                // Get blob size
                int blob_size = sqlite3_blob_bytes(blob);
                
                // Consume data for blob operations
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(1024, blob_size));
                size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, std::max(0, blob_size - (int)read_size));
                
                if (read_size > 0 && offset + read_size <= (size_t)blob_size) {
                    // Allocate buffer for reading
                    std::vector<uint8_t> read_buffer(read_size);
                    
                    // Read from blob
                    rc = sqlite3_blob_read(blob, read_buffer.data(), read_size, offset);
                    
                    // If we have enough fuzzer input, test writing
                    if (fdp.remaining_bytes() >= read_size) {
                        // Consume write data from fuzzer input
                        std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(read_size);
                        
                        // Write to blob
                        rc = sqlite3_blob_write(blob, write_data.data(), write_data.size(), offset);
                    }
                    
                    // Test blob_reopen if we have another row
                    if (fdp.ConsumeBool()) {
                        // Consume data for second row from fuzzer input
                        std::string another_data = fdp.ConsumeRandomLengthString(128);
                        if (another_data.empty()) {
                            another_data = "another";
                        }
                        int another_blob_size = fdp.ConsumeIntegralInRange<int>(1, 2048);
                        
                        // Insert another row for reopening test using consumed data
                        std::string insert_another = "INSERT INTO test_table(data, blob_data) VALUES('" + 
                                                    another_data + "', zeroblob(" + std::to_string(another_blob_size) + "));";
                        sqlite3_exec(db, insert_another.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                        
                        // Try to reopen blob to new row (rowid 2)
                        rc = sqlite3_blob_reopen(blob, 2);
                    }
                }
                
                // Close blob handle
                sqlite3_blob_close(blob);
            }
            break;
        }
    }
    
    // Clean up: drop test table
    rc = sqlite3_exec(db, "DROP TABLE IF EXISTS test_table;", exec_callback, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Drop any views/indexes created
    rc = sqlite3_exec(db, "DROP VIEW IF EXISTS test_obj_view;", exec_callback, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
    }
    
    // Close database
    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
    }
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_002
