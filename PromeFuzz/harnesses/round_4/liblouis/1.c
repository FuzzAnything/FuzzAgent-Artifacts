// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static char* create_query_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    
    char* query = malloc(size + 1);
    if (!query) return NULL;
    
    memcpy(query, data, size);
    query[size] = '\0';
    return query;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Set log level to minimal to reduce output noise
    lou_setLogLevel(0);
    
    // Create a proper NULL-terminated array for lou_indexTables
    const char* tables[] = {".", NULL};
    lou_indexTables(tables);
    
    // Create query string from fuzz input
    char* query = create_query_string(Data, Size);
    if (!query) return 0;
    
    // First call to lou_findTable
    char* table1 = lou_findTable(query);
    if (table1) free(table1);
    
    // Second call to lou_findTable
    char* table2 = lou_findTable(query);
    if (table2) free(table2);
    
    // Call lou_findTables
    char** tables_found = lou_findTables(query);
    if (tables_found) {
        for (int i = 0; tables_found[i] != NULL; i++) {
            free(tables_found[i]);
        }
        free(tables_found);
    }
    
    // Cleanup
    free(query);
    lou_free();
    
    return 0;
}