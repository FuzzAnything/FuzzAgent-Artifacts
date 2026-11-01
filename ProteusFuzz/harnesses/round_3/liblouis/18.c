// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void cleanup_table_info(char *info) {
    if (info != NULL) {
        lou_freeTableInfo(info);
    }
}

static void cleanup_table_files(char **files) {
    if (files != NULL) {
        lou_freeTableFiles(files);
    }
}

static void cleanup_table_file(char *file) {
    if (file != NULL) {
        lou_freeTableFile(file);
    }
}

static void cleanup_string_array(char **array) {
    if (array == NULL) return;
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for string operations
    char *input = (char *)malloc(Size + 1);
    if (input == NULL) return 0;
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // Fuzz lou_findTables
    char **tables = lou_findTables(input);
    if (tables != NULL) {
        cleanup_string_array(tables);
    }

    // Fuzz lou_listTables
    char **allTables = lou_listTables();
    if (allTables != NULL) {
        cleanup_string_array(allTables);
    }

    // Fuzz lou_getTableInfo with input as table name and key
    char *info1 = lou_getTableInfo(input, input);
    cleanup_table_info(info1);

    // Fuzz lou_getTableInfo with fixed table name and input as key
    char *info2 = lou_getTableInfo("en-gb", input);
    cleanup_table_info(info2);

    // Fuzz lou_getTableInfo with input as table name and fixed key
    char *info3 = lou_getTableInfo(input, "language");
    cleanup_table_info(info3);

    // Fuzz lou_getTableInfo with fixed table name and fixed key
    char *info4 = lou_getTableInfo("en-gb", "language");
    cleanup_table_info(info4);

    // Note: lou_loadTableFile and lou_loadTableFiles are not part of the provided API list
    // and are not declared in the provided header context, so they are omitted to prevent compilation errors.
    // The fuzzing focuses on the explicitly listed functions:
    // lou_freeTableInfo, lou_findTables, lou_getTableInfo, lou_freeTableFile, lou_listTables, lou_freeTableFiles.
    // Since we cannot load tables without the corresponding load functions, we only test the free functions with NULL
    // to ensure they handle NULL safely as per their descriptions.

    // Test lou_freeTableFile with NULL (safe)
    lou_freeTableFile(NULL);

    // Test lou_freeTableFiles with NULL (safe)
    lou_freeTableFiles(NULL);

    // Test lou_freeTableInfo with NULL (safe)
    lou_freeTableInfo(NULL);

    free(input);
    return 0;
}