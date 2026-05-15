#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_005 {
/*
 * Fuzzing harness for SQLite table data retrieval APIs
 * Targets: sqlite3_get_table (0% coverage, 789 undiscovered branches)
 *          sqlite3_free_table (0% coverage, 26 undiscovered branches)
 * 
 * This harness specifically targets SQLite table data retrieval APIs which have
 * 815 total undiscovered branches according to coverage analysis.
 * Implementation follows exact sequence: 
 * 1) sqlite3_open_v2 to create database
 * 2) sqlite3_exec to create test tables and insert sample data  
 * 3) sqlite3_get_table with various SELECT queries (simple columns, JOINs, WHERE, ORDER BY, aggregates)
 * 4) Process returned array checking row/column counts and data integrity
 * 5) sqlite3_free_table to clean up results
 * 6) sqlite3_close
 * 
 * Includes error conditions, empty result sets, invalid SQL.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Core UTF-8 APIs (prepared statements, exec)
 * - harness_001: Database file parsing (deserialization)
 * - harness_002: BLOB operations
 * - harness_003: UTF-16/Unicode specific APIs
 * - harness_004: Database backup operations
 * - harness_005: Table data retrieval (get_table/free_table)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful get_table testing
    // Need enough for configuration + SQL queries + data
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool use_memory_db = fdp.ConsumeBool();
    bool test_error_paths = fdp.ConsumeBool();
    bool test_empty_results = fdp.ConsumeBool();
    bool test_invalid_sql = fdp.ConsumeBool();
    bool test_aggregates = fdp.ConsumeBool();
    bool test_joins = fdp.ConsumeBool();
    
    int num_test_tables = fdp.ConsumeIntegralInRange<int>(1, 5);
    int max_rows_per_table = fdp.ConsumeIntegralInRange<int>(0, 50);
    int max_query_length = fdp.ConsumeIntegralInRange<int>(10, 1024);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    const char* db_name = use_memory_db ? ":memory:" : "fuzz_get_table.db";
    
    // Open database connection
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (use_memory_db) {
        open_flags |= SQLITE_OPEN_MEMORY;
    }
    
    int rc = sqlite3_open_v2(db_name, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 50);
    sqlite3_limit(db, SQLITE_LIMIT_EXPR_DEPTH, 20);
    
    char* err_msg = nullptr;
    
    // Step 1: Create test tables
    for (int table_idx = 0; table_idx < num_test_tables && fdp.remaining_bytes() > 0; table_idx++) {
        // Generate table name from fuzzer input
        std::string table_name = "tbl" + std::to_string(table_idx);
        
        // Create table with various column types
        std::string create_sql = "CREATE TABLE IF NOT EXISTS " + table_name + 
            " (id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "value REAL, "
            "data BLOB, "
            "flag INTEGER);";
        
        rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
        if (rc != SQLITE_OK && err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert sample data if requested
        if (max_rows_per_table > 0 && fdp.remaining_bytes() > 0) {
            for (int row_idx = 0; row_idx < max_rows_per_table && fdp.remaining_bytes() > 0; row_idx++) {
                // Generate INSERT statement with fuzzed data
                std::string name = fdp.ConsumeRandomLengthString(50);
                double value = fdp.ConsumeFloatingPoint<double>();
                int flag = fdp.ConsumeIntegral<int>();
                
                // For BLOB data, consume some bytes
                size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                std::vector<uint8_t> blob_data;
                if (blob_size > 0 && fdp.remaining_bytes() >= blob_size) {
                    blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
                }
                
                // Build INSERT statement
                std::string insert_sql = "INSERT INTO " + table_name + 
                    " (name, value, data, flag) VALUES ('" + name + "', " +
                    std::to_string(value) + ", ?, " + std::to_string(flag) + ");";
                
                // Use prepared statement for BLOB binding
                sqlite3_stmt* stmt = nullptr;
                rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    if (!blob_data.empty()) {
                        sqlite3_bind_blob(stmt, 1, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                    } else {
                        sqlite3_bind_null(stmt, 1);
                    }
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }
        }
    }
    
    // Step 2: Test sqlite3_get_table with various queries
    int num_queries = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int query_idx = 0; query_idx < num_queries && fdp.remaining_bytes() > 0; query_idx++) {
        // Determine query type based on fuzzer input
        int query_type = fdp.ConsumeIntegralInRange<int>(0, 9);
        std::string sql_query;
        
        // Select a random table index for the query
        int table_idx_for_query = fdp.ConsumeIntegralInRange<int>(0, num_test_tables - 1);
        std::string table_name = "tbl" + std::to_string(table_idx_for_query);
        
        switch (query_type) {
            case 0: // Simple SELECT from specific table
                sql_query = "SELECT * FROM " + table_name + ";";
                break;
            case 1: // SELECT with WHERE using fuzzed condition
                {
                    int where_flag = fdp.ConsumeIntegral<int>();
                    sql_query = "SELECT id, name FROM " + table_name + " WHERE flag > " + std::to_string(where_flag) + ";";
                }
                break;
            case 2: // SELECT with ORDER BY using fuzzed column
                {
                    int order_col = fdp.ConsumeIntegralInRange<int>(0, 2); // 0=id, 1=name, 2=value
                    const char* col_names[] = {"id", "name", "value"};
                    bool desc = fdp.ConsumeBool();
                    sql_query = "SELECT " + std::string(col_names[order_col]) + " FROM " + table_name + 
                               " ORDER BY " + std::string(col_names[order_col]) + (desc ? " DESC" : " ASC") + ";";
                }
                break;
            case 3: // SELECT with aggregate (if enabled)
                if (test_aggregates) {
                    sql_query = "SELECT COUNT(*), AVG(value), MAX(value), MIN(value) FROM " + table_name + ";";
                } else {
                    sql_query = "SELECT * FROM " + table_name + " LIMIT 5;";
                }
                break;
            case 4: // SELECT with JOIN (if enabled)
                if (test_joins && num_test_tables > 1) {
                    int table2_idx = fdp.ConsumeIntegralInRange<int>(0, num_test_tables - 1);
                    while (table2_idx == table_idx_for_query && num_test_tables > 1) {
                        table2_idx = (table2_idx + 1) % num_test_tables;
                    }
                    std::string table2_name = "tbl" + std::to_string(table2_idx);
                    sql_query = "SELECT t1.name, t2.value FROM " + table_name + 
                               " t1 JOIN " + table2_name + " t2 ON t1.id = t2.id;";
                } else {
                    sql_query = "SELECT * FROM " + table_name + " WHERE id < 10;";
                }
                break;
            case 5: // SELECT with GROUP BY
                sql_query = "SELECT flag, COUNT(*), AVG(value) FROM " + table_name + " GROUP BY flag;";
                break;
            case 6: // Empty result set (if enabled)
                if (test_empty_results) {
                    sql_query = "SELECT * FROM " + table_name + " WHERE id < 0;"; // Should return empty
                } else {
                    sql_query = "SELECT * FROM " + table_name + ";";
                }
                break;
            case 7: // SELECT with LIMIT
                {
                    int limit_val = fdp.ConsumeIntegralInRange<int>(0, 100);
                    sql_query = "SELECT * FROM " + table_name + " LIMIT " + std::to_string(limit_val) + ";";
                }
                break;
            case 8: // SELECT specific columns
                {
                    bool select_id = fdp.ConsumeBool();
                    bool select_name = fdp.ConsumeBool();
                    bool select_value = fdp.ConsumeBool();
                    std::string cols = "";
                    if (select_id) cols += "id, ";
                    if (select_name) cols += "name, ";
                    if (select_value) cols += "value, ";
                    if (cols.empty()) cols = "*";
                    else cols = cols.substr(0, cols.length() - 2); // Remove trailing ", "
                    sql_query = "SELECT " + cols + " FROM " + table_name + ";";
                }
                break;
            case 9: // Potentially invalid SQL (if enabled)
                if (test_invalid_sql && fdp.remaining_bytes() > 0) {
                    sql_query = fdp.ConsumeRandomLengthString(max_query_length);
                } else {
                    sql_query = "SELECT id, name, value FROM " + table_name + ";";
                }
                break;
            default:
                sql_query = "SELECT * FROM " + table_name + ";";
                break;
        }

        
        // Variables for sqlite3_get_table results
        char** pazResult = nullptr;
        int pnRow = 0;
        int pnColumn = 0;
        char* pzErrmsg = nullptr;
        
        // Call sqlite3_get_table - the main target API
        rc = sqlite3_get_table(db, sql_query.c_str(), &pazResult, &pnRow, &pnColumn, &pzErrmsg);
        
        // Process results based on return code
        if (rc == SQLITE_OK) {
            // Success - verify and process the result table
            
            // Check that pazResult is not null (should be allocated even for empty results)
            if (pazResult != nullptr) {
                // Verify expected array size: (pnRow + 1) * pnColumn
                int expected_size = (pnRow + 1) * pnColumn;
                
                // Access column names (first pnColumn entries)
                for (int col = 0; col < pnColumn; col++) {
                    const char* col_name = pazResult[col];
                    // Column name could be null for unnamed columns
                    (void)col_name; // Use variable to avoid unused warning
                }
                
                // Access data rows
                for (int row = 0; row < pnRow; row++) {
                    for (int col = 0; col < pnColumn; col++) {
                        int idx = (row + 1) * pnColumn + col;
                        const char* cell_value = pazResult[idx];
                        // Cell value could be null for NULL SQL values
                        (void)cell_value; // Use variable to avoid unused warning
                        
                        // Basic integrity check: if not null, should be valid string
                        if (cell_value != nullptr) {
                            // Verify it's null-terminated (sqlite3_get_table ensures this)
                            size_t len = strlen(cell_value);
                            (void)len; // Use variable
                        }
                    }
                }
            }
        } else {
            // Error case - check error message
            if (pzErrmsg != nullptr) {
                // Error message should be valid string
                size_t err_len = strlen(pzErrmsg);
                (void)err_len; // Use variable
            }
        }
        
        // Always call sqlite3_free_table to clean up - the second target API
        sqlite3_free_table(pazResult);
        
        // Free error message if allocated
        if (pzErrmsg != nullptr) {
            sqlite3_free(pzErrmsg);
        }
        
        // Test error paths if enabled
        if (test_error_paths && fdp.remaining_bytes() > 0) {
            // Test with null parameters
            char** dummy_result = nullptr;
            int dummy_row = 0, dummy_col = 0;
            char* dummy_err = nullptr;
            
            // Test 1: Null database pointer (should crash or return error)
            // sqlite3_get_table(nullptr, "SELECT 1;", &dummy_result, &dummy_row, &dummy_col, &dummy_err);
            // Not testing as it would cause crash
            
            // Test 2: Empty SQL string
            rc = sqlite3_get_table(db, "", &dummy_result, &dummy_row, &dummy_col, &dummy_err);
            sqlite3_free_table(dummy_result);
            if (dummy_err != nullptr) sqlite3_free(dummy_err);
            
            // Test 3: SQL string with only whitespace
            rc = sqlite3_get_table(db, "   ", &dummy_result, &dummy_row, &dummy_col, &dummy_err);
            sqlite3_free_table(dummy_result);
            if (dummy_err != nullptr) sqlite3_free(dummy_err);
            
            // Test 4: Very long SQL (potential buffer issues)
            std::string long_sql = "SELECT ";
            for (int i = 0; i < 100 && fdp.remaining_bytes() > 0; i++) {
                long_sql += "name, ";
            }
            long_sql += "id FROM tbl0;";
            
            rc = sqlite3_get_table(db, long_sql.c_str(), &dummy_result, &dummy_row, &dummy_col, &dummy_err);
            sqlite3_free_table(dummy_result);
            if (dummy_err != nullptr) sqlite3_free(dummy_err);
        }
    }
    
    // Step 3: Test sqlite3_errmsg for error reporting
    if (test_invalid_sql) {
        // Execute invalid SQL to generate error
        rc = sqlite3_exec(db, "INVALID SQL STATEMENT", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            const char* error_str = sqlite3_errmsg(db);
            (void)error_str; // Use variable
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_005
