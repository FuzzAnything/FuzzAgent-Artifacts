// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void init_widechar_buffer(const uint8_t *data, size_t size, widechar *buf, int *len) {
    int max_len = size;
    if (max_len > 10000) max_len = 10000;
    *len = 0;
    for (int i = 0; i < max_len; i++) {
        if (i + 1 < size) {
            buf[*len] = (widechar)(data[i] | (data[i+1] << 8));
            i++;
        } else {
            buf[*len] = (widechar)data[i];
        }
        (*len)++;
    }
}

static void init_formtype_buffer(const uint8_t *data, size_t size, formtype *buf, int *len) {
    int max_len = size;
    if (max_len > 10000) max_len = 10000;
    *len = 0;
    for (int i = 0; i < max_len; i++) {
        buf[*len] = (formtype)(data[i] & 0xFF);
        (*len)++;
    }
}

static void init_int_buffer(const uint8_t *data, size_t size, int *buf, int *len) {
    int max_len = size / sizeof(int);
    if (max_len > 10000) max_len = 10000;
    *len = 0;
    for (int i = 0; i < max_len; i++) {
        if (i * sizeof(int) + sizeof(int) <= size) {
            buf[*len] = *(int*)(data + i * sizeof(int));
        } else {
            buf[*len] = (int)data[i * sizeof(int)];
        }
        (*len)++;
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    const int MAX_BUF = 4096;
    widechar inbuf[MAX_BUF] = {0};
    widechar outbuf[MAX_BUF] = {0};
    formtype typeform[MAX_BUF] = {0};
    char spacing[MAX_BUF] = {0};
    int outputPos[MAX_BUF] = {0};
    int inputPos[MAX_BUF] = {0};
    char hyphens[MAX_BUF] = {0};
    char inputHyphens[MAX_BUF] = {0};
    char outputHyphens[MAX_BUF] = {0};

    int inlen = 0;
    int outlen = MAX_BUF;
    int cursorPos = 0;
    int mode = 0;

    init_widechar_buffer(Data, Size, inbuf, &inlen);
    init_formtype_buffer(Data, Size, typeform, &inlen);
    init_int_buffer(Data, Size, outputPos, &inlen);
    init_int_buffer(Data, Size, inputPos, &inlen);
    cursorPos = (Size > 0) ? (int)Data[0] : 0;
    mode = (Size > 1) ? (int)Data[1] : 0;

    const char *tableList = "en-gb-8dot";
    
    // Fuzz lou_translate
    if (inlen > 0) {
        int tempInLen = inlen;
        int tempOutLen = MAX_BUF;
        int tempCursor = cursorPos;
        lou_translate(tableList, inbuf, &tempInLen, outbuf, &tempOutLen,
                      typeform, spacing, outputPos, inputPos, &tempCursor, mode);
    }

    // Fuzz lou_hyphenate
    if (inlen > 0) {
        lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    }

    // Fuzz lou_dotsToChar
    if (inlen > 0) {
        lou_dotsToChar(tableList, inbuf, outbuf, inlen, mode);
    }

    // Fuzz lou_translatePrehyphenated
    if (inlen > 0) {
        int tempInLen = inlen;
        int tempOutLen = MAX_BUF;
        int tempCursor = cursorPos;
        lou_translatePrehyphenated(tableList, inbuf, &tempInLen, outbuf, &tempOutLen,
                                   typeform, spacing, outputPos, inputPos, &tempCursor,
                                   inputHyphens, outputHyphens, mode);
    }

    // Fuzz lou_charToDots
    if (inlen > 0) {
        lou_charToDots(tableList, inbuf, outbuf, inlen, mode);
    }

    // Fuzz lou_backTranslate
    if (inlen > 0) {
        int tempInLen = inlen;
        int tempOutLen = MAX_BUF;
        int tempCursor = cursorPos;
        lou_backTranslate(tableList, inbuf, &tempInLen, outbuf, &tempOutLen,
                          typeform, spacing, outputPos, inputPos, &tempCursor, mode);
    }

    return 0;
}