// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_lou_findTables(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Ensure null termination
    char *query = (char *)malloc(Size + 1);
    if (!query) return;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    char **tables = lou_findTables(query);
    if (tables) {
        for (int i = 0; tables[i] != NULL; i++) {
            free(tables[i]);
        }
        free(tables);
    }
    free(query);
}

static void fuzz_lou_listTables(void) {
    char **tables = lou_listTables();
    if (tables) {
        for (int i = 0; tables[i] != NULL; i++) {
            free(tables[i]);
        }
        free(tables);
    }
}

static void fuzz_lou_getTableInfo(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Split data into table name and key
    size_t mid = Size / 2;
    char *table = (char *)malloc(mid + 1);
    char *key = (char *)malloc(Size - mid + 1);
    if (!table || !key) {
        free(table);
        free(key);
        return;
    }
    
    memcpy(table, Data, mid);
    table[mid] = '\0';
    memcpy(key, Data + mid, Size - mid);
    key[Size - mid] = '\0';

    char *info = lou_getTableInfo(table, key);
    if (info) {
        free(info);
    }
    free(table);
    free(key);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz lou_findTables
    fuzz_lou_findTables(Data, Size);

    // Fuzz lou_listTables (no input needed)
    fuzz_lou_listTables();

    // Fuzz lou_getTableInfo
    fuzz_lou_getTableInfo(Data, Size);

    // Additional edge cases: try with empty strings or single char
    if (Size >= 1) {
        char single[2] = {Data[0], '\0'};
        char **tables = lou_findTables(single);
        if (tables) {
            for (int i = 0; tables[i] != NULL; i++) {
                free(tables[i]);
            }
            free(tables);
        }
    }

    return 0;
}