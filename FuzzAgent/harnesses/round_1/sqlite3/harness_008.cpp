#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <strings.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_008 {
/*
 * Fuzzing harness for SQLite advanced features with semantic diversity
 * Targets uncovered APIs from existing harnesses:
 * 1) Window functions (OVER clause, ranking functions, sqlite3_create_window_function)
 * 2) JSON extension functions (json_object, json_array, json_extract, json_*)
 * 3) R*Tree module spatial queries  
 * 4) Advanced FTS5 features (special commands, auxiliary functions)
 * 5) Collation functions and user-defined collations (sqlite3_create_collation)
 * 6) Progress handler and interrupt callback (sqlite3_progress_handler, sqlite3_interrupt)
 * 
 * This harness targets SQLite features not covered by existing harnesses:
 * - harness_000: Core UTF-8 APIs (prepared statements, exec)
 * - harness_001: Database file parsing (deserialization)
 * - harness_002: BLOB operations
 * - harness_003: UTF-16/Unicode specific APIs
 * - harness_004: Database backup operations
 * - harness_005: Table data retrieval (get_table/free_table)
 * - harness_006: Database serialization/deserialization APIs
 * - harness_007: VACUUM/transactions/virtual tables/FTS
 * 
 * Semantic differentiation: Comprehensive testing of window functions, JSON extensions,
 * R*Tree spatial queries, FTS5 advanced features, collation APIs, and progress handlers.
 */



// Window function callbacks for sqlite3_create_window_function
static void window_step_callback(sqlite3_context* ctx, int nArgs, sqlite3_value** argv) {
    // Simple window function that sums values
    sqlite3_int64* pSum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum == nullptr) return;
    
    if (nArgs > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        *pSum += sqlite3_value_int64(argv[0]);
    }
}

static void window_final_callback(sqlite3_context* ctx) {
    sqlite3_int64* pSum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum != nullptr) {
        sqlite3_result_int64(ctx, *pSum);
    } else {
        sqlite3_result_int64(ctx, 0);
    }
}

static void window_value_callback(sqlite3_context* ctx) {
    // Return current value for window function
    sqlite3_int64* pSum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum != nullptr) {
        sqlite3_result_int64(ctx, *pSum);
    } else {
        sqlite3_result_int64(ctx, 0);
    }
}

static void window_inverse_callback(sqlite3_context* ctx, int nArgs, sqlite3_value** argv) {
    // Inverse operation for window function
    sqlite3_int64* pSum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (pSum != nullptr && nArgs > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        *pSum -= sqlite3_value_int64(argv[0]);
    }
}

// Custom collation function
static int custom_collation(void* pArg, int n1, const void* p1, int n2, const void* p2) {
    // Simple case-insensitive collation
    int min_len = n1 < n2 ? n1 : n2;
    int cmp = strncasecmp((const char*)p1, (const char*)p2, min_len);
    if (cmp != 0) return cmp;
    return n1 - n2;
}

// Progress handler callback
static int progress_handler_callback(void* pArg) {
    int* pCounter = (int*)pArg;
    if (pCounter != nullptr) {
        (*pCounter)++;
        // Return non-zero to interrupt after certain number of callbacks
        return (*pCounter > 1000) ? 1 : 0;
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of multiple advanced features
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool test_window_functions = fdp.ConsumeBool();
    bool test_json_functions = fdp.ConsumeBool();
    bool test_rtree = fdp.ConsumeBool();
    bool test_fts5 = fdp.ConsumeBool();
    bool test_collations = fdp.ConsumeBool();
    bool test_progress_handler = fdp.ConsumeBool();
    bool test_interrupt = fdp.ConsumeBool();
    
    // Feature-specific parameters
    int window_func_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    int json_func_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    int rtree_dimensions = fdp.ConsumeIntegralInRange<int>(2, 5);
    int fts5_command_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    int collation_type = fdp.ConsumeIntegralInRange<int>(0, 2);
    int progress_frequency = fdp.ConsumeIntegralInRange<int>(1, 1000);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    // Use in-memory database to avoid file system issues
    const char* db_name = ":memory:";
    
    // Open database connection
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    int rc = sqlite3_open_v2(db_name, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 50);
    sqlite3_limit(db, SQLITE_LIMIT_EXPR_DEPTH, 10);
    
    char* err_msg = nullptr;
    int progress_counter = 0;
    
    // Set progress handler if requested
    if (test_progress_handler) {
        sqlite3_progress_handler(db, progress_frequency, progress_handler_callback, &progress_counter);
    }
    
    // Step 1: Create test tables for various features
    const char* create_tables[] = {
        "CREATE TABLE IF NOT EXISTS test_data (id INTEGER PRIMARY KEY, name TEXT, value REAL, group_id INTEGER);",
        "CREATE TABLE IF NOT EXISTS test_json (id INTEGER PRIMARY KEY, json_data TEXT);",
        "CREATE TABLE IF NOT EXISTS test_spatial (id INTEGER PRIMARY KEY, x REAL, y REAL, z REAL);"
    };
    
    for (size_t i = 0; i < sizeof(create_tables)/sizeof(create_tables[0]); i++) {
        sqlite3_exec(db, create_tables[i], nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Insert some test data
    if (fdp.remaining_bytes() > 100) {
        // Insert sample data for window function testing
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 20; i++) {
            std::string name = fdp.ConsumeRandomLengthString(20);
            double value = fdp.ConsumeFloatingPoint<double>();
            int group_id = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            sqlite3_stmt* stmt = nullptr;
            const char* insert_sql = "INSERT INTO test_data (id, name, value, group_id) VALUES (?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_int(stmt, 1, i+1);
                sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_double(stmt, 3, value);
                sqlite3_bind_int(stmt, 4, group_id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    // Step 2: Test window functions if requested
    if (test_window_functions) {
        // Register a custom window function
        rc = sqlite3_create_window_function(
            db, "fuzz_window_sum", 1, SQLITE_UTF8, nullptr,
            window_step_callback, window_final_callback,
            window_value_callback, window_inverse_callback, nullptr);
        
        if (rc == SQLITE_OK) {
            // Test window functions with various OVER clauses
            const char* window_queries[] = {
                "SELECT id, name, value, SUM(value) OVER (PARTITION BY group_id ORDER BY id) FROM test_data;",
                "SELECT id, name, value, ROW_NUMBER() OVER (PARTITION BY group_id ORDER BY value DESC) FROM test_data;",
                "SELECT id, name, value, RANK() OVER (ORDER BY value) FROM test_data;",
                "SELECT id, name, value, DENSE_RANK() OVER (PARTITION BY group_id ORDER BY value) FROM test_data;",
                "SELECT id, name, value, fuzz_window_sum(value) OVER (ORDER BY id ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING) FROM test_data;"
            };
            
            for (int i = 0; i < sizeof(window_queries)/sizeof(window_queries[0]) && i <= window_func_type; i++) {
                sqlite3_exec(db, window_queries[i], nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
    }
    
    // Step 3: Test JSON functions if requested
    if (test_json_functions && fdp.remaining_bytes() > 50) {
        // Generate JSON data from fuzzer input
        std::string json_input = fdp.ConsumeRandomLengthString(500);
        
        // Test various JSON functions
        const char* json_queries[] = {
            "SELECT json('{\"test\": 1}');",
            "SELECT json_array(1, 2.5, 'hello', null);",
            "SELECT json_object('id', 1, 'name', 'test', 'value', 3.14);",
            "SELECT json_extract('{\"a\": {\"b\": 123}}', '$.a.b');",
            "SELECT json_insert('{\"a\": 1}', '$.b', 2);",
            "SELECT json_replace('{\"a\": 1}', '$.a', 99);",
            "SELECT json_set('{\"a\": 1}', '$.b', 2, '$.c', 3);",
            "SELECT json_remove('{\"a\": 1, \"b\": 2}', '$.b');",
            "SELECT json_type('{\"a\": [1, 2, 3]}', '$.a');",
            "SELECT json_valid('{\"test\": 123}');",
            "SELECT json_quote('test\"string');",
            "SELECT json_group_array(id) FROM test_data;",
            "SELECT json_group_object(name, value) FROM test_data LIMIT 5;"
        };
        
        for (int i = 0; i < sizeof(json_queries)/sizeof(json_queries[0]) && i < json_func_count; i++) {
            sqlite3_exec(db, json_queries[i], nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Test with fuzzed JSON input
        if (!json_input.empty()) {
            std::string json_test_query = "SELECT json_valid('" + json_input + "');";
            sqlite3_exec(db, json_test_query.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Step 4: Test R*Tree if requested (requires R*Tree extension)
    if (test_rtree) {
        // Create virtual R*Tree table
        std::string rtree_create = "CREATE VIRTUAL TABLE IF NOT EXISTS rtree_test USING rtree(id, minX, maxX, minY, maxY";
        for (int i = 2; i < rtree_dimensions; i++) {
            rtree_create += ", minZ" + std::to_string(i) + ", maxZ" + std::to_string(i);
        }
        rtree_create += ");";
        
        sqlite3_exec(db, rtree_create.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        } else {
            // Insert some spatial data
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; i++) {
                double minX = fdp.ConsumeFloatingPoint<double>();
                double maxX = fdp.ConsumeFloatingPoint<double>();
                double minY = fdp.ConsumeFloatingPoint<double>();
                double maxY = fdp.ConsumeFloatingPoint<double>();
                
                std::string insert_sql = "INSERT INTO rtree_test VALUES (" + std::to_string(i+1) + ", " +
                    std::to_string(minX) + ", " + std::to_string(maxX) + ", " +
                    std::to_string(minY) + ", " + std::to_string(maxY);
                
                for (int j = 2; j < rtree_dimensions; j++) {
                    double minZ = fdp.ConsumeFloatingPoint<double>();
                    double maxZ = fdp.ConsumeFloatingPoint<double>();
                    insert_sql += ", " + std::to_string(minZ) + ", " + std::to_string(maxZ);
                }
                insert_sql += ");";
                
                sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Query R*Tree data
            const char* rtree_queries[] = {
                "SELECT * FROM rtree_test WHERE minX >= 0 AND maxX <= 100 AND minY >= 0 AND maxY <= 100;",
                "SELECT id FROM rtree_test WHERE maxX < minX OR maxY < minY;",  // Invalid ranges
                "SELECT rtree_test.id FROM rtree_test JOIN test_data ON rtree_test.id = test_data.id;"
            };
            
            for (size_t i = 0; i < sizeof(rtree_queries)/sizeof(rtree_queries[0]); i++) {
                sqlite3_exec(db, rtree_queries[i], nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
    }
    // Step 5: Test FTS5 if requested (requires FTS5 extension)
    if (test_fts5) {
        // Create FTS5 virtual table
        const char* fts5_create = "CREATE VIRTUAL TABLE IF NOT EXISTS fts5_test USING fts5(content, tokenize='porter');";
        sqlite3_exec(db, fts5_create, nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        } else {
            // Insert some text data
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 30; i++) {
                std::string content = fdp.ConsumeRandomLengthString(100);
                std::string insert_sql = "INSERT INTO fts5_test(content) VALUES ('" + content + "');";
                sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test various FTS5 queries and commands
            const char* fts5_commands[] = {
                "SELECT * FROM fts5_test WHERE fts5_test MATCH 'test';",
                "SELECT * FROM fts5_test('test');",
                "SELECT snippet(fts5_test) FROM fts5_test WHERE fts5_test MATCH 'test';",
                "SELECT highlight(fts5_test, 0, '<b>', '</b>') FROM fts5_test WHERE fts5_test MATCH 'test';",
                "SELECT bm25(fts5_test) FROM fts5_test WHERE fts5_test MATCH 'test';",
                "INSERT INTO fts5_test(fts5_test, rank) VALUES('optimize', 1);",  // FTS5 optimize command
                "INSERT INTO fts5_test(fts5_test) VALUES('rebuild');",  // FTS5 rebuild command
                "INSERT INTO fts5_test(fts5_test) VALUES('integrity-check');",  // FTS5 integrity check
                "SELECT fts5_test.* FROM fts5_test LEFT JOIN test_data ON fts5_test.rowid = test_data.id;"
            };
            
            for (int i = 0; i < sizeof(fts5_commands)/sizeof(fts5_commands[0]) && i <= fts5_command_type; i++) {
                sqlite3_exec(db, fts5_commands[i], nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
    }
    
    // Step 6: Test collation functions if requested
    if (test_collations) {
        // Register custom collation
        rc = sqlite3_create_collation(db, "fuzz_collation", SQLITE_UTF8, nullptr, custom_collation);
        
        if (rc == SQLITE_OK) {
            // Test collation in queries
            const char* collation_queries[] = {
                "SELECT name FROM test_data ORDER BY name COLLATE fuzz_collation;",
                "SELECT name FROM test_data WHERE name COLLATE fuzz_collation LIKE 'test%';",
                "CREATE INDEX IF NOT EXISTS idx_collation ON test_data(name COLLATE fuzz_collation);",
                "SELECT DISTINCT name COLLATE fuzz_collation FROM test_data;"
            };
            
            for (int i = 0; i < sizeof(collation_queries)/sizeof(collation_queries[0]) && i <= collation_type; i++) {
                sqlite3_exec(db, collation_queries[i], nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
        
        // Test built-in collations
        const char* builtin_collations[] = {
            "SELECT name FROM test_data ORDER BY name COLLATE NOCASE;",
            "SELECT name FROM test_data ORDER BY name COLLATE BINARY;",
            "SELECT name FROM test_data ORDER BY name COLLATE RTRIM;"
        };
        
        for (size_t i = 0; i < sizeof(builtin_collations)/sizeof(builtin_collations[0]); i++) {
            sqlite3_exec(db, builtin_collations[i], nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Step 7: Test interrupt if requested
    if (test_interrupt) {
        // Start a potentially long-running query
        std::string long_query = "WITH RECURSIVE counter(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM counter LIMIT 1000) SELECT * FROM counter;";
        
        // Execute in a way that can be interrupted
        sqlite3_exec(db, long_query.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Call interrupt (though timing is tricky in fuzzing context)
        sqlite3_interrupt(db);
    }
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_008
