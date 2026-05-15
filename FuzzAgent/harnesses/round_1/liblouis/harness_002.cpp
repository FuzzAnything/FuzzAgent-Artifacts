#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_002 {
/*
 * liblouis Table Discovery and Metadata Operations Fuzzing Harness
 * 
 * This harness targets the table discovery and metadata APIs in liblouis with 0% coverage.
 * Focuses on exercising table discovery, indexing, and metadata retrieval workflows.
 * 
 * APIs targeted (from coverage guidance):
 * - lou_findTables: Find all matches for a query
 * - lou_findTable: Find single best match for a query
 * - lou_getTableInfo: Get metadata from table files
 * - lou_listTables: List all available tables
 * - lou_indexTables: Index translation tables
 * - lou_logFile, lou_logPrint: Logging functions
 * 
 * Required helper APIs:
 * - lou_setDataPath: Set data path for table discovery
 * - lou_registerTableResolver: Register custom table resolver
 * - lou_setLogLevel, lou_registerLogCallback: Logging setup
 * - lou_freeTableInfo, lou_freeTableFiles, lou_free: Cleanup
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Tests translation with temporary fuzzer-created tables
 * - harness_001.cpp: Tests translation with real project tables
 * - This harness: Focuses on table discovery/metadata operations, not translation
 */


extern "C" {
}

// Custom table resolver for testing
static char **test_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL (no custom resolution)
    // This allows testing the resolver registration without actual resolution
    (void)table;
    (void)base;
    return NULL;
}

// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress all logging during fuzzing
}

// Available table directories for testing
static const char* table_directories[] = {
    "/root/src/liblouis/tables",
    "/usr/share/liblouis/tables",
    "/usr/local/share/liblouis/tables",
    "./tables",
    ""  // Empty string for default path
};

static const size_t num_dirs = sizeof(table_directories) / sizeof(table_directories[0]);

// Sample table files for testing (full paths)
static const char* sample_tables[] = {
    "/root/src/liblouis/tables/en-us-g2.ctb",
    "/root/src/liblouis/tables/en-us-g1.ctb", 
    "/root/src/liblouis/tables/en-us-comp8.ctb",
    "/root/src/liblouis/tables/afr-za-g1.ctb",
    "/root/src/liblouis/tables/afr-za-g2.ctb",
    "/root/src/liblouis/tables/Es-Es-G0.utb",
    "/root/src/liblouis/tables/IPA.utb",
    "/root/src/liblouis/tables/Lv-Lv-g1.utb",
    "/root/src/liblouis/tables/Pl-Pl-g1.utb"
};

static const size_t num_sample_tables = sizeof(sample_tables) / sizeof(sample_tables[0]);

// Sample metadata keys to query
static const char* metadata_keys[] = {
    "locale",
    "language",
    "type",
    "version",
    "description",
    "author",
    "license",
    "dots",
    "cells",
    "rows"
};

static const size_t num_keys = sizeof(metadata_keys) / sizeof(metadata_keys[0]);

// Sample table queries for testing
static const char* table_queries[] = {
    "en-us",
    "en",
    "us",
    "afr",
    "za",
    "g1",
    "g2",
    "comp",
    "Es",
    "Es-G0",
    "IPA",
    "Lv",
    "Pl",
    "ctb",
    "utb",
    "braille",
    "*"  // Wildcard
};

static const size_t num_queries = sizeof(table_queries) / sizeof(table_queries[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize logging once
    static int logging_initialized = 0;
    if (!logging_initialized) {
        lou_registerLogCallback(fuzz_log_callback);
        lou_setLogLevel(LOU_LOG_INFO);
        logging_initialized = 1;
    }
    
    // Need minimum input for meaningful testing
    // We need: operation selection + path selection + query data
    if (size < 12) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Setup: Configure data path for table discovery
    uint8_t dir_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_dirs - 1);
    const char* data_path = table_directories[dir_index];
    
    // Set data path (deprecated but still available for testing)
    char* old_path = lou_setDataPath(data_path);
    // Note: old_path points to a static buffer and should NOT be freed
    // The comment below was incorrect - DO NOT free static buffers
    
    // 2. Setup: Register custom table resolver
    lou_registerTableResolver(test_table_resolver);
    
    // 3. Determine which operations to test based on input
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // 4. Configure logging output
    if (fdp.ConsumeBool()) {
        // Test file logging
        std::string log_filename = fdp.ConsumeRandomLengthString(64);
        if (!log_filename.empty()) {
            lou_logFile(log_filename.c_str());
        }
    }
    
    // 5. Test different table discovery workflows based on operation mode
    switch (operation_mode) {
        case 0: {
            // Workflow: Index -> List -> Find -> GetInfo
            
            // 5.1. Index tables first
            if (fdp.ConsumeBool() && num_sample_tables > 0) {
                // Create a NULL-terminated array of table names
                const char** tables_to_index = new const char*[num_sample_tables + 1];
                for (size_t i = 0; i < num_sample_tables; i++) {
                    tables_to_index[i] = sample_tables[i];
                }
                tables_to_index[num_sample_tables] = NULL;
                
                lou_indexTables(tables_to_index);
                
                delete[] tables_to_index;
            }
            
            // 5.2. List available tables
            char** table_list = lou_listTables();
            if (table_list) {
                // Process the list
                int table_count = 0;
                for (char** ptr = table_list; *ptr != NULL; ptr++) {
                    table_count++;
                    
                    // 5.3. For some tables, get metadata
                    if (table_count <= 3 && fdp.ConsumeBool()) {
                        uint8_t key_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_keys - 1);
                        char* info = lou_getTableInfo(*ptr, metadata_keys[key_index]);
                        if (info) {
                            lou_freeTableInfo(info);
                        }
                    }
                    
                    // 5.4. For some tables, try to find them again
                    if (table_count <= 2 && fdp.ConsumeBool()) {
                        char* found = lou_findTable(*ptr);
                        if (found) {
                            lou_freeTableFile(found);
                        }
                    }
                }
                
                // 5.5. Find tables with queries
                if (fdp.remaining_bytes() > 0) {
                    uint8_t query_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_queries - 1);
                    char** found_tables = lou_findTables(table_queries[query_index]);
                    if (found_tables) {
                        lou_freeTableFiles(found_tables);
                    }
                }
                
                lou_freeTableFiles(table_list);
            }
            break;
        }
        
        case 1: {
            // Workflow: Find -> GetInfo -> Check
            
            // 5.1. Find table with query
            if (fdp.remaining_bytes() > 0) {
                uint8_t query_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_queries - 1);
                char* found_table = lou_findTable(table_queries[query_index]);
                
                if (found_table) {
                    // 5.2. Get metadata for found table
                    if (fdp.remaining_bytes() > 0) {
                        uint8_t key_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_keys - 1);
                        char* info = lou_getTableInfo(found_table, metadata_keys[key_index]);
                        if (info) {
                            lou_freeTableInfo(info);
                        }
                    }
                    
                    // 5.3. Check if table is valid
                    int check_result = lou_checkTable(found_table);
                    (void)check_result; // Use result to avoid unused warning
                    
                    // 5.4. Try to get the table
                    const void* table_ptr = lou_getTable(found_table);
                    (void)table_ptr; // Use result to avoid unused warning
                    
                    lou_freeTableFile(found_table);
                }
            }
            break;
        }
        
        case 2: {
            // Workflow: List -> Index -> Comprehensive queries
            
            // 5.1. List tables first
            char** table_list = lou_listTables();
            if (table_list) {
                // Count tables
                int table_count = 0;
                for (char** ptr = table_list; *ptr != NULL; ptr++) {
                    table_count++;
                }
                
                // 5.2. If we have tables, index them
                if (table_count > 0 && fdp.ConsumeBool()) {
                    const char** tables_to_index = new const char*[table_count + 1];
                    int idx = 0;
                    for (char** ptr = table_list; *ptr != NULL; ptr++) {
                        tables_to_index[idx++] = *ptr;
                    }
                    tables_to_index[table_count] = NULL;
                    
                    lou_indexTables(tables_to_index);
                    
                    delete[] tables_to_index;
                }
                
                // 5.3. Test multiple find operations
                int max_queries = std::min(3, (int)fdp.remaining_bytes());
                for (int i = 0; i < max_queries && fdp.remaining_bytes() > 0; i++) {
                    std::string custom_query = fdp.ConsumeRandomLengthString(32);
                    if (!custom_query.empty()) {
                        char** found_tables = lou_findTables(custom_query.c_str());
                        if (found_tables) {
                            lou_freeTableFiles(found_tables);
                        }
                    }
                }
                
                lou_freeTableFiles(table_list);
            }
            break;
        }
        
        case 3: {
            // Workflow: Direct operations with fuzzer input
            
            // 5.1. Test logging functions with fuzzer input
            if (fdp.remaining_bytes() > 10) {
                std::string log_msg = fdp.ConsumeRandomLengthString(128);
                if (!log_msg.empty()) {
                    lou_logPrint("Fuzzer: %s", log_msg.c_str());
                }
            }
            
            // 5.2. Test find with custom query from fuzzer input
            if (fdp.remaining_bytes() > 5) {
                std::string custom_query = fdp.ConsumeRandomLengthString(64);
                if (!custom_query.empty()) {
                    char* found = lou_findTable(custom_query.c_str());
                    if (found) {
                        lou_freeTableFile(found);
                    }
                }
            }
            
            // 5.3. Test getTableInfo with custom key from fuzzer input
            if (fdp.remaining_bytes() > 10 && num_sample_tables > 0) {
                uint8_t table_index = fdp.ConsumeIntegralInRange<uint8_t>(0, num_sample_tables - 1);
                std::string custom_key = fdp.ConsumeRandomLengthString(32);
                if (!custom_key.empty()) {
                    char* info = lou_getTableInfo(sample_tables[table_index], custom_key.c_str());
                    if (info) {
                        lou_freeTableInfo(info);
                    }
                }
            }
            break;
        }
    }
    
    // 6. Cleanup
    lou_free();
    
    // 7. Close logging if file was opened
    lou_logEnd();
    
    return 0;
}

} // namespace harness_002
