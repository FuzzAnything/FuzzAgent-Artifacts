#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_015 {
/*
 * Fuzzing harness for SQLite FTS5 (Full Text Search version 5) extension.
 * Targets comprehensive coverage of FTS5 features not fully covered by existing harnesses.
 * 
 * This harness focuses exclusively on FTS5 with deep semantic diversity:
 * - FTS5 virtual table creation with various options (contentless, prefix indexes, etc.)
 * - Contentless tables and contentless_delete option
 * - Advanced query syntax: prefix queries, phrase queries, NEAR queries
 * - FTS5 auxiliary functions: highlight(), snippet(), bm25()
 * - FTS5 special commands: optimize, integrity-check, merge, delete-all
 * - Column-specific queries and tokenizer options
 * 
 * Existing harness coverage:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search - FTS3/FTS4)
 * - harness_005.cpp (backup/memory/configuration APIs)
 * - harness_006.cpp (basic database operations)
 * - harness_007.cpp (WAL mode, PRAGMA, basic JSON/R*Tree)
 * - harness_008.cpp (FTS5, GEOPOLY, window functions, CTEs) - has basic FTS5
 * - harness_009.cpp (minimal open/close)
 * - harness_010.cpp (basic open/exec/close)
 * - harness_011.cpp (advanced transactions/PRAGMA/VACUUM)
 * - harness_012.cpp (security/monitoring features)
 * - harness_013.cpp (JSON1 extension functions)
 * - harness_014.cpp (fuzzer virtual table, advanced RTREE)
 * 
 * This harness provides deeper FTS5 coverage than harness_008.cpp with more
 * comprehensive testing of FTS5-specific features and edge cases.
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
    // Minimum size check: need enough data for meaningful FTS5 testing
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
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    char* err_msg = nullptr;
    
    switch (operation) {
        case 0: {
            // Test FTS5 virtual table creation with various options
            std::string table_name = "fts5_" + fdp.ConsumeRandomLengthString(8);
            std::string col1 = fdp.ConsumeRandomLengthString(12);
            if (col1.empty()) col1 = "content";
            std::string col2 = fdp.ConsumeRandomLengthString(12);
            if (col2.empty()) col2 = "title";
            
            // Build FTS5 table creation SQL with various options
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + " USING fts5(" + col1 + ", " + col2;
            
            // Add various FTS5 options based on fuzzer input
            std::vector<std::string> options;
            
            // Contentless table option
            if (fdp.ConsumeBool()) {
                options.push_back("content=''");
                // If contentless, also optionally add contentless_delete
                if (fdp.ConsumeBool()) {
                    options.push_back("contentless_delete=1");
                }
            }
            
            // Prefix indexes
            if (fdp.ConsumeBool()) {
                std::string prefix_val = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 4));
                if (fdp.ConsumeBool()) {
                    prefix_val += " " + std::to_string(fdp.ConsumeIntegralInRange<int>(5, 8));
                }
                options.push_back("prefix='" + prefix_val + "'");
            }
            
            // Columnsize option
            if (fdp.ConsumeBool()) {
                options.push_back("columnsize=" + std::to_string(fdp.ConsumeBool() ? 1 : 0));
            }
            
            // Detail option
            if (fdp.ConsumeBool()) {
                std::string detail_options[] = {"none", "full", "column"};
                options.push_back("detail=" + detail_options[fdp.ConsumeIntegral<uint8_t>() % 3]);
            }
            
            // Tokenizer option
            if (fdp.ConsumeBool()) {
                std::string tokenizer_options[] = {"porter", "unicode61", "simple"};
                options.push_back("tokenize='" + tokenizer_options[fdp.ConsumeIntegral<uint8_t>() % 3] + "'");
            }
            
            // Add options to CREATE statement
            for (size_t i = 0; i < options.size(); i++) {
                create_sql += ", " + options[i];
            }
            
            create_sql += ");";
            
            rc = sqlite3_exec(db, create_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            // If table creation succeeded, test various FTS5 operations
            if (rc == SQLITE_OK) {
                // Insert test documents
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                    std::string doc_content = fdp.ConsumeRandomLengthString(256);
                    std::string doc_title = fdp.ConsumeRandomLengthString(64);
                    
                    std::string insert_sql = "INSERT INTO " + table_name + " VALUES('" + 
                                            doc_content + "', '" + doc_title + "');";
                    
                    sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test various FTS5 query types
                if (fdp.remaining_bytes() > 10) {
                    std::string search_term = fdp.ConsumeRandomLengthString(24);
                    if (!search_term.empty()) {
                        // Test different query syntax patterns
                        std::vector<std::string> queries;
                        
                        // Basic match
                        queries.push_back("MATCH '" + search_term + "'");
                        
                        // Phrase query
                        queries.push_back("MATCH '\"" + search_term + "\"'");
                        
                        // Prefix query
                        queries.push_back("MATCH '" + search_term + "*'");
                        
                        // Column-specific query
                        queries.push_back("MATCH '" + col1 + ":" + search_term + "'");
                        
                        // NEAR query
                        if (search_term.length() > 3) {
                            std::string term1 = search_term.substr(0, search_term.length()/2);
                            std::string term2 = search_term.substr(search_term.length()/2);
                            queries.push_back("MATCH '\"" + term1 + " NEAR " + term2 + "\"'");
                        }
                        
                        // Boolean operators
                        queries.push_back("MATCH '" + search_term + " OR test'");
                        queries.push_back("MATCH '" + search_term + " AND sample'");
                        queries.push_back("MATCH 'NOT " + search_term + "'");
                        
                        // Execute a random query
                        uint8_t query_idx = fdp.ConsumeIntegral<uint8_t>() % queries.size();
                        std::string select_sql = "SELECT * FROM " + table_name + 
                                                " WHERE " + queries[query_idx] + ";";
                        
                        sqlite3_exec(db, select_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                }
                
                // Test FTS5 auxiliary functions
                if (fdp.ConsumeBool()) {
                    // Test highlight() function
                    std::string highlight_sql = "SELECT highlight(" + table_name + ", 0, '[', ']') FROM " + 
                                               table_name + " WHERE " + col1 + " MATCH 'test';";
                    sqlite3_exec(db, highlight_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Test snippet() function  
                    if (fdp.ConsumeBool()) {
                        std::string snippet_sql = "SELECT snippet(" + table_name + ", 0, '[', ']', '...', 10) FROM " + 
                                                 table_name + " WHERE " + col1 + " MATCH 'test';";
                        sqlite3_exec(db, snippet_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                    
                    // Test bm25() function
                    if (fdp.ConsumeBool()) {
                        std::string bm25_sql = "SELECT bm25(" + table_name + ") FROM " + 
                                              table_name + " WHERE " + col1 + " MATCH 'test';";
                        sqlite3_exec(db, bm25_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                }
                
                // Test FTS5 special commands
                std::string commands[] = {
                    "optimize",
                    "integrity-check",
                    "merge=0,4",
                    "delete-all",
                    "rebuild"
                };
                
                uint8_t cmd_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
                std::string cmd_sql = "INSERT INTO " + table_name + "(" + table_name + 
                                     ") VALUES('" + commands[cmd_idx] + "');";
                
                sqlite3_exec(db, cmd_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
            break;
        }
        
        case 1: {
            // Test contentless FTS5 tables specifically
            std::string table_name = "fts5_cl_" + fdp.ConsumeRandomLengthString(8);
            
            // Create contentless table
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                                    " USING fts5(content, content=''";
            
            // Optionally add contentless_delete
            if (fdp.ConsumeBool()) {
                create_sql += ", contentless_delete=1";
            }
            
            create_sql += ");";
            
            rc = sqlite3_exec(db, create_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            if (rc == SQLITE_OK) {
                // Insert into contentless table
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                    std::string content = fdp.ConsumeRandomLengthString(200);
                    int rowid = fdp.ConsumeIntegralInRange<int>(1, 1000);
                    
                    std::string insert_sql = "INSERT INTO " + table_name + 
                                            "(rowid, content) VALUES(" + 
                                            std::to_string(rowid) + ", '" + content + "');";
                    
                    sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test queries on contentless table
                if (fdp.remaining_bytes() > 5) {
                    std::string search_term = fdp.ConsumeRandomLengthString(16);
                    if (!search_term.empty()) {
                        std::string query_sql = "SELECT rowid FROM " + table_name + 
                                               " WHERE " + table_name + " MATCH '" + search_term + "';";
                        
                        sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                        
                        // Test table-valued function syntax
                        std::string tvf_sql = "SELECT rowid FROM " + table_name + "('" + search_term + "');";
                        sqlite3_exec(db, tvf_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                }
                
                // Test deletions on contentless table
                if (fdp.ConsumeBool()) {
                    int delete_rowid = fdp.ConsumeIntegralInRange<int>(1, 100);
                    std::string delete_sql = "DELETE FROM " + table_name + 
                                            " WHERE rowid = " + std::to_string(delete_rowid) + ";";
                    
                    sqlite3_exec(db, delete_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test FTS5 prefix queries and indexing
            std::string table_name = "fts5_prefix_" + fdp.ConsumeRandomLengthString(8);
            
            // Create table with prefix indexes
            std::string prefix_spec = "'" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3)) + 
                                     " " + std::to_string(fdp.ConsumeIntegralInRange<int>(4, 6)) + "'";
            
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                                    " USING fts5(content, prefix=" + prefix_spec + ");";
            
            rc = sqlite3_exec(db, create_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            if (rc == SQLITE_OK) {
                // Insert test data with varying length words
                std::string words[] = {"apple", "application", "apply", "appliance", 
                                      "banana", "band", "bandage", "bank",
                                      "cat", "catalog", "category", "caterpillar"};
                
                for (int i = 0; i < 6 && fdp.remaining_bytes() > 5; i++) {
                    std::string content;
                    for (int j = 0; j < 3; j++) {
                        content += words[fdp.ConsumeIntegralInRange<int>(0, 11)] + " ";
                    }
                    
                    std::string insert_sql = "INSERT INTO " + table_name + 
                                            " VALUES('" + content + "');";
                    
                    sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test prefix queries
                std::string prefixes[] = {"app*", "ban*", "cat*", "a*", "b*", "c*"};
                
                for (int i = 0; i < 2 && fdp.remaining_bytes() > 5; i++) {
                    std::string prefix = prefixes[fdp.ConsumeIntegralInRange<int>(0, 5)];
                    std::string query_sql = "SELECT * FROM " + table_name + 
                                           " WHERE " + table_name + " MATCH '" + prefix + "';";
                    
                    sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test FTS5 phrase queries and NEAR operator
            std::string table_name = "fts5_phrase_" + fdp.ConsumeRandomLengthString(8);
            
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                                    " USING fts5(content);";
            
            rc = sqlite3_exec(db, create_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            if (rc == SQLITE_OK) {
                // Insert phrase-rich content
                std::string phrases[] = {
                    "quick brown fox jumps over lazy dog",
                    "the rain in spain falls mainly on the plain",
                    "to be or not to be that is the question",
                    "all work and no play makes jack a dull boy",
                    "early to bed and early to rise makes a man healthy wealthy and wise"
                };
                
                for (int i = 0; i < 3 && i < 5; i++) {
                    std::string insert_sql = "INSERT INTO " + table_name + 
                                            " VALUES('" + phrases[i] + "');";
                    
                    sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test phrase queries
                std::string phrase_queries[] = {
                    "'\"quick brown\"'",
                    "'\"rain in spain\"'",
                    "'\"to be\"'",
                    "'\"all work\"'",
                    "'\"early to\"'"
                };
                
                // Test NEAR queries
                std::string near_queries[] = {
                    "'\"quick NEAR fox\"'",
                    "'\"rain NEAR plain\"'",
                    "'\"be NEAR question\"'",
                    "'\"work NEAR play\"'",
                    "'\"bed NEAR rise\"'"
                };
                
                if (fdp.remaining_bytes() > 5) {
                    // Test a phrase query
                    uint8_t query_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
                    std::string phrase_sql = "SELECT * FROM " + table_name + 
                                            " WHERE " + table_name + " MATCH " + phrase_queries[query_idx] + ";";
                    
                    sqlite3_exec(db, phrase_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                    
                    // Test a NEAR query
                    if (fdp.ConsumeBool()) {
                        query_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
                        std::string near_sql = "SELECT * FROM " + table_name + 
                                              " WHERE " + table_name + " MATCH " + near_queries[query_idx] + ";";
                        
                        sqlite3_exec(db, near_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test FTS5 with external content table
            std::string content_table = "content_" + fdp.ConsumeRandomLengthString(8);
            std::string fts_table = "fts5_ext_" + fdp.ConsumeRandomLengthString(8);
            
            // Create external content table
            std::string create_content_sql = "CREATE TABLE " + content_table + 
                                            "(id INTEGER PRIMARY KEY, title TEXT, body TEXT);";
            
            rc = sqlite3_exec(db, create_content_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            if (rc == SQLITE_OK) {
                // Create FTS5 table that references external content
                std::string create_fts_sql = "CREATE VIRTUAL TABLE " + fts_table + 
                                            " USING fts5(title, body, content=" + content_table + 
                                            ", content_rowid=id);";
                
                rc = sqlite3_exec(db, create_fts_sql.c_str(), exec_callback, nullptr, &err_msg);
                if (err_msg != nullptr) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
                
                if (rc == SQLITE_OK) {
                    // Insert into external content table
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
                        std::string title = fdp.ConsumeRandomLengthString(50);
                        std::string body = fdp.ConsumeRandomLengthString(200);
                        
                        std::string insert_sql = "INSERT INTO " + content_table + 
                                                "(title, body) VALUES('" + title + "', '" + body + "');";
                        
                        sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                        if (err_msg != nullptr) {
                            sqlite3_free(err_msg);
                            err_msg = nullptr;
                        }
                    }
                    
                    // Test queries on FTS5 table with external content
                    if (fdp.remaining_bytes() > 5) {
                        std::string search_term = fdp.ConsumeRandomLengthString(12);
                        if (!search_term.empty()) {
                            std::string query_sql = "SELECT * FROM " + fts_table + 
                                                   " WHERE " + fts_table + " MATCH '" + search_term + "';";
                            
                            sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
                            if (err_msg != nullptr) {
                                sqlite3_free(err_msg);
                                err_msg = nullptr;
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 5: {
            // Test FTS5 tokenizers and custom tokenization
            std::string table_name = "fts5_token_" + fdp.ConsumeRandomLengthString(8);
            
            // Choose tokenizer
            std::string tokenizer;
            uint8_t tok_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (tok_choice) {
                case 0:
                    tokenizer = "porter";
                    break;
                case 1:
                    tokenizer = "unicode61";
                    break;
                case 2:
                    tokenizer = "simple";
                    break;
                default:
                    tokenizer = "unicode61";
            }
            
            std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                                    " USING fts5(content, tokenize='" + tokenizer + "');";
            
            rc = sqlite3_exec(db, create_sql.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
            
            if (rc == SQLITE_OK) {
                // Insert text that will be tokenized differently by different tokenizers
                std::string test_texts[] = {
                    "running jumping swimming",
                    "café résumé naïve",
                    "TEST test Test",
                    "hello-world good_bye",
                    "1.2.3.4 5-6-7-8"
                };
                
                for (int i = 0; i < 2 && i < 5; i++) {
                    std::string insert_sql = "INSERT INTO " + table_name + 
                                            " VALUES('" + test_texts[i] + "');";
                    
                    sqlite3_exec(db, insert_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
                }
                
                // Test queries that exercise tokenizer behavior
                std::string token_queries[] = {
                    "'run*'",      // Porter stemmer should match "running"
                    "'cafe'",      // Unicode61 should match "café"
                    "'test'",      // Should match "TEST", "test", "Test"
                    "'hello'",     // Simple tokenizer
                    "'1.2'",       // Numbers with dots
                };
                
                if (fdp.remaining_bytes() > 5 && tok_choice < 5) {
                    std::string query_sql = "SELECT * FROM " + table_name + 
                                           " WHERE " + table_name + " MATCH " + token_queries[tok_choice] + ";";
                    
                    sqlite3_exec(db, query_sql.c_str(), exec_callback, nullptr, &err_msg);
                    if (err_msg != nullptr) {
                        sqlite3_free(err_msg);
                        err_msg = nullptr;
                    }
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

} // namespace harness_015
