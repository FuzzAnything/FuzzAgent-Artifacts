// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
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
    if (list == NULL) {
        return;
    }
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *query = malloc(Size + 1);
    if (query == NULL) {
        return 0;
    }
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Iterate and optionally get info for each table
        for (int i = 0; tables[i] != NULL; i++) {
            char *info = lou_getTableInfo(tables[i], "language");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(tables);
    }

    // Test lou_findTables with the fuzzed query
    char **found = lou_findTables(query);
    if (found != NULL) {
        for (int i = 0; found[i] != NULL; i++) {
            char *info = lou_getTableInfo(found[i], "language");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(found);
    }

    // Test lou_getTableInfo with a potentially invalid table name from input
    char *info = lou_getTableInfo(query, "language");
    if (info != NULL) {
        lou_freeTableInfo(info);
    }

    // Test with different keys
    const char *keys[] = {"language", "grade", "script", NULL};
    for (int k = 0; keys[k] != NULL; k++) {
        info = lou_getTableInfo(query, keys[k]);
        if (info != NULL) {
            lou_freeTableInfo(info);
        }
    }

    free(query);
    return 0;
}