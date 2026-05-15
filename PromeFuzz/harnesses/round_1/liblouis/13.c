// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void initialize_widechar_buffer(const uint8_t *data, size_t size, widechar **buf, int *len) {
    if (size == 0) {
        *len = 1;
        *buf = (widechar *)malloc(*len * sizeof(widechar));
        (*buf)[0] = 0;
        return;
    }
    
    *len = size / sizeof(widechar);
    if (*len == 0) *len = 1;
    *buf = (widechar *)malloc(*len * sizeof(widechar));
    
    size_t copy_len = (size < *len * sizeof(widechar)) ? size : *len * sizeof(widechar);
    memcpy(*buf, data, copy_len);
    
    for (size_t i = copy_len / sizeof(widechar); i < (size_t)*len; i++) {
        (*buf)[i] = 0;
    }
}

static char *extract_table_list(const uint8_t *data, size_t size, size_t *consumed) {
    if (size == 0) {
        *consumed = 0;
        char *table = (char *)malloc(1);
        table[0] = '\0';
        return table;
    }
    
    size_t max_len = (size > 256) ? 256 : size;
    size_t len = 0;
    while (len < max_len && data[len] != 0) {
        len++;
    }
    
    char *table = (char *)malloc(len + 1);
    memcpy(table, data, len);
    table[len] = '\0';
    
    *consumed = (len < max_len) ? len + 1 : len;
    return table;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t consumed = 0;
    char *tableList = extract_table_list(Data, Size, &consumed);
    
    if (consumed >= Size) {
        free(tableList);
        return 0;
    }
    
    const uint8_t *remaining_data = Data + consumed;
    size_t remaining_size = Size - consumed;
    
    widechar *inbuf = NULL;
    int inlen = 0;
    initialize_widechar_buffer(remaining_data, remaining_size, &inbuf, &inlen);
    
    int outlen = inlen * 4 + 10;
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    
    int mode = Data[0] % 256;
    int use_typeform = Data[1] & 1;
    int use_spacing = Data[1] & 2;
    int use_pos = Data[1] & 4;
    int use_hyphens = Data[1] & 8;
    
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;
    
    if (use_typeform) {
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        memset(typeform, 0, outlen * sizeof(formtype));
    }
    
    if (use_spacing) {
        spacing = (char *)malloc((outlen + 1) * sizeof(char));
        memset(spacing, 0, (outlen + 1) * sizeof(char));
    }
    
    if (use_pos) {
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        memset(outputPos, 0, inlen * sizeof(int));
        memset(inputPos, 0, outlen * sizeof(int));
        cursorPos = inlen > 0 ? (Data[1] % inlen) : 0;
    }
    
    if (use_hyphens) {
        inputHyphens = (char *)malloc((inlen + 1) * sizeof(char));
        outputHyphens = (char *)malloc((outlen + 1) * sizeof(char));
        memset(inputHyphens, 0, (inlen + 1) * sizeof(char));
        memset(outputHyphens, 0, (outlen + 1) * sizeof(char));
    }
    
    int selector = Data[0] % 6;
    int result = 0;
    
    switch (selector) {
        case 0: {
            int local_inlen = inlen;
            int local_outlen = outlen;
            result = lou_translateString(tableList, inbuf, &local_inlen, outbuf, 
                                        &local_outlen, typeform, spacing, mode);
            break;
        }
        
        case 1: {
            int local_inlen = inlen;
            int local_outlen = outlen;
            result = lou_translatePrehyphenated(tableList, inbuf, &local_inlen, outbuf,
                                               &local_outlen, typeform, spacing,
                                               outputPos, inputPos, &cursorPos,
                                               inputHyphens, outputHyphens, mode);
            break;
        }
        
        case 2: {
            const void *table = lou_getTable(tableList);
            (void)table;
            break;
        }
        
        case 3: {
            char *hyphens = (char *)malloc((inlen + 1) * sizeof(char));
            result = lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
            free(hyphens);
            break;
        }
        
        case 4: {
            int local_inlen = inlen;
            int local_outlen = outlen;
            result = lou_translate(tableList, inbuf, &local_inlen, outbuf,
                                  &local_outlen, typeform, spacing,
                                  outputPos, inputPos, &cursorPos, mode);
            break;
        }
        
        case 5: {
            int local_inlen = inlen;
            int local_outlen = outlen;
            result = lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf,
                                            &local_outlen, typeform, spacing, mode);
            break;
        }
    }
    
    free(tableList);
    free(inbuf);
    free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (inputHyphens) free(inputHyphens);
    if (outputHyphens) free(outputHyphens);
    
    return 0;
}