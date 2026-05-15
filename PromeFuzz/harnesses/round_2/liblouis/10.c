// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void free_emph_classes(char const **emph_classes) {
    if (emph_classes) {
        for (int i = 0; emph_classes[i] != NULL; i++) {
            free((char *)emph_classes[i]);
        }
        free((char **)emph_classes);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into two null-terminated strings for table/key parameters
    size_t split = Size / 2;
    char *str1 = malloc(split + 1);
    char *str2 = malloc(Size - split + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    memcpy(str1, Data, split);
    memcpy(str2, Data + split, Size - split);
    str1[split] = '\0';
    str2[Size - split] = '\0';

    // Test lou_getTableInfo and lou_freeTableInfo
    char *table_info = lou_getTableInfo(str1, str2);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // Test lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(str1);
    if (emph_classes) {
        free_emph_classes(emph_classes);
    }

    // Test lou_getTable
    const void *table = lou_getTable(str1);
    (void)table; // Use result to avoid unused variable warning

    // Test lou_findTable - don't call lou_indexTables to avoid crash
    char *found_table = lou_findTable(str1);
    if (found_table) {
        free(found_table);
    }

    // Test lou_dotsToChar
    if (Size >= 4) {
        int length = Size / 4;
        if (length > 0) {
            widechar *inbuf = (widechar *)Data;
            widechar *outbuf = malloc(length * sizeof(widechar));
            if (outbuf) {
                int mode = 0; // Deprecated parameter
                lou_dotsToChar(str1, inbuf, outbuf, length, mode);
                free(outbuf);
            }
        }
    }

    free(str1);
    free(str2);
    return 0;
}