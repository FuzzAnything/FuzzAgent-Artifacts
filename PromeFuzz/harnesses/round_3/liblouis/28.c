// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_lou_findTable(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *query = malloc(size + 1);
    if (!query) return;
    
    memcpy(query, data, size);
    query[size] = '\0';
    
    char *result = lou_findTable(query);
    if (result) {
        free(result);
    }
    
    free(query);
}

static void fuzz_lou_listTables(void) {
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }
}

static void fuzz_lou_getTable(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *tableList = malloc(size + 1);
    if (!tableList) return;
    
    memcpy(tableList, data, size);
    tableList[size] = '\0';
    
    const void *table = lou_getTable(tableList);
    if (table) {
        lou_freeTableFile((char *)table);
    }
    
    free(tableList);
}

static void fuzz_lou_findTables(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *query = malloc(size + 1);
    if (!query) return;
    
    memcpy(query, data, size);
    query[size] = '\0';
    
    char **tables = lou_findTables(query);
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    free(query);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    lou_indexTables(NULL);
    
    fuzz_lou_findTable(Data, Size);
    fuzz_lou_listTables();
    fuzz_lou_getTable(Data, Size);
    fuzz_lou_findTables(Data, Size);
    
    return 0;
}