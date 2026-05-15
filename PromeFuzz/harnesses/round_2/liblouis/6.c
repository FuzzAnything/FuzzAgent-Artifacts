// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void free_table_list(char **tables) {
    if (tables) {
        lou_freeTableFiles(tables);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Ensure we have some data to work with
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzz data
    char *query = malloc(Size + 1);
    if (!query) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    free_table_list(tables);

    // 2. Test lou_indexTables with valid NULL-terminated array
    const char *index_tables[] = {".", NULL};
    lou_indexTables(index_tables);

    // Also test with empty array
    const char *empty_tables[] = {NULL};
    lou_indexTables(empty_tables);

    // 3. Test lou_findTables with the fuzzed query
    char **found_tables = lou_findTables(query);
    free_table_list(found_tables);

    // 4. Test lou_findTable with the fuzzed query
    char *found_table = lou_findTable(query);
    if (found_table) {
        free(found_table);
    }

    // 5. Test lou_getTypeformForEmphClass
    // Use the first part of data for tableList and second part for emphClass
    size_t split = Size / 2;
    char *tableList = malloc(split + 1);
    char *emphClass = malloc(Size - split + 1);
    
    if (tableList && emphClass) {
        memcpy(tableList, Data, split);
        tableList[split] = '\0';
        
        memcpy(emphClass, Data + split, Size - split);
        emphClass[Size - split] = '\0';
        
        lou_getTypeformForEmphClass(tableList, emphClass);
    }
    
    free(tableList);
    free(emphClass);
    free(query);
    
    return 0;
}