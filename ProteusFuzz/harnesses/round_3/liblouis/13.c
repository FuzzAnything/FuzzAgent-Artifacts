// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <liblouis.h>

static void
free_string_array(char **arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
    free(arr);
}

static void
free_string(char *str) {
    if (str != NULL) {
        free(str);
    }
}

int
LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) {
        return 0;
    }
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Try to index all available tables
        lou_indexTables((const char **)tables);
        
        // Try to get info for the first table if it exists
        if (tables[0] != NULL) {
            char *info = lou_getTableInfo(tables[0], "language");
            free_string(info);
            
            // Try with a potentially invalid key
            info = lou_getTableInfo(tables[0], "invalid_key_xyz");
            free_string(info);
        }
        
        // Clean up the array returned by lou_listTables
        free_string_array(tables);
    }

    // 2. Test lou_findTables
    char **matches = lou_findTables(query);
    if (matches != NULL) {
        // Optionally iterate and get info for some matches
        for (int i = 0; matches[i] != NULL; i++) {
            char *info = lou_getTableInfo(matches[i], "language");
            free_string(info);
        }
        free_string_array(matches);
    }

    // 3. Test lou_findTable
    char *bestTable = lou_findTable(query);
    if (bestTable != NULL) {
        char *info = lou_getTableInfo(bestTable, "language");
        free_string(info);
        free_string(bestTable);
    }

    // 4. Test lou_indexTables with a custom array (potentially containing the query or parts of it)
    // We create a small array to pass to lou_indexTables
    char *customTables[2];
    customTables[0] = query; // Use the fuzz input as a potential table name
    customTables[1] = NULL;
    lou_indexTables((const char **)customTables);

    // 5. Test lou_freeTableFiles with NULL (safe no-op)
    lou_freeTableFiles(NULL);
    
    // Note: lou_freeTableFiles is meant for arrays returned by lou_loadTableFiles.
    // Since lou_loadTableFiles is not in the target list, we do not generate such an array here.
    // Passing NULL is safe as per the documentation.

    free(query);
    return 0;
}