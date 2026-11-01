// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void free_string_array(char **array) {
    if (array == NULL) return;
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for string operations
    char *input = (char *)malloc(Size + 1);
    if (input == NULL) return 0;
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Use the list to index tables
        lou_indexTables((const char **)tables);
        // Free the list
        free_string_array(tables);
    }

    // 2. Test lou_findTables with the input as a query
    char **foundTables = lou_findTables(input);
    if (foundTables != NULL) {
        // Optionally index these found tables
        lou_indexTables((const char **)foundTables);
        free_string_array(foundTables);
    }

    // 3. Test lou_findTable with the input as a query
    char *bestTable = lou_findTable(input);
    if (bestTable != NULL) {
        // Test lou_getTable with the found table name
        const void *tableHeader = lou_getTable(bestTable);
        (void)tableHeader; // Suppress unused variable warning if needed

        // Test lou_getTableInfo with the found table name
        char *info = lou_getTableInfo(bestTable, "language");
        if (info != NULL) {
            free(info);
        }

        free(bestTable);
    }

    // 4. Directly test lou_getTable with input as table name (if it looks like a name)
    const void *directTable = lou_getTable(input);
    (void)directTable;

    // 5. Test lou_getTableInfo with input as table name and a fixed key
    char *directInfo = lou_getTableInfo(input, "description");
    if (directInfo != NULL) {
        free(directInfo);
    }

    free(input);
    return 0;
}