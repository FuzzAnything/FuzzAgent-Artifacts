// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // 1. lou_listTables and lou_freeTableFiles
    char **tables_list = lou_listTables();
    if (tables_list != NULL) {
        lou_freeTableFiles(tables_list);
    }

    // 2. lou_indexTables
    // Create a NULL-terminated array of strings using the fuzz input
    // We'll use up to 4 strings from the input data, each up to 256 bytes
    const char *tables[5] = {NULL, NULL, NULL, NULL, NULL}; // 4 strings + NULL terminator
    int table_count = 0;
    size_t offset = 0;
    
    // Parse up to 4 table names from the input data
    for (int i = 0; i < 4 && offset < Size; i++) {
        // Find next null byte or end of data
        size_t str_len = 0;
        while (offset + str_len < Size && Data[offset + str_len] != 0 && str_len < 256) {
            str_len++;
        }
        
        if (str_len > 0) {
            char *table_name = (char *)malloc(str_len + 1);
            if (table_name) {
                memcpy(table_name, Data + offset, str_len);
                table_name[str_len] = '\0';
                tables[table_count++] = table_name;
            }
            offset += str_len;
        }
        
        // Skip the null byte if present
        if (offset < Size && Data[offset] == 0) {
            offset++;
        }
    }
    
    // Ensure array is NULL-terminated
    tables[table_count] = NULL;
    
    // Call lou_indexTables with the parsed array
    lou_indexTables(tables);
    
    // Free allocated table names
    for (int i = 0; i < table_count; i++) {
        free((void *)tables[i]);
    }

    // 3. lou_findTable and lou_findTables
    // Create a query string from the remaining input data
    if (offset < Size) {
        size_t query_len = Size - offset;
        if (query_len > 256) query_len = 256;
        
        char *query = (char *)malloc(query_len + 1);
        if (query) {
            memcpy(query, Data + offset, query_len);
            query[query_len] = '\0';
            
            // Call lou_findTable
            char *found_table = lou_findTable(query);
            if (found_table) {
                free(found_table);
            }
            
            // Call lou_findTables
            char **found_tables = lou_findTables(query);
            if (found_tables) {
                lou_freeTableFiles(found_tables);
            }
            
            free(query);
        }
    } else {
        // If no data left for query, use a simple default query
        char *found_table = lou_findTable("en");
        if (found_table) {
            free(found_table);
        }
        
        char **found_tables = lou_findTables("en");
        if (found_tables) {
            lou_freeTableFiles(found_tables);
        }
    }

    // 4. lou_version
    const char *version = lou_version();
    // Just call it, no need to do anything with the result

    return 0;
}