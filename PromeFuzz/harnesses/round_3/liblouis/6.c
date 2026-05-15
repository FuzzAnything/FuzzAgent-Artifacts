// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void cleanup_string_array(char **arr) {
    if (arr) {
        lou_freeTableFiles(arr);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzz data
    char *data_copy = malloc(Size + 1);
    if (!data_copy) {
        return 0;
    }
    memcpy(data_copy, Data, Size);
    data_copy[Size] = '\0';

    // Split data into multiple strings for different parameters
    // Use first half for tableList/emphClass, second half for queries
    size_t split_point = Size / 2;
    char *first_part = data_copy;
    char *second_part = data_copy + split_point;
    
    // Ensure second part is null-terminated
    if (split_point < Size) {
        data_copy[split_point] = '\0';
    }

    // 1. Test lou_getTypeformForEmphClass
    formtype typeform = lou_getTypeformForEmphClass(first_part, second_part);
    (void)typeform; // Use result to avoid unused variable warning

    // 2. Test lou_indexTables with proper NULL-terminated array
    const char *tables[] = {first_part, second_part, NULL};
    lou_indexTables(tables);

    // Also test with empty array
    const char *empty_tables[] = {NULL};
    lou_indexTables(empty_tables);

    // 3. Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list) {
        cleanup_string_array(table_list);
    }

    // 4. Test lou_findTable
    char *found_table = lou_findTable(first_part);
    if (found_table) {
        free(found_table);
    }

    // 5. Test lou_findTables
    char **found_tables = lou_findTables(second_part);
    if (found_tables) {
        cleanup_string_array(found_tables);
    }

    // Cleanup
    free(data_copy);

    return 0;
}