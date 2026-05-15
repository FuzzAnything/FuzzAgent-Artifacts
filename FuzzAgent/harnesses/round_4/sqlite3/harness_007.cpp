#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_007 {
/*
 * Fuzzing harness for SQLite library - targeting testing/debugging and status monitoring APIs
 * as identified by coverage analysis.
 * 
 * This harness targets testing and status introspection APIs:
 * - sqlite3_test_control (120 branches, 0% coverage) - testing/debugging interface
 * - sqlite3_db_status/sqlite3_db_status64 (108 combined branches, 0% coverage) - database status monitoring
 * - sqlite3_db_config (26 branches, 0% coverage) - database configuration
 * - sqlite3_stmt_status (8 branches, 0% coverage) - statement performance metrics
 * - sqlite3_stmt_explain (18 branches, 0% coverage) - prepared statement explanation
 * - Helper functions: sqlite3_open (0% coverage), sqlite3_close_v2 (0% coverage), sqlite3_prepare (0% coverage)
 * 
 * Following the invocation sequence:
 * 1. Call sqlite3_initialize for global initialization.
 * 2. Call sqlite3_open to create database connection (target uncovered API).
 * 3. Call sqlite3_prepare to create statement (target uncovered API).
 * 4. Call sqlite3_test_control with various opcodes for testing/debugging.
 * 5. Call sqlite3_db_status and sqlite3_db_status64 to query database status.
 * 6. Call sqlite3_db_config to configure database settings.
 * 7. Call sqlite3_stmt_status to get statement performance metrics.
 * 8. Call sqlite3_stmt_explain to get prepared statement explanation.
 * 9. Call sqlite3_close_v2 to clean up (target uncovered API).
 * 10. Call sqlite3_shutdown for global cleanup.
 * 
 * Semantic differentiation from existing harnesses:
 * - Focuses on testing/debugging and status monitoring vs prepared statements, BLOB ops, backup, virtual tables, serialization, WAL, UTF-16
 * - Targets internal testing interfaces (sqlite3_test_control) and introspection APIs
 * - Exercises database and statement performance monitoring paths
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Global initialization
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    
    // Step 2: Create database connection using sqlite3_open (not sqlite3_open_v2)
    // Consume database filename from fuzzer input
    std::string db_name = fdp.ConsumeRandomLengthString(64);
    if (db_name.empty()) {
        db_name = ":memory:";
    }
    
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 3: Create prepared statement
    std::string sql = fdp.ConsumeRandomLengthString(256);
    if (sql.empty()) {
        sql = "SELECT 1, 2, 3;";
    }
    
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare(db, sql.c_str(), -1, &stmt, &tail);
    
    // Step 4: Test sqlite3_test_control with various opcodes
    // Use fuzzer input to select test control opcodes
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int test_op = fdp.ConsumeIntegralInRange<int>(SQLITE_TESTCTRL_FIRST, SQLITE_TESTCTRL_LAST);
        
        // Try different test control operations based on opcode
        switch (test_op) {
            case SQLITE_TESTCTRL_PRNG_SAVE:
            case SQLITE_TESTCTRL_PRNG_RESTORE: {
                // These take no parameters
                sqlite3_test_control(test_op);
                break;
            }
            case SQLITE_TESTCTRL_PRNG_SEED: {
                // PRNG_SEED takes seed and optional db pointer
                int seed = fdp.ConsumeIntegral<int>();
                sqlite3_test_control(test_op, seed, db);
                break;
            }
            case SQLITE_TESTCTRL_OPTIMIZATIONS: {
                // SQLITE_TESTCTRL_OPTIMIZATIONS requires db and mask parameters
                int mask = fdp.ConsumeIntegral<int>();
                sqlite3_test_control(test_op, db, mask);
                break;
            }
            case SQLITE_TESTCTRL_ASSERT: {
                // SQLITE_TESTCTRL_ASSERT with X=0 triggers intentional assertion failure
                // Only call with enable=1 to avoid expected crashes
                sqlite3_test_control(test_op, 1);  // Always enable=1
                break;
            }
            case SQLITE_TESTCTRL_ALWAYS:
            case SQLITE_TESTCTRL_NEVER_CORRUPT:
            case SQLITE_TESTCTRL_EXTRA_SCHEMA_CHECKS: {
                int enable = fdp.ConsumeBool() ? 1 : 0;
                sqlite3_test_control(test_op, enable);
                break;
            }
            case SQLITE_TESTCTRL_BYTEORDER:
            case SQLITE_TESTCTRL_ISINIT: {
                // These return values
                int result = 0;
                sqlite3_test_control(test_op, &result);
                break;
            }
            case SQLITE_TESTCTRL_FK_NO_ACTION:
            case SQLITE_TESTCTRL_SORTER_MMAP: {
                int val = fdp.ConsumeIntegral<int>();
                sqlite3_test_control(test_op, db, val);
                break;
            }
            case SQLITE_TESTCTRL_INTERNAL_FUNCTIONS: {
                sqlite3_test_control(test_op, db);
                break;
            }
            case SQLITE_TESTCTRL_LOCALTIME_FAULT: {
                int val = fdp.ConsumeIntegralInRange<int>(0, 2);
                // We need a testLocaltime function pointer, use nullptr for now
                sqlite3_test_control(test_op, val, nullptr);
                break;
            }
            case SQLITE_TESTCTRL_TRACEFLAGS: {
                unsigned int flags = fdp.ConsumeIntegral<unsigned int>();
                sqlite3_test_control(test_op, 1, &flags);
                sqlite3_test_control(test_op, 0, &flags);
                break;
            }
            case SQLITE_TESTCTRL_IMPOSTER: {
                // Requires db, dbName, onOff, tnum - use simple values
                std::string dbName = fdp.ConsumeRandomLengthString(32);
                int onOff = fdp.ConsumeBool() ? 1 : 0;
                int tnum = fdp.ConsumeIntegral<int>();
                sqlite3_test_control(test_op, db, dbName.c_str(), onOff, tnum);
                break;
            }
            case SQLITE_TESTCTRL_ATOF: {
                // SQLITE_TESTCTRL_ATOF requires const char *z and double *p
                std::string test_str = fdp.ConsumeRandomLengthString(256);
                double result = 0.0;
                sqlite3_test_control(test_op, test_str.c_str(), &result);
                break;
            }
            default:
                // For opcodes that require arguments, we need to handle them properly
                // Some opcodes like SQLITE_TESTCTRL_ATOF require specific arguments
                // For safety, skip unhandled opcodes to avoid SEGV
                break;
        }
    }
    
    // Step 5: Query database status using sqlite3_db_status and sqlite3_db_status64
    if (fdp.remaining_bytes() >= sizeof(int)) {
        // Valid SQLITE_DBSTATUS opcodes are 0-13 inclusive
        int db_status_op = fdp.ConsumeIntegralInRange<int>(0, SQLITE_DBSTATUS_MAX);
        int current_val = 0;
        int highwater_val = 0;
        sqlite3_int64 current_val64 = 0;
        sqlite3_int64 highwater_val64 = 0;
        int reset_flag = fdp.ConsumeBool() ? 1 : 0;
        
        // Try both 32-bit and 64-bit versions
        sqlite3_db_status(db, db_status_op, &current_val, &highwater_val, reset_flag);
        sqlite3_db_status64(db, db_status_op, &current_val64, &highwater_val64, reset_flag);
        
        // Try a few specific status operations with different reset flags
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current_val, &highwater_val, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current_val, &highwater_val, 1);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_HIT, &current_val, &highwater_val, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_MISS, &current_val, &highwater_val, 1);
    }
    // Step 6: Configure database settings using sqlite3_db_config
    if (fdp.remaining_bytes() >= sizeof(int)) {
        // Valid SQLITE_DBCONFIG opcodes: 1000-1023 inclusive
        int valid_db_config_opcodes[] = {
            SQLITE_DBCONFIG_MAINDBNAME,           // 1000
            SQLITE_DBCONFIG_LOOKASIDE,            // 1001
            SQLITE_DBCONFIG_ENABLE_FKEY,          // 1002
            SQLITE_DBCONFIG_ENABLE_TRIGGER,       // 1003
            SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, // 1004
            SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, // 1005
            SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE,     // 1006
            SQLITE_DBCONFIG_ENABLE_QPSG,          // 1007
            SQLITE_DBCONFIG_TRIGGER_EQP,          // 1008
            SQLITE_DBCONFIG_RESET_DATABASE,       // 1009
            SQLITE_DBCONFIG_DEFENSIVE,            // 1010
            SQLITE_DBCONFIG_WRITABLE_SCHEMA,      // 1011
            SQLITE_DBCONFIG_LEGACY_ALTER_TABLE,   // 1012
            SQLITE_DBCONFIG_DQS_DML,              // 1013
            SQLITE_DBCONFIG_DQS_DDL,              // 1014
            SQLITE_DBCONFIG_ENABLE_VIEW,          // 1015
            SQLITE_DBCONFIG_LEGACY_FILE_FORMAT,   // 1016
            SQLITE_DBCONFIG_TRUSTED_SCHEMA,       // 1017
            SQLITE_DBCONFIG_STMT_SCANSTATUS,      // 1018
            SQLITE_DBCONFIG_REVERSE_SCANORDER,    // 1019
            SQLITE_DBCONFIG_ENABLE_ATTACH_CREATE, // 1020
            SQLITE_DBCONFIG_ENABLE_ATTACH_WRITE,  // 1021
            SQLITE_DBCONFIG_ENABLE_COMMENTS,      // 1022
            SQLITE_DBCONFIG_FP_DIGITS             // 1023
        };
        int num_valid_opcodes = sizeof(valid_db_config_opcodes) / sizeof(valid_db_config_opcodes[0]);
        
        // Pick a valid opcode from the array
        int db_config_op = valid_db_config_opcodes[fdp.ConsumeIntegralInRange<size_t>(0, num_valid_opcodes - 1)];
        
        switch (db_config_op) {
            case SQLITE_DBCONFIG_MAINDBNAME: {
                std::string maindb_name = fdp.ConsumeRandomLengthString(64);
                sqlite3_db_config(db, SQLITE_DBCONFIG_MAINDBNAME, maindb_name.c_str());
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_FKEY:
            case SQLITE_DBCONFIG_ENABLE_TRIGGER:
            case SQLITE_DBCONFIG_ENABLE_VIEW:
            case SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION:
            case SQLITE_DBCONFIG_DEFENSIVE:
            case SQLITE_DBCONFIG_WRITABLE_SCHEMA:
            case SQLITE_DBCONFIG_TRUSTED_SCHEMA: {
                int enable = fdp.ConsumeBool() ? 1 : 0;
                int prev_value = 0;
                sqlite3_db_config(db, db_config_op, enable, &prev_value);
                break;
            }
            case SQLITE_DBCONFIG_LOOKASIDE: {
                // Requires buffer parameters - use NULL for simplicity
                sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, nullptr, 0, 0);
                break;
            }
            default: {
                // Try with a simple integer parameter
                int param = fdp.ConsumeIntegral<int>();
                int prev_value = 0;
                sqlite3_db_config(db, db_config_op, param, &prev_value);
                break;
            }
        }
    }
    // Step 7: Get statement performance metrics using sqlite3_stmt_status
    if (stmt != nullptr && fdp.remaining_bytes() >= sizeof(int)) {
        // Valid SQLITE_STMTSTATUS opcodes: 1-8, 99
        // Array of valid stmt status opcodes
        int valid_stmt_status_opcodes[] = {
            SQLITE_STMTSTATUS_FULLSCAN_STEP,    // 1
            SQLITE_STMTSTATUS_SORT,             // 2
            SQLITE_STMTSTATUS_AUTOINDEX,        // 3
            SQLITE_STMTSTATUS_VM_STEP,          // 4
            SQLITE_STMTSTATUS_REPREPARE,        // 5
            SQLITE_STMTSTATUS_RUN,              // 6
            SQLITE_STMTSTATUS_FILTER_MISS,      // 7
            SQLITE_STMTSTATUS_FILTER_HIT,       // 8
            SQLITE_STMTSTATUS_MEMUSED           // 99
        };
        int num_valid_opcodes = sizeof(valid_stmt_status_opcodes) / sizeof(valid_stmt_status_opcodes[0]);
        
        // Pick a valid opcode from the array
        int stmt_status_op = valid_stmt_status_opcodes[fdp.ConsumeIntegralInRange<size_t>(0, num_valid_opcodes - 1)];
        int reset_flag = fdp.ConsumeBool() ? 1 : 0;
        
        // Call stmt_status for various operations
        sqlite3_stmt_status(stmt, stmt_status_op, reset_flag);
        
        // Try a few specific status operations
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_SORT, 0);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_VM_STEP, 1);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_RUN, 0);
        
        // Step the statement to generate some activity
        if (reset_flag) {
            sqlite3_reset(stmt);
        }
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Get status after stepping
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_VM_STEP, 0);
    }
    // Step 8: Get prepared statement explanation using sqlite3_stmt_explain
    if (stmt != nullptr && fdp.remaining_bytes() >= sizeof(int)) {
        int explain_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
        
        // Try different explanation modes
        sqlite3_stmt_explain(stmt, explain_mode);
        
        // Try specific modes
        sqlite3_stmt_explain(stmt, 0);  // Normal mode
        sqlite3_stmt_explain(stmt, 1);  // EXPLAIN mode
        sqlite3_stmt_explain(stmt, 2);  // EXPLAIN QUERY PLAN mode
        
        // Reset to normal mode
        sqlite3_stmt_explain(stmt, 0);
    }
    
    // Step 9: Clean up statement if created
    if (stmt != nullptr) {
        sqlite3_finalize(stmt);
    }
    
    // Step 10: Close database using sqlite3_close_v2
    if (db != nullptr) {
        sqlite3_close_v2(db);
    }
    
    // Final global cleanup
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_007
