// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <liblouis.h>

static void
init_widechar_buffer(const uint8_t *data, size_t size, widechar *buf, int *len) {
    size_t max_len = size / sizeof(widechar);
    if (max_len > 1000) max_len = 1000;
    *len = 0;
    if (max_len == 0) return;

    for (size_t i = 0; i < max_len; i++) {
        buf[i] = (widechar)data[i];
        (*len)++;
    }
}

static void
init_formtype_buffer(const uint8_t *data, size_t size, formtype *buf, int len) {
    for (int i = 0; i < len; i++) {
        if (i < size) {
            buf[i] = (formtype)data[i];
        } else {
            buf[i] = 0;
        }
    }
}

static void
init_char_buffer(const uint8_t *data, size_t size, char *buf, int len) {
    for (int i = 0; i < len; i++) {
        if (i < size) {
            buf[i] = (char)data[i];
        } else {
            buf[i] = 0;
        }
    }
}

static void
init_int_buffer(const uint8_t *data, size_t size, int *buf, int len) {
    for (int i = 0; i < len; i++) {
        if (i < size) {
            buf[i] = data[i];
        } else {
            buf[i] = 0;
        }
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Allocate buffers
    const int MAX_LEN = 1024;
    widechar inbuf[MAX_LEN];
    widechar outbuf[MAX_LEN];
    widechar outbuf2[MAX_LEN];
    formtype typeform[MAX_LEN];
    char spacing[MAX_LEN];
    char hyphens[MAX_LEN];
    char inputHyphens[MAX_LEN];
    char outputHyphens[MAX_LEN];
    int outputPos[MAX_LEN];
    int inputPos[MAX_LEN];
    int cursorPos = 0;
    int inlen = 0;
    int outlen = MAX_LEN;
    int outlen2 = MAX_LEN;

    // Initialize buffers from fuzz data
    init_widechar_buffer(Data, Size, inbuf, &inlen);
    if (inlen == 0) return 0;

    // Ensure output buffers are zeroed
    memset(outbuf, 0, sizeof(outbuf));
    memset(outbuf2, 0, sizeof(outbuf2));
    memset(hyphens, 0, sizeof(hyphens));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));

    // Initialize auxiliary buffers
    init_formtype_buffer(Data, Size, typeform, inlen);
    init_char_buffer(Data, Size, spacing, inlen);
    init_int_buffer(Data, Size, outputPos, inlen);
    init_int_buffer(Data, Size, inputPos, MAX_LEN);

    // Table list: use a simple valid table name or empty to test error handling
    const char *tableList = "en-gb-8";
    // Fuzz tableList by trying different prefixes if data allows
    if (Size > 20) {
        static char customTable[64];
        size_t copyLen = Size > 63 ? 63 : Size;
        memcpy(customTable, Data, copyLen);
        customTable[copyLen] = '\0';
        tableList = customTable;
    }

    int mode = (Size > 1) ? (Data[0] % 2) : 0;

    // 1. lou_hyphenate
    memset(hyphens, 0, sizeof(hyphens));
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);

    // 2. lou_translatePrehyphenated
    memset(outputHyphens, 0, sizeof(outputHyphens));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    // Prepare inputHyphens with some data
    for (int i = 0; i < inlen && i < Size; i++) {
        inputHyphens[i] = Data[i] % 2 ? '-' : ' ';
    }
    int inlen_copy = inlen;
    int outlen_copy = MAX_LEN;
    int cursorPos_copy = 0;
    lou_translatePrehyphenated(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                               typeform, spacing, outputPos, inputPos, &cursorPos_copy,
                               inputHyphens, outputHyphens, mode);

    // 3. lou_dotsToChar
    memset(outbuf2, 0, sizeof(outbuf2));
    int length_copy = inlen;
    lou_dotsToChar(tableList, inbuf, outbuf2, length_copy, mode);

    // 4. lou_translate
    memset(outbuf, 0, sizeof(outbuf));
    int inlen_copy2 = inlen;
    int outlen_copy2 = MAX_LEN;
    int cursorPos_copy2 = 0;
    lou_translate(tableList, inbuf, &inlen_copy2, outbuf, &outlen_copy2,
                  typeform, spacing, outputPos, inputPos, &cursorPos_copy2, mode);

    // 5. lou_charToDots
    memset(outbuf2, 0, sizeof(outbuf2));
    int length_copy2 = inlen;
    lou_charToDots(tableList, inbuf, outbuf2, length_copy2, mode);

    // 6. lou_backTranslate
    memset(outbuf, 0, sizeof(outbuf));
    int inlen_copy3 = inlen;
    int outlen_copy3 = MAX_LEN;
    int cursorPos_copy3 = 0;
    lou_backTranslate(tableList, inbuf, &inlen_copy3, outbuf, &outlen_copy3,
                      typeform, spacing, outputPos, inputPos, &cursorPos_copy3, mode);

    return 0;
}