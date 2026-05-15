// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static char *create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    size_t len = size > max_len ? max_len : size;
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create test strings from fuzzing input
    size_t split = Size / 2;
    char *str1 = create_null_terminated_string(Data, split, 256);
    char *str2 = create_null_terminated_string(Data + split, Size - split, 256);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }

    // Test lou_setDataPath and lou_getDataPath
    char *path_result = lou_setDataPath(str1);
    char *current_path = lou_getDataPath();
    (void)path_result;
    (void)current_path;

    // Test lou_getTableInfo and lou_freeTableInfo
    char *table_info = lou_getTableInfo(str1, str2);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // Test lou_getEmphClasses and lou_freeEmphClasses
    const char **emph_classes = lou_getEmphClasses(str1);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }

    // Test with file-based input
    write_dummy_file(Data, Size);
    char *file_table_info = lou_getTableInfo("./dummy_file", str2);
    if (file_table_info) {
        lou_freeTableInfo(file_table_info);
    }

    // Cleanup
    free(str1);
    free(str2);
    
    return 0;
}