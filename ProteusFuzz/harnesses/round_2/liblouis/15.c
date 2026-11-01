// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
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

static void safe_free_string(char *ptr) {
    if (ptr != NULL) {
        lou_freeTableInfo(ptr);
    }
}

static void safe_free_table_list(char **list) {
    if (list == NULL) return;
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test lou_findTable
    char *found_table = lou_findTable(input_str);
    if (found_table != NULL) {
        // Use the found table name to query info
        char *info = lou_getTableInfo(found_table, "language");
        safe_free_string(info);
        info = lou_getTableInfo(found_table, "description");
        safe_free_string(info);
        // Free the table name returned by lou_findTable
        free(found_table);
    }

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        for (int i = 0; tables[i] != NULL; i++) {
            // Query info for each listed table
            char *info = lou_getTableInfo(tables[i], "language");
            safe_free_string(info);
            // Try to find table using its name as query
            char *found = lou_findTable(tables[i]);
            if (found != NULL) {
                free(found);
            }
        }
        safe_free_table_list(tables);
    }

    // Test with various keys from input
    char *info = lou_getTableInfo(input_str, "language");
    safe_free_string(info);
    info = lou_getTableInfo(input_str, "description");
    safe_free_string(info);
    info = lou_getTableInfo(input_str, "author");
    safe_free_string(info);
    info = lou_getTableInfo(input_str, "version");
    safe_free_string(info);

    // Test edge cases: empty strings, special characters
    char empty[] = "";
    info = lou_getTableInfo(empty, "language");
    safe_free_string(info);

    // Test lou_freeTableInfo with NULL (should be safe)
    lou_freeTableInfo(NULL);

    free(input_str);
    return 0;
}