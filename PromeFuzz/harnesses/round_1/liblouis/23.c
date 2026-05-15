// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_lou_getTableInfo(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    char *table = (char *)malloc(split + 1);
    char *key = (char *)malloc(size - split + 1);
    
    if (!table || !key) {
        free(table);
        free(key);
        return;
    }
    
    memcpy(table, data, split);
    memcpy(key, data + split, size - split);
    table[split] = '\0';
    key[size - split] = '\0';
    
    char *result = lou_getTableInfo(table, key);
    if (result) {
        lou_freeTableInfo(result);
    }
    
    free(table);
    free(key);
}

static void fuzz_lou_dotsToChar(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    size_t table_len = data[0] % 32 + 1;
    if (table_len >= size) return;
    
    char *table = (char *)malloc(table_len + 1);
    if (!table) return;
    
    memcpy(table, data + 1, table_len);
    table[table_len] = '\0';
    
    size_t remaining = size - 1 - table_len;
    if (remaining < 2) {
        free(table);
        return;
    }
    
    int length = (remaining / 2) % 256;
    if (length <= 0) {
        free(table);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(length * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(length * sizeof(widechar));
    
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        free(table);
        return;
    }
    
    for (int i = 0; i < length; i++) {
        size_t offset = 1 + table_len + i * 2;
        if (offset + 1 < size) {
            inbuf[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            inbuf[i] = 0;
        }
    }
    
    lou_dotsToChar(table, inbuf, outbuf, length, 0);
    
    free(inbuf);
    free(outbuf);
    free(table);
}

static void fuzz_lou_getTypeformForEmphClass(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    char *table = (char *)malloc(split + 1);
    char *emphClass = (char *)malloc(size - split + 1);
    
    if (!table || !emphClass) {
        free(table);
        free(emphClass);
        return;
    }
    
    memcpy(table, data, split);
    memcpy(emphClass, data + split, size - split);
    table[split] = '\0';
    emphClass[size - split] = '\0';
    
    lou_getTypeformForEmphClass(table, emphClass);
    
    free(table);
    free(emphClass);
}

static void fuzz_lou_getEmphClasses(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *table = (char *)malloc(size + 1);
    if (!table) return;
    
    memcpy(table, data, size);
    table[size] = '\0';
    
    char const **classes = lou_getEmphClasses(table);
    if (classes) {
        lou_freeEmphClasses(classes);
    }
    
    free(table);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    if (fuzz_size == 0) return 0;
    
    switch (selector % 4) {
        case 0:
            fuzz_lou_getTableInfo(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_lou_dotsToChar(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_lou_getTypeformForEmphClass(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_lou_getEmphClasses(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}