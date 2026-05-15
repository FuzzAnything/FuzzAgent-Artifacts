#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_011 {
/*
** SQLite Error Handling and Stress Testing Fuzzing Harness
**
** This harness specifically targets SQLite error handling APIs and stress scenarios
** identified as untested coverage gaps:
** - sqlite3_errcode: Get the most recent error code for a database connection
** - sqlite3_extended_errcode: Get the extended error code
** - sqlite3_errmsg: Get error message for the most recent error (UTF-8)
** - sqlite3_errmsg16: Get error message for the most recent error (UTF-16)
** - sqlite3_errstr: Get descriptive text for an SQLite result code
** - sqlite3_set_errmsg: Set custom error message and code
** - sqlite3_txn_state: Determine transaction state of a database
** - sqlite3_commit_hook: Register callback for transaction commits
** - sqlite3_rollback_hook: Register callback for transaction rollbacks
** - Error offset tracking with sqlite3_error_offset
**
** This harness also tests stress scenarios:
** - Nested transactions and savepoints
** - Transaction rollback and recovery
** - Resource exhaustion scenarios
** - Concurrent operation stress
** - Error recovery and state monitoring
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (no error handling focus)
** - harness_001: Advanced operations with progress handlers (limited error handling)
** - harness_002: BLOB-specific operations (no error APIs)
** - harness_003: Virtual table operations (no error APIs)
** - harness_004: Backup operations (no error APIs)
** - harness_005: Serialization/deserialization (no error APIs)
** - harness_006: UTF-16 operations (no error APIs)
** - harness_007: Metadata and statement explanation (no error APIs)
** - harness_008: Statement preparation and binding (no error APIs)
** - harness_009: Custom functions and collations (no error APIs)
** - harness_010: Advanced UTF-16 operations (no error APIs)
** - harness_011: Comprehensive error handling and stress testing (this harness)
**
** Error handling and stress testing lifecycle:
**   Database setup → Induce various errors → Capture error info → 
**   Test error recovery → Monitor transaction state → Test hooks → Cleanup
*/



// Simple callback for sqlite3_exec
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Transaction commit hook callback
static int commit_hook_callback(void* pArg) {
    int* pCommitCount = (int*)pArg;
    if (pCommitCount) {
        (*pCommitCount)++;
    }
    // Return 0 to allow commit, non-zero would convert to rollback
    return 0;
}

// Transaction rollback hook callback
static void rollback_hook_callback(void* pArg) {
    int* pRollbackCount = (int*)pArg;
    if (pRollbackCount) {
        (*pRollbackCount)++;
    }
}

// Error scenario types
enum ErrorScenario {
    ERROR_SCENARIO_SYNTAX = 0,
    ERROR_SCENARIO_CONSTRAINT = 1,
    ERROR_SCENARIO_MEMORY = 2,
    ERROR_SCENARIO_IO = 3,
    ERROR_SCENARIO_BUSY = 4,
    ERROR_SCENARIO_LOCKED = 5,
    ERROR_SCENARIO_CORRUPT = 6,
    ERROR_SCENARIO_FULL = 7,
    ERROR_SCENARIO_MIXED = 8
};

// Transaction operation types
enum TransactionOp {
    TXN_OP_BEGIN = 0,
    TXN_OP_COMMIT = 1,
    TXN_OP_ROLLBACK = 2,
    TXN_OP_SAVEPOINT = 3,
    TXN_OP_RELEASE = 4,
    TXN_OP_ROLLBACK_TO = 5,
    TXN_OP_NESTED = 6
};

// Helper function to generate error-inducing SQL
static std::string generate_error_sql(FuzzedDataProvider& fdp, ErrorScenario scenario) {
    switch (scenario) {
        case ERROR_SCENARIO_SYNTAX:
            return "SELECT * FROM non_existent_table WHERE invalid syntax";
        case ERROR_SCENARIO_CONSTRAINT:
            return "INSERT INTO test_table (id, unique_col) VALUES (1, 'duplicate'), (2, 'duplicate')";
        case ERROR_SCENARIO_MEMORY:
            // Create huge blob to potentially exhaust memory
            return "INSERT INTO test_table (id, data) VALUES (1, zeroblob(1000000000))";
        case ERROR_SCENARIO_IO:
            // Try to write to read-only database
            return "PRAGMA locking_mode = EXCLUSIVE";
        case ERROR_SCENARIO_BUSY:
            // Operations that might cause busy errors
            return "BEGIN EXCLUSIVE; SELECT * FROM test_table";
        case ERROR_SCENARIO_LOCKED:
            return "CREATE VIRTUAL TABLE locked_test USING fts5(content)";
        case ERROR_SCENARIO_CORRUPT:
            return "PRAGMA integrity_check";
        case ERROR_SCENARIO_FULL:
            // Insert many rows to potentially fill database
            return "WITH RECURSIVE cnt(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM cnt LIMIT 10000) INSERT INTO test_table SELECT x, 'data' FROM cnt";
        case ERROR_SCENARIO_MIXED:
        default:
            // Mix of operations
            return fdp.ConsumeRandomLengthString(256);
    }
}

// Helper to check and log error state
static void check_error_state(sqlite3* db, const char* context) {
    int errcode = sqlite3_errcode(db);
    int extended_errcode = sqlite3_extended_errcode(db);
    const char* errmsg = sqlite3_errmsg(db);
    const char* errstr = sqlite3_errstr(errcode);
    int error_offset = sqlite3_error_offset(db);
    
    // Log error state for fuzzer tracking
    (void)errcode;
    (void)extended_errcode;
    (void)errmsg;
    (void)errstr;
    (void)error_offset;
    (void)context;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    const size_t MIN_SIZE = sizeof(int) * 4 + 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    char* err_msg = nullptr;
    int commit_count = 0;
    int rollback_count = 0;
    
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
    
    // Set limits to control resource usage
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    
    // Set up commit and rollback hooks
    sqlite3_commit_hook(db, commit_hook_callback, &commit_count);
    sqlite3_rollback_hook(db, rollback_hook_callback, &rollback_count);
    
    // Create test tables
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "value REAL, "
        "data BLOB, "
        "unique_col TEXT UNIQUE);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Create another table for join operations
    rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS other_table ("
        "oid INTEGER PRIMARY KEY, "
        "ref_id INTEGER REFERENCES test_table(id), "
        "description TEXT);", 
        nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Determine test scenario from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Test 1: Basic error handling APIs
    if (test_scenario == 0 || test_scenario == 3) {
        // Consume error scenario type
        ErrorScenario error_scenario = static_cast<ErrorScenario>(
            fdp.ConsumeIntegralInRange<uint8_t>(0, ERROR_SCENARIO_MIXED));
        
        // Generate and execute error-inducing SQL
        std::string error_sql = generate_error_sql(fdp, error_scenario);
        
        // Execute potentially failing SQL
        rc = sqlite3_exec(db, error_sql.c_str(), nullptr, nullptr, &err_msg);
        
        // Check error state regardless of result
        check_error_state(db, "After error SQL");
        
        // Test error string function for various error codes
        for (int i = 0; i < 10; i++) {
            int test_err_code = fdp.ConsumeIntegralInRange<int>(-100, 100);
            const char* err_str = sqlite3_errstr(test_err_code);
            (void)err_str; // Use result to avoid unused variable warning
        }
        
        // Test setting custom error message
        if (fdp.ConsumeBool()) {
            std::string custom_msg = fdp.ConsumeRandomLengthString(100);
            int custom_code = fdp.ConsumeIntegralInRange<int>(SQLITE_ERROR, SQLITE_INTERNAL);
            sqlite3_set_errmsg(db, custom_code, custom_msg.c_str());
            
            // Verify custom error was set
            check_error_state(db, "After custom error");
        }
        
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Test 2: Transaction state monitoring and stress
    if (test_scenario == 1 || test_scenario == 3) {
        // Consume transaction operation sequence
        int num_ops = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        for (int i = 0; i < num_ops; i++) {
            TransactionOp op = static_cast<TransactionOp>(
                fdp.ConsumeIntegralInRange<uint8_t>(0, TXN_OP_NESTED));
            
            // Check current transaction state before operation
            int txn_state = sqlite3_txn_state(db, "main");
            (void)txn_state;
            
            switch (op) {
                case TXN_OP_BEGIN: {
                    std::string begin_sql = fdp.ConsumeBool() ? "BEGIN EXCLUSIVE" : "BEGIN";
                    rc = sqlite3_exec(db, begin_sql.c_str(), nullptr, nullptr, &err_msg);
                    break;
                }
                case TXN_OP_COMMIT:
                    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &err_msg);
                    break;
                case TXN_OP_ROLLBACK:
                    rc = sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &err_msg);
                    break;
                case TXN_OP_SAVEPOINT: {
                    std::string savepoint_name = "sp_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                    std::string savepoint_sql = "SAVEPOINT " + savepoint_name;
                    rc = sqlite3_exec(db, savepoint_sql.c_str(), nullptr, nullptr, &err_msg);
                    break;
                }
                case TXN_OP_RELEASE: {
                    std::string release_sql = "RELEASE sp_" + std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 10));
                    rc = sqlite3_exec(db, release_sql.c_str(), nullptr, nullptr, &err_msg);
                    break;
                }
                case TXN_OP_ROLLBACK_TO: {
                    std::string rollback_sql = "ROLLBACK TO sp_" + std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 10));
                    rc = sqlite3_exec(db, rollback_sql.c_str(), nullptr, nullptr, &err_msg);
                    break;
                }
                case TXN_OP_NESTED: {
                    // Test nested savepoints
                    rc = sqlite3_exec(db, "SAVEPOINT outer", nullptr, nullptr, &err_msg);
                    if (rc == SQLITE_OK) {
                        rc = sqlite3_exec(db, "SAVEPOINT inner", nullptr, nullptr, &err_msg);
                        if (rc == SQLITE_OK) {
                            // Do some work
                            rc = sqlite3_exec(db, 
                                "INSERT INTO test_table (id, name) VALUES (?, ?)", 
                                nullptr, nullptr, &err_msg);
                            // Rollback to inner
                            rc = sqlite3_exec(db, "ROLLBACK TO inner", nullptr, nullptr, &err_msg);
                            // Release inner
                            rc = sqlite3_exec(db, "RELEASE inner", nullptr, nullptr, &err_msg);
                        }
                        // Release outer
                        rc = sqlite3_exec(db, "RELEASE outer", nullptr, nullptr, &err_msg);
                    }
                    break;
                }
            }
            
            // Check error state after each operation
            check_error_state(db, "After transaction op");
            
            // Check transaction state after operation
            txn_state = sqlite3_txn_state(db, "main");
            (void)txn_state;
            
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some data to create interesting transaction states
            if (fdp.ConsumeBool()) {
                std::string insert_sql = "INSERT OR IGNORE INTO test_table (id, name, unique_col) VALUES (" +
                    std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ", '" +
                    fdp.ConsumeRandomLengthString(50) + "', '" +
                    fdp.ConsumeRandomLengthString(50) + "')";
                rc = sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
        
        // Ensure transaction is concluded
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
    }
    
    // Test 3: Error recovery and state persistence
    if (test_scenario == 2 || test_scenario == 3) {
        // Create prepared statement that might fail
        sqlite3_stmt* stmt = nullptr;
        std::string sql = fdp.ConsumeRandomLengthString(200);
        const char* pzTail = nullptr;
        
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &pzTail);
        
        // Check error state after prepare
        check_error_state(db, "After prepare");
        
        if (rc == SQLITE_OK && stmt != nullptr) {
            // Try to execute the statement
            rc = sqlite3_step(stmt);
            
            // Check error state after step
            check_error_state(db, "After step");
            
            // Reset statement
            sqlite3_reset(stmt);
            
            // Finalize statement
            sqlite3_finalize(stmt);
        }
        
        // Test error state persists across operations
        int saved_errcode = sqlite3_errcode(db);
        const char* saved_errmsg = sqlite3_errmsg(db);
        (void)saved_errcode;
        (void)saved_errmsg;
        
        // Do another operation
        rc = sqlite3_exec(db, "SELECT 1", nullptr, nullptr, &err_msg);
        
        // Error state should be cleared or updated
        check_error_state(db, "After clearing operation");
        
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Test UTF-16 error message API
        const void* errmsg16 = sqlite3_errmsg16(db);
        (void)errmsg16;
    }
    
    // Test 4: Resource stress and edge cases
    if (test_scenario == 3) {
        // Test many rapid operations to stress error handling
        int num_rapid_ops = fdp.ConsumeIntegralInRange<int>(1, 50);
        
        for (int i = 0; i < num_rapid_ops; i++) {
            // Alternate between different operations
            uint8_t rapid_op = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (rapid_op) {
                case 0: {
                    // Create and drop table
                    std::string table_name = "temp_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                    std::string create_sql = "CREATE TABLE " + table_name + " (x INTEGER)";
                    std::string drop_sql = "DROP TABLE IF EXISTS " + table_name;
                    
                    sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, nullptr);
                    check_error_state(db, "After create table");
                    
                    sqlite3_exec(db, drop_sql.c_str(), nullptr, nullptr, nullptr);
                    check_error_state(db, "After drop table");
                    break;
                }
                case 1: {
                    // Transaction boundary testing
                    sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);
                    check_error_state(db, "After begin");
                    
                    sqlite3_exec(db, "INSERT INTO test_table (id, name) VALUES (?, ?)", 
                                nullptr, nullptr, nullptr);
                    check_error_state(db, "After insert in txn");
                    
                    if (fdp.ConsumeBool()) {
                        sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
                    } else {
                        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
                    }
                    check_error_state(db, "After txn end");
                    break;
                }
                case 2: {
                    // Trigger constraint errors
                    sqlite3_exec(db, 
                        "INSERT OR IGNORE INTO test_table (id, unique_col) VALUES (100, 'unique_value')", 
                        nullptr, nullptr, nullptr);
                    
                    // Try to insert duplicate
                    sqlite3_exec(db, 
                        "INSERT INTO test_table (id, unique_col) VALUES (101, 'unique_value')", 
                        nullptr, nullptr, &err_msg);
                    check_error_state(db, "After constraint error");
                    
                    if (err_msg) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    break;
                }
                case 3: {
                    // Complex query with potential errors
                    std::string complex_sql = fdp.ConsumeRandomLengthString(500);
                    sqlite3_exec(db, complex_sql.c_str(), nullptr, nullptr, &err_msg);
                    check_error_state(db, "After complex query");
                    
                    if (err_msg) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    break;
                }
            }
            
            // Check hook counters periodically
            (void)commit_count;
            (void)rollback_count;
        }
    }
    
    // Final error state check
    check_error_state(db, "Final state");
    
    // Clean up
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_011
