#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_004 {
/*
 * Fuzzing harness for SQLite Full Text Search (FTS) functions.
 * Targets uncovered areas identified by coverage analysis:
 * - FTS3 Full Text Search: CREATE VIRTUAL TABLE USING fts3, MATCH queries
 * - FTS4 Full Text Search: More advanced FTS features
 * 
 * This harness complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * 
 * Focuses on FTS table creation, querying, and various FTS operations.
 * Uses FuzzedDataProvider to consume fuzzer input for FTS content and queries.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for meaningful testing
    if (size < 16) {
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
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (operation) {
        case 0: {
            // Test FTS3 virtual table creation and basic operations
            
            // Create FTS virtual table with columns from fuzzer input
            std::string table_name = fdp.ConsumeRandomLengthString(32);
            if (table_name.empty()) {
                table_name = "fts_test";
            }
            
            // Consume column names for FTS table
            std::string col1 = fdp.ConsumeRandomLengthString(16);
            if (col1.empty()) col1 = "title";
            
            std::string col2 = fdp.ConsumeRandomLengthString(16);
            if (col2.empty()) col2 = "content";
            
            std::string col3 = fdp.ConsumeRandomLengthString(16);
            if (col3.empty()) col3 = "author";
            
            // Build CREATE VIRTUAL TABLE statement
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                                     " USING fts3(" + col1 + ", " + col2 + ", " + col3 + ")";
            
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
            
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // If table creation succeeded, test some FTS operations
            if (rc == SQLITE_OK) {
                // Insert some test data using fuzzer input
                std::string title_text = fdp.ConsumeRandomLengthString(256);
                std::string content_text = fdp.ConsumeRandomLengthString(512);
                std::string author_text = fdp.ConsumeRandomLengthString(128);
                
                std::string insert_sql = "INSERT INTO " + table_name + 
                                         " VALUES('" + title_text + "', '" + 
                                         content_text + "', '" + author_text + "')";
                
                rc = sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                // Try FTS MATCH queries with terms from fuzzer input
                std::string search_term1 = fdp.ConsumeRandomLengthString(32);
                if (!search_term1.empty()) {
                    std::string match_sql = "SELECT * FROM " + table_name + 
                                           " WHERE " + col2 + " MATCH '" + search_term1 + "'";
                    rc = sqlite3_exec(db, match_sql.c_str(), nullptr, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test multiple search terms
                std::string search_term2 = fdp.ConsumeRandomLengthString(32);
                if (!search_term2.empty()) {
                    std::string match_sql2 = "SELECT * FROM " + table_name + 
                                            " WHERE " + col2 + " MATCH '" + search_term1 + 
                                            " OR " + search_term2 + "'";
                    rc = sqlite3_exec(db, match_sql2.c_str(), nullptr, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test FTS auxiliary functions like offsets(), snippet()
                std::string aux_sql = "SELECT offsets(" + table_name + ") FROM " + 
                                     table_name + " WHERE " + col2 + " MATCH 'test'";
                rc = sqlite3_exec(db, aux_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            break;
        }
        
        case 1: {
            // Test FTS4 virtual table with more advanced features
            
            // Create FTS4 virtual table
            const char* create_fts4 = 
                "CREATE VIRTUAL TABLE IF NOT EXISTS fts4_docs USING fts4(title, body, author);";
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, create_fts4, nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert multiple rows using fuzzer input
            int num_rows = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < num_rows; i++) {
                std::string title = fdp.ConsumeRandomLengthString(128);
                std::string body = fdp.ConsumeRandomLengthString(512);
                std::string author = fdp.ConsumeRandomLengthString(64);
                
                std::string insert_sql = "INSERT INTO fts4_docs VALUES('" + 
                                         title + "', '" + body + "', '" + author + "')";
                
                rc = sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            // Test various FTS query types
            std::string query_type = fdp.ConsumeRandomLengthString(16);
            if (!query_type.empty()) {
                std::string search_term = fdp.ConsumeRandomLengthString(32);
                if (!search_term.empty()) {
                    // Different types of FTS queries
                    std::vector<std::string> queries = {
                        "SELECT * FROM fts4_docs WHERE body MATCH '" + search_term + "'",
                        "SELECT snippet(fts4_docs) FROM fts4_docs WHERE body MATCH '" + search_term + "'",
                        "SELECT offsets(fts4_docs) FROM fts4_docs WHERE body MATCH '" + search_term + "'",
                        "SELECT matchinfo(fts4_docs) FROM fts4_docs WHERE body MATCH '" + search_term + "'"
                    };
                    
                    for (const auto& query : queries) {
                        rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                }
            }
            
            // Test FTS prefix queries
            std::string prefix_term = fdp.ConsumeRandomLengthString(16);
            if (!prefix_term.empty()) {
                std::string prefix_query = "SELECT * FROM fts4_docs WHERE body MATCH '" + 
                                          prefix_term + "*'";
                rc = sqlite3_exec(db, prefix_query.c_str(), nullptr, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            
            break;
        }
        
        case 2: {
            // Test FTS with prepared statements and parameter binding
            
            // Create FTS table using fts3
            const char* create_fts = 
                "CREATE VIRTUAL TABLE IF NOT EXISTS prepared_fts USING fts3(doc_title, doc_content);";
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, create_fts, nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // Insert data using prepared statement with fuzzer input
            sqlite3_stmt* stmt = nullptr;
            const char* insert_sql = "INSERT INTO prepared_fts(doc_title, doc_content) VALUES(?, ?)";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Bind values from fuzzer input
                std::string title = fdp.ConsumeRandomLengthString(128);
                std::string content = fdp.ConsumeRandomLengthString(1024);
                
                sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_TRANSIENT);
                
                rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            // Test FTS MATCH with prepared statement
            std::string search_term = fdp.ConsumeRandomLengthString(64);
            if (!search_term.empty()) {
                std::string match_sql = "SELECT * FROM prepared_fts WHERE doc_content MATCH ?";
                rc = sqlite3_prepare_v2(db, match_sql.c_str(), -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, search_term.c_str(), -1, SQLITE_TRANSIENT);
                    
                    // Execute query
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // Process rows if any (just consume them)
                        // Could retrieve column data if needed
                    }
                    sqlite3_finalize(stmt);
                }
            }
            
            // Test more complex FTS queries with multiple conditions
            std::string search_term2 = fdp.ConsumeRandomLengthString(32);
            if (!search_term2.empty()) {
                std::string complex_query = 
                    "SELECT doc_title, snippet(prepared_fts) FROM prepared_fts " 
                    "WHERE doc_content MATCH ? OR doc_title MATCH ?";
                
                rc = sqlite3_prepare_v2(db, complex_query.c_str(), -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    sqlite3_bind_text(stmt, 1, search_term2.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 2, search_term2.c_str(), -1, SQLITE_TRANSIENT);
                    
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // Process rows
                    }
                    sqlite3_finalize(stmt);
                }
            }
            
            break;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_004
