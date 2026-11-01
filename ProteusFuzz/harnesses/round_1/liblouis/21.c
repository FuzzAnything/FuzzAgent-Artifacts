// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
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

static void safe_free_table_info(char *ptr) {
    if (ptr != NULL) {
        lou_freeTableInfo(ptr);
    }
}

static void safe_free_table_array(char **ptr) {
    if (ptr == NULL) return;
    for (int i = 0; ptr[i] != NULL; i++) {
        free(ptr[i]);
    }
    free(ptr);
}

static void safe_free_table_files(char **ptr) {
    if (ptr == NULL) return;
    lou_freeTableFiles(ptr);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string operations
    char *table_name = NULL;
    char *key = NULL;
    char *query = NULL;

    // Create null-terminated strings from input
    table_name = (char *)malloc(Size + 1);
    key = (char *)malloc(Size + 1);
    query = (char *)malloc(Size + 1);

    if (!table_name || !key || !query) {
        free(table_name);
        free(key);
        free(query);
        return 0;
    }

    memcpy(table_name, Data, Size);
    table_name[Size] = '\0';
    memcpy(key, Data, Size);
    key[Size] = '\0';
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_getTableInfo
    char *info = lou_getTableInfo(table_name, key);
    if (info != NULL) {
        // Use the info briefly if needed, then free
        safe_free_table_info(info);
    }

    // Test lou_findTables
    char **tables = lou_findTables(query);
    if (tables != NULL) {
        safe_free_table_array(tables);
    }

    // Test lou_listTables
    char **all_tables = lou_listTables();
    if (all_tables != NULL) {
        safe_free_table_array(all_tables);
    }

    // Test lou_freeTableFile with a dummy pointer (safe if NULL)
    // We cannot generate a valid pointer from lou_loadTableFile without more context,
    // so we test the NULL case which is documented as safe.
    lou_freeTableFile(NULL);

    // Test lou_freeTableFiles with NULL
    lou_freeTableFiles(NULL);

    // Cleanup allocated buffers
    free(table_name);
    free(key);
    free(query);

    return 0;
}