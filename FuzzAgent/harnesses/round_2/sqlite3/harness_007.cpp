#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_007 {
/*
 * Fuzzing harness for SQLite advanced features not covered in previous harnesses.
 * Targets uncovered advanced SQLite features:
 * - WAL (Write-Ahead Log) mode APIs: sqlite3_wal_hook, sqlite3_wal_autocheckpoint, 
 *   sqlite3_wal_checkpoint, sqlite3_wal_checkpoint_v2
 * - PRAGMA statements via sqlite3_exec for various database settings
 * - JSON functions (if available): json(), json_extract(), json_set(), etc.
 * - Virtual table operations with sqlite3_module interface
 * - R*Tree module operations (if available)
 * 
 * This complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search)
 * - harness_005.cpp (backup/memory/configuration APIs)
 * - harness_006.cpp (basic database operations)
 * 
 * Focuses on advanced features requiring complex setup and specialized APIs.
 * Uses FuzzedDataProvider to consume fuzzer input for WAL operations, pragma settings,
 * JSON data, and virtual table operations.
 */



/*
 * WAL hook callback function for testing sqlite3_wal_hook API.
 * Returns SQLITE_OK to allow checkpoint, SQLITE_BUSY to defer, etc.
 */
static int test_wal_hook(void* pArg, sqlite3* db, const char* dbName, int nFrame) {
    (void)pArg;
    (void)db;
    (void)dbName;
    (void)nFrame;
    // Return SQLITE_OK to allow checkpoint, could vary based on fuzzer input
    return SQLITE_OK;
}

/*
 * Simple callback for sqlite3_exec that does nothing.
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
    
    // Open in-memory database with WAL journal mode enabled
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            // Test WAL mode and checkpoint APIs
            
            // First enable WAL mode via PRAGMA
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Register a WAL hook callback
            void* hook_ctx = (void*)(intptr_t)fdp.ConsumeIntegral<int>();
            sqlite3_wal_hook(db, test_wal_hook, hook_ctx);
            
            // Configure auto-checkpoint
            int checkpoint_threshold = fdp.ConsumeIntegralInRange<int>(-100, 10000);
            sqlite3_wal_autocheckpoint(db, checkpoint_threshold);
            
            // Create a test table and insert some data
            const char* create_table_sql = 
                "CREATE TABLE IF NOT EXISTS wal_test(id INTEGER PRIMARY KEY, data TEXT);";
            rc = sqlite3_exec(db, create_table_sql, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some test data using fuzzer input
            std::string test_data = fdp.ConsumeRandomLengthString(256);
            if (!test_data.empty()) {
                std::string insert_sql = "INSERT INTO wal_test(data) VALUES('" + test_data + "');";
                rc = sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test checkpoint APIs
            int checkpoint_mode = fdp.ConsumeIntegral<int>() % 4; // 0-3 for different modes
            
            if (checkpoint_mode == 0) {
                // Simple checkpoint
                rc = sqlite3_wal_checkpoint(db, "main");
            } else {
                // Advanced checkpoint with parameters
                int nLog = 0, nCkpt = 0;
                rc = sqlite3_wal_checkpoint_v2(db, "main", checkpoint_mode, &nLog, &nCkpt);
            }
            
            break;
        }
        
        case 1: {
            // Test various PRAGMA statements
            
            // Consume pragma type from fuzzer input
            uint8_t pragma_type = fdp.ConsumeIntegral<uint8_t>() % 8;
            char* err_msg = nullptr;
            std::string pragma_sql;
            
            switch (pragma_type) {
                case 0:
                    pragma_sql = "PRAGMA journal_mode = ";
                    pragma_sql += fdp.ConsumeBool() ? "WAL" : "DELETE";
                    break;
                case 1:
                    pragma_sql = "PRAGMA synchronous = ";
                    pragma_sql += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 3));
                    break;
                case 2:
                    pragma_sql = "PRAGMA cache_size = ";
                    pragma_sql += std::to_string(fdp.ConsumeIntegralInRange<int>(-2000, 2000));
                    break;
                case 3:
                    pragma_sql = "PRAGMA temp_store = ";
                    pragma_sql += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 3));
                    break;
                case 4:
                    pragma_sql = "PRAGMA foreign_keys = ";
                    pragma_sql += fdp.ConsumeBool() ? "ON" : "OFF";
                    break;
                case 5:
                    pragma_sql = "PRAGMA recursive_triggers = ";
                    pragma_sql += fdp.ConsumeBool() ? "ON" : "OFF";
                    break;
                case 6:
                    pragma_sql = "PRAGMA secure_delete = ";
                    pragma_sql += fdp.ConsumeBool() ? "ON" : "OFF";
                    break;
                case 7:
                    pragma_sql = "PRAGMA mmap_size = ";
                    pragma_sql += std::to_string(fdp.ConsumeIntegralInRange<size_t>(0, 1048576));
                    break;
            }
            
            pragma_sql += ";";
            rc = sqlite3_exec(db, pragma_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Also test some read-only pragmas
            rc = sqlite3_exec(db, "PRAGMA schema_version;", exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            rc = sqlite3_exec(db, "PRAGMA database_list;", exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            break;
        }
        
        case 2: {
            // Test JSON functions (if available in SQLite build)
            
            // Create a test table for JSON data
            char* err_msg = nullptr;
            const char* create_table_sql = 
                "CREATE TABLE IF NOT EXISTS json_test(id INTEGER PRIMARY KEY, json_data TEXT);";
            rc = sqlite3_exec(db, create_table_sql, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Generate JSON data from fuzzer input
            std::string json_key = fdp.ConsumeRandomLengthString(32);
            std::string json_value = fdp.ConsumeRandomLengthString(128);
            std::string json_data = "{\"" + json_key + "\": \"" + json_value + "\"}";
            
            // Insert JSON data
            std::string insert_sql = "INSERT INTO json_test(json_data) VALUES('" + json_data + "');";
            rc = sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Try to use JSON functions if available
            // These will fail silently if JSON extension is not compiled in
            std::string json_path = "$.\"" + json_key + "\"";
            std::string query_sql = "SELECT json_extract(json_data, '" + json_path + "') FROM json_test;";
            rc = sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test json() function
            query_sql = "SELECT json('{\"test\": " + std::to_string(fdp.ConsumeIntegral<int>()) + "}');";
            rc = sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            break;
        }
        
        case 3: {
            // Test virtual table operations
            
            // Try to create a virtual table (may fail if no module registered)
            char* err_msg = nullptr;
            
            // First try FTS virtual table (should work if FTS is compiled in)
            std::string table_name = fdp.ConsumeRandomLengthString(16);
            if (table_name.empty()) table_name = "virtual_test";
            
            std::string create_virtual_sql = "CREATE VIRTUAL TABLE IF NOT EXISTS " + table_name + 
                                            " USING fts4(content TEXT);";
            rc = sqlite3_exec(db, create_virtual_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
                // If FTS failed, try a regular table instead
                create_virtual_sql = "CREATE TABLE IF NOT EXISTS " + table_name + 
                                    "(id INTEGER, content TEXT);";
                rc = sqlite3_exec(db, create_virtual_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // If table creation succeeded, insert and query data
            if (rc == SQLITE_OK) {
                std::string content = fdp.ConsumeRandomLengthString(512);
                if (!content.empty()) {
                    std::string insert_sql = "INSERT INTO " + table_name + 
                                           "(content) VALUES('" + content + "');";
                    rc = sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Query the virtual/regular table
                    std::string query_sql = "SELECT * FROM " + table_name + ";";
                    rc = sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            
            // Try to use R*Tree if available (this is another virtual table module)
            std::string rtree_name = "rtree_test_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
            std::string create_rtree_sql = "CREATE VIRTUAL TABLE IF NOT EXISTS " + rtree_name + 
                                          " USING rtree(id, minX, maxX, minY, maxY);";
            rc = sqlite3_exec(db, create_rtree_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            break;
        }
    }
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_007
