#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_013 {
/*
 * Fuzzing harness for SQLite JSON1 extension functions with comprehensive testing.
 * Targets uncovered areas identified by analysis:
 * - Comprehensive JSON1 extension functions: json(), json_extract(), json_set(), json_insert(), 
 *   json_replace(), json_remove(), json_array(), json_object(), json_group_array(),
 *   json_group_object(), json_valid(), json_quote(), json_type(), json_array_length(),
 *   json_each(), json_tree(), json_patch(), json_merge_patch()
 * - JSONB functionality: Testing both text JSON and JSONB binary format
 * - Advanced JSON operations: Complex JSON manipulation and validation
 * 
 * This harness complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search - FTS3/FTS4)
 * - harness_005.cpp (backup/memory/configuration APIs)
 * - harness_006.cpp (basic database operations)
 * - harness_007.cpp (WAL mode, PRAGMA, basic JSON/R*Tree) - has some basic JSON
 * - harness_008.cpp (FTS5, GEOPOLY, window functions, CTEs)
 * - harness_009.cpp (minimal open/close)
 * - harness_010.cpp (basic open/exec/close)
 * - harness_011.cpp (advanced transactions/PRAGMA/VACUUM)
 * - harness_012.cpp (security/monitoring features)
 * 
 * Focuses on comprehensive JSON function testing with advanced features.
 * Uses FuzzedDataProvider to consume fuzzer input for JSON data and operations.
 */



/*
 * Simple callback for sqlite3_exec.
 */
static int exec_callback(void* NotUsed, int argc, char** argv, char** col_names) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    // Open in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    char* err_msg = nullptr;
    
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    switch (operation) {
        case 0: {
            // Test JSON text functions with fuzzer input
            
            // Create test table
            const char* create_sql = 
                "CREATE TABLE IF NOT EXISTS json_data ("
                "id INTEGER PRIMARY KEY, "
                "json_text TEXT, "
                "value INTEGER"
                ");";
            
            rc = sqlite3_exec(db, create_sql, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Generate JSON data from fuzzer input
            std::string json_str = fdp.ConsumeRandomLengthString(512);
            if (json_str.empty()) {
                json_str = "{\"test\": 123}";
            }
            
            // Insert JSON data
            std::string insert_sql = "INSERT INTO json_data(json_text, value) VALUES('" + 
                                    json_str + "', " + std::to_string(fdp.ConsumeIntegral<int>()) + ");";
            
            rc = sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test various JSON functions
            // Consume which JSON function to test
            uint8_t json_func = fdp.ConsumeIntegral<uint8_t>() % 12;
            
            // Create a test query with different JSON functions
            std::string test_query;
            switch (json_func) {
                case 0:
                    test_query = "SELECT json_valid(json_text) FROM json_data;";
                    break;
                case 1:
                    test_query = "SELECT json_type(json_text) FROM json_data;";
                    break;
                case 2:
                    test_query = "SELECT json_extract(json_text, '$.test') FROM json_data;";
                    break;
                case 3:
                    test_query = "SELECT json_array_length(json_text) FROM json_data;";
                    break;
                case 4:
                    test_query = "SELECT json_quote(json_text) FROM json_data;";
                    break;
                case 5:
                    test_query = "SELECT json('{\"id\": 1, \"name\": \"test\"}');";
                    break;
                case 6:
                    test_query = "SELECT json_object('id', 1, 'name', 'test');";
                    break;
                case 7:
                    test_query = "SELECT json_array(1, 2, 3, 'test');";
                    break;
                case 8:
                    test_query = "SELECT json_set('{\"a\":1}', '$.b', 2);";
                    break;
                case 9:
                    test_query = "SELECT json_insert('{\"a\":1}', '$.b', 2);";
                    break;
                case 10:
                    test_query = "SELECT json_replace('{\"a\":1}', '$.a', 2);";
                    break;
                case 11:
                    test_query = "SELECT json_remove('{\"a\":1, \"b\":2}', '$.b');";
                    break;
                default:
                    test_query = "SELECT json_valid(json_text) FROM json_data;";
            }
            
            rc = sqlite3_exec(db, test_query.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            break;
        }
        
        case 1: {
            // Test more advanced JSON operations and JSONB
            
            // Create table with JSONB column
            const char* create_sql = 
                "CREATE TABLE IF NOT EXISTS jsonb_test ("
                "id INTEGER PRIMARY KEY, "
                "data BLOB,  -- For JSONB"
                "metadata TEXT"
                ");";
            
            rc = sqlite3_exec(db, create_sql, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Generate binary data that could be JSONB
            size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
            
            // Insert binary data (potentially JSONB)
            if (!blob_data.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const char* insert_sql = "INSERT INTO jsonb_test(data, metadata) VALUES(?, ?);";
                
                rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // Bind blob
                    rc = sqlite3_bind_blob(stmt, 1, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                    
                    // Bind metadata
                    std::string metadata = fdp.ConsumeRandomLengthString(128);
                    rc = sqlite3_bind_text(stmt, 2, metadata.c_str(), -1, SQLITE_TRANSIENT);
                    
                    // Execute
                    rc = sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }
            
            // Test JSON functions on the binary data
            // json() and json_valid() should work on both text and JSONB blobs
            const char* json_test_queries[] = {
                "SELECT json_valid(data) FROM jsonb_test;",
                "SELECT json(data) FROM jsonb_test;",
                "SELECT json_extract(data, '$') FROM jsonb_test;",
                "SELECT json_type(data) FROM jsonb_test;"
            };
            
            size_t num_queries = sizeof(json_test_queries) / sizeof(json_test_queries[0]);
            uint8_t query_idx = fdp.ConsumeIntegral<uint8_t>() % num_queries;
            
            rc = sqlite3_exec(db, json_test_queries[query_idx], exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test json_each() and json_tree() table-valued functions
            // These require a valid JSON input
            std::string test_json = "{\"array\": [1, 2, 3], \"object\": {\"key\": \"value\"}}";
            std::string json_each_query = "SELECT * FROM json_each('" + test_json + "');";
            std::string json_tree_query = "SELECT * FROM json_tree('" + test_json + "');";
            
            // Try json_each
            rc = sqlite3_exec(db, json_each_query.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Try json_tree
            rc = sqlite3_exec(db, json_tree_query.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test json_patch() and json_merge_patch()
            std::string patch_test = "SELECT json_patch('{\"a\":1}', '{\"b\":2}');";
            std::string merge_test = "SELECT json_merge_patch('{\"a\":1}', '{\"a\":2}');";
            
            rc = sqlite3_exec(db, patch_test.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            rc = sqlite3_exec(db, merge_test.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test json_group_array() and json_group_object() aggregate functions
            // Create a table with multiple rows for aggregation
            const char* create_agg_sql = 
                "CREATE TABLE IF NOT EXISTS json_agg_test ("
                "id INTEGER PRIMARY KEY, "
                "name TEXT, "
                "score INTEGER"
                ");";
            
            rc = sqlite3_exec(db, create_agg_sql, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some rows for aggregation
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string name = fdp.ConsumeRandomLengthString(16);
                int score = fdp.ConsumeIntegral<int>();
                std::string insert_agg = "INSERT INTO json_agg_test(name, score) VALUES('" + 
                                        name + "', " + std::to_string(score) + ");";
                rc = sqlite3_exec(db, insert_agg.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test aggregation functions
            std::string agg_query1 = "SELECT json_group_array(name) FROM json_agg_test;";
            std::string agg_query2 = "SELECT json_group_object(name, score) FROM json_agg_test;";
            
            rc = sqlite3_exec(db, agg_query1.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            rc = sqlite3_exec(db, agg_query2.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            break;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_013
