// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void init_buffer(widechar *buf, size_t size, const uint8_t *data, size_t data_size) {
    size_t i;
    for (i = 0; i < size && i < data_size; i++) {
        buf[i] = (widechar)(data[i] & 0xFF);
    }
    for (; i < size; i++) {
        buf[i] = 0;
    }
}

static void init_formtype(formtype *buf, size_t size, const uint8_t *data, size_t data_size) {
    size_t i;
    for (i = 0; i < size && i < data_size; i++) {
        buf[i] = (formtype)(data[i] & 0xFF);
    }
    for (; i < size; i++) {
        buf[i] = 0;
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    const size_t buf_size = 256;
    const size_t int_buf_size = 256;
    const size_t char_buf_size = 256;

    widechar *inbuf = malloc(buf_size * sizeof(widechar));
    widechar *outbuf = malloc(buf_size * sizeof(widechar));
    widechar *outbuf2 = malloc(buf_size * sizeof(widechar));
    formtype *typeform = malloc(buf_size * sizeof(formtype));
    char *spacing = malloc(buf_size * sizeof(char));
    int *outputPos = malloc(int_buf_size * sizeof(int));
    int *inputPos = malloc(int_buf_size * sizeof(int));
    int *cursorPos = malloc(sizeof(int));
    char *hyphens = malloc(char_buf_size * sizeof(char));
    char *inputHyphens = malloc(char_buf_size * sizeof(char));
    char *outputHyphens = malloc(char_buf_size * sizeof(char));

    if (!inbuf || !outbuf || !outbuf2 || !typeform || !spacing || 
        !outputPos || !inputPos || !cursorPos || !hyphens || 
        !inputHyphens || !outputHyphens) {
        free(inbuf); free(outbuf); free(outbuf2); free(typeform); free(spacing);
        free(outputPos); free(inputPos); free(cursorPos); free(hyphens);
        free(inputHyphens); free(outputHyphens);
        return 0;
    }

    init_buffer(inbuf, buf_size, Data, Size);
    init_buffer(outbuf, buf_size, Data, Size);
    init_buffer(outbuf2, buf_size, Data, Size);
    init_formtype(typeform, buf_size, Data, Size);
    memset(spacing, 0, buf_size);
    memset(outputPos, 0, int_buf_size * sizeof(int));
    memset(inputPos, 0, int_buf_size * sizeof(int));
    memset(hyphens, 0, char_buf_size);
    memset(inputHyphens, 0, char_buf_size);
    memset(outputHyphens, 0, char_buf_size);

    int inlen = (Size < buf_size) ? (int)Size : (int)buf_size;
    int outlen = (int)buf_size;
    int cursor = 0;
    if (inlen > 0 && Size > 0) {
        cursor = Data[0] % inlen;
    }
    *cursorPos = cursor;

    char tableList[64];
    size_t tableLen = (Size > 60) ? 60 : Size;
    for (size_t i = 0; i < tableLen; i++) {
        if (Data[i] < 32 || Data[i] > 126) tableList[i] = 'x';
        else tableList[i] = (char)Data[i];
    }
    tableList[tableLen] = '\0';
    if (tableLen == 0) strcpy(tableList, "en-gb-gct");

    int mode = (Size > 1) ? Data[1] : 0;

    // 1. lou_dotsToChar
    int len_dots = (Size > 2) ? Data[2] % buf_size : 10;
    if (len_dots > 0) {
        lou_dotsToChar(tableList, inbuf, outbuf, len_dots, mode);
    }

    // 2. lou_charToDots
    int len_chars = (Size > 3) ? Data[3] % buf_size : 10;
    if (len_chars > 0) {
        lou_charToDots(tableList, inbuf, outbuf, len_chars, mode);
    }

    // 3. lou_translate
    inlen = (Size > 4) ? Data[4] % buf_size : 10;
    outlen = buf_size;
    if (inlen > 0 && Size > 5) {
        *cursorPos = Data[5] % inlen;
    } else {
        *cursorPos = 0;
    }
    
    inbuf[inlen] = 0;
    
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, 
                  typeform, spacing, outputPos, inputPos, cursorPos, mode);

    // 4. lou_backTranslate
    inlen = (Size > 6) ? Data[6] % buf_size : 10;
    outlen = buf_size;
    if (inlen > 0 && Size > 7) {
        *cursorPos = Data[7] % inlen;
    } else {
        *cursorPos = 0;
    }
    inbuf[inlen] = 0;
    
    lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, 
                      typeform, spacing, outputPos, inputPos, cursorPos, mode);

    // 5. lou_translatePrehyphenated
    inlen = (Size > 8) ? Data[8] % buf_size : 10;
    outlen = buf_size;
    if (inlen > 0 && Size > 9) {
        *cursorPos = Data[9] % inlen;
    } else {
        *cursorPos = 0;
    }
    inbuf[inlen] = 0;
    
    memset(inputHyphens, 0, char_buf_size);
    memset(outputHyphens, 0, char_buf_size);
    
    size_t hyphen_limit = (inlen < (int)char_buf_size) ? (size_t)inlen : char_buf_size;
    for (size_t i = 0; i < hyphen_limit; i++) {
        if (i < Size) {
            inputHyphens[i] = (Data[i] & 1) ? '-' : ' ';
        } else {
            inputHyphens[i] = ' ';
        }
    }

    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, 
                               typeform, spacing, outputPos, inputPos, cursorPos, 
                               inputHyphens, outputHyphens, mode);

    // 6. lou_hyphenate
    int inlen_hyp = (Size > 10) ? Data[10] % buf_size : 10;
    if (inlen_hyp > 100) inlen_hyp = 100;
    
    memset(hyphens, 0, char_buf_size);
    inbuf[inlen_hyp] = 0;
    
    lou_hyphenate(tableList, inbuf, inlen_hyp, hyphens, mode);

    free(inbuf);
    free(outbuf);
    free(outbuf2);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(hyphens);
    free(inputHyphens);
    free(outputHyphens);

    return 0;
}