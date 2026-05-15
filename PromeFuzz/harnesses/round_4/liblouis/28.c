// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void fuzz_dotsToChar(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    size_t tableListLen = size < 256 ? size : 256;
    char *tableList = malloc(tableListLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, data, tableListLen);
    tableList[tableListLen] = '\0';
    
    size_t remaining = size - tableListLen;
    size_t widecharCount = remaining / sizeof(widechar);
    
    if (widecharCount > 0) {
        widechar *inbuf = malloc(widecharCount * sizeof(widechar));
        widechar *outbuf = malloc(widecharCount * sizeof(widechar));
        
        if (inbuf && outbuf) {
            memcpy(inbuf, data + tableListLen, widecharCount * sizeof(widechar));
            
            lou_dotsToChar(tableList, inbuf, outbuf, widecharCount, 0);
            
            free(outbuf);
        }
        
        free(inbuf);
    }
    
    free(tableList);
}

static void fuzz_getTable(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    size_t tableListLen = size < 256 ? size : 256;
    char *tableList = malloc(tableListLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, data, tableListLen);
    tableList[tableListLen] = '\0';
    
    const void *table = lou_getTable(tableList);
    if (table) {
        lou_freeTableFile((char *)table);
    }
    
    free(tableList);
}

static void fuzz_getEmphClasses(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    size_t tableListLen = size < 256 ? size : 256;
    char *tableList = malloc(tableListLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, data, tableListLen);
    tableList[tableListLen] = '\0';
    
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        for (int i = 0; emphClasses[i] != NULL; i++) {
            free((void *)emphClasses[i]);
        }
        free((void *)emphClasses);
    }
    
    free(tableList);
}

static void fuzz_getTableInfo(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t half = size / 2;
    size_t tableLen = half < 128 ? half : 128;
    size_t keyLen = (size - half) < 128 ? (size - half) : 128;
    
    char *table = malloc(tableLen + 1);
    char *key = malloc(keyLen + 1);
    
    if (!table || !key) {
        free(table);
        free(key);
        return;
    }
    
    memcpy(table, data, tableLen);
    table[tableLen] = '\0';
    
    memcpy(key, data + half, keyLen);
    key[keyLen] = '\0';
    
    char *info = lou_getTableInfo(table, key);
    if (info) {
        lou_freeTableInfo(info);
    }
    
    free(table);
    free(key);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;
    
    if (remaining_size == 0) return 0;
    
    if (selector % 4 == 0) {
        fuzz_dotsToChar(remaining_data, remaining_size);
    } else if (selector % 4 == 1) {
        fuzz_getTable(remaining_data, remaining_size);
    } else if (selector % 4 == 2) {
        fuzz_getEmphClasses(remaining_data, remaining_size);
    } else {
        fuzz_getTableInfo(remaining_data, remaining_size);
    }
    
    return 0;
}