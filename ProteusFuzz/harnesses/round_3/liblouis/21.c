// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
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

static void cleanup_table_info(char *info) {
    if (info) {
        lou_freeTableInfo(info);
    }
}

static void cleanup_table_files(char **files) {
    if (files) {
        lou_freeTableFiles(files);
    }
}

static void cleanup_table_file(char *file) {
    if (file) {
        lou_freeTableFile(file);
    }
}

static void cleanup_table_list(char **list) {
    if (list) {
        for (int i = 0; list[i] != NULL; i++) {
            free(list[i]);
        }
        free(list);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string operations
    char *query = malloc(Size + 1);
    if (!query) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_findTables
    char **foundTables = lou_findTables(query);
    if (foundTables) {
        for (int i = 0; foundTables[i] != NULL; i++) {
            // Test lou_getTableInfo with found table names
            char *info = lou_getTableInfo(foundTables[i], "language");
            cleanup_table_info(info);

            info = lou_getTableInfo(foundTables[i], "description");
            cleanup_table_info(info);

            info = lou_getTableInfo(foundTables[i], "author");
            cleanup_table_info(info);
        }
        // Free the array and its strings
        for (int i = 0; foundTables[i] != NULL; i++) {
            free(foundTables[i]);
        }
        free(foundTables);
    }

    // Test lou_listTables
    char **allTables = lou_listTables();
    if (allTables) {
        for (int i = 0; allTables[i] != NULL; i++) {
            // Test lou_getTableInfo with listed table names
            char *info = lou_getTableInfo(allTables[i], "language");
            cleanup_table_info(info);

            info = lou_getTableInfo(allTables[i], "description");
            cleanup_table_info(info);
        }
        // Free the array and its strings
        cleanup_table_list(allTables);
    }

    // Test lou_getTableInfo with the input as a table name (might fail, but safe)
    char *info = lou_getTableInfo(query, "language");
    cleanup_table_info(info);

    info = lou_getTableInfo(query, "description");
    cleanup_table_info(info);

    free(query);
    return 0;
}