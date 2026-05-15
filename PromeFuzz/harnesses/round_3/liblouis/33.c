// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void cleanup_table_files(char **table_files) {
    if (table_files) {
        lou_freeTableFiles(table_files);
    }
}

static void cleanup_table_file(char *table_file) {
    if (table_file) {
        lou_freeTableFile(table_file);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for string operations
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test lou_setDataPath with fuzzer input
    char *set_path_result = lou_setDataPath(input_str);
    (void)set_path_result; // Result may be NULL, just ignore

    // 2. Test lou_getDataPath (deprecated, returns current path)
    char *get_path_result = lou_getDataPath();
    (void)get_path_result;

    // 3. Test lou_version (compile-time constant)
    const char *version = lou_version();
    (void)version;

    // 4. Test lou_findTables with fuzzer input as query
    char **table_files = lou_findTables(input_str);
    
    // 5. Clean up table files if found
    cleanup_table_files(table_files);
    
    // 6. Test lou_freeTableFile with individual allocations
    // Create a dummy table file string and free it
    char *dummy_table = (char *)malloc(Size + 1);
    if (dummy_table) {
        memcpy(dummy_table, Data, Size);
        dummy_table[Size] = '\0';
        cleanup_table_file(dummy_table);
    }

    // Additional test: Try lou_setDataPath with NULL
    lou_setDataPath(NULL);

    // Additional test: Try lou_findTables with empty string
    if (Size > 1) {
        char **empty_query_result = lou_findTables("");
        cleanup_table_files(empty_query_result);
    }

    // Additional test: Try lou_findTables with NULL
    char **null_query_result = lou_findTables(NULL);
    cleanup_table_files(null_query_result);

    free(input_str);
    return 0;
}