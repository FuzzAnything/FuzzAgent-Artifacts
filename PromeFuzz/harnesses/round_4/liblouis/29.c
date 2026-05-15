// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_dotsToChar(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t tableLen = 0;
    while (tableLen < size && data[tableLen] != 0) {
        tableLen++;
    }
    if (tableLen >= size) return;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    memcpy(tableList, data, tableLen);
    tableList[tableLen] = '\0';
    
    size_t remaining = size - (tableLen + 1);
    size_t widechars = remaining / sizeof(widechar);
    if (widechars == 0) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(widechars * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(widechars * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        return;
    }
    
    memcpy(inbuf, data + tableLen + 1, widechars * sizeof(widechar));
    
    lou_dotsToChar(tableList, inbuf, outbuf, widechars, 0);
    
    free(tableList);
    free(inbuf);
    free(outbuf);
}

static void fuzz_getEmphClasses(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return;
    memcpy(tableList, data, size);
    tableList[size] = '\0';
    
    char const **classes = lou_getEmphClasses(tableList);
    if (classes) {
        lou_freeEmphClasses(classes);
    }
    
    free(tableList);
}

static void fuzz_getTable(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return;
    memcpy(tableList, data, size);
    tableList[size] = '\0';
    
    lou_getTable(tableList);
    
    free(tableList);
}

static void fuzz_getTableInfo(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t tableLen = 0;
    while (tableLen < size && data[tableLen] != 0) {
        tableLen++;
    }
    if (tableLen >= size) return;
    
    size_t keyStart = tableLen + 1;
    size_t keyLen = 0;
    while (keyStart + keyLen < size && data[keyStart + keyLen] != 0) {
        keyLen++;
    }
    
    char *table = (char *)malloc(tableLen + 1);
    char *key = (char *)malloc(keyLen + 1);
    if (!table || !key) {
        free(table);
        free(key);
        return;
    }
    
    memcpy(table, data, tableLen);
    table[tableLen] = '\0';
    memcpy(key, data + keyStart, keyLen);
    key[keyLen] = '\0';
    
    char *info = lou_getTableInfo(table, key);
    if (info) {
        lou_freeTableInfo(info);
    }
    
    free(table);
    free(key);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 4;
    const uint8_t *remainingData = Data + 1;
    size_t remainingSize = Size - 1;
    
    switch (selector) {
        case 0:
            fuzz_dotsToChar(remainingData, remainingSize);
            break;
        case 1:
            fuzz_getEmphClasses(remainingData, remainingSize);
            break;
        case 2:
            fuzz_getTable(remainingData, remainingSize);
            break;
        case 3:
            fuzz_getTableInfo(remainingData, remainingSize);
            break;
    }
    
    return 0;
}