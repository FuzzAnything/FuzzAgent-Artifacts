// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void convert_utf8_to_widechar(const uint8_t *data, size_t size, widechar *outbuf, int *outlen) {
    int i = 0;
    int j = 0;
    while (i < (int)size && j < 1000) {
        unsigned char c = data[i];
        if (c < 0x80) {
            outbuf[j++] = (widechar)c;
            i++;
        } else if ((c & 0xE0) == 0xC0) {
            if (i + 1 < (int)size) {
                outbuf[j++] = (widechar)((c & 0x1F) << 6 | (data[i + 1] & 0x3F));
                i += 2;
            } else {
                i++;
            }
        } else if ((c & 0xF0) == 0xE0) {
            if (i + 2 < (int)size) {
                outbuf[j++] = (widechar)((c & 0x0F) << 12 | (data[i + 1] & 0x3F) << 6 | (data[i + 2] & 0x3F));
                i += 3;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
    *outlen = j;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    widechar inbuf[1024];
    widechar outbuf[1024];
    widechar outbuf2[1024];
    int inlen = 0, outlen = 0, outlen2 = 0;
    formtype typeform[1024];
    char spacing[1024];
    int outputPos[1024];
    int inputPos[1024];
    int cursorPos = 0;
    char hyphens[1024];
    char inputHyphens[1024];
    char outputHyphens[1024];

    convert_utf8_to_widechar(Data, Size, inbuf, &inlen);

    // Limit input length to prevent excessive memory usage
    if (inlen > 1000) inlen = 1000;

    // Initialize buffers
    memset(outbuf, 0, sizeof(outbuf));
    memset(outbuf2, 0, sizeof(outbuf2));
    memset(typeform, 0, sizeof(typeform));
    memset(spacing, 0, sizeof(spacing));
    memset(outputPos, 0, sizeof(outputPos));
    memset(inputPos, 0, sizeof(inputPos));
    memset(hyphens, 0, sizeof(hyphens));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));

    // Prepare table list (use a common default table)
    const char *tableList = "en-gb-8.dot";

    // Test lou_translate
    outlen = 1024;
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, 0);

    // Test lou_backTranslate
    int backInLen = outlen;
    int backOutLen = 1024;
    lou_backTranslate(tableList, outbuf, &backInLen, outbuf2, &backOutLen, typeform, spacing, outputPos, inputPos, &cursorPos, 0);

    // Test lou_charToDots
    int charToDotsLen = inlen;
    lou_charToDots(tableList, inbuf, outbuf, charToDotsLen, 0);

    // Test lou_dotsToChar
    int dotsToCharLen = inlen;
    lou_dotsToChar(tableList, inbuf, outbuf, dotsToCharLen, 0);

    // Test lou_hyphenate (text mode)
    memset(hyphens, 0, sizeof(hyphens));
    lou_hyphenate(tableList, inbuf, inlen, hyphens, 0);

    // Test lou_hyphenate (braille mode)
    memset(hyphens, 0, sizeof(hyphens));
    lou_hyphenate(tableList, inbuf, inlen, hyphens, 1);

    // Test lou_translatePrehyphenated
    int preHyphInLen = inlen;
    int preHyphOutLen = 1024;
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));
    // Fill inputHyphens with some hyphens
    for (int i = 0; i < inlen; i++) {
        inputHyphens[i] = (i % 5 == 0) ? '-' : ' ';
    }
    lou_translatePrehyphenated(tableList, inbuf, &preHyphInLen, outbuf, &preHyphOutLen, typeform, spacing, outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, 0);

    return 0;
}