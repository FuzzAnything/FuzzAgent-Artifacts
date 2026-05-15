// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static char **custom_resolver(const char *table, const char *base) {
    // Simple resolver that returns a dummy table list
    char **list = malloc(3 * sizeof(char *));
    if (!list) return NULL;
    list[0] = strdup("en-us-g2.ctb");
    list[1] = strdup("test.ctb");
    list[2] = NULL;
    return list;
}

static void cleanup_string_array(char **array) {
    if (!array) return;
    for (int i = 0; array[i]; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Register custom table resolver
    lou_registerTableResolver(custom_resolver);

    // Call lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        cleanup_string_array(tables);
    }

    // Prepare data for lou_dotsToChar
    // Use first byte as mode (deprecated but required)
    int mode = Data[0] % 2;
    // Use second byte to determine length (1-16)
    int length = (Data[1] % 16) + 1;
    if (length * sizeof(widechar) > Size - 2) {
        length = (Size - 2) / sizeof(widechar);
        if (length <= 0) return 0;
    }

    // Create table list from input data
    char *tableList = malloc(length + 1);
    if (!tableList) return 0;
    for (int i = 0; i < length && i + 2 < Size; i++) {
        tableList[i] = Data[i + 2] ? Data[i + 2] : 'a';
    }
    tableList[length] = '\0';

    widechar *inbuf = malloc(length * sizeof(widechar));
    widechar *outbuf = malloc(length * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        return 0;
    }

    // Initialize inbuf with data
    for (int i = 0; i < length; i++) {
        if (i * sizeof(widechar) + 2 + length < Size) {
            inbuf[i] = Data[2 + length + i] | (Data[2 + length + i + 1] << 8);
        } else {
            inbuf[i] = i + 1;
        }
    }

    // Call lou_dotsToChar
    lou_dotsToChar(tableList, inbuf, outbuf, length, mode);

    free(inbuf);
    free(outbuf);

    // Call lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        // Note: According to documentation, caller must free the array
        // but the function returns const char **, so we cast to free
        cleanup_string_array((char **)emphClasses);
    }

    // Prepare key for lou_getTableInfo
    int key_len = length > 8 ? 8 : length;
    char *key = malloc(key_len + 1);
    if (key) {
        for (int i = 0; i < key_len && i + 2 + length * 2 < Size; i++) {
            key[i] = Data[2 + length * 2 + i] ? Data[2 + length * 2 + i] : 'k';
        }
        key[key_len] = '\0';

        // Call lou_getTableInfo
        char *tableInfo = lou_getTableInfo(tableList, key);
        if (tableInfo) {
            lou_freeTableInfo(tableInfo);
        }

        free(key);
    }

    // Try with different table/key combinations
    if (Size > 10) {
        // Use last few bytes as alternative table name
        int alt_len = Data[Size - 1] % 8 + 1;
        if (alt_len < Size) {
            char *altTable = malloc(alt_len + 1);
            if (altTable) {
                for (int i = 0; i < alt_len; i++) {
                    altTable[i] = Data[Size - alt_len + i] ? Data[Size - alt_len + i] : 't';
                }
                altTable[alt_len] = '\0';

                char *altInfo = lou_getTableInfo(altTable, "language");
                if (altInfo) {
                    lou_freeTableInfo(altInfo);
                }

                free(altTable);
            }
        }
    }

    free(tableList);
    return 0;
}