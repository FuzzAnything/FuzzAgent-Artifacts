// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
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

static void free_table_array(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

static void free_table_info(char *info) {
    if (info != NULL) {
        lou_freeTableInfo(info);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for string operations
    char *input = (char *)malloc(Size + 1);
    if (input == NULL) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // Fuzz lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Optionally iterate and use table names
        for (int i = 0; tables[i] != NULL; i++) {
            // Use tables[i] if needed
            (void)tables[i];
        }
        free_table_array(tables);
    }

    // Fuzz lou_findTables with the input as a query
    char **found_tables = lou_findTables(input);
    if (found_tables != NULL) {
        for (int i = 0; found_tables[i] != NULL; i++) {
            // Use found_tables[i] if needed
            (void)found_tables[i];
        }
        free_table_array(found_tables);
    }

    // Fuzz lou_getTableInfo with input as table name and key
    char *info = lou_getTableInfo(input, "language");
    free_table_info(info);

    // Fuzz lou_getTableInfo with a fixed table name and input as key
    info = lou_getTableInfo("en-gb", input);
    free_table_info(info);

    // Fuzz lou_getTableInfo with input as both table and key
    info = lou_getTableInfo(input, input);
    free_table_info(info);

    free(input);
    return 0;
}