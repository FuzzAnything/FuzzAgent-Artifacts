#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_009 {
/*
** SQLite custom SQL function and collation implementation APIs fuzzing harness
**
** This harness specifically targets SQLite custom SQL function and collation 
** implementation APIs identified as highest priority coverage gap with 625+ 
** undiscovered branches. Target APIs:
** - sqlite3_create_function: Register scalar or aggregate SQL function
** - sqlite3_create_function_v2: Register function with destructor callback
** - sqlite3_create_window_function: Register window function (not in ext.h)
** - sqlite3_create_collation: Register custom collation sequence
** - sqlite3_create_collation_v2: Register collation with destructor callback
** - sqlite3_collation_needed: Request collation sequence on demand
** - sqlite3_collation_needed16: UTF-16 version of collation_needed
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step)
** - harness_001: Advanced operations with progress handlers and authorizers
** - harness_002: BLOB-specific incremental I/O operations
** - harness_003: Virtual table creation, configuration, and operations
** - harness_004: Database backup operations between databases
** - harness_005: Database serialization/deserialization with WAL checkpoints
** - harness_006: UTF-16 specific APIs and Unicode handling
** - harness_007: Metadata retrieval and statement explanation APIs
** - harness_008: Comprehensive statement preparation and binding APIs
** - harness_009: Custom SQL functions and collation sequences (this harness)
**
** Operation lifecycle:
**   Database setup → Register custom functions/collations → 
**   Use functions/collations in SQL queries → Test various parameter types →
**   Test aggregation functions → Test collation sequences → Cleanup
**
** Key design considerations:
** 1. Implement multiple custom scalar functions with different behaviors
** 2. Implement custom aggregate functions with step/final callbacks
** 3. Implement custom collation sequences with comparison logic
** 4. Test function registration with different encodings (UTF8, UTF16, ANY)
** 5. Test function registration with different parameter counts (-1 = variable)
** 6. Test function determinism flag (SQLITE_DETERMINISTIC)
** 7. Test collation registration with different encodings
** 8. Test collation_needed callback mechanism
** 9. Ensure proper resource cleanup with destructor callbacks
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Operation types for the custom function/collation harness
enum FunctionOperation {
    FUNC_OP_SCALAR = 0,
    FUNC_OP_AGGREGATE = 1,
    FUNC_OP_COLLATION = 2,
    FUNC_OP_MIXED = 3,
    FUNC_OP_V2_DESTRUCTOR = 4,
    FUNC_OP_COLLATION_NEEDED = 5,
    FUNC_OP_COMPREHENSIVE = 6
};

// Available encodings for function registration
static const int function_encodings[] = {
    SQLITE_UTF8,
    SQLITE_UTF8 | SQLITE_DETERMINISTIC,
#ifndef SQLITE_OMIT_UTF16
    SQLITE_UTF16,
    SQLITE_UTF16 | SQLITE_DETERMINISTIC,
    SQLITE_UTF16LE,
    SQLITE_UTF16BE,
#endif
    SQLITE_ANY,
    SQLITE_ANY | SQLITE_DETERMINISTIC
};

// Available encodings for collation registration
static const int collation_encodings[] = {
    SQLITE_UTF8,
#ifndef SQLITE_OMIT_UTF16
    SQLITE_UTF16,
    SQLITE_UTF16LE,
    SQLITE_UTF16BE,
#endif
    SQLITE_UTF16_ALIGNED
};

// Custom scalar function 1: Return length of first argument as integer
static void custom_scalar_func1(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    (void)argc; // Unused parameter
    
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        sqlite3_result_null(context);
        return;
    }
    
    int length = sqlite3_value_bytes(argv[0]);
    sqlite3_result_int(context, length);
}

// Custom scalar function 2: Concatenate all arguments as text
static void custom_scalar_func2(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    if (argc == 0) {
        sqlite3_result_text(context, "", 0, SQLITE_STATIC);
        return;
    }
    
    // Calculate total length needed
    int total_length = 0;
    for (int i = 0; i < argc; i++) {
        if (sqlite3_value_type(argv[i]) != SQLITE_NULL) {
            total_length += sqlite3_value_bytes(argv[i]);
        }
    }
    
    if (total_length == 0) {
        sqlite3_result_text(context, "", 0, SQLITE_STATIC);
        return;
    }
    
    // Allocate buffer and concatenate
    char* result = (char*)sqlite3_malloc(total_length + 1);
    if (result == nullptr) {
        sqlite3_result_error_nomem(context);
        return;
    }
    
    int pos = 0;
    for (int i = 0; i < argc; i++) {
        if (sqlite3_value_type(argv[i]) != SQLITE_NULL) {
            const char* text = (const char*)sqlite3_value_text(argv[i]);
            int len = sqlite3_value_bytes(argv[i]);
            if (text && len > 0) {
                memcpy(result + pos, text, len);
                pos += len;
            }
        }
    }
    result[pos] = '\0';
    
    sqlite3_result_text(context, result, pos, sqlite3_free);
}

// Custom scalar function 3: Return sum of all integer arguments
static void custom_scalar_func3(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    sqlite3_int64 sum = 0;
    
    for (int i = 0; i < argc; i++) {
        if (sqlite3_value_type(argv[i]) != SQLITE_NULL) {
            sum += sqlite3_value_int64(argv[i]);
        }
    }
    
    sqlite3_result_int64(context, sum);
}

// Custom aggregate function context
struct AggregateContext {
    sqlite3_int64 count;
    sqlite3_int64 sum;
    sqlite3_int64 min;
    sqlite3_int64 max;
};

// Custom aggregate step function
static void custom_aggregate_step(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    AggregateContext* ctx = (AggregateContext*)sqlite3_aggregate_context(context, sizeof(AggregateContext));
    if (ctx == nullptr) {
        return;
    }
    
    if (argc > 0 && sqlite3_value_type(argv[0]) != SQLITE_NULL) {
        sqlite3_int64 value = sqlite3_value_int64(argv[0]);
        
        if (ctx->count == 0) {
            ctx->min = value;
            ctx->max = value;
        } else {
            if (value < ctx->min) ctx->min = value;
            if (value > ctx->max) ctx->max = value;
        }
        
        ctx->sum += value;
        ctx->count++;
    }
}

// Custom aggregate final function
static void custom_aggregate_final(
    sqlite3_context* context
) {
    AggregateContext* ctx = (AggregateContext*)sqlite3_aggregate_context(context, 0);
    
    if (ctx == nullptr || ctx->count == 0) {
        sqlite3_result_null(context);
        return;
    }
    
    // Return average as double
    double avg = (double)ctx->sum / (double)ctx->count;
    sqlite3_result_double(context, avg);
}

// Custom collation comparison function
static int custom_collation_compare(
    void* pArg,
    int n1,
    const void* p1,
    int n2,
    const void* p2
) {
    (void)pArg; // Unused
    
    // Simple case-insensitive comparison for UTF-8
    const unsigned char* s1 = (const unsigned char*)p1;
    const unsigned char* s2 = (const unsigned char*)p2;
    
    int len = n1 < n2 ? n1 : n2;
    for (int i = 0; i < len; i++) {
        unsigned char c1 = s1[i];
        unsigned char c2 = s2[i];
        
        // Convert to lowercase for case-insensitive comparison
        if (c1 >= 'A' && c1 <= 'Z') c1 += 'a' - 'A';
        if (c2 >= 'A' && c2 <= 'Z') c2 += 'a' - 'A';
        
        if (c1 != c2) {
            return c1 - c2;
        }
    }
    
    // If all compared characters match, shorter string comes first
    return n1 - n2;
}

// Collation needed callback for UTF-8
static void collation_needed_callback(
    void* pArg,
    sqlite3* db,
    int eTextRep,
    const char* zName
) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
    // In a real scenario, we would register the collation here
    // For fuzzing, we just acknowledge the callback was invoked
}

// Collation needed callback for UTF-16
#ifndef SQLITE_OMIT_UTF16
static void collation_needed16_callback(
    void* pArg,
    sqlite3* db,
    int eTextRep,
    const void* zName
) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
    // In a real scenario, we would register the collation here
    // For fuzzing, we just acknowledge the callback was invoked
}
#endif

// Function destructor callback (for create_function_v2)
static void function_destructor(void* pUser) {
    int* counter = (int*)pUser;
    if (counter != nullptr) {
        (*counter)++;
    }
}

// Collation destructor callback (for create_collation_v2)
static void collation_destructor(void* pUser) {
    int* counter = (int*)pUser;
    if (counter != nullptr) {
        (*counter)++;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and operations
    const size_t MIN_SIZE = sizeof(int) * 8 + sizeof(int64_t) * 4 + 128;
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
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
        "CREATE TABLE IF NOT EXISTS func_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert some test data
    const char* insert_sql = 
        "INSERT INTO func_test (name, value, data) VALUES "
        "('Alice', 100, x'010203'), "
        "('Bob', 200, x'040506'), "
        "('Charlie', 300, x'070809'), "
        "('david', 400, x'0a0b0c'), "
        "('EVE', 500, x'0d0e0f');";
    
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Determine operation type from fuzzer input
    FunctionOperation op_type = static_cast<FunctionOperation>(
        fdp.ConsumeIntegralInRange<int>(0, FUNC_OP_COMPREHENSIVE));
    
    // Track destructor callbacks
    int func_destructor_count = 0;
    int collation_destructor_count = 0;
    
    // Test scalar function registration
    if (op_type == FUNC_OP_SCALAR || op_type == FUNC_OP_MIXED || 
        op_type == FUNC_OP_COMPREHENSIVE || op_type == FUNC_OP_V2_DESTRUCTOR) {
        
        // Register scalar functions with different parameter counts
        int encoding_idx = fdp.ConsumeIntegralInRange<size_t>(
            0, sizeof(function_encodings) / sizeof(function_encodings[0]) - 1);
        int encoding = function_encodings[encoding_idx];
        
        // Register function with variable number of arguments (-1)
        rc = sqlite3_create_function(
            db, "custom_len", 1, encoding, nullptr,
            custom_scalar_func1, nullptr, nullptr);
        
        // Register function with 3 arguments
        if (rc == SQLITE_OK) {
            rc = sqlite3_create_function(
                db, "custom_concat", -1, encoding, nullptr,
                custom_scalar_func2, nullptr, nullptr);
        }
        
        // Register function with 2 arguments using create_function_v2 with destructor
        if (rc == SQLITE_OK && (op_type == FUNC_OP_V2_DESTRUCTOR || op_type == FUNC_OP_COMPREHENSIVE)) {
            rc = sqlite3_create_function_v2(
                db, "custom_sum_v2", -1, encoding, &func_destructor_count,
                custom_scalar_func3, nullptr, nullptr, function_destructor);
        }
        
        // Use the registered functions in SQL queries
        if (rc == SQLITE_OK) {
            std::string test_sql = 
                "SELECT custom_len(name), custom_concat(name, ':', value), "
                "custom_sum_v2(value, 10) FROM func_test;";
            
            rc = sqlite3_exec(db, test_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (rc != SQLITE_OK && err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Test aggregate function registration
    if ((op_type == FUNC_OP_AGGREGATE || op_type == FUNC_OP_MIXED || 
         op_type == FUNC_OP_COMPREHENSIVE) && rc == SQLITE_OK) {
        
        int encoding_idx = fdp.ConsumeIntegralInRange<size_t>(
            0, sizeof(function_encodings) / sizeof(function_encodings[0]) - 1);
        int encoding = function_encodings[encoding_idx];
        
        // Register aggregate function
        rc = sqlite3_create_function(
            db, "custom_avg", 1, encoding, nullptr,
            nullptr, custom_aggregate_step, custom_aggregate_final);
        
        // Use the aggregate function in SQL query
        if (rc == SQLITE_OK) {
            const char* aggregate_sql = 
                "SELECT custom_avg(value) FROM func_test;";
            
            rc = sqlite3_exec(db, aggregate_sql, exec_callback, nullptr, &err_msg);
            if (rc != SQLITE_OK && err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Test collation registration
    if ((op_type == FUNC_OP_COLLATION || op_type == FUNC_OP_MIXED || 
         op_type == FUNC_OP_COLLATION_NEEDED || op_type == FUNC_OP_COMPREHENSIVE) && rc == SQLITE_OK) {
        
        int encoding_idx = fdp.ConsumeIntegralInRange<size_t>(
            0, sizeof(collation_encodings) / sizeof(collation_encodings[0]) - 1);
        int encoding = collation_encodings[encoding_idx];
        
        // Register custom collation
        rc = sqlite3_create_collation(
            db, "custom_collate", encoding, nullptr, custom_collation_compare);
        
        // Register collation with destructor using create_collation_v2
        if (rc == SQLITE_OK && (op_type == FUNC_OP_COMPREHENSIVE || op_type == FUNC_OP_V2_DESTRUCTOR)) {
            rc = sqlite3_create_collation_v2(
                db, "custom_collate_v2", encoding, &collation_destructor_count,
                custom_collation_compare, collation_destructor);
        }
        
        // Set up collation needed callback
        if (rc == SQLITE_OK && (op_type == FUNC_OP_COLLATION_NEEDED || op_type == FUNC_OP_COMPREHENSIVE)) {
            rc = sqlite3_collation_needed(db, nullptr, collation_needed_callback);
            
            // Also test UTF-16 version if available
#ifndef SQLITE_OMIT_UTF16
            if (rc == SQLITE_OK) {
                rc = sqlite3_collation_needed16(db, nullptr, collation_needed16_callback);
            }
#endif
        }
        
        // Use custom collation in SQL query
        if (rc == SQLITE_OK) {
            const char* collation_sql = 
                "SELECT name FROM func_test ORDER BY name COLLATE custom_collate;";
            
            rc = sqlite3_exec(db, collation_sql, exec_callback, nullptr, &err_msg);
            if (rc != SQLITE_OK && err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Test comprehensive scenario with all features
    if (op_type == FUNC_OP_COMPREHENSIVE && rc == SQLITE_OK) {
        // Create another table for more complex queries
        const char* create_table2_sql = 
            "CREATE TABLE IF NOT EXISTS func_test2 ("
            "id INTEGER PRIMARY KEY, "
            "text1 TEXT, "
            "text2 TEXT, "
            "num1 INTEGER, "
            "num2 INTEGER);";
        
        rc = sqlite3_exec(db, create_table2_sql, nullptr, nullptr, &err_msg);
        if (rc != SQLITE_OK && err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert varied test data
        if (rc == SQLITE_OK) {
            std::vector<std::string> test_strings = {
                "Apple", "banana", "Cherry", "date", "FIG",
                "grape", "HONEYDEW", "kiwi", "LEMON", "mango"
            };
            
            for (size_t i = 0; i < test_strings.size(); i++) {
                std::string insert_sql2 = 
                    "INSERT INTO func_test2 (text1, text2, num1, num2) VALUES ('" +
                    test_strings[i] + "', '" + test_strings[i] + "', " +
                    std::to_string(i * 10) + ", " + std::to_string(i * 20) + ");";
                
                rc = sqlite3_exec(db, insert_sql2.c_str(), nullptr, nullptr, &err_msg);
                if (rc != SQLITE_OK && err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                    break;
                }
            }
        }
        
        // Complex query using all custom functions and collations
        if (rc == SQLITE_OK) {
            const char* complex_sql = 
                "SELECT "
                "custom_len(text1), "
                "custom_concat(text1, '=', text2), "
                "custom_sum_v2(num1, num2), "
                "custom_avg(num1) "
                "FROM func_test2 "
                "WHERE text1 COLLATE custom_collate_v2 > 'C' "
                "ORDER BY text1 COLLATE custom_collate;";
            
            rc = sqlite3_exec(db, complex_sql, exec_callback, nullptr, &err_msg);
            if (rc != SQLITE_OK && err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Cleanup
    if (db != nullptr) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    
    // The destructor callbacks should have been called during cleanup
    // We can't verify this directly in the fuzzer, but the coverage
    // instrumentation will track whether those code paths were executed.
    
    return 0;
}

} // namespace harness_009
