#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <liblouis.h>

namespace harness_001 {
/*
 * Fuzzing harness for liblouis table metadata and discovery operations
 * Targets: lou_findTables, lou_findTable, lou_getTableInfo, lou_listTables, lou_indexTables
 * Required helpers: lou_setDataPath, lou_freeTableFile, lou_freeTableInfo, lou_freeTableFiles
 */



// Log callback to avoid excessive logging during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper function to create NULL-terminated array of strings for lou_indexTables
static const char** create_table_array(std::vector<std::string>& tables) {
    if (tables.empty()) {
        return nullptr;
    }
    
    const char** array = (const char**)malloc((tables.size() + 1) * sizeof(const char*));
    if (!array) {
        return nullptr;
    }
    
    for (size_t i = 0; i < tables.size(); i++) {
        array[i] = tables[i].c_str();
    }
    array[tables.size()] = nullptr;
    
    return array;
}

// Helper function to free a NULL-terminated array of strings (but not the strings themselves)
static void free_table_array(const char** array) {
    if (array) {
        free(array);
    }
}

// Helper to check if string is valid for liblouis (not empty and reasonable length)
static bool is_valid_string(const std::string& str) {
    return !str.empty() && str.length() < 256;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - we need enough for basic operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }

    // Consume operation type to decide which API flow to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume a data path (optional, can be empty)
    std::string data_path = fdp.ConsumeRandomLengthString(128);
    
    // Set data path if we have a non-empty path
    if (!data_path.empty()) {
        char* result = lou_setDataPath(data_path.c_str());
        // Result can be NULL if path is too long, but that's OK for fuzzing
    }
    
    // Consume query string for find operations
    std::string query = fdp.ConsumeRandomLengthString(128);
    
    // Consume table name for getTableInfo
    std::string table_name = fdp.ConsumeRandomLengthString(128);
    
    // Consume key for getTableInfo
    std::string key = fdp.ConsumeRandomLengthString(64);
    
    // Variables to hold API results
    char* found_table = nullptr;
    char** found_tables = nullptr;
    char* table_info = nullptr;
    char** listed_tables = nullptr;
    
    // Execute the selected metadata operation
    switch (operation) {
        case 0: // lou_findTable - find single best match
            if (is_valid_string(query)) {
                found_table = lou_findTable(query.c_str());
                if (found_table) {
                    // Got a table, try to get its info with a common key
                    if (is_valid_string(key)) {
                        table_info = lou_getTableInfo(found_table, key.c_str());
                        if (table_info) {
                            lou_freeTableInfo(table_info);
                            table_info = nullptr;
                        }
                    }
                    lou_freeTableFile(found_table);
                    found_table = nullptr;
                }
            }
            break;
            
        case 1: // lou_findTables - find all matches
            if (is_valid_string(query)) {
                found_tables = lou_findTables(query.c_str());
                if (found_tables) {
                    // Process found tables
                    for (int i = 0; found_tables[i] != nullptr; i++) {
                        // Try to get info for each found table with various keys
                        const char* common_keys[] = {"locale", "language", "contraction", "type"};
                        for (size_t k = 0; k < sizeof(common_keys)/sizeof(common_keys[0]); k++) {
                            if (fdp.ConsumeBool()) {  // Randomly decide to get info
                                table_info = lou_getTableInfo(found_tables[i], common_keys[k]);
                                if (table_info) {
                                    lou_freeTableInfo(table_info);
                                    table_info = nullptr;
                                }
                            }
                        }
                    }
                    lou_freeTableFiles(found_tables);
                    found_tables = nullptr;
                }
            }
            break;
            
        case 2: // lou_getTableInfo - get metadata for specific table
            if (is_valid_string(table_name) && is_valid_string(key)) {
                table_info = lou_getTableInfo(table_name.c_str(), key.c_str());
                if (table_info) {
                    lou_freeTableInfo(table_info);
                    table_info = nullptr;
                }
            }
            break;
            
        case 3: // lou_listTables - list all available tables
            listed_tables = lou_listTables();
            if (listed_tables) {
                // Process listed tables
                for (int i = 0; listed_tables[i] != nullptr; i++) {
                    // Try to get info for some tables
                    if (fdp.ConsumeBool() && is_valid_string(key)) {
                        table_info = lou_getTableInfo(listed_tables[i], key.c_str());
                        if (table_info) {
                            lou_freeTableInfo(table_info);
                            table_info = nullptr;
                        }
                    }
                }
                lou_freeTableFiles(listed_tables);
                listed_tables = nullptr;
            }
            break;
            
        case 4: // lou_indexTables - index specific tables
            {
                // Create a small array of table names to index
                std::vector<std::string> tables_to_index;
                size_t table_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                
                for (size_t i = 0; i < table_count; i++) {
                    std::string table = fdp.ConsumeRandomLengthString(64);
                    if (is_valid_string(table)) {
                        tables_to_index.push_back(table);
                    }
                }
                
                if (!tables_to_index.empty()) {
                    const char** table_array = create_table_array(tables_to_index);
                    if (table_array) {
                        lou_indexTables(table_array);
                        free_table_array(table_array);
                    }
                }
            }
            break;
            
        case 5: // Comprehensive workflow - test multiple APIs in sequence
            // First, list tables
            listed_tables = lou_listTables();
            if (listed_tables) {
                // If we got tables, try to get info for first few
                int max_tables = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; listed_tables[i] != nullptr && i < max_tables; i++) {
                    const char* common_keys[] = {"locale", "language", "type"};
                    for (size_t k = 0; k < sizeof(common_keys)/sizeof(common_keys[0]); k++) {
                        if (fdp.ConsumeBool()) {
                            table_info = lou_getTableInfo(listed_tables[i], common_keys[k]);
                            if (table_info) {
                                lou_freeTableInfo(table_info);
                                table_info = nullptr;
                            }
                        }
                    }
                }
                lou_freeTableFiles(listed_tables);
                listed_tables = nullptr;
            }
            
            // Then try to find tables with a query
            if (is_valid_string(query)) {
                found_tables = lou_findTables(query.c_str());
                if (found_tables) {
                    lou_freeTableFiles(found_tables);
                    found_tables = nullptr;
                }
            }
            break;
    }
    
    // Clean up any remaining resources (should be null at this point)
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    if (listed_tables) {
        lou_freeTableFiles(listed_tables);
    }
    
    return 0;
}

} // namespace harness_001
