// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
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

static void free_table_list(char **tables) {
    if (tables == NULL) {
        return;
    }
    for (int i = 0; tables[i] != NULL; i++) {
        free(tables[i]);
    }
    free(tables);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure input is null-terminated for string operations
    char *input = malloc(Size + 1);
    if (input == NULL) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // Test lou_version (no cleanup needed, returns static string)
    const char *version = lou_version();
    (void)version;

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Optionally use tables for other tests
        // For fuzzing, we just ensure we can free them
        free_table_list(tables);
    }

    // Test lou_indexTables with a NULL-terminated array
    // We create a simple array with the input string and NULL
    char *table_names[2];
    table_names[0] = input;
    table_names[1] = NULL;
    lou_indexTables((const char **)table_names);

    // Test lou_findTable with the input as a query
    char *found_table = lou_findTable(input);
    if (found_table != NULL) {
        free(found_table);
    }

    // Test lou_getTableInfo with the input as table name and key
    // We try a few common keys
    const char *keys[] = {"language", "description", "author", NULL};
    for (int i = 0; keys[i] != NULL; i++) {
        char *info = lou_getTableInfo(input, keys[i]);
        if (info != NULL) {
            lou_freeTableInfo(info);
        }
    }

    // Also test with found_table if it was not NULL (re-fetch)
    // But since we freed it, we can't use it directly. 
    // Instead, we can try to find it again or use a known table if available.
    // For simplicity, we skip this or re-find.
    char *found_table2 = lou_findTable(input);
    if (found_table2 != NULL) {
        for (int i = 0; keys[i] != NULL; i++) {
            char *info = lou_getTableInfo(found_table2, keys[i]);
            if (info != NULL) {
                lou_freeTableInfo(info);
            }
        }
        free(found_table2);
    }

    free(input);
    return 0;
}