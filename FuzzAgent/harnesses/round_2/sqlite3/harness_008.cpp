#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/*
 * Fuzzing harness for SQLite advanced extensions and features not covered in previous harnesses.
 * Targets uncovered areas identified by coverage analysis:
 * - FTS5 extension (distinct from FTS4): Advanced full-text search features
 * - GEOPOLY extension: Geometric polygon operations  
 * - Advanced SQL features: Window functions, Common Table Expressions (CTEs), recursive queries
 * - Database maintenance operations: VACUUM, ANALYZE
 * - Advanced error handling APIs and configuration
 * 
 * This harness complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search - FTS3/FTS4)
 * - harness_005.cpp (backup/memory/configuration APIs)
 * - harness_006.cpp (basic database operations)
 * - harness_007.cpp (WAL mode, PRAGMA, basic JSON/R*Tree)
 * 
 * Focuses on advanced extensions requiring complex setup and specialized APIs.
 * Uses FuzzedDataProvider to consume fuzzer input for FTS5 queries, GEOPOLY data,
 * complex SQL statements, and maintenance operations.
 */



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
/*
 * Callback for error handling test.
 */
static void error_handler(void* pArg, int iErrCode, const char* zMsg) {
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
    // Just consume the error for fuzzing purposes
}

/*
 * Busy handler callback for testing.
 */
static int busy_handler(void* data, int count) {
    (void)data;
    (void)count;
    return 0;  // Return 0 to abort operation
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
    int rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    char* err_msg = nullptr;
    
    switch (operation) {
        case 0: {
            // Test FTS5 extension (advanced full-text search)
            // Different from FTS3/FTS4 tested in harness_004.cpp
            
            // Create FTS5 virtual table
            std::string fts5_table = "fts5_test_" + fdp.ConsumeRandomLengthString(8);
            std::string col1 = fdp.ConsumeRandomLengthString(16);
            if (col1.empty()) col1 = "title";
            std::string col2 = fdp.ConsumeRandomLengthString(16);
            if (col2.empty()) col2 = "content";
            std::string col3 = fdp.ConsumeRandomLengthString(16);
            if (col3.empty()) col3 = "author";
            
            // FTS5 supports additional options like contentless tables, prefix indexes, etc.
            std::string fts5_options = "";
            if (fdp.ConsumeBool()) {
                fts5_options = "content=''";  // Contentless table
            } else if (fdp.ConsumeBool()) {
                fts5_options = "prefix='2 4'";  // Prefix indexes
            }
            
            std::string create_fts5_sql = "CREATE VIRTUAL TABLE " + fts5_table + 
                                         " USING fts5(" + col1 + ", " + col2 + ", " + col3;
            if (!fts5_options.empty()) {
                create_fts5_sql += ", " + fts5_options;
            }
            create_fts5_sql += ");";
            
            rc = sqlite3_exec(db, create_fts5_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // If FTS5 table creation succeeded, test advanced FTS5 features
            if (rc == SQLITE_OK) {
                // Insert test data
                std::string title = fdp.ConsumeRandomLengthString(128);
                std::string content = fdp.ConsumeRandomLengthString(512);
                std::string author = fdp.ConsumeRandomLengthString(64);
                
                std::string insert_sql = "INSERT INTO " + fts5_table + 
                                        " VALUES('" + title + "', '" + content + "', '" + author + "');";
                sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test FTS5 MATCH queries with various operators
                std::string search_term = fdp.ConsumeRandomLengthString(32);
                if (!search_term.empty()) {
                    // Try different FTS5 query syntax
                    std::string match_queries[] = {
                        "MATCH '" + search_term + "'",
                        "MATCH '\"" + search_term + "\"'",  // Phrase search
                        "MATCH '" + search_term + "*'",     // Prefix search
                        "MATCH 'NEAR(" + search_term + ")'"
                    };
                    
                    uint8_t query_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
                    std::string select_sql = "SELECT * FROM " + fts5_table + 
                                            " WHERE " + match_queries[query_idx] + ";";
                    
                    sqlite3_exec(db, select_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test FTS5 auxiliary functions like highlight(), snippet(), bm25()
                if (fdp.ConsumeBool()) {
                    std::string aux_sql = "SELECT highlight(" + fts5_table + ", 1, '<b>', '</b>') FROM " + 
                                         fts5_table + " WHERE " + col2 + " MATCH 'test';";
                    sqlite3_exec(db, aux_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test GEOPOLY extension (geometric polygon operations)
            // First check if GEOPOLY extension is available by creating a geopoly table
            
            std::string geopoly_table = "geopoly_test_" + fdp.ConsumeRandomLengthString(8);
            std::string create_geopoly_sql = "CREATE VIRTUAL TABLE " + geopoly_table + 
                                            " USING geopoly(id, shape);";
            
            rc = sqlite3_exec(db, create_geopoly_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // If geopoly table creation succeeded, test geopoly operations
            if (rc == SQLITE_OK) {
                // Insert polygon data using geopoly format
                // Geopoly format: polygon encoded as binary or JSON
                std::string polygon_data = "";
                
                if (fdp.ConsumeBool()) {
                    // Simple triangle
                    polygon_data = "[[0,0],[10,0],[5,10],[0,0]]";
                } else {
                    // Rectangle
                    polygon_data = "[[0,0],[20,0],[20,15],[0,15],[0,0]]";
                }
                
                std::string id_val = std::to_string(fdp.ConsumeIntegral<int>());
                std::string insert_sql = "INSERT INTO " + geopoly_table + 
                                        " VALUES(" + id_val + ", '" + polygon_data + "');";
                
                sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test geopoly functions if data was inserted
                if (rc == SQLITE_OK) {
                    // Test geopoly functions: geopoly_area(), geopoly_bbox(), geopoly_contains_point()
                    std::string area_sql = "SELECT geopoly_area(shape) FROM " + geopoly_table + ";";
                    sqlite3_exec(db, area_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Test geopoly_bbox
                    std::string bbox_sql = "SELECT geopoly_bbox(shape) FROM " + geopoly_table + ";";
                    sqlite3_exec(db, bbox_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Test geopoly_contains_point with random point
                    int point_x = fdp.ConsumeIntegralInRange<int>(-10, 30);
                    int point_y = fdp.ConsumeIntegralInRange<int>(-10, 30);
                    std::string contains_sql = "SELECT geopoly_contains_point(shape, " + 
                                              std::to_string(point_x) + ", " + 
                                              std::to_string(point_y) + ") FROM " + 
                                              geopoly_table + ";";
                    sqlite3_exec(db, contains_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test advanced SQL features: Window functions, CTEs, recursive queries
            
            // Create test tables for complex queries
            std::string create_emp_sql = 
                "CREATE TABLE IF NOT EXISTS employees ("
                "id INTEGER PRIMARY KEY, "
                "name TEXT, "
                "department TEXT, "
                "salary INTEGER, "
                "hire_date TEXT);";
            
            rc = sqlite3_exec(db, create_emp_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some test data
            std::string depts[] = {"Sales", "Engineering", "Marketing", "HR"};
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
                std::string name = fdp.ConsumeRandomLengthString(20);
                std::string dept = depts[fdp.ConsumeIntegral<uint8_t>() % 4];
                int salary = fdp.ConsumeIntegralInRange<int>(30000, 150000);
                std::string hire_date = "202" + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 3)) + 
                                       "-" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 12)) + 
                                       "-" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 28));
                
                std::string insert_sql = "INSERT INTO employees(name, department, salary, hire_date) "
                                        "VALUES('" + name + "', '" + dept + "', " + 
                                        std::to_string(salary) + ", '" + hire_date + "');";
                
                sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test Window Functions
            std::string window_sql = 
                "SELECT department, name, salary, "
                "RANK() OVER (PARTITION BY department ORDER BY salary DESC) as rank_in_dept, "
                "AVG(salary) OVER (PARTITION BY department) as avg_dept_salary "
                "FROM employees;";
            
            rc = sqlite3_exec(db, window_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test Common Table Expressions (CTEs)
            std::string cte_sql = 
                "WITH dept_summary AS ("
                "  SELECT department, COUNT(*) as emp_count, AVG(salary) as avg_salary "
                "  FROM employees "
                "  GROUP BY department"
                ") "
                "SELECT * FROM dept_summary WHERE emp_count > 0;";
            
            rc = sqlite3_exec(db, cte_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test Recursive CTEs
            std::string recursive_cte_sql = 
                "WITH RECURSIVE cnt(x) AS ("
                "  SELECT 1 "
                "  UNION ALL "
                "  SELECT x+1 FROM cnt WHERE x < 5"
                ") "
                "SELECT x FROM cnt;";
            
            rc = sqlite3_exec(db, recursive_cte_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            break;
        }
        
        case 3: {
            // Test database maintenance operations: VACUUM, ANALYZE
            // First create and populate a table
            
            std::string maint_table = "maint_test_" + fdp.ConsumeRandomLengthString(8);
            std::string create_maint_sql = 
                "CREATE TABLE " + maint_table + " ("
                "id INTEGER PRIMARY KEY, "
                "data TEXT, "
                "value INTEGER);";
            
            rc = sqlite3_exec(db, create_maint_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some data
            for (int i = 0; i < 10 && fdp.remaining_bytes() > 5; i++) {
                std::string data_val = fdp.ConsumeRandomLengthString(50);
                int int_val = fdp.ConsumeIntegral<int>();
                
                std::string insert_sql = "INSERT INTO " + maint_table + 
                                        "(data, value) VALUES('" + data_val + "', " + 
                                        std::to_string(int_val) + ");";
                
                sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Delete some rows to create fragmentation
            std::string delete_sql = "DELETE FROM " + maint_table + " WHERE id % 2 = 0;";
            sqlite3_exec(db, delete_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test VACUUM operation
            rc = sqlite3_exec(db, "VACUUM;", exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test ANALYZE operation
            rc = sqlite3_exec(db, "ANALYZE;", exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test ANALYZE on specific table
            std::string analyze_table_sql = "ANALYZE " + maint_table + ";";
            rc = sqlite3_exec(db, analyze_table_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Test REINDEX
            std::string reindex_sql = "REINDEX;";
            rc = sqlite3_exec(db, reindex_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            break;
        }
        
        case 4: {
            // Test advanced error handling and configuration APIs
            
            // Register error handler callback
            void* error_handler_arg = (void*)(intptr_t)fdp.ConsumeIntegral<int>();
            sqlite3_config(SQLITE_CONFIG_LOG, error_handler, error_handler_arg);
            
            // Test various configuration options
            int config_opt = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (config_opt) {
                case 0:
                    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                    break;
                case 1:
                    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                    break;
                case 2:
                    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                    break;
            }
            
            // Test database configuration
            sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 
                            fdp.ConsumeBool() ? 1 : 0, nullptr);
            sqlite3_db_config(db, SQLITE_DBCONFIG_TRIGGER_EQP,
                            fdp.ConsumeBool() ? 1 : 0, nullptr);
            sqlite3_db_config(db, SQLITE_DBCONFIG_DEFENSIVE,
                            fdp.ConsumeBool() ? 1 : 0, nullptr);
            
            // Test extended error codes
            // First create a situation that might cause an error
            std::string problematic_sql = fdp.ConsumeRandomLengthString(256);
            if (!problematic_sql.empty()) {
                rc = sqlite3_exec(db, problematic_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    // Get extended error code
                    int extended_rc = sqlite3_extended_errcode(db);
                    (void)extended_rc;  // Use extended error code
                    
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test sqlite3_errmsg and sqlite3_errstr
            const char* err_str = sqlite3_errmsg(db);
            (void)err_str;  // Use error message
            
            int test_err_code = fdp.ConsumeIntegralInRange<int>(0, 100);
            const char* err_desc = sqlite3_errstr(test_err_code);
            (void)err_desc;  // Use error description
            
            // Test busy handler
            sqlite3_busy_handler(db, busy_handler, nullptr);
            
            break;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_008
