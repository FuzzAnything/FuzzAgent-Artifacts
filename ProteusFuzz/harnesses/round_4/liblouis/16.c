// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
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

static void free_string_array(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string operations
    char *input = malloc(Size + 1);
    if (input == NULL) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Iterate and print (optional, for debugging)
        for (int i = 0; tables[i] != NULL; i++) {
            // Use the table name for further testing if needed
            const void *header = lou_getTable(tables[i]);
            if (header != NULL) {
                // Table loaded successfully
            }
            char *info = lou_getTableInfo(tables[i], "language");
            if (info != NULL) {
                free(info);
            }
        }
        free_string_array(tables);
    }

    // 2. Test lou_indexTables with a custom list derived from input
    // Create a simple list of table names based on input chunks
    char **custom_tables = NULL;
    int table_count = 0;
    // Heuristic: split input by null bytes or just use the whole input as a fake name
    // Since we don't have real table names, we try to index with the input string
    // Note: This might fail to load, which is expected behavior for fuzzing
    const char *dummy_tables[2];
    dummy_tables[0] = input;
    dummy_tables[1] = NULL;
    lou_indexTables(dummy_tables);

    // 3. Test lou_findTables with the input as a query
    char **found_tables = lou_findTables(input);
    if (found_tables != NULL) {
        // Iterate and clean up
        for (int i = 0; found_tables[i] != NULL; i++) {
            // Optional: verify the found table
            const void *header = lou_getTable(found_tables[i]);
            if (header != NULL) {
                // Valid table
            }
        }
        free_string_array(found_tables);
    }

    // 4. Test lou_findTable with the input as a query
    char *best_table = lou_findTable(input);
    if (best_table != NULL) {
        // Use the best table
        const void *header = lou_getTable(best_table);
        if (header != NULL) {
            // Valid table
        }
        free(best_table);
    }

    // 5. Test lou_getTable directly with input as table name
    const void *header = lou_getTable(input);
    if (header != NULL) {
        // Table loaded
    }

    // 6. Test lou_getTableInfo with input as table and key
    char *info = lou_getTableInfo(input, input);
    if (info != NULL) {
        free(info);
    }

    // Cleanup
    free(input);
    return 0;
}