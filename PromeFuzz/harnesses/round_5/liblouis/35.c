// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static char **dummy_resolver(const char *table, const char *base) {
    (void)table;
    (void)base;
    return NULL;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for string operations
    if (Size < 1) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Create a second string by splitting the input in half
    size_t half_size = Size / 2;
    char *second_str = (char *)malloc(half_size + 1);
    if (second_str) {
        memcpy(second_str, Data, half_size);
        second_str[half_size] = '\0';
    }

    // 1. Test lou_getTableInfo
    char *table_info = lou_getTableInfo(input_str, second_str ? second_str : "language");
    if (table_info) {
        free(table_info);
    }

    // 2. Test lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        for (int i = 0; tables[i] != NULL; i++) {
            free(tables[i]);
        }
        free(tables);
    }

    // 3. Test lou_getEmphClasses
    const char **emph_classes = lou_getEmphClasses(input_str);
    if (emph_classes) {
        free(emph_classes);
    }

    // 4. Test lou_registerTableResolver
    lou_registerTableResolver(dummy_resolver);

    // 5. Test lou_findTable - first ensure tables are indexed
    // Create a NULL-terminated array for lou_indexTables
    const char *table_array[] = {NULL};
    lou_indexTables(table_array);
    
    char *found_table = lou_findTable(input_str);
    if (found_table) {
        free(found_table);
    }

    // 6. Test lou_getTypeformForEmphClass
    formtype typeform = lou_getTypeformForEmphClass(input_str, 
                                                   second_str ? second_str : "emphasis");
    (void)typeform; // Use result to avoid unused variable warning

    // Cleanup
    free(input_str);
    if (second_str) {
        free(second_str);
    }

    return 0;
}