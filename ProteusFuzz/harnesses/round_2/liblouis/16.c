// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void free_char_array(char **arr) {
    if (arr == NULL) return;
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
    free(arr);
}

static void free_char_ptr(char *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure input is null-terminated for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // Test lou_findTable
    char *table = lou_findTable(query);
    free_char_ptr(table);

    // Test lou_findTables
    char **tables = lou_findTables(query);
    free_char_array(tables);

    // Test lou_listTables
    char **all_tables = lou_listTables();
    free_char_array(all_tables);

    // Test lou_indexTables with a single table name derived from input
    // We construct a simple array with one element and a NULL terminator
    char **index_tables = (char **)malloc(2 * sizeof(char *));
    if (index_tables != NULL) {
        // Use the query as a potential table name, but ensure it's valid
        // We just pass the query string directly; liblouis will handle invalid names
        index_tables[0] = query;
        index_tables[1] = NULL;
        lou_indexTables((const char **)index_tables);
        free(index_tables);
    }

    // Test lou_freeTableFile with NULL (safe)
    lou_freeTableFile(NULL);

    // Test lou_freeTableFiles with NULL (safe)
    lou_freeTableFiles(NULL);

    free(query);
    return 0;
}