// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void cleanup_string_array(char **array) {
    if (array) {
        lou_freeTableFiles(array);
    }
}

static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *str = (char *)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Test lou_listTables and lou_freeTableFiles
    char **tables = lou_listTables();
    if (tables) {
        // Count tables and potentially use them
        for (int i = 0; tables[i] != NULL; i++) {
            // Just access the table name, don't do anything with it
            (void)tables[i];
        }
        lou_freeTableFiles(tables);
    }
    
    // Test lou_getDataPath
    char *data_path = lou_getDataPath();
    (void)data_path; // Use the result to avoid unused variable warning
    
    // Test lou_setDataPath with fuzzed input
    char *path_str = create_null_terminated_string(Data, Size);
    if (path_str) {
        char *old_path = lou_setDataPath(path_str);
        (void)old_path; // Use the result
        free(path_str);
    }
    
    // Test lou_findTables with fuzzed input as query
    char *query_str = create_null_terminated_string(Data, Size);
    if (query_str) {
        char **found_tables = lou_findTables(query_str);
        cleanup_string_array(found_tables);
        free(query_str);
    }
    
    // Test lou_setDataPath with empty string
    char *empty_path = lou_setDataPath("");
    (void)empty_path;
    
    // Test lou_findTables with empty query
    char **empty_query_tables = lou_findTables("");
    cleanup_string_array(empty_query_tables);
    
    // Test lou_findTables with NULL query
    char **null_query_tables = lou_findTables(NULL);
    cleanup_string_array(null_query_tables);
    
    // Test lou_setDataPath with NULL path
    char *null_path_result = lou_setDataPath(NULL);
    (void)null_path_result;
    
    // Test with a dummy file path for lou_setDataPath
    // Create a dummy file with fuzzed data
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
        
        // Use the dummy file path
        char *dummy_path_result = lou_setDataPath("./dummy_file");
        (void)dummy_path_result;
        
        // Clean up
        remove("./dummy_file");
    }
    
    return 0;
}