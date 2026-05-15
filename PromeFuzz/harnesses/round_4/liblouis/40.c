// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static const char* get_table_list(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return "en-us-g2.ctb";
    
    size_t len = data[*offset] % 64;
    if (len == 0) len = 1;
    if (*offset + len >= size) return "en-us-g2.ctb";
    
    char *table = (char*)malloc(len + 1);
    if (!table) return "en-us-g2.ctb";
    
    memcpy(table, data + *offset, len);
    table[len] = '\0';
    *offset += len;
    
    static char static_table[256];
    strncpy(static_table, table, sizeof(static_table) - 1);
    static_table[sizeof(static_table) - 1] = '\0';
    free(table);
    
    return static_table;
}

static void init_widechar_buffer(const uint8_t *data, size_t size, size_t *offset, 
                                 widechar **buf, int *len, int max_len) {
    if (*offset >= size) {
        *len = 0;
        *buf = NULL;
        return;
    }
    
    *len = data[*offset] % max_len;
    if (*len == 0) *len = 1;
    (*offset)++;
    
    if (*offset + *len * sizeof(widechar) > size) {
        *len = (size - *offset) / sizeof(widechar);
    }
    
    if (*len > 0) {
        *buf = (widechar*)malloc(*len * sizeof(widechar));
        if (*buf) {
            memcpy(*buf, data + *offset, *len * sizeof(widechar));
            *offset += *len * sizeof(widechar);
        }
    } else {
        *buf = NULL;
        *len = 0;
    }
}

static formtype* init_typeform(int len, const uint8_t *data, size_t size, size_t *offset) {
    if (len <= 0) return NULL;
    
    formtype *typeform = (formtype*)malloc(len * sizeof(formtype));
    if (!typeform) return NULL;
    
    for (int i = 0; i < len; i++) {
        if (*offset < size) {
            typeform[i] = data[*offset] % 256;
            (*offset)++;
        } else {
            typeform[i] = 0;
        }
    }
    return typeform;
}

static char* init_spacing(int len, const uint8_t *data, size_t size, size_t *offset) {
    if (len <= 0) return NULL;
    
    char *spacing = (char*)malloc(len * sizeof(char));
    if (!spacing) return NULL;
    
    for (int i = 0; i < len; i++) {
        if (*offset < size) {
            spacing[i] = data[*offset] % 128;
            (*offset)++;
        } else {
            spacing[i] = 0;
        }
    }
    return spacing;
}

static int* init_int_array(int len, const uint8_t *data, size_t size, size_t *offset) {
    if (len <= 0) return NULL;
    
    int *arr = (int*)malloc(len * sizeof(int));
    if (!arr) return NULL;
    
    for (int i = 0; i < len; i++) {
        if (*offset + sizeof(int) <= size) {
            memcpy(&arr[i], data + *offset, sizeof(int));
            *offset += sizeof(int);
        } else {
            arr[i] = 0;
        }
    }
    return arr;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    const char *tableList = get_table_list(Data, Size, &offset);
    
    widechar *inbuf = NULL;
    int inlen = 0;
    init_widechar_buffer(Data, Size, &offset, &inbuf, &inlen, 256);
    
    if (inlen <= 0 || !inbuf) {
        free(inbuf);
        return 0;
    }
    
    int outlen = inlen * 4;
    widechar *outbuf = (widechar*)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    int mode = 0;
    if (offset < Size) {
        mode = Data[offset] % 256;
        offset++;
    }
    
    formtype *typeform = init_typeform(inlen, Data, Size, &offset);
    char *spacing = init_spacing(inlen + 1, Data, Size, &offset);
    
    int inlen_copy = inlen;
    int outlen_copy = outlen;
    
    lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, 
                       typeform, spacing, mode);
    
    free(typeform);
    free(spacing);
    
    inlen_copy = inlen;
    outlen_copy = outlen;
    
    int *outputPos = init_int_array(inlen, Data, Size, &offset);
    int *inputPos = init_int_array(outlen, Data, Size, &offset);
    int cursorPos = 0;
    if (offset < Size) {
        cursorPos = Data[offset] % (inlen + 1);
        offset++;
    }
    
    typeform = init_typeform(outlen, Data, Size, &offset);
    spacing = init_spacing(outlen, Data, Size, &offset);
    
    lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    
    char *hyphens = (char*)malloc(inlen * sizeof(char));
    if (hyphens) {
        lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
        free(hyphens);
    }
    
    inlen_copy = inlen;
    outlen_copy = outlen;
    
    typeform = init_typeform(outlen, Data, Size, &offset);
    spacing = init_spacing(outlen, Data, Size, &offset);
    
    lou_backTranslateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                           typeform, spacing, mode);
    
    free(typeform);
    free(spacing);
    
    inlen_copy = inlen;
    outlen_copy = outlen;
    
    outputPos = init_int_array(inlen, Data, Size, &offset);
    inputPos = init_int_array(outlen, Data, Size, &offset);
    cursorPos = 0;
    if (offset < Size) {
        cursorPos = Data[offset] % (inlen + 1);
        offset++;
    }
    
    typeform = (formtype*)malloc(outlen * sizeof(formtype));
    if (typeform) {
        memset(typeform, 0, outlen * sizeof(formtype));
    }
    spacing = init_spacing(inlen, Data, Size, &offset);
    
    lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                 typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    
    inlen_copy = inlen;
    outlen_copy = outlen;
    
    outputPos = init_int_array(inlen, Data, Size, &offset);
    inputPos = init_int_array(outlen, Data, Size, &offset);
    cursorPos = 0;
    if (offset < Size) {
        cursorPos = Data[offset] % (inlen + 1);
        offset++;
    }
    
    typeform = (formtype*)malloc(outlen * sizeof(formtype));
    if (typeform) {
        memset(typeform, 0, outlen * sizeof(formtype));
    }
    spacing = init_spacing(inlen, Data, Size, &offset);
    
    char *inputHyphens = (char*)malloc(inlen * sizeof(char));
    char *outputHyphens = (char*)malloc(outlen * sizeof(char));
    if (inputHyphens && outputHyphens) {
        for (int i = 0; i < inlen; i++) {
            inputHyphens[i] = (i % 3 == 0) ? 1 : 0;
        }
        for (int i = 0; i < outlen; i++) {
            outputHyphens[i] = 0;
        }
        
        lou_translatePrehyphenated(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                                  typeform, spacing, outputPos, inputPos, &cursorPos,
                                  inputHyphens, outputHyphens, mode);
    }
    
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(inputHyphens);
    free(outputHyphens);
    
    return 0;
}