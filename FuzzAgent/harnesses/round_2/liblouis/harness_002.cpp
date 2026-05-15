#include <config.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_002 {
// Targeted liblouis fuzzing harness for table metadata discovery operations
// Harness_002: Specifically targets table metadata discovery functions in metadata.c
// Focuses on table discovery lifecycle: setup → query → metadata → enumeration → cleanup
// Targets: lou_findTable (16 undiscovered branches), lou_findTables (16), 
//          lou_getTableInfo (12), lou_listTables (10), lou_indexTables (6)




// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Destructor to free liblouis resources
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis logging once
    static int initialized = 0;
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }
    
    // Operation type from fuzzer input
    // 0: Full table discovery lifecycle
    // 1: findTable variations
    // 2: findTables variations  
    // 3: getTableInfo variations
    // 4: listTables with different setups
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume a path for lou_setDataPath (optional)
    bool set_path = fdp.ConsumeBool();
    std::string data_path;
    if (set_path && fdp.remaining_bytes() > 10) {
        data_path = fdp.ConsumeRandomLengthString(256);
        char* path_result = lou_setDataPath(data_path.c_str());
        // Result can be ignored for fuzzing purposes
    }
    
    // Common metadata queries from liblouis documentation
    const char* query_types[] = {
        "language:",    // Language query
        "locale:",      // Locale query  
        "id:",          // Table ID query
        "contraction:", // Contraction grade
        "unicode:",     // Unicode range
        "typeform:",    // Typeform support
        "computer:",    // Computer braille
        "emph:",        // Emphasis support
        "dots:",        // Dots pattern
        "display:",     // Display name
        NULL
    };
    
    // Common metadata keys for lou_getTableInfo
    const char* metadata_keys[] = {
        "language",
        "locale", 
        "id",
        "contraction",
        "unicode",
        "typeform",
        "computer",
        "emph",
        "dots",
        "display",
        "description",
        "author",
        "copyright",
        "license",
        "version",
        NULL
    };
    
    // Use some actual table files from the liblouis tables directory
    // These provide real metadata for testing
    const char* available_tables[] = {
        "en-us-g2.ctb",      // US English Grade 2 (has metadata)
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "en-us-comp6.ctb",   // US computer braille 6-dot
        "en-us-comp8.ctb",   // US computer braille 8-dot  
        "de-g0.utb",         // German Grade 0
        "sv-g1.ctb",         // Swedish Grade 1
        "zhcn-g2.ctb",       // Chinese Grade 2
        "da-dk-g16.ctb",     // Danish Grade 1.6
        NULL
    };
    
    // Consume parameters for queries
    std::string query_base;
    int query_type_idx = fdp.ConsumeIntegralInRange<int>(0, 9);  // 0-9 for query_types
    std::string query_value;
    
    if (fdp.remaining_bytes() > 20) {
        query_value = fdp.ConsumeRandomLengthString(100);
        query_base = std::string(query_types[query_type_idx]) + query_value;
    }
    
    // Consume table index for operations that need a specific table
    int table_idx = fdp.ConsumeIntegralInRange<int>(0, 7);  // 0-7 for available_tables
    const char* table_name = available_tables[table_idx];
    
    // Consume metadata key index
    int key_idx = fdp.ConsumeIntegralInRange<int>(0, 14);  // 0-14 for metadata_keys
    const char* metadata_key = metadata_keys[key_idx];
    
    // Main operation switch
    switch (operation) {
        case 0: {
            // Full table discovery lifecycle
            // 1. First index some tables
            const char* tables_to_index[] = {
                available_tables[table_idx % 8],
                available_tables[(table_idx + 1) % 8],
                available_tables[(table_idx + 2) % 8],
                NULL
            };
            lou_indexTables(tables_to_index);
            
            // 2. Find a table with query
            char* found_table = NULL;
            if (!query_base.empty()) {
                found_table = lou_findTable(query_base.c_str());
            }
            
            // 3. Find multiple tables
            char** found_tables = NULL;
            if (!query_base.empty() && fdp.ConsumeBool()) {
                found_tables = lou_findTables(query_base.c_str());
            }
            
            // 4. Get table info for found table(s)
            if (found_table) {
                char* table_info = lou_getTableInfo(found_table, metadata_key);
                if (table_info) {
                    lou_freeTableInfo(table_info);
                }
            }
            
            if (found_tables) {
                for (int i = 0; found_tables[i] != NULL; i++) {
                    char* table_info = lou_getTableInfo(found_tables[i], metadata_key);
                    if (table_info) {
                        lou_freeTableInfo(table_info);
                    }
                }
            }
            
            // 5. List all tables
            char** all_tables = lou_listTables();
            
            // 6. Cleanup
            if (found_table) {
                lou_freeTableFile(found_table);  // Free string returned by lou_findTable
            }
            
            if (found_tables) {
                for (int i = 0; found_tables[i] != NULL; i++) {
                    free(found_tables[i]);
                }
                free(found_tables);
            }
            
            if (all_tables) {
                for (int i = 0; all_tables[i] != NULL; i++) {
                    free(all_tables[i]);
                }
                free(all_tables);
            }
            break;
        }
        case 1: {
            // findTable variations
            // First ensure tables are indexed
            const char* tables_to_index[] = {table_name, NULL};
            lou_indexTables(tables_to_index);
            
            // Try different query variations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string query = std::string(query_types[fdp.ConsumeIntegralInRange<int>(0, 9)]) + 
                                   fdp.ConsumeRandomLengthString(50);
                char* result = lou_findTable(query.c_str());
                if (result) {
                    lou_freeTableInfo(result);
                }
            }
            break;
        }
        
        case 2: {
            // findTables variations
            // First ensure tables are indexed
            const char* tables_to_index[] = {
                available_tables[0],
                available_tables[1],
                available_tables[2],
                NULL
            };
            lou_indexTables(tables_to_index);
            
            // Try multiple queries
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
                std::string query = std::string(query_types[fdp.ConsumeIntegralInRange<int>(0, 9)]) +
                                   fdp.ConsumeRandomLengthString(50);
                char** results = lou_findTables(query.c_str());
                if (results) {
                    for (int j = 0; results[j] != NULL; j++) {
                        free(results[j]);
                    }
                    free(results);
                }
            }
            break;
        }
        
        case 3: {
            // getTableInfo variations
            // First ensure tables are indexed
            const char* tables_to_index[] = {table_name, NULL};
            lou_indexTables(tables_to_index);
            
            // Try getting different metadata keys
            for (int i = 0; i < 4 && i < 15 && fdp.remaining_bytes() > 5; i++) {
                int key_idx = fdp.ConsumeIntegralInRange<int>(0, 14);
                char* info = lou_getTableInfo(table_name, metadata_keys[key_idx]);
                if (info) {
                    lou_freeTableInfo(info);
                }
            }
            break;
        }
        
        case 4: {
            // listTables with different setups
            // Index varying numbers of tables
            int num_tables = fdp.ConsumeIntegralInRange<int>(1, 4);
            const char* tables_to_index[5];  // Max 4 + NULL terminator
            
            for (int i = 0; i < num_tables; i++) {
                tables_to_index[i] = available_tables[(table_idx + i) % 8];
            }
            tables_to_index[num_tables] = NULL;
            
            lou_indexTables(tables_to_index);
            
            // List tables multiple times
            for (int i = 0; i < 2; i++) {
                char** tables = lou_listTables();
                if (tables) {
                    for (int j = 0; tables[j] != NULL; j++) {
                        free(tables[j]);
                    }
                    free(tables);
                }
            }
            break;
        }
    }
    
    // Note: lou_free() is called automatically by destructor
    return 0;
}

} // namespace harness_002
