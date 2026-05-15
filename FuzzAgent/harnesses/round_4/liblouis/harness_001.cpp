#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_001 {
// liblouis table metadata fuzzing harness
// Targets metadata and table lookup APIs: lou_setDataPath, lou_getDataPath, 
// lou_findTable, lou_findTables, lou_listTables, lou_getTableInfo, 
// lou_freeTableInfo, lou_freeTableFiles
// Based on coverage analysis guidance to target completely uncovered metadata.c module


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for path, query strings, and key
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // 1. Call lou_setDataPath to configure table search path
    // Consume path string from fuzzer input
    size_t max_path_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string data_path = fdp.ConsumeRandomLengthString(max_path_len);
    
    char* set_path_result = lou_setDataPath(data_path.c_str());
    // Note: lou_setDataPath returns NULL if path is NULL or too long
    // We don't need to check the result for fuzzing purposes
    
    // 2. Call lou_getDataPath to verify configuration
    char* retrieved_path = lou_getDataPath();
    // Retrieved path may be NULL or valid string
    
    // 3. Call lou_findTable to locate a specific translation table
    // Consume table query from fuzzer input
    size_t max_query_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string table_query = fdp.ConsumeRandomLengthString(max_query_len);
    
    char* found_table = lou_findTable(table_query.c_str());
    
    // 4. Call lou_findTables to search for multiple tables with criteria
    // Use same or different query
    size_t max_tables_query_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string tables_query = fdp.ConsumeRandomLengthString(max_tables_query_len);
    
    char** found_tables = lou_findTables(tables_query.c_str());
    
    // 5. Call lou_listTables to enumerate available tables
    char** listed_tables = lou_listTables();
    
    // 6. Call lou_getTableInfo to retrieve detailed metadata for found tables
    // First check if we found any tables
    char* table_for_info = NULL;
    char* table_key = NULL;
    
    if (found_table != NULL) {
        // Use the found table for metadata query
        table_for_info = found_table;
    } else if (found_tables != NULL && found_tables[0] != NULL) {
        // Use first table from found_tables array
        table_for_info = found_tables[0];
    } else if (listed_tables != NULL && listed_tables[0] != NULL) {
        // Use first table from listed tables
        table_for_info = listed_tables[0];
    }
    // Consume metadata key from fuzzer input if we have a table
    if (table_for_info != NULL) {
        size_t max_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string key_str = fdp.ConsumeRandomLengthString(max_key_len);
        table_key = const_cast<char*>(key_str.c_str());
        
        char* table_info = lou_getTableInfo(table_for_info, table_key);
        
        // 7. Call lou_freeTableInfo to clean up table info structures
        if (table_info != NULL) {
            lou_freeTableInfo(table_info);
        }
    }
    
    // 8. Call lou_freeTableFiles and lou_freeTableFile to clean up table file structures
    // Free the table files arrays if they were allocated
    
    if (found_table != NULL) {
        // lou_findTable returns a single string that should be freed with lou_freeTableFile
        lou_freeTableFile(found_table);
    }
    
    if (found_tables != NULL) {
        lou_freeTableFiles(found_tables);
    }
    
    if (listed_tables != NULL) {
        lou_freeTableFiles(listed_tables);
    }
    
    // Clean up liblouis memory
    lou_free();
    
    return 0;
}

} // namespace harness_001
