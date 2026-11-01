// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void free_string_array(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) {
        return 0;
    }
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        free_string_array(tables);
    }

    // Test lou_findTable
    char *foundTable = lou_findTable(query);
    if (foundTable != NULL) {
        free(foundTable);
    }

    // Test lou_findTables
    char **foundTables = lou_findTables(query);
    if (foundTables != NULL) {
        free_string_array(foundTables);
    }

    // Test lou_indexTables with a single table name derived from input
    const char *indexTable = query;
    const char *indexArray[] = {indexTable, NULL};
    lou_indexTables(indexArray);

    // Test lou_getTable
    const void *tableHeader = lou_getTable(query);
    // No direct free for tableHeader as per API, it's managed internally

    // Test lou_freeTableFiles
    // We must not pass a manually allocated array to lou_freeTableFiles 
    // because it expects an array returned by liblouis functions like lou_loadTableFiles.
    // Passing a manually allocated array leads to double-free or use-after-free.
    // Instead, we can safely call it with NULL to test the null-check path.
    lou_freeTableFiles(NULL);

    free(query);
    return 0;
}