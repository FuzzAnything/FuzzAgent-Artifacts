// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
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

static void free_string_array(char **array) {
    if (array == NULL) return;
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 2) return 0;

    // Create two null-terminated strings from the input data
    // First string: use first half of data
    size_t str1_len = Size / 2;
    char *str1 = malloc(str1_len + 1);
    if (str1 == NULL) return 0;
    memcpy(str1, Data, str1_len);
    str1[str1_len] = '\0';

    // Second string: use second half of data
    size_t str2_len = Size - str1_len;
    char *str2 = malloc(str2_len + 1);
    if (str2 == NULL) {
        free(str1);
        return 0;
    }
    memcpy(str2, Data + str1_len, str2_len);
    str2[str2_len] = '\0';

    // Create dummy file for table operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 1. Test lou_getTypeformForEmphClass
    formtype typeform = lou_getTypeformForEmphClass(str1, str2);

    // 2. Test lou_getTableInfo
    char *table_info = lou_getTableInfo(str1, str2);
    if (table_info != NULL) {
        free(table_info);
    }

    // 3. Test lou_indexTables with proper NULL-terminated array
    const char *tables[] = {"./dummy_file", NULL};
    lou_indexTables(tables);

    // 4. Test lou_findTable
    char *found_table = lou_findTable(str1);
    if (found_table != NULL) {
        free(found_table);
    }

    // 5. Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list != NULL) {
        free_string_array(table_list);
    }

    // 6. Test lou_findTables
    char **found_tables = lou_findTables(str1);
    if (found_tables != NULL) {
        free_string_array(found_tables);
    }

    // Clean up
    free(str1);
    free(str2);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}