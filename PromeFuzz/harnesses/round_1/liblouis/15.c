// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void free_string_array(char **array) {
    if (array) {
        for (int i = 0; array[i]; i++) {
            free(array[i]);
        }
        free(array);
    }
}

static void free_const_string_array(const char **array) {
    if (array) {
        for (int i = 0; array[i]; i++) {
            free((char*)array[i]);
        }
        free((char**)array);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract table name and key from fuzzer input
    size_t table_len = Size / 2;
    size_t key_len = Size - table_len;
    
    char *table_name = (char*)malloc(table_len + 1);
    char *key = (char*)malloc(key_len + 1);
    
    if (!table_name || !key) {
        free(table_name);
        free(key);
        return 0;
    }
    
    memcpy(table_name, Data, table_len);
    memcpy(key, Data + table_len, key_len);
    table_name[table_len] = '\0';
    key[key_len] = '\0';

    // Test lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        free_string_array(tables);
    }

    // Test lou_getTableInfo
    char *info = lou_getTableInfo(table_name, key);
    if (info) {
        lou_freeTableInfo(info);
    }

    // Test lou_getTable
    const void *table_ptr = lou_getTable(table_name);
    (void)table_ptr; // Use result to avoid unused variable warning

    // Test lou_getEmphClasses
    const char **emph_classes = lou_getEmphClasses(table_name);
    if (emph_classes) {
        free_const_string_array(emph_classes);
    }

    // Test lou_dotsToChar with dummy data
    if (Size >= 4) {
        int length = Size / 2;
        widechar *inbuf = (widechar*)malloc(length * sizeof(widechar));
        widechar *outbuf = (widechar*)malloc(length * sizeof(widechar));
        
        if (inbuf && outbuf) {
            // Convert bytes to widechars
            for (int i = 0; i < length && i * 2 + 1 < Size; i++) {
                inbuf[i] = (widechar)((Data[i * 2] << 8) | Data[i * 2 + 1]);
            }
            
            lou_dotsToChar(table_name, inbuf, outbuf, length, 0);
            
            free(inbuf);
            free(outbuf);
        } else {
            free(inbuf);
            free(outbuf);
        }
    }

    // Cleanup
    free(table_name);
    free(key);

    return 0;
}