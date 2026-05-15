#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_014 {
/*
 * Fuzzing harness for SQLite advanced extensions not well-covered by existing harnesses.
 * Targets uncovered areas identified by analysis:
 * - Fuzzer virtual table: String transformation virtual table for spelling correction
 * - Advanced RTREE operations: Beyond basic CREATE VIRTUAL TABLE to include queries, constraints
 * - Database session APIs (basic simulation if extension not available)
 * 
 * This harness complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search - FTS3/FTS4)
 * - harness_005.cpp (backup/memory/configuration APIs)
 * - harness_006.cpp (basic database operations)
 * - harness_007.cpp (WAL mode, PRAGMA, basic JSON/R*Tree) - has minimal R*Tree
 * - harness_008.cpp (FTS5, GEOPOLY, window functions, CTEs)
 * - harness_009.cpp (minimal open/close)
 * - harness_010.cpp (basic open/exec/close)
 * - harness_011.cpp (advanced transactions/PRAGMA/VACUUM)
 * - harness_012.cpp (security/monitoring features)
 * - harness_013.cpp (JSON1 extension functions)
 * 
 * Focuses on fuzzer virtual table for string transformations and advanced RTREE query operations.
 * Uses FuzzedDataProvider to consume fuzzer input for fuzzer rules and RTREE queries.
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
    
    // Open in-memory database (use simple open for compatibility)
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    char* err_msg = nullptr;
    
    switch (operation) {
        case 0: {
            // Test Fuzzer Virtual Table (string transformation virtual table)
            
            // First create the fuzzer data table required by the fuzzer virtual table
            const char* create_fuzzer_data = 
                "CREATE TABLE IF NOT EXISTS fuzzer_data ("
                "ruleset INTEGER, "
                "cFrom TEXT, "
                "cTo TEXT, "
                "cost INTEGER"
                ");";
            
            rc = sqlite3_exec(db, create_fuzzer_data, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert some transformation rules using fuzzer input
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                int ruleset = fdp.ConsumeIntegralInRange<int>(0, 2);
                std::string cFrom = fdp.ConsumeRandomLengthString(2);
                std::string cTo = fdp.ConsumeRandomLengthString(2);
                int cost = fdp.ConsumeIntegralInRange<int>(1, 200);
                
                std::string insert_rule = "INSERT INTO fuzzer_data VALUES(" +
                                         std::to_string(ruleset) + ", '" +
                                         cFrom + "', '" + cTo + "', " +
                                         std::to_string(cost) + ");";
                
                rc = sqlite3_exec(db, insert_rule.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Try to create fuzzer virtual table (may fail if fuzzer extension not compiled in)
            std::string create_fuzzer_vtab = "CREATE VIRTUAL TABLE IF NOT EXISTS f USING fuzzer(fuzzer_data);";
            rc = sqlite3_exec(db, create_fuzzer_vtab.c_str(), exec_callback, nullptr, &err_msg);
            
            if (rc == SQLITE_OK) {
                // Test fuzzer table queries with fuzzer input
                std::string search_word = fdp.ConsumeRandomLengthString(32);
                if (search_word.empty()) {
                    search_word = "test";
                }
                
                int max_distance = fdp.ConsumeIntegralInRange<int>(50, 500);
                
                std::string fuzzer_query = "SELECT word, distance FROM f "
                                          "WHERE word MATCH '" + search_word + "' "
                                          "AND distance < " + std::to_string(max_distance) + " "
                                          "LIMIT 10;";
                
                rc = sqlite3_exec(db, fuzzer_query.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test with ruleset constraint
                int ruleset = fdp.ConsumeIntegralInRange<int>(0, 2);
                std::string fuzzer_query_with_ruleset = "SELECT word, distance FROM f "
                                                       "WHERE word MATCH '" + search_word + "' "
                                                       "AND distance < " + std::to_string(max_distance) + " "
                                                       "AND ruleset = " + std::to_string(ruleset) + " "
                                                       "LIMIT 5;";
                
                rc = sqlite3_exec(db, fuzzer_query_with_ruleset.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            } else {
                // Fuzzer virtual table not available, test regular string functions
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test SQL string functions instead
                std::string test_string = fdp.ConsumeRandomLengthString(128);
                if (!test_string.empty()) {
                    std::string string_test = "SELECT LENGTH('" + test_string + "'), "
                                             "UPPER('" + test_string + "'), "
                                             "LOWER('" + test_string + "'), "
                                             "SUBSTR('" + test_string + "', 1, 5);";
                    
                    rc = sqlite3_exec(db, string_test.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            
            break;
        }
        
        case 1: {
            // Test Advanced RTREE operations (beyond basic CREATE VIRTUAL TABLE)
            
            // Try to create RTREE virtual table (may fail if RTREE extension not compiled in)
            std::string rtree_table = "rtree_advanced_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
            std::string create_rtree = "CREATE VIRTUAL TABLE IF NOT EXISTS " + rtree_table + 
                                      " USING rtree(id, minX, maxX, minY, maxY, minZ, maxZ);";
            
            rc = sqlite3_exec(db, create_rtree.c_str(), exec_callback, nullptr, &err_msg);
            
            if (rc == SQLITE_OK) {
                // Insert RTREE data using fuzzer input
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 24; i++) {
                    int id = i + 1;
                    double minX = static_cast<double>(fdp.ConsumeIntegral<int>()) / 100.0;
                    double maxX = minX + static_cast<double>(fdp.ConsumeIntegralInRange<int>(1, 100)) / 100.0;
                    double minY = static_cast<double>(fdp.ConsumeIntegral<int>()) / 100.0;
                    double maxY = minY + static_cast<double>(fdp.ConsumeIntegralInRange<int>(1, 100)) / 100.0;
                    double minZ = static_cast<double>(fdp.ConsumeIntegral<int>()) / 100.0;
                    double maxZ = minZ + static_cast<double>(fdp.ConsumeIntegralInRange<int>(1, 100)) / 100.0;
                    
                    std::string insert_rtree = "INSERT INTO " + rtree_table + 
                                              " VALUES(" + std::to_string(id) + ", " +
                                              std::to_string(minX) + ", " + std::to_string(maxX) + ", " +
                                              std::to_string(minY) + ", " + std::to_string(maxY) + ", " +
                                              std::to_string(minZ) + ", " + std::to_string(maxZ) + ");";
                    
                    rc = sqlite3_exec(db, insert_rtree.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test various RTREE queries with different constraints
                if (fdp.remaining_bytes() > 8) {
                    // Query 1: Basic containment query
                    double query_minX = static_cast<double>(fdp.ConsumeIntegral<int>()) / 100.0;
                    double query_maxX = query_minX + static_cast<double>(fdp.ConsumeIntegralInRange<int>(10, 200)) / 100.0;
                    double query_minY = static_cast<double>(fdp.ConsumeIntegral<int>()) / 100.0;
                    double query_maxY = query_minY + static_cast<double>(fdp.ConsumeIntegralInRange<int>(10, 200)) / 100.0;
                    
                    std::string rtree_query1 = "SELECT id FROM " + rtree_table + 
                                              " WHERE minX >= " + std::to_string(query_minX) +
                                              " AND maxX <= " + std::to_string(query_maxX) +
                                              " AND minY >= " + std::to_string(query_minY) +
                                              " AND maxY <= " + std::to_string(query_maxY) + ";";
                    
                    rc = sqlite3_exec(db, rtree_query1.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Query 2: Overlap query
                    std::string rtree_query2 = "SELECT id FROM " + rtree_table + 
                                              " WHERE maxX >= " + std::to_string(query_minX) +
                                              " AND minX <= " + std::to_string(query_maxX) +
                                              " AND maxY >= " + std::to_string(query_minY) +
                                              " AND minY <= " + std::to_string(query_maxY) + ";";
                    
                    rc = sqlite3_exec(db, rtree_query2.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Query 3: Distance-based query (if supported)
                    double centerX = (query_minX + query_maxX) / 2.0;
                    double centerY = (query_minY + query_maxY) / 2.0;
                    double radius = static_cast<double>(fdp.ConsumeIntegralInRange<int>(10, 100)) / 100.0;
                    
                    std::string rtree_query3 = "SELECT id FROM " + rtree_table + 
                                              " WHERE (minX - " + std::to_string(centerX) + ") * (minX - " + std::to_string(centerX) + ") + " +
                                              "(minY - " + std::to_string(centerY) + ") * (minY - " + std::to_string(centerY) + ") <= " +
                                              std::to_string(radius * radius) + ";";
                    
                    rc = sqlite3_exec(db, rtree_query3.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test RTREE with ORDER BY
                std::string rtree_order_query = "SELECT id, minX, maxX FROM " + rtree_table + 
                                               " ORDER BY minX, minY LIMIT 3;";
                
                rc = sqlite3_exec(db, rtree_order_query.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
            } else {
                // RTREE not available, test regular spatial queries on regular table
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Create a regular table for spatial-like queries
                const char* create_spatial = 
                    "CREATE TABLE IF NOT EXISTS spatial_test ("
                    "id INTEGER PRIMARY KEY, "
                    "x1 REAL, y1 REAL, x2 REAL, y2 REAL"
                    ");";
                
                rc = sqlite3_exec(db, create_spatial, exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            break;
        }
        
        case 2: {
            // Test combination of features: create tables with different extensions
            
            // Create a table for testing
            const char* create_combo = 
                "CREATE TABLE IF NOT EXISTS combo_test ("
                "id INTEGER PRIMARY KEY, "
                "text_data TEXT, "
                "num_data INTEGER"
                ");";
            
            rc = sqlite3_exec(db, create_combo, exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert data using fuzzer input
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                std::string text = fdp.ConsumeRandomLengthString(64);
                int number = fdp.ConsumeIntegral<int>();
                
                std::string insert_combo = "INSERT INTO combo_test(text_data, num_data) VALUES('" +
                                          text + "', " + std::to_string(number) + ");";
                
                rc = sqlite3_exec(db, insert_combo.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test various SQL features that might work with any table
            if (fdp.remaining_bytes() > 4) {
                // Test aggregate functions
                std::string aggregate_query = "SELECT COUNT(*), SUM(num_data), AVG(num_data), "
                                             "MIN(num_data), MAX(num_data) FROM combo_test;";
                
                rc = sqlite3_exec(db, aggregate_query.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test string functions on the text column
                std::string string_func_query = "SELECT id, LENGTH(text_data), UPPER(text_data), "
                                               "LOWER(text_data), SUBSTR(text_data, 1, 10) "
                                               "FROM combo_test;";
                
                rc = sqlite3_exec(db, string_func_query.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Test joins (self-join for simplicity)
                std::string join_query = "SELECT a.id, b.id FROM combo_test a, combo_test b "
                                        "WHERE a.num_data = b.num_data LIMIT 5;";
                
                rc = sqlite3_exec(db, join_query.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
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

} // namespace harness_014
