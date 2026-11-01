// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void init_widechar_buffer(const uint8_t *data, size_t size, widechar *buf, int *len) {
    int max_len = 1000; 
    if (size > (size_t)max_len) size = max_len;
    
    *len = 0;
    for (size_t i = 0; i < size; i++) {
        buf[i] = (widechar)data[i];
        (*len)++;
    }
    if (*len < 1000) buf[*len] = 0;
}

static void init_char_buffer(const uint8_t *data, size_t size, char *buf, int *len) {
    int max_len = 1000;
    if (size > (size_t)max_len) size = max_len;
    
    *len = 0;
    for (size_t i = 0; i < size; i++) {
        buf[i] = (char)data[i];
        (*len)++;
    }
    if (*len < 1000) buf[*len] = 0;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate buffers
    widechar inbuf[1000] = {0};
    widechar outbuf[2000] = {0};
    char hyphens[1000] = {0};
    char spacing[1000] = {0};
    char inputHyphens[1000] = {0};
    char outputHyphens[1000] = {0};
    formtype typeform[1000] = {0};
    int outputPos[1000] = {0};
    int inputPos[2000] = {0};
    int cursorPos = 0;
    
    int inlen = 0;
    int outlen = 0;
    int inlen_copy = 0;
    int outlen_copy = 0;

    // Initialize buffers from fuzz data
    init_widechar_buffer(Data, Size, inbuf, &inlen);
    init_char_buffer(Data, Size, hyphens, &inlen);
    init_char_buffer(Data, Size, spacing, &inlen);
    init_char_buffer(Data, Size, inputHyphens, &inlen);
    init_char_buffer(Data, Size, outputHyphens, &inlen);
    
    // Initialize typeform
    for (int i = 0; i < inlen; i++) {
        typeform[i] = (formtype)(Data[i] & 0xFF);
    }

    // Prepare table list safely
    // We use a fixed valid table name to avoid heap buffer overflow from fuzz data
    // when the library tries to parse the table list string.
    const char *tableList = "en-ga-8";

    // 1. Test lou_compileString
    char compileStr[1000];
    init_char_buffer(Data, Size, compileStr, &inlen);
    // Ensure compileStr is null-terminated for safety
    compileStr[999] = '\0';
    lou_compileString(tableList, compileStr);

    // 2. Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    // We do not free emphClasses here to avoid complexity and potential double-free issues
    // as the allocation strategy is not fully specified for the fuzzer context.

    // 3. Test lou_hyphenate
    int hyphenMode = 0;
    if (Size > 0) {
        hyphenMode = (Data[0] % 2);
    }
    lou_hyphenate(tableList, inbuf, inlen, hyphens, hyphenMode);

    // 4. Test lou_translate
    inlen_copy = inlen;
    outlen_copy = sizeof(outbuf) / sizeof(widechar);
    int mode = 0;
    if (Size > 1) {
        mode = (Data[1] % 4);
    }
    lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // 5. Test lou_backTranslate
    inlen_copy = inlen;
    outlen_copy = sizeof(outbuf) / sizeof(widechar);
    lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // 6. Test lou_translatePrehyphenated
    inlen_copy = inlen;
    outlen_copy = sizeof(outbuf) / sizeof(widechar);
    lou_translatePrehyphenated(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, typeform, spacing, outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, mode);

    return 0;
}