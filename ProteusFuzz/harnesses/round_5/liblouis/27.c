// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
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

static void free_table_list(char **list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
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

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Iterate and potentially use table names
        for (int i = 0; tables[i] != NULL; i++) {
            char *info = lou_getTableInfo(tables[i], "language");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
            info = lou_getTableInfo(tables[i], "name");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(tables);
    }

    // Test lou_findTables with the fuzzer input as query
    char **foundTables = lou_findTables(query);
    if (foundTables != NULL) {
        for (int i = 0; foundTables[i] != NULL; i++) {
            char *info = lou_getTableInfo(foundTables[i], "language");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
            // Test with various keys
            info = lou_getTableInfo(foundTables[i], "grade");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free_table_list(foundTables);
    }

    // Test edge cases with empty or malformed queries
    char **emptyResult = lou_findTables("");
    if (emptyResult != NULL) {
        free_table_list(emptyResult);
    }

    // Test lou_getTableInfo with non-existent tables
    char *info = lou_getTableInfo("nonexistent_table", "language");
    if (info != NULL) {
        lou_freeTableInfo(info);
    }

    // Test with NULL key
    info = lou_getTableInfo("en:grade2", NULL);
    if (info != NULL) {
        lou_freeTableInfo(info);
    }

    // Cleanup
    free(query);

    return 0;
}