#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include "liblouis.h"

namespace harness_007 {
// liblouis metadata and table information fuzzing harness
// Targets: lou_charSize (uncovered getter), lou_version (uncovered getter), 
//          lou_freeEmphClasses (uncovered cleanup), lou_freeTableInfo (uncovered cleanup)
// Based on coverage analysis: lou_freeTableInfo and lou_freeEmphClasses have been reached 
// but show 0 hits, indicating they're likely being called with NULL/invalid parameters.
// Focus: Provide valid parameters to lou_freeTableInfo and lou_freeEmphClasses by proper setup
// Required helpers: lou_getTableInfo (setup for lou_freeTableInfo), 
//                   lou_getEmphClasses (setup for lou_freeEmphClasses),
//                   lou_findTable/lou_findTables (optional helper for valid table names),
//                   lou_free (general cleanup)
// Invocation sequence: 1. Call lou_findTable/lou_findTables to discover available translation tables.
//                      2. Call lou_getTableInfo on a discovered table to obtain table information structure.
//                      3. Call lou_getEmphClasses to obtain emphasis classes structure.
//                      4. Call lou_charSize and lou_version (simple getters that require no setup).
//                      5. Call lou_freeTableInfo to free the table information from step 2.
//                      6. Call lou_freeEmphClasses to free the emphasis classes from step 3.
//                      7. Call lou_free for any additional cleanup if needed.
// Semantic differentiation: This harness specifically targets metadata and cleanup functions
//                           that have been reached but with 0 hits due to invalid parameters.


// Include liblouis public header
extern "C" {
}

// Common table metadata keys based on liblouis documentation
static const char* table_info_keys[] = {
    "locale",
    "language",
    "contraction",
    "grade",
    "direction",
    "dots",
    "cells",
    "author",
    "copyright",
    "license",
    "description",
    "notes"
};
static const size_t num_keys = sizeof(table_info_keys) / sizeof(table_info_keys[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table selection and parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Call lou_findTable/lou_findTables to discover available translation tables
    
    // First, try to get available tables using lou_listTables (which internally indexes if needed)
    char** table_list = lou_listTables();
    if (table_list == NULL) {
        // No tables available, try lou_findTable with a fuzzed query
        size_t query_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string query = fdp.ConsumeRandomLengthString(query_len);
        char* table_found = lou_findTable(query.c_str());
        
        if (table_found != NULL) {
            // We found a table via findTable
            
            // Step 2: Call lou_getTableInfo on the discovered table
            // Select a random metadata key
            size_t key_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_keys - 1);
            const char* key = table_info_keys[key_idx];
            
            char* table_info = lou_getTableInfo(table_found, key);
            
            // Step 3: Call lou_getEmphClasses to obtain emphasis classes structure
            char const** emph_classes = lou_getEmphClasses(table_found);
            
            // Step 4: Call lou_charSize and lou_version (simple getters)
            int char_size = lou_charSize();
            const char* version = lou_version();
            
            // Step 5: Call lou_freeTableInfo to free the table information
            if (table_info != NULL) {
                lou_freeTableInfo(table_info);
            }
            
            // Step 6: Call lou_freeEmphClasses to free the emphasis classes
            if (emph_classes != NULL) {
                lou_freeEmphClasses(emph_classes);
            }
            
            // Free the table found by lou_findTable
            lou_freeTableFile(table_found);
        }
    } else {
        // We have a list of tables from lou_listTables
        // Count tables in the NULL-terminated array
        size_t table_count = 0;
        while (table_list[table_count] != NULL) {
            table_count++;
        }
        
        if (table_count > 0) {
            // Select a random table from the list
            size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, table_count - 1);
            const char* table = table_list[table_idx];
            
            // Step 2: Call lou_getTableInfo on the discovered table
            // Select a random metadata key
            size_t key_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_keys - 1);
            const char* key = table_info_keys[key_idx];
            
            char* table_info = lou_getTableInfo(table, key);
            
            // Step 3: Call lou_getEmphClasses to obtain emphasis classes structure
            char const** emph_classes = lou_getEmphClasses(table);
            
            // Step 4: Call lou_charSize and lou_version (simple getters)
            int char_size = lou_charSize();
            const char* version = lou_version();
            
            // Step 5: Call lou_freeTableInfo to free the table information
            if (table_info != NULL) {
                lou_freeTableInfo(table_info);
            }
            
            // Step 6: Call lou_freeEmphClasses to free the emphasis classes
            if (emph_classes != NULL) {
                lou_freeEmphClasses(emph_classes);
            }
        }
        
        // Free the table list
        lou_freeTableFiles(table_list);
    }
    
    // Step 7: Call lou_free for any additional cleanup if needed
    // Use fuzzer input to decide whether to call lou_free
    if (fdp.ConsumeBool()) {
        lou_free();
    }
    
    return 0;
}

} // namespace harness_007
