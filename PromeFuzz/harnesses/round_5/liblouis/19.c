// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void init_widechar_buffer(const uint8_t *data, size_t size, widechar **buf, int *len) {
    *len = size / sizeof(widechar);
    if (*len > 0) {
        *buf = (widechar *)malloc(*len * sizeof(widechar));
        memcpy(*buf, data, *len * sizeof(widechar));
    } else {
        *buf = NULL;
        *len = 0;
    }
}

static const char *get_table_list(const uint8_t *data, size_t size) {
    static char table_buf[256];
    if (size > 255) size = 255;
    memcpy(table_buf, data, size);
    table_buf[size] = '\0';
    return table_buf;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    const char *tableList = get_table_list(Data, Size > 100 ? 100 : Size);
    
    widechar *inbuf = NULL;
    int inlen = 0;
    init_widechar_buffer(Data, Size, &inbuf, &inlen);
    
    if (inlen == 0) {
        free(inbuf);
        return 0;
    }

    int outlen = inlen * 2;
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    
    int mode = Data[0] & 0x0F;
    int function_selector = Data[1] % 6;

    switch (function_selector) {
        case 0: {
            lou_getTable(tableList);
            break;
        }
        case 1: {
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, NULL, NULL, mode);
            break;
        }
        case 2: {
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            memset(typeform, 0, outlen * sizeof(formtype));
            char *spacing = (char *)malloc((inlen + 1) * sizeof(char));
            memset(spacing, 0, (inlen + 1) * sizeof(char));
            
            lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, typeform, spacing, 
                         outputPos, inputPos, &cursorPos, mode);
            
            free(outputPos);
            free(inputPos);
            free(typeform);
            free(spacing);
            break;
        }
        case 3: {
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            memset(typeform, 0, outlen * sizeof(formtype));
            char *spacing = (char *)malloc((outlen + 1) * sizeof(char));
            memset(spacing, 0, (outlen + 1) * sizeof(char));
            
            lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, typeform, spacing,
                            outputPos, inputPos, &cursorPos, mode);
            
            free(outputPos);
            free(inputPos);
            free(typeform);
            free(spacing);
            break;
        }
        case 4: {
            char *hyphens = (char *)malloc((inlen + 1) * sizeof(char));
            memset(hyphens, 0, (inlen + 1) * sizeof(char));
            lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
            free(hyphens);
            break;
        }
        case 5: {
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            int *outputPos = (int *)malloc(inlen * sizeof(int));
            int *inputPos = (int *)malloc(outlen * sizeof(int));
            int cursorPos = 0;
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            memset(typeform, 0, outlen * sizeof(formtype));
            char *spacing = (char *)malloc((inlen + 1) * sizeof(char));
            memset(spacing, 0, (inlen + 1) * sizeof(char));
            char *inputHyphens = (char *)malloc((inlen + 1) * sizeof(char));
            char *outputHyphens = (char *)malloc((outlen + 1) * sizeof(char));
            memset(inputHyphens, 0, (inlen + 1) * sizeof(char));
            memset(outputHyphens, 0, (outlen + 1) * sizeof(char));
            
            lou_translatePrehyphenated(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, 
                                      typeform, spacing, outputPos, inputPos, &cursorPos,
                                      inputHyphens, outputHyphens, mode);
            
            free(outputPos);
            free(inputPos);
            free(typeform);
            free(spacing);
            free(inputHyphens);
            free(outputHyphens);
            break;
        }
    }

    free(inbuf);
    free(outbuf);
    return 0;
}