// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void free_string_array(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Prepare dummy table file
    FILE *fp = fopen("./dummy_file", "w");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 1. Test lou_indexTables with proper NULL-terminated array
    const char *tables[] = {"./dummy_file", NULL};
    lou_indexTables(tables);

    // Also test with empty array
    const char *empty_tables[] = {NULL};
    lou_indexTables(empty_tables);

    // 2. Test lou_getTableInfo
    char *table_info = lou_getTableInfo("./dummy_file", input_str);
    if (table_info != NULL) {
        free(table_info);
    }

    // 3. Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list != NULL) {
        free_string_array(table_list);
    }

    // 4. Test lou_findTable
    char *found_table = lou_findTable(input_str);
    if (found_table != NULL) {
        free(found_table);
    }

    // 5. Test lou_getTypeformForEmphClass
    formtype typeform = lou_getTypeformForEmphClass("./dummy_file", input_str);
    (void)typeform; // Use result to avoid unused variable warning

    // 6. Test lou_findTables
    char **found_tables = lou_findTables(input_str);
    if (found_tables != NULL) {
        free_string_array(found_tables);
    }

    // Cleanup
    free(input_str);
    remove("./dummy_file");

    return 0;
}