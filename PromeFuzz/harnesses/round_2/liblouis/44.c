// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void cleanup_table_info(char *info) {
    if (info) {
        lou_freeTableInfo(info);
    }
}

static void cleanup_emph_classes(char const **classes) {
    if (classes) {
        lou_freeEmphClasses(classes);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzz data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Split input into two parts for table and key if possible
    char *table = input_str;
    char *key = input_str;
    
    // Try to find a delimiter to split the input
    for (size_t i = 0; i < Size; i++) {
        if (input_str[i] == '\n' || input_str[i] == '\t' || input_str[i] == '|') {
            input_str[i] = '\0';
            key = &input_str[i + 1];
            break;
        }
    }

    // Test lou_setDataPath
    char *path_result = lou_setDataPath(input_str);
    (void)path_result; // Result may be NULL or valid path pointer

    // Test lou_getDataPath
    char *current_path = lou_getDataPath();
    (void)current_path; // Result may be NULL or valid path pointer

    // Test lou_getTableInfo
    char *table_info = lou_getTableInfo(table, key);
    cleanup_table_info(table_info);

    // Test with swapped table and key
    char *table_info_swapped = lou_getTableInfo(key, table);
    cleanup_table_info(table_info_swapped);

    // Test lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(table);
    cleanup_emph_classes(emph_classes);

    // Test with key as table list
    char const **emph_classes_key = lou_getEmphClasses(key);
    cleanup_emph_classes(emph_classes_key);

    // Test with empty string
    char const **emph_classes_empty = lou_getEmphClasses("");
    cleanup_emph_classes(emph_classes_empty);

    // Test with NULL pointer (should be handled by library)
    char const **emph_classes_null = lou_getEmphClasses(NULL);
    cleanup_emph_classes(emph_classes_null);

    // Test lou_setDataPath with empty string
    char *empty_path = lou_setDataPath("");
    (void)empty_path;

    // Test lou_setDataPath with NULL
    char *null_path = lou_setDataPath(NULL);
    (void)null_path;

    // Test lou_getTableInfo with NULL arguments
    char *null_info1 = lou_getTableInfo(NULL, key);
    cleanup_table_info(null_info1);
    
    char *null_info2 = lou_getTableInfo(table, NULL);
    cleanup_table_info(null_info2);
    
    char *null_info3 = lou_getTableInfo(NULL, NULL);
    cleanup_table_info(null_info3);

    free(input_str);
    return 0;
}