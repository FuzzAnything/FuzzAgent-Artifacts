// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void free_string_array(char **arr) {
    if (arr == NULL) return;
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
    free(arr);
}

static void free_string(char *str) {
    if (str != NULL) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from input for use as query or table name
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Optionally use the list to test other functions
        for (int i = 0; tables[i] != NULL; i++) {
            // Test lou_getTable with each table name
            const void *table = lou_getTable(tables[i]);
            (void)table; // Suppress unused variable warning

            // Test lou_getTableInfo with each table name
            char *info = lou_getTableInfo(tables[i], "language");
            free_string(info);
        }
        free_string_array(tables);
    }

    // Test lou_indexTables with a custom list (using input as table name if valid)
    // We create a small static list for safety, but could use input if validated
    const char *custom_tables[] = { input_str, NULL };
    lou_indexTables(custom_tables);

    // Test lou_findTables with input as query
    char **found_tables = lou_findTables(input_str);
    if (found_tables != NULL) {
        for (int i = 0; found_tables[i] != NULL; i++) {
            // Test lou_getTable with found table
            const void *table = lou_getTable(found_tables[i]);
            (void)table;

            // Test lou_getTableInfo with found table
            char *info = lou_getTableInfo(found_tables[i], "language");
            free_string(info);
        }
        free_string_array(found_tables);
    }

    // Test lou_findTable with input as query
    char *best_table = lou_findTable(input_str);
    if (best_table != NULL) {
        // Test lou_getTable with the best table
        const void *table = lou_getTable(best_table);
        (void)table;

        // Test lou_getTableInfo with the best table
        char *info = lou_getTableInfo(best_table, "language");
        free_string(info);
        
        free_string(best_table);
    }

    // Test lou_getTable directly with input
    const void *direct_table = lou_getTable(input_str);
    (void)direct_table;

    // Test lou_getTableInfo directly with input
    char *direct_info = lou_getTableInfo(input_str, "language");
    free_string(direct_info);

    free(input_str);
    return 0;
}