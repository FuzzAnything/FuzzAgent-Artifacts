// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_lou_listTables(void) {
    char **tables = lou_listTables();
    if (tables != NULL) {
        lou_freeTableFiles(tables);
    }
}

static void fuzz_lou_freeTableFile(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *table_file = malloc(size + 1);
    if (table_file == NULL) return;
    
    memcpy(table_file, data, size);
    table_file[size] = '\0';
    
    lou_freeTableFile(table_file);
}

static void fuzz_lou_getDataPath(void) {
    char *path = lou_getDataPath();
    (void)path;
}

static void fuzz_lou_findTables(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *query = malloc(size + 1);
    if (query == NULL) return;
    
    memcpy(query, data, size);
    query[size] = '\0';
    
    char **tables = lou_findTables(query);
    if (tables != NULL) {
        lou_freeTableFiles(tables);
    }
    
    free(query);
}

static void fuzz_lou_setDataPath(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *path = malloc(size + 1);
    if (path == NULL) return;
    
    memcpy(path, data, size);
    path[size] = '\0';
    
    char *result = lou_setDataPath(path);
    (void)result;
    
    free(path);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    fuzz_lou_listTables();
    fuzz_lou_freeTableFile(data, size);
    fuzz_lou_getDataPath();
    fuzz_lou_findTables(data, size);
    fuzz_lou_setDataPath(data, size);
    
    return 0;
}