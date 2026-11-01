// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <liblouis.h>

static void init_widechar_buffer(const uint8_t *data, size_t size, widechar *buf, int *len) {
    if (size == 0) {
        *len = 0;
        return;
    }
    size_t limit = (size > 1000) ? 1000 : size;
    *len = 0;
    for (size_t i = 0; i < limit; i++) {
        buf[i] = (widechar)data[i];
        (*len)++;
    }
}

static void init_formtype_buffer(const uint8_t *data, size_t size, formtype *buf, int len) {
    if (buf == NULL || len <= 0) return;
    for (int i = 0; i < len; i++) {
        buf[i] = (formtype)(data[i % size]);
    }
}

static void init_spacing_buffer(const uint8_t *data, size_t size, char *buf, int len) {
    if (buf == NULL || len <= 0) return;
    for (int i = 0; i < len; i++) {
        buf[i] = (char)(data[i % size]);
    }
}

static void init_int_buffer(int *buf, int len) {
    if (buf == NULL || len <= 0) return;
    for (int i = 0; i < len; i++) {
        buf[i] = i;
    }
}

static void init_char_buffer(const uint8_t *data, size_t size, char *buf, int len) {
    if (buf == NULL || len <= 0) return;
    for (int i = 0; i < len; i++) {
        buf[i] = (char)(data[i % size]);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *tableList = "en-gb";
    const int maxLen = 1000;
    
    widechar inbuf[maxLen];
    widechar outbuf[maxLen];
    formtype typeform[maxLen];
    char spacing[maxLen];
    int outputPos[maxLen];
    int inputPos[maxLen];
    int cursorPos = 0;
    char hyphens[maxLen];
    char inputHyphens[maxLen];
    char outputHyphens[maxLen];

    int inlen = 0;
    int outlen = maxLen;

    init_widechar_buffer(Data, Size, inbuf, &inlen);
    if (inlen == 0) return 0;

    outlen = maxLen;
    memset(outbuf, 0, sizeof(outbuf));
    memset(typeform, 0, sizeof(typeform));
    memset(spacing, 0, sizeof(spacing));
    memset(outputPos, 0, sizeof(outputPos));
    memset(inputPos, 0, sizeof(inputPos));
    memset(hyphens, 0, sizeof(hyphens));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));

    init_formtype_buffer(Data, Size, typeform, inlen);
    init_spacing_buffer(Data, Size, spacing, inlen);
    init_int_buffer(outputPos, inlen);
    init_int_buffer(inputPos, maxLen);
    init_char_buffer(Data, Size, hyphens, inlen);
    init_char_buffer(Data, Size, inputHyphens, inlen);
    init_char_buffer(Data, Size, outputHyphens, maxLen);

    int mode = (Data[0] % 3);

    // Fuzz lou_translate
    outlen = maxLen;
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // Fuzz lou_hyphenate
    memset(hyphens, 0, sizeof(hyphens));
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);

    // Fuzz lou_dotsToChar
    memset(outbuf, 0, sizeof(outbuf));
    lou_dotsToChar(tableList, inbuf, outbuf, inlen, mode);

    // Fuzz lou_translatePrehyphenated
    inlen = 0;
    init_widechar_buffer(Data, Size, inbuf, &inlen);
    if (inlen > 0) {
        outlen = maxLen;
        memset(outputHyphens, 0, sizeof(outputHyphens));
        lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, mode);
    }

    // Fuzz lou_charToDots
    memset(outbuf, 0, sizeof(outbuf));
    lou_charToDots(tableList, inbuf, outbuf, inlen, mode);

    // Fuzz lou_backTranslate
    inlen = 0;
    init_widechar_buffer(Data, Size, inbuf, &inlen);
    if (inlen > 0) {
        outlen = maxLen;
        lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }

    return 0;
}