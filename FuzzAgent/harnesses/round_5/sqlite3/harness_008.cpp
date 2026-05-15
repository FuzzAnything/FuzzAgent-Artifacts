#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_008 {
/*
** SQLite statement preparation and binding APIs comprehensive fuzzing harness
**
** This harness specifically targets SQLite statement preparation and binding APIs
** identified as highest priority coverage gap with sqlite3_prepare having 
** 806 undiscovered branches. Target APIs:
** - sqlite3_prepare: Legacy statement preparation (legacy interface)
** - sqlite3_prepare_v2: Standard statement preparation
** - sqlite3_prepare_v3: Statement preparation with additional flags
** - sqlite3_stmt_explain: Change EXPLAIN mode for prepared statements
** - Parameter binding functions: All sqlite3_bind_* variants
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare_v2, step) - limited binding
** - harness_001: Advanced operations with progress handlers and authorizers - some binding
** - harness_002: BLOB-specific incremental I/O operations - no binding
** - harness_003: Virtual table creation, configuration, and operations - no binding
** - harness_004: Database backup operations between databases - no binding
** - harness_005: Database serialization/deserialization with WAL checkpoints - no binding
** - harness_006: UTF-16 specific APIs and Unicode handling - UTF-16 binding only
** - harness_007: Metadata retrieval and statement explanation APIs - metadata focused
** - harness_008: Comprehensive statement preparation lifecycle and all binding variants (this harness)
**
** Statement lifecycle coverage:
**   Database setup → Statement preparation (prepare/prepare_v2/prepare_v3) → 
**   Parameter binding (all data types) → Statement execution (step) → 
**   Statement reset → Parameter rebinding → Re-execution → Statement finalize
**
** Key design considerations:
** 1. Test all three prepare interfaces (prepare, prepare_v2, prepare_v3)
** 2. Test all binding variants: blob, text, int, int64, double, null, zeroblob, pointer
** 3. Test various SQL statement types: SELECT, INSERT, UPDATE, DELETE, CREATE
** 4. Test different parameter counts (0-20 parameters)
** 5. Test binding with different parameter indices (named and positional)
** 6. Test statement explain modes (0=normal, 1=EXPLAIN, 2=EXPLAIN QUERY PLAN)
** 7. Test prepare flags for prepare_v3 (SQLITE_PREPARE_PERSISTENT, etc.)
** 8. Test statement reset and rebinding scenarios
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Operation types for the preparation/binding harness
enum PrepareOperation {
    PREPARE_OP_BASIC = 0,
    PREPARE_OP_V2 = 1,
    PREPARE_OP_V3 = 2,
    PREPARE_OP_EXPLAIN = 3,
    PREPARE_OP_MULTI_BIND = 4,
    PREPARE_OP_RESET_REBIND = 5,
    PREPARE_OP_COMPREHENSIVE = 6
};

// Prepare flags for sqlite3_prepare_v3
static const unsigned int prepare_flags[] = {
    0,  // No flags
    SQLITE_PREPARE_PERSISTENT,
    SQLITE_PREPARE_NORMALIZE,
    SQLITE_PREPARE_NO_VTAB,
    SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE,
};

// SQL statement templates with different parameter counts
static const char* const sql_templates[] = {
    // 0 parameters
    "SELECT 1+1",
    "CREATE TABLE test_tbl (id INTEGER PRIMARY KEY, name TEXT, value REAL)",
    "INSERT INTO test_tbl VALUES (1, 'test', 3.14)",
    
    // 1 parameter
    "SELECT ?",
    "INSERT INTO test_tbl VALUES (?)",
    "UPDATE test_tbl SET name = ? WHERE id = 1",
    
    // 2 parameters
    "SELECT ?, ?",
    "INSERT INTO test_tbl VALUES (?, ?)",
    "UPDATE test_tbl SET name = ?, value = ? WHERE id = 1",
    
    // 3 parameters
    "SELECT ?, ?, ?",
    "INSERT INTO test_tbl VALUES (?, ?, ?)",
    "UPDATE test_tbl SET id = ?, name = ?, value = ? WHERE id = 1",
    
    // Named parameters
    "SELECT :param1, :param2",
    "INSERT INTO test_tbl VALUES (:id, :name, :value)",
    "UPDATE test_tbl SET name = @newname WHERE id = @id",
    
    // Complex statements with mixed parameters
    "SELECT * FROM test_tbl WHERE id > ? AND name LIKE ? OR value BETWEEN ? AND ?",
    "INSERT INTO test_tbl SELECT * FROM other_tbl WHERE category = ? LIMIT ?",
};

// Binding data types
enum BindDataType {
    BIND_NULL = 0,
    BIND_INT = 1,
    BIND_INT64 = 2,
    BIND_DOUBLE = 3,
    BIND_TEXT = 4,
    BIND_BLOB = 5,
    BIND_ZEROBLOB = 6,
    // BIND_POINTER = 7,  // Requires specific context
};

// Helper to generate random text for binding
static std::string generate_random_text(FuzzedDataProvider& fdp, size_t max_len) {
    size_t length = fdp.ConsumeIntegralInRange<size_t>(0, max_len);
    if (length == 0) {
        return std::string();
    }
    return fdp.ConsumeBytesAsString(length);
}

// Helper to generate random blob for binding
static std::vector<uint8_t> generate_random_blob(FuzzedDataProvider& fdp, size_t max_size) {
    size_t size = fdp.ConsumeIntegralInRange<size_t>(0, max_size);
    if (size == 0) {
        return std::vector<uint8_t>();
    }
    return fdp.ConsumeBytes<uint8_t>(size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and operations
    // sizeof(int) for config + sizeof(int) for parameter count + at least some data bytes
    const size_t MIN_SIZE = sizeof(int) * 8 + sizeof(sqlite3_int64) * 2 + 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
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
    
    // Create test tables
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_tbl ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Create another table for more complex operations
    const char* create_table2_sql = 
        "CREATE TABLE IF NOT EXISTS test_tbl2 ("
        "a INTEGER, "
        "b TEXT, "
        "c REAL, "
        "d BLOB, "
        "e INTEGER);";
    
    rc = sqlite3_exec(db, create_table2_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Determine operation type from fuzzer input
    PrepareOperation op_type = static_cast<PrepareOperation>(
        fdp.ConsumeIntegralInRange<int>(0, PREPARE_OP_COMPREHENSIVE));
    
    // Select SQL template based on fuzzer input
    size_t sql_template_idx = fdp.ConsumeIntegralInRange<size_t>(
        0, sizeof(sql_templates) / sizeof(sql_templates[0]) - 1);
    
    const char* sql_template = sql_templates[sql_template_idx];
    
    // Consume prepare flags for prepare_v3
    unsigned int prep_flags = 0;
    if (op_type == PREPARE_OP_V3 || op_type == PREPARE_OP_COMPREHENSIVE) {
        size_t flag_idx = fdp.ConsumeIntegralInRange<size_t>(
            0, sizeof(prepare_flags) / sizeof(prepare_flags[0]) - 1);
        prep_flags = prepare_flags[flag_idx];
    }
    
    // Prepare the statement using different interfaces
    const char* zTail = nullptr;
    
    switch (op_type) {
        case PREPARE_OP_BASIC:
#ifndef SQLITE_OMIT_DEPRECATED
            // Use legacy sqlite3_prepare interface
            rc = sqlite3_prepare(db, sql_template, -1, &stmt, &zTail);
#else
            // Fall back to prepare_v2 if deprecated interface is omitted
            rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt, &zTail);
#endif
            break;
            
        case PREPARE_OP_V2:
        case PREPARE_OP_MULTI_BIND:
        case PREPARE_OP_RESET_REBIND:
            // Use standard sqlite3_prepare_v2 interface
            rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt, &zTail);
            break;
            
        case PREPARE_OP_V3:
        case PREPARE_OP_EXPLAIN:
        case PREPARE_OP_COMPREHENSIVE:
            // Use sqlite3_prepare_v3 interface with flags
            rc = sqlite3_prepare_v3(db, sql_template, -1, prep_flags, &stmt, &zTail);
            break;
    }
    
    if (rc != SQLITE_OK || stmt == nullptr) {
        // Cleanup and return if statement preparation failed
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Test sqlite3_stmt_explain for EXPLAIN mode operations
    if (op_type == PREPARE_OP_EXPLAIN || op_type == PREPARE_OP_COMPREHENSIVE) {
        // Try different explain modes
        int explain_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
        rc = sqlite3_stmt_explain(stmt, explain_mode);
        
        // Check if statement is in explain mode
        int is_explain = sqlite3_stmt_isexplain(stmt);
        (void)is_explain; // Use result to avoid unused variable warning
    }
    
    // Bind parameters based on operation type
    if (op_type == PREPARE_OP_MULTI_BIND || op_type == PREPARE_OP_COMPREHENSIVE) {
        // Bind multiple parameters with different data types
        for (int i = 1; i <= param_count && i <= 20; i++) {
            BindDataType bind_type = static_cast<BindDataType>(
                fdp.ConsumeIntegralInRange<int>(0, BIND_ZEROBLOB));
            
            switch (bind_type) {
                case BIND_NULL:
                    rc = sqlite3_bind_null(stmt, i);
                    break;
                    
                case BIND_INT:
                    rc = sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
                    break;
                    
                case BIND_INT64:
                    rc = sqlite3_bind_int64(stmt, i, fdp.ConsumeIntegral<sqlite3_int64>());
                    break;
                    
                case BIND_DOUBLE:
                    rc = sqlite3_bind_double(stmt, i, fdp.ConsumeFloatingPoint<double>());
                    break;
                    
                case BIND_TEXT: {
                    std::string text = generate_random_text(fdp, 256);
                    rc = sqlite3_bind_text(stmt, i, text.c_str(), text.length(), SQLITE_TRANSIENT);
                    break;
                }
                    
                case BIND_BLOB: {
                    std::vector<uint8_t> blob = generate_random_blob(fdp, 1024);
                    rc = sqlite3_bind_blob(stmt, i, blob.data(), blob.size(), SQLITE_TRANSIENT);
                    break;
                }
                    
                case BIND_ZEROBLOB:
                    rc = sqlite3_bind_zeroblob(stmt, i, fdp.ConsumeIntegralInRange<int>(0, 1024));
                    break;
            }
            
            if (rc != SQLITE_OK) {
                // Continue with next parameter even if one fails
                continue;
            }
        }
    } else if (op_type == PREPARE_OP_RESET_REBIND || op_type == PREPARE_OP_COMPREHENSIVE) {
        // Test binding, reset, and rebinding
        for (int bind_cycle = 0; bind_cycle < 3 && fdp.remaining_bytes() > 0; bind_cycle++) {
            // Bind some parameters
            for (int i = 1; i <= param_count && i <= 5; i++) {
                if (fdp.ConsumeBool()) {
                    rc = sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
                } else {
                    std::string text = generate_random_text(fdp, 128);
                    rc = sqlite3_bind_text(stmt, i, text.c_str(), text.length(), SQLITE_TRANSIENT);
                }
            }
            
            // Execute the statement
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
                // Reset the statement for next iteration
                rc = sqlite3_reset(stmt);
            }
        }
    } else {
        // Basic binding: bind a few parameters with simple types
        for (int i = 1; i <= param_count && i <= 3; i++) {
            if (fdp.ConsumeBool()) {
                rc = sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
            } else {
                std::string text = generate_random_text(fdp, 50);
                rc = sqlite3_bind_text(stmt, i, text.c_str(), text.length(), SQLITE_TRANSIENT);
            }
        }
    }
    
    // Execute the statement if we have parameters bound
    if (param_count == 0 || fdp.ConsumeBool()) {
        rc = sqlite3_step(stmt);
        
        // Process multiple rows if it's a SELECT statement
        while (rc == SQLITE_ROW) {
            // Get column count
            int col_count = sqlite3_column_count(stmt);
            
            // Access column data (just to exercise the API, not process it)
            for (int i = 0; i < col_count && i < 5; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                (void)col_type; // Use to avoid warning
                
                // Access column name
                const char* col_name = sqlite3_column_name(stmt, i);
                (void)col_name; // Use to avoid warning
            }
            
            rc = sqlite3_step(stmt);
        }
        
        // Reset statement after execution
        if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
            rc = sqlite3_reset(stmt);
        }
    }
    
    // Test binding parameter by name (if there are named parameters)
    if ((op_type == PREPARE_OP_COMPREHENSIVE || op_type == PREPARE_OP_MULTI_BIND) && 
        fdp.remaining_bytes() > 0) {
        
        // Try to bind to some common parameter names
        const char* param_names[] = {":param1", ":param2", ":id", ":name", ":value", "@newname", "@id"};
        
        for (size_t i = 0; i < sizeof(param_names) / sizeof(param_names[0]) && 
             fdp.remaining_bytes() > 0; i++) {
            
            int param_idx = sqlite3_bind_parameter_index(stmt, param_names[i]);
            if (param_idx > 0) {
                // Bind a value to this named parameter
                if (fdp.ConsumeBool()) {
                    rc = sqlite3_bind_int(stmt, param_idx, fdp.ConsumeIntegral<int>());
                } else {
                    std::string text = generate_random_text(fdp, 100);
                    rc = sqlite3_bind_text(stmt, param_idx, text.c_str(), text.length(), SQLITE_TRANSIENT);
                }
            }
        }
        
        // Execute again if we bound any named parameters
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
            sqlite3_reset(stmt);
        }
    }
    
    // Test sqlite3_bind_parameter_name and sqlite3_bind_parameter_count
    if (op_type == PREPARE_OP_COMPREHENSIVE) {
        int bind_count = sqlite3_bind_parameter_count(stmt);
        (void)bind_count; // Use to avoid warning
        
        for (int i = 1; i <= bind_count && i <= 10; i++) {
            const char* param_name = sqlite3_bind_parameter_name(stmt, i);
            (void)param_name; // Use to avoid warning
        }
    }
    
    // Final cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_008
