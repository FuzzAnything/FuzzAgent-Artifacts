#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_001 {
/*
** SQLite advanced fuzzing harness with patterns from existing SQLite fuzzers
**
** This harness incorporates patterns from ossfuzz.c and dbfuzz2.c:
** - Progress handler with timeout mechanism
** - Various SQLite limits to prevent timeouts
** - Multiple operation modes based on fuzzer input
** - Prepared statements with parameter binding
** - Transaction handling
** - Error handling and cleanup
*/



// Simple callback function for sqlite3_exec
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Progress handler callback (pattern from ossfuzz.c and dbfuzz2.c)
static int progress_handler(void* pClientData) {
    static int callback_count = 0;
    callback_count++;
    
    // Simulate timeout check - return non-zero to abort if too many callbacks
    if (callback_count > 1000) {
        return 1;  // Abort
    }
    return 0;
}

// Authorizer callback to block debug pragmas (pattern from ossfuzz.c)
static int block_debug_pragmas(
    void* NotUsed,
    int eCode,
    const char* zArg1,
    const char* zArg2,
    const char* zArg3,
    const char* zArg4
) {
    (void)NotUsed;
    (void)zArg2;
    (void)zArg3;
    (void)zArg4;
    
    if (eCode == SQLITE_PRAGMA) {
        // Block vdbe_* and parser_trace pragmas
        if (zArg1 && (strncmp(zArg1, "vdbe_", 5) == 0 || strcmp(zArg1, "parser_trace") == 0)) {
            return SQLITE_DENY;
        }
    }
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check (pattern from ossfuzz.c)
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    char* err_msg = nullptr;
    
    // Extract configuration from fuzzer input (pattern from ossfuzz.c)
    uint8_t selector = 0;
    if (size >= 2 && data[1] == '\n') {
        // If second byte is newline, use first byte as selector
        selector = data[0];
    } else {
        selector = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Open in-memory database with various flags
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    // Add mutex flags based on selector
    if (selector & 0x01) {
        open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    if (selector & 0x02) {
        open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    if (selector & 0x04) {
        open_flags |= SQLITE_OPEN_URI;
    }
    
    // Open database
    int rc = sqlite3_open_v2(":memory:", &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set various limits to prevent timeouts (patterns from ossfuzz.c)
    int max_opcodes = fdp.ConsumeIntegralInRange<int>(1000, 50000);
    int max_length = fdp.ConsumeIntegralInRange<int>(1000, 50000);
    int like_pattern_length = fdp.ConsumeIntegralInRange<int>(50, 500);
    
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, max_opcodes);
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, max_length);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, like_pattern_length);
    
    // Set heap limit (pattern from ossfuzz.c)
    sqlite3_int64 heap_limit = fdp.ConsumeIntegralInRange<sqlite3_int64>(1000000, 50000000);
    sqlite3_hard_heap_limit64(heap_limit);
    
    // Configure database based on selector (pattern from ossfuzz.c)
    int fk_enabled = (selector >> 3) & 0x01;
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, fk_enabled, &rc);
    
    // Set authorizer to block debug pragmas (pattern from ossfuzz.c)
    sqlite3_set_authorizer(db, block_debug_pragmas, nullptr);
    
    // Install progress handler (patterns from ossfuzz.c and dbfuzz2.c)
    sqlite3_progress_handler(db, 10, progress_handler, nullptr);
    
    // Determine operation mode based on selector
    uint8_t operation_mode = selector >> 4;  // Use high 4 bits for operation mode
    
    // Test 1: Basic SQL operations with sqlite3_exec (pattern from dbfuzz2.c)
    if (operation_mode & 0x01) {
        // Fixed set of SQL statements similar to dbfuzz2.c
        const char* sql_statements[] = {
            "CREATE TABLE IF NOT EXISTS t1 (a INTEGER, b TEXT);",
            "CREATE TABLE IF NOT EXISTS t2 (x INTEGER, y REAL, z TEXT);",
            "INSERT INTO t1 VALUES (1, 'test1');",
            "INSERT INTO t1 VALUES (2, 'test2');",
            "INSERT INTO t2 VALUES (100, 3.14, 'pi');",
            "INSERT INTO t2 VALUES (200, 2.71, 'e');",
            "SELECT * FROM t1;",
            "SELECT * FROM t2;",
            "UPDATE t1 SET b = 'updated' WHERE a = 1;",
            "DELETE FROM t2 WHERE x = 100;",
            "PRAGMA integrity_check;",
            "VACUUM;"
        };
        
        for (size_t i = 0; i < sizeof(sql_statements) / sizeof(sql_statements[0]); i++) {
            sqlite3_exec(db, sql_statements[i], exec_callback, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Test 2: Prepared statements with parameter binding
    if (operation_mode & 0x02 && fdp.remaining_bytes() > 50) {
        // Create a table for parameterized operations
        sqlite3_exec(db, 
            "CREATE TABLE IF NOT EXISTS param_test ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "age INTEGER, "
            "score REAL, "
            "active INTEGER);",
            nullptr, nullptr, nullptr);
        
        // Prepare INSERT statement with parameters
        sqlite3_stmt* insert_stmt = nullptr;
        rc = sqlite3_prepare_v2(db,
            "INSERT INTO param_test (name, age, score, active) VALUES (?, ?, ?, ?);",
            -1, &insert_stmt, nullptr);
        
        if (rc == SQLITE_OK && insert_stmt != nullptr) {
            // Bind multiple sets of parameters from fuzzer input
            int num_inserts = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_inserts && fdp.remaining_bytes() > 20; i++) {
                std::string name = fdp.ConsumeRandomLengthString(50);
                int age = fdp.ConsumeIntegralInRange<int>(0, 100);
                double score = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                int active = fdp.ConsumeBool() ? 1 : 0;
                
                sqlite3_bind_text(insert_stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(insert_stmt, 2, age);
                sqlite3_bind_double(insert_stmt, 3, score);
                sqlite3_bind_int(insert_stmt, 4, active);
                
                sqlite3_step(insert_stmt);
                sqlite3_reset(insert_stmt);
            }
            
            sqlite3_finalize(insert_stmt);
        }
        
        // Prepare and execute SELECT statement
        sqlite3_stmt* select_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, "SELECT * FROM param_test;", -1, &select_stmt, nullptr);
        
        if (rc == SQLITE_OK && select_stmt != nullptr) {
            while (sqlite3_step(select_stmt) == SQLITE_ROW) {
                // Process row data
                int id = sqlite3_column_int(select_stmt, 0);
                const unsigned char* name = sqlite3_column_text(select_stmt, 1);
                int age = sqlite3_column_int(select_stmt, 2);
                double score = sqlite3_column_double(select_stmt, 3);
                int active = sqlite3_column_int(select_stmt, 4);
                
                (void)id;
                (void)name;
                (void)age;
                (void)score;
                (void)active;
            }
            sqlite3_finalize(select_stmt);
        }
    }
    
    // Test 3: Transaction handling with rollback/commit
    if (operation_mode & 0x04) {
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        
        // Perform some operations within transaction
        std::string trans_sql1 = "CREATE TABLE trans_test (id INTEGER, data TEXT);";
        sqlite3_exec(db, trans_sql1.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert some data
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
            std::string data = fdp.ConsumeRandomLengthString(20);
            std::string insert_sql = "INSERT INTO trans_test VALUES (" + 
                                    std::to_string(i) + ", '" + data + "');";
            sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Commit or rollback based on fuzzer input
        if (fdp.ConsumeBool()) {
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        } else {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
    }
    
    // Test 4: Complex queries with JOINs and aggregations
    if (operation_mode & 0x08 && fdp.remaining_bytes() > 100) {
        // Create tables for complex queries
        sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS employees ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "department_id INTEGER, "
            "salary REAL);",
            nullptr, nullptr, nullptr);
            
        sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS departments ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "budget REAL);",
            nullptr, nullptr, nullptr);
        
        // Prepare complex SELECT with JOIN
        sqlite3_stmt* complex_stmt = nullptr;
        rc = sqlite3_prepare_v2(db,
            "SELECT d.name, COUNT(e.id), AVG(e.salary) "
            "FROM departments d "
            "LEFT JOIN employees e ON d.id = e.department_id "
            "GROUP BY d.id;",
            -1, &complex_stmt, nullptr);
        
        if (rc == SQLITE_OK && complex_stmt != nullptr) {
            while (sqlite3_step(complex_stmt) == SQLITE_ROW) {
                // Process results
                const unsigned char* dept_name = sqlite3_column_text(complex_stmt, 0);
                int employee_count = sqlite3_column_int(complex_stmt, 1);
                double avg_salary = sqlite3_column_double(complex_stmt, 2);
                
                (void)dept_name;
                (void)employee_count;
                (void)avg_salary;
            }
            sqlite3_finalize(complex_stmt);
        }
    }
    
    // Test 5: SQL functions and pragmas
    if (fdp.remaining_bytes() > 10) {
        // Test various SQLite functions
        const char* function_tests[] = {
            "SELECT abs(-10), abs(10);",
            "SELECT length('test'), lower('TEST'), upper('test');",
            "SELECT random(), randomblob(10);",
            "SELECT datetime('now'), date('now'), time('now');",
            "PRAGMA cache_size;",
            "PRAGMA page_size;",
            "PRAGMA encoding;"
        };
        
        for (size_t i = 0; i < sizeof(function_tests) / sizeof(function_tests[0]); i++) {
            sqlite3_exec(db, function_tests[i], exec_callback, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Cleanup
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    if (db) {
        // Final cleanup similar to ossfuzz.c
        sqlite3_exec(db, "PRAGMA temp_store_directory=''", nullptr, nullptr, nullptr);
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_001
