#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <string>

namespace harness_002 {
// liblouis fuzzing harness - Metadata discovery operations focus
// Targets: lou_getTableInfo, lou_listTables, lou_findTable, lou_findTables, lou_indexTables, 
//          lou_freeTableInfo, lou_freeTableFile, lou_version
// Uses FuzzedDataProvider for structured input processing
// Focus on metadata.c module with semantic diversity from existing harnesses


// Global initialization flag
static int initialized = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Helper function to create NULL-terminated array of table names from fuzzer input
static const char** create_table_list(FuzzedDataProvider& fdp, size_t max_tables = 10) {
    // First consume number of tables (1 to max_tables)
    size_t num_tables = fdp.ConsumeIntegralInRange<size_t>(1, max_tables);
    
    // Allocate array (num_tables + 1 for NULL terminator)
    const char** tables = (const char**)malloc((num_tables + 1) * sizeof(char*));
    if (!tables) return NULL;
    
    // Create table names from fuzzer input
    for (size_t i = 0; i < num_tables; i++) {
        // Consume table name with reasonable length limit
        std::string table_name = fdp.ConsumeRandomLengthString(128);
        // Even if empty, use it as-is - fuzzer will learn to provide non-empty input
        tables[i] = strdup(table_name.c_str());
    }
    tables[num_tables] = NULL; // NULL terminate
    
    return tables;
}

// Helper to free table list
static void free_table_list(const char** tables) {
    if (!tables) return;
    for (size_t i = 0; tables[i] != NULL; i++) {
        free((void*)tables[i]);
    }
    free(tables);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for metadata operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // For operations that need table/indexing, create table list
    const char** tables = NULL;
    if (operation == 1 || operation == 2) {
        tables = create_table_list(fdp, 5);
        if (!tables) return 0;
    }
    
    // Consume query string for find operations
    std::string query = fdp.ConsumeRandomLengthString(256);
    
    // Consume table name for getTableInfo
    std::string table_name = fdp.ConsumeRandomLengthString(128);
    
    // Consume key for getTableInfo
    std::string key = fdp.ConsumeRandomLengthString(64);
    
    switch (operation) {
        case 0: {
            // Test lou_version - simple API, no parameters
            const char* version = lou_version();
            (void)version; // Result ignored for fuzzing
            break;
        }
        
        case 1: {
            // Test lou_indexTables
            lou_indexTables(tables);
            break;
        }
        
        case 2: {
            // Test lou_indexTables followed by lou_listTables
            lou_indexTables(tables);
            
            char** table_list = lou_listTables();
            if (table_list) {
                // Free the returned table list using the proper API
                lou_freeTableFiles(table_list);
            }
            break;
        }
        
        case 3: {
            // Test lou_findTable
            char* found_table = lou_findTable(query.c_str());
            if (found_table) {
                lou_freeTableFile(found_table);
            }
            break;
        }
        
        case 4: {
            // Test lou_findTables
            char** found_tables = lou_findTables(query.c_str());
            if (found_tables) {
                // Free the returned tables using the proper API
                lou_freeTableFiles(found_tables);
            }
            break;
        }
        
        case 5: {
            // Test lou_getTableInfo
            char* table_info = lou_getTableInfo(table_name.c_str(), key.c_str());
            if (table_info) {
                lou_freeTableInfo(table_info);
            }
            break;
        }
        
        case 6: {
            // Test combination: index, find, and get info
            // Create a small table list
            const char** combo_tables = create_table_list(fdp, 3);
            if (combo_tables) {
                lou_indexTables(combo_tables);
                
                // Try to find a table
                char* found = lou_findTable(query.c_str());
                if (found) {
                    // Try to get info about found table
                    char* info = lou_getTableInfo(found, key.c_str());
                    if (info) {
                        lou_freeTableInfo(info);
                    }
                    lou_freeTableFile(found);
                }
                
                free_table_list(combo_tables);
            }
            break;
        }
        
        case 7: {
            // Test edge cases: empty query, invalid table names
            // Test with empty query
            char* empty_result = lou_findTable("");
            if (empty_result) {
                lou_freeTableFile(empty_result);
            }
            
            // Test with very long table name
            std::string long_table = fdp.ConsumeRandomLengthString(512);
            char* long_info = lou_getTableInfo(long_table.c_str(), key.c_str());
            if (long_info) {
                lou_freeTableInfo(long_info);
            }
            
            // Test with various keys
            const char* test_keys[] = {"language", "locale", "contraction", "grade", "direction"};
            for (size_t i = 0; i < sizeof(test_keys)/sizeof(test_keys[0]); i++) {
                char* test_info = lou_getTableInfo(table_name.c_str(), test_keys[i]);
                if (test_info) {
                    lou_freeTableInfo(test_info);
                }
            }
            break;
        }
    }
    
    // Clean up
    if (tables) {
        free_table_list(tables);
    }
    
    return 0;
}

} // namespace harness_002
