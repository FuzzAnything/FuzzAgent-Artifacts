// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_table_list(char **list) {
    if (list == NULL) return;
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_findTables
    char **foundTables = lou_findTables(query);
    if (foundTables != NULL) {
        free_table_list(foundTables);
    }

    // Test lou_findTable
    char *foundTable = lou_findTable(query);
    if (foundTable != NULL) {
        free(foundTable);
    }

    // Test lou_listTables
    char **allTables = lou_listTables();
    if (allTables != NULL) {
        // Optionally use the list for indexing or further testing
        lou_indexTables((const char **)allTables);
        free_table_list(allTables);
    }

    // Test lou_getTableInfo with a dummy table name if available
    // Since we don't have a guaranteed valid table name, we try with the query as table name
    // This may return NULL, which is acceptable
    char *tableInfo = lou_getTableInfo(query, "language");
    if (tableInfo != NULL) {
        lou_freeTableInfo(tableInfo);
    }

    free(query);
    return 0;
}