// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
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
#include <stdio.h>

static void free_table_list(char **list) {
    if (list == NULL) return;
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input data is null-terminated for string operations
    char *query = NULL;
    if (Size < 2) {
        query = malloc(2);
        if (query == NULL) return 0;
        query[0] = Data[0];
        query[1] = '\0';
    } else {
        query = malloc(Size + 1);
        if (query == NULL) return 0;
        memcpy(query, Data, Size);
        query[Size] = '\0';
    }

    // Fuzz lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Iterate and optionally fuzz lou_getTableInfo with found table names
        for (int i = 0; tables[i] != NULL; i++) {
            char *info = lou_getTableInfo(tables[i], query);
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(tables);
    }

    // Fuzz lou_findTables with the generated query
    char **foundTables = lou_findTables(query);
    if (foundTables != NULL) {
        for (int i = 0; foundTables[i] != NULL; i++) {
            char *info = lou_getTableInfo(foundTables[i], query);
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(foundTables);
    }

    // Cleanup
    free(query);

    return 0;
}