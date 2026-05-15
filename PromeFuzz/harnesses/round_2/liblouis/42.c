// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    // 1. Test lou_setDataPath with fuzzer-provided data
    if (Size > 0) {
        // Create null-terminated string from fuzzer input
        char *path = (char *)malloc(Size + 1);
        if (path) {
            memcpy(path, Data, Size);
            path[Size] = '\0';
            char *result = lou_setDataPath(path);
            free(path);
        }
    }
    
    // 2. Test lou_freeTableFile with NULL (safe to call)
    lou_freeTableFile(NULL);
    
    // 3. Test lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        // Free the array returned by lou_listTables
        lou_freeTableFiles(tables);
    }
    
    // 4. Test lou_getDataPath
    char *data_path = lou_getDataPath();
    // No need to free, returns internal pointer
    
    // 5. Test lou_findTables with fuzzer-provided query
    if (Size > 0) {
        // Create null-terminated string for query
        char *query = (char *)malloc(Size + 1);
        if (query) {
            memcpy(query, Data, Size);
            query[Size] = '\0';
            char **found_tables = lou_findTables(query);
            if (found_tables) {
                lou_freeTableFiles(found_tables);
            }
            free(query);
        }
    }
    
    // 6. Test lou_freeTableFiles with NULL (safe to call)
    lou_freeTableFiles(NULL);
    
    return 0;
}