// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void cleanup_string_array(char **array) {
    if (array == NULL) return;
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create null-terminated strings from fuzzer input
    size_t half = Size / 2;
    char *str1 = malloc(half + 1);
    char *str2 = malloc(Size - half + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    memcpy(str1, Data, half);
    str1[half] = '\0';
    memcpy(str2, Data + half, Size - half);
    str2[Size - half] = '\0';

    // Test lou_getTypeformForEmphClass
    formtype typeform = lou_getTypeformForEmphClass(str1, str2);
    (void)typeform;

    // Test lou_getTableInfo
    char *table_info = lou_getTableInfo(str1, str2);
    if (table_info) free(table_info);

    // Test lou_indexTables with proper NULL-terminated array
    const char *tables[] = {"./dummy_file", NULL};
    lou_indexTables(tables);

    // Write dummy table file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Re-index with dummy file
    lou_indexTables(tables);

    // Test lou_findTable
    char *found_table = lou_findTable(str1);
    if (found_table) free(found_table);

    // Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list) cleanup_string_array(table_list);

    // Test lou_findTables
    char **found_tables = lou_findTables(str1);
    if (found_tables) cleanup_string_array(found_tables);

    // Clean up dummy file
    remove("./dummy_file");

    free(str1);
    free(str2);
    return 0;
}