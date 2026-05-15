#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_017 {
/*
 * Fuzzing harness for SQLite library - targeting SQL Preparation and Value Manipulation APIs cluster
 * as identified by coverage analysis with high undiscovered complexity.
 * 
 * This harness specifically targets SQL Preparation APIs with high undiscovered branches:
 * - sqlite3_prepare_v3 (Primary Target: 843 undiscovered branches)
 * - sqlite3_prepare16_v3 (Primary Target: UTF-16 variant)
 * - sqlite3_value_dup (Primary Target: 539 undiscovered branches)
 * - sqlite3_value_free (Primary Target: Value cleanup)
 * 
 * Supporting APIs for comprehensive testing:
 * - sqlite3_set_clientdata / sqlite3_get_clientdata - Per-connection user data
 * - sqlite3_set_auxdata / sqlite3_get_auxdata - Per-function user data
 * - Various sqlite3_bind_* functions - Parameter binding
 * - sqlite3_column_value - Get values from result sets
 * 
 * Invocation Sequence as specified by coverage guidance:
 * 1. Setup: sqlite3_initialize() → sqlite3_open_v2() → Create test tables with sqlite3_exec()
 * 2. Client Data Setup: sqlite3_set_clientdata() with test data
 * 3. SQL Preparation: sqlite3_prepare_v3() with various SQL strings and flag combinations, sqlite3_prepare16_v3() for UTF-16 SQL testing
 * 4. Parameter Binding: Use sqlite3_bind_*() functions with different data types
 * 5. Value Manipulation: Get values via sqlite3_column_value(), duplicate with sqlite3_value_dup(), free with sqlite3_value_free()
 * 6. Auxiliary Data: Test sqlite3_set_auxdata() / sqlite3_get_auxdata() in custom functions
 * 7. Execution: sqlite3_step() through results
 * 8. Cleanup: sqlite3_finalize() → sqlite3_close()
 * 
 * Test Scenarios:
 * - Valid SQL with different prepare flags
 * - Invalid SQL strings (error path testing)
 * - UTF-8 vs UTF-16 SQL parsing
 * - Client data persistence across statements
 * - Auxiliary data lifecycle in custom functions
 * - Value duplication edge cases
 * 
 * Semantic differentiation from existing harnesses (000-016):
 * - Specifically targets newer _v3 prepare APIs (sqlite3_prepare_v3, sqlite3_prepare16_v3)
 * - Focuses on value manipulation APIs (sqlite3_value_dup, sqlite3_value_free)
 * - Tests client data and auxiliary data APIs for per-connection and per-function state
 * - Different from harness_000 which uses sqlite3_prepare_v2
 * - Different from harness_003 which focuses on virtual table APIs
 * - Provides comprehensive SQL preparation lifecycle with extended flags
 * 
 * These functions represent ~2,000+ undiscovered branches in the SQL preparation and value manipulation cluster.
 */


/* Custom function for testing auxiliary data APIs */
static void test_auxdata_func(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    (void)argc;
    (void)argv;
    
    // Set auxiliary data for testing
    int aux_data = 42;
    sqlite3_set_auxdata(context, 0, &aux_data, nullptr);
    
    // Get auxiliary data back
    void* retrieved = sqlite3_get_auxdata(context, 0);
    if (retrieved != nullptr) {
        sqlite3_result_int(context, *(int*)retrieved);
    } else {
        sqlite3_result_int(context, 0);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough for: operation type, SQL strings, flags, binding values, etc.
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Open in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Register custom function for auxiliary data testing
    rc = sqlite3_create_function(db, "test_auxdata", 0, SQLITE_UTF8, nullptr,
                                 test_auxdata_func, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 1: Create test tables
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_data ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER, "
        "data BLOB);";
    
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        // Continue anyway - table might already exist
    }
    
    // Step 2: Set client data (per-connection user data)
    int client_data_value = fdp.ConsumeIntegral<int>();
    void* client_data_ptr = &client_data_value;
    // Generate a random client data name
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(fdp.remaining_bytes(), 64));
    std::string client_name = fdp.ConsumeBytesAsString(name_len);
    rc = sqlite3_set_clientdata(db, client_name.c_str(), client_data_ptr, nullptr);
    
    // Get client data back to verify
    void* retrieved_client_data = sqlite3_get_clientdata(db, client_name.c_str());
    
    // Step 3: Consume operation type from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume SQL statement - use random length string
    size_t sql_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)4096));
    std::string sql_utf8 = fdp.ConsumeBytesAsString(sql_len);
    
    // If no SQL provided, use parameterized queries for binding tests
    if (sql_utf8.empty()) {
        switch (operation) {
            case 0:
                sql_utf8 = "SELECT ?1, ?2, ?3, ?4, ?5 FROM test_data;";
                break;
            case 1:
                sql_utf8 = "INSERT INTO test_data (name, value, data) VALUES (?, ?, ?);";
                break;
            case 2:
                sql_utf8 = "UPDATE test_data SET value = ? WHERE id = ?;";
                break;
            case 3:
                sql_utf8 = "DELETE FROM test_data WHERE id = ?;";
                break;
        }
    }
    // Prepare flags for sqlite3_prepare_v3
    unsigned int prep_flags = 0;
    uint8_t flags_choice = fdp.ConsumeIntegral<uint8_t>() % 16;
    switch (flags_choice) {
        case 0:
            prep_flags = SQLITE_PREPARE_PERSISTENT;
            break;
        case 1:
            prep_flags = SQLITE_PREPARE_NORMALIZE;
            break;
        case 2:
            prep_flags = SQLITE_PREPARE_NO_VTAB;
            break;
        case 3:
            prep_flags = SQLITE_PREPARE_DONT_LOG;
            break;
        case 4:
            prep_flags = SQLITE_PREPARE_FROM_DDL;
            break;
        case 5:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE;
            break;
        case 6:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NO_VTAB;
            break;
        case 7:
            prep_flags = SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_NO_VTAB;
            break;
        case 8:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_DONT_LOG;
            break;
        case 9:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_FROM_DDL;
            break;
        case 10:
            prep_flags = SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_FROM_DDL;
            break;
        case 11:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_NO_VTAB;
            break;
        case 12:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_DONT_LOG;
            break;
        case 13:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_FROM_DDL;
            break;
        case 14:
            prep_flags = SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE | SQLITE_PREPARE_NO_VTAB | SQLITE_PREPARE_FROM_DDL;
            break;
        case 15:
            prep_flags = 0;  // Default flags
            break;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    
    // Step 4: Test sqlite3_prepare_v3 with UTF-8 SQL
    rc = sqlite3_prepare_v3(db, sql_utf8.c_str(), -1, prep_flags, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != nullptr) {
        // Step 5: Bind parameters based on operation type
        int bind_param_count = sqlite3_bind_parameter_count(stmt);
        for (int i = 1; i <= bind_param_count && fdp.remaining_bytes() > 0; i++) {
            uint8_t bind_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            
            switch (bind_type) {
                case 0: {
                    // Bind integer
                    int int_val = fdp.ConsumeIntegral<int>();
                    sqlite3_bind_int(stmt, i, int_val);
                    break;
                }
                case 1: {
                    // Bind double
                    double double_val = fdp.ConsumeFloatingPoint<double>();
                    sqlite3_bind_double(stmt, i, double_val);
                    break;
                }
                case 2: {
                    // Bind text
                    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)256));
                    std::string text = fdp.ConsumeBytesAsString(text_len);
                    sqlite3_bind_text(stmt, i, text.c_str(), text.size(), SQLITE_TRANSIENT);
                    break;
                }
                case 3: {
                    // Bind blob
                    size_t blob_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)512));
                    std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_len);
                    if (!blob_data.empty()) {
                        sqlite3_bind_blob(stmt, i, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                    }
                    break;
                }
                case 4: {
                    // Bind null
                    sqlite3_bind_null(stmt, i);
                    break;
                }
                case 5: {
                    // Bind zero blob
                    size_t zero_blob_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
                    sqlite3_bind_zeroblob(stmt, i, zero_blob_size);
                    break;
                }
            }
        }
        
        // Step 6: Execute statement and test value manipulation
        int max_steps = fdp.ConsumeIntegralInRange<int>(0, 10);
        for (int step = 0; step < max_steps; step++) {
            rc = sqlite3_step(stmt);
            
            if (rc == SQLITE_ROW) {
                // Get column count
                int col_count = sqlite3_column_count(stmt);
                
                // Test value manipulation for each column
                for (int col = 0; col < col_count; col++) {
                    // Get sqlite3_value from column
                    sqlite3_value* col_value = sqlite3_column_value(stmt, col);
                    if (col_value != nullptr) {
                        // Duplicate the value
                        sqlite3_value* dup_value = sqlite3_value_dup(col_value);
                        
                        if (dup_value != nullptr) {
                            // Test some operations on duplicated value
                            int value_type = sqlite3_value_type(dup_value);
                            // Free the duplicated value
                            sqlite3_value_free(dup_value);
                        }
                    }
                }
            } else if (rc != SQLITE_DONE) {
                break;
            }
        }
        
        // Step 7: Test auxiliary data API via custom function
        if (fdp.remaining_bytes() > 0) {
            sqlite3_stmt* aux_stmt = nullptr;
            const char* aux_sql = "SELECT test_auxdata();";
            rc = sqlite3_prepare_v2(db, aux_sql, -1, &aux_stmt, nullptr);
            if (rc == SQLITE_OK && aux_stmt != nullptr) {
                sqlite3_step(aux_stmt);
                sqlite3_finalize(aux_stmt);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Step 8: Test sqlite3_prepare16_v3 with UTF-16 SQL (if enough input remains)
    if (fdp.remaining_bytes() > 64) {
        // Convert UTF-8 SQL to UTF-16 for testing
        size_t utf16_sql_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes() / 2, (size_t)2048));
        std::vector<uint16_t> utf16_sql(utf16_sql_len);
        for (size_t i = 0; i < utf16_sql_len && fdp.remaining_bytes() > 0; i++) {
            utf16_sql[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        
        if (!utf16_sql.empty()) {
            // Ensure null termination
            utf16_sql.push_back(0);
            
            const void* utf16_sql_ptr = utf16_sql.data();
            sqlite3_stmt* stmt16 = nullptr;
            const void* tail16 = nullptr;
            
            // Test sqlite3_prepare16_v3
            rc = sqlite3_prepare16_v3(db, utf16_sql_ptr, -1, prep_flags, &stmt16, &tail16);
            
            if (rc == SQLITE_OK && stmt16 != nullptr) {
                // Execute briefly
                sqlite3_step(stmt16);
                sqlite3_finalize(stmt16);
            }
        }
    }
    
    // Step 9: Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_017
