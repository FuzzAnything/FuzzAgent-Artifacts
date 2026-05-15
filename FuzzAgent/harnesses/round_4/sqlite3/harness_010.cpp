#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_010 {
/*
 * Fuzzing harness for SQLite library - targeting JSON extension APIs (JSON1)
 * This harness targets SQLite JSON functions which are semantically distinct from existing harnesses (000-009).
 * 
 * This harness targets JSON extension APIs:
 * - json() - Parse JSON text into JSON value
 * - json_array() - Create JSON array from values
 * - json_object() - Create JSON object from key-value pairs
 * - json_extract() - Extract values from JSON using path expressions
 * - json_insert() - Insert values into JSON at specified paths
 * - json_replace() - Replace values in JSON at specified paths
 * - json_set() - Set values in JSON (insert or replace)
 * - json_remove() - Remove values from JSON at specified paths
 * - json_type() - Get type of JSON element at specified path
 * - json_valid() - Validate JSON text
 * - json_quote() - Quote text as JSON string
 * - json_group_array() - Aggregate function to create JSON array
 * - json_group_object() - Aggregate function to create JSON object
 * - json_each() - Table-valued function to enumerate JSON elements
 * - json_tree() - Table-valued function to enumerate JSON tree
 * 
 * Focus on creating JSON data, querying JSON, modifying JSON, and JSON aggregation functions.
 * 
 * Following the invocation sequence:
 * 1. Initialize SQLite and open database
 * 2. Create table to store JSON data
 * 3. Generate JSON strings from fuzzer input
 * 4. Test json() function for parsing JSON
 * 5. Test json_array() and json_object() for creating JSON
 * 6. Test json_extract() for querying JSON with path expressions
 * 7. Test json_insert(), json_replace(), json_set() for modifying JSON
 * 8. Test json_remove() for removing JSON elements
 * 9. Test json_type() and json_valid() for type checking and validation
 * 10. Test json_quote() for string quoting
 * 11. Test json_each() and json_tree() table-valued functions
 * 12. Test json_group_array() and json_group_object() aggregate functions
 * 13. Clean up resources
 * 
 * Semantic differentiation from existing harnesses:
 * - Focuses specifically on JSON extension APIs vs general SQL APIs
 * - Tests JSON path expression syntax and operations
 * - Exercises JSON parsing, validation, and manipulation functions
 * - Tests table-valued JSON functions (json_each, json_tree)
 * - Tests JSON aggregate functions (json_group_array, json_group_object)
 * - Tests JSON modification operations (insert, replace, set, remove)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful JSON testing
    // Minimum: operation type + small JSON string
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
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
    
    // Enable JSON extensions
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, "SELECT json(1);", nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Create table for storing JSON data
    rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS json_test ("
        "  id INTEGER PRIMARY KEY, "
        "  json_data TEXT, "
        "  jsonb_data BLOB"
        ");", 
        nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Consume operation type from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Generate JSON string from fuzzer input
    size_t json_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(4096, fdp.remaining_bytes()));
    std::string json_str = fdp.ConsumeBytesAsString(json_len);
    
    // Generate path expressions for JSON operations
    std::vector<std::string> paths;
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
        size_t path_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
        std::string path = fdp.ConsumeBytesAsString(path_len);
        // Ensure path starts with $ for JSON path syntax
        if (path.empty() || path[0] != '$') {
            path = "$." + path;
        }
        paths.push_back(path);
    }
    
    // Generate values for JSON operations
    std::vector<std::string> values;
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(512, fdp.remaining_bytes()));
        std::string value = fdp.ConsumeBytesAsString(value_len);
        values.push_back(value);
    }
    
    // Test different JSON operations based on operation type
    switch (operation) {
        case 0: {
            // Test json() function - parse JSON
            sqlite3_stmt* stmt = nullptr;
            const char* sql = "SELECT json(?1);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // JSON parsed successfully
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case 1: {
            // Test json_array() function
            sqlite3_stmt* stmt = nullptr;
            const char* sql = "SELECT json_array(?1, ?2, ?3);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                for (int i = 0; i < 3 && i < values.size(); i++) {
                    sqlite3_bind_text(stmt, i + 1, values[i].c_str(), -1, SQLITE_TRANSIENT);
                }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // JSON array created successfully
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case 2: {
            // Test json_object() function
            if (values.size() >= 2) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_object(?1, ?2, ?3, ?4);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    for (int i = 0; i < 4 && i < values.size(); i++) {
                        sqlite3_bind_text(stmt, i + 1, values[i].c_str(), -1, SQLITE_TRANSIENT);
                    }
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON object created successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 3: {
            // Test json_extract() function
            if (!paths.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_extract(?1, ?2);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, paths[0].c_str(), -1, SQLITE_TRANSIENT);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON extracted successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 4: {
            // Test json_insert() function
            if (!paths.empty() && !values.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_insert(?1, ?2, ?3);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, paths[0].c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, values[0].c_str(), -1, SQLITE_TRANSIENT);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON inserted successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 5: {
            // Test json_replace() function
            if (!paths.empty() && !values.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_replace(?1, ?2, ?3);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, paths[0].c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, values[0].c_str(), -1, SQLITE_TRANSIENT);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON replaced successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 6: {
            // Test json_set() function
            if (!paths.empty() && !values.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_set(?1, ?2, ?3);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, paths[0].c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, values[0].c_str(), -1, SQLITE_TRANSIENT);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON set successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 7: {
            // Test json_remove() function
            if (!paths.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* sql = "SELECT json_remove(?1, ?2);";
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, paths[0].c_str(), -1, SQLITE_TRANSIENT);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // JSON element removed successfully
                    }
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case 8: {
            // Test json_type() and json_valid() functions
            sqlite3_stmt* stmt = nullptr;
            const char* sql = "SELECT json_type(?1), json_valid(?1);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // JSON type and validity checked
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case 9: {
            // Test json_quote() function
            sqlite3_stmt* stmt = nullptr;
            const char* sql = "SELECT json_quote(?1);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // JSON quoted successfully
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
    }
    
    // Test json_each() and json_tree() table-valued functions
    if (fdp.remaining_bytes() > 100) {
        // Insert some JSON data into the table
        sqlite3_stmt* stmt = nullptr;
        const char* insert_sql = "INSERT INTO json_test (json_data) VALUES (?1);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Test json_each()
        stmt = nullptr;
        const char* each_sql = "SELECT * FROM json_each(?1);";
        rc = sqlite3_prepare_v2(db, each_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Process json_each results
            }
            sqlite3_finalize(stmt);
        }
        
        // Test json_tree()
        stmt = nullptr;
        const char* tree_sql = "SELECT * FROM json_tree(?1);";
        rc = sqlite3_prepare_v2(db, tree_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            sqlite3_bind_text(stmt, 1, json_str.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Process json_tree results
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Test aggregate functions if we have multiple rows
    if (fdp.remaining_bytes() > 200) {
        // Insert multiple rows for aggregate testing
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
            size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
            std::string extra_json = fdp.ConsumeBytesAsString(extra_len);
            
            sqlite3_stmt* stmt = nullptr;
            const char* insert_sql = "INSERT INTO json_test (json_data) VALUES (?1);";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                sqlite3_bind_text(stmt, 1, extra_json.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
        
        // Test json_group_array()
        sqlite3_stmt* stmt = nullptr;
        const char* group_array_sql = "SELECT json_group_array(json_data) FROM json_test;";
        rc = sqlite3_prepare_v2(db, group_array_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Aggregate array created
            }
            sqlite3_finalize(stmt);
        }
        
        // Test json_group_object() with id as key
        stmt = nullptr;
        const char* group_object_sql = "SELECT json_group_object(id, json_data) FROM json_test;";
        rc = sqlite3_prepare_v2(db, group_object_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Aggregate object created
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_010
