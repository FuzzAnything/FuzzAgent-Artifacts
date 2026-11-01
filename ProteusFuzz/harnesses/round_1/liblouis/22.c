// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <liblouis.h>

static void safe_free_table_info(char *ptr) {
    if (ptr != NULL) {
        lou_freeTableInfo(ptr);
    }
}

static void safe_free_table_array(char **ptr) {
    if (ptr == NULL) {
        return;
    }
    for (int i = 0; ptr[i] != NULL; i++) {
        free(ptr[i]);
    }
    free(ptr);
}

static void safe_free_table_files(char **ptr) {
    if (ptr != NULL) {
        lou_freeTableFiles(ptr);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Fuzz lou_getTableInfo
    // Use the input as table name and key
    char *info = lou_getTableInfo(input_str, input_str);
    safe_free_table_info(info);

    // Fuzz lou_findTables
    char **tables = lou_findTables(input_str);
    safe_free_table_array(tables);

    // Fuzz lou_listTables
    char **all_tables = lou_listTables();
    safe_free_table_array(all_tables);

    // Fuzz lou_freeTableFile with a dummy string (simulating a loaded table file)
    // Note: lou_freeTableFile expects a pointer returned by lou_loadTableFile.
    // Since we don't have a valid table file path from the fuzzer input that is guaranteed to load,
    // we skip direct fuzzing of lou_freeTableFile with arbitrary input to avoid undefined behavior.
    // However, we can test the NULL safety.
    lou_freeTableFile(NULL);

    // Fuzz lou_freeTableFiles with NULL
    lou_freeTableFiles(NULL);

    // Additional exploration: Try different splits of input for table and key
    if (Size > 1) {
        char *mid = input_str + (Size / 2);
        *mid = '\0';
        char *table_part = input_str;
        char *key_part = mid + 1;

        info = lou_getTableInfo(table_part, key_part);
        safe_free_table_info(info);

        tables = lou_findTables(table_part);
        safe_free_table_array(tables);
    }

    free(input_str);
    return 0;
}