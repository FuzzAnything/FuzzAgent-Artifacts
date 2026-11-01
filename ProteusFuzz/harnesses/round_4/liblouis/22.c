// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
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

static void cleanup_buffers(widechar *inbuf, widechar *outbuf, int *inlen, int *outlen,
                            formtype *typeform, char *spacing, int *outputPos, int *inputPos) {
    if (inbuf) free(inbuf);
    if (outbuf) free(outbuf);
    if (inlen) free(inlen);
    if (outlen) free(outlen);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into parts for different parameters
    size_t tableLen = Size / 8;
    if (tableLen > 256) tableLen = 256;
    if (tableLen < 1) tableLen = 1;

    size_t inbufLen = (Size - tableLen) / 4;
    if (inbufLen > 1024) inbufLen = 1024;
    if (inbufLen < 1) inbufLen = 1;

    // Create table name list (null-terminated)
    char *tableList = malloc(tableLen + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Create input buffer
    widechar *inbuf = malloc(inbufLen * sizeof(widechar));
    if (!inbuf) { free(tableList); return 0; }
    for (size_t i = 0; i < inbufLen; i++) {
        inbuf[i] = Data[tableLen + i] & 0xFF;
    }

    // Create output buffer (larger to handle expansion)
    int outbufLen = inbufLen * 2;
    widechar *outbuf = malloc(outbufLen * sizeof(widechar));
    if (!outbuf) { free(tableList); free(inbuf); return 0; }

    // Create length pointers
    int *inlen = malloc(sizeof(int));
    int *outlen = malloc(sizeof(int));
    if (!inlen || !outlen) {
        free(tableList); free(inbuf); free(outbuf);
        if (inlen) free(inlen);
        if (outlen) free(outlen);
        return 0;
    }
    *inlen = inbufLen;
    *outlen = outbufLen;

    // Create optional parameters
    formtype *typeform = malloc(inbufLen * sizeof(formtype));
    char *spacing = malloc(inbufLen * sizeof(char));
    int *outputPos = malloc(inbufLen * sizeof(int));
    int *inputPos = malloc(outbufLen * sizeof(int));
    int cursorPos = 0;

    if (!typeform || !spacing || !outputPos || !inputPos) {
        cleanup_buffers(inbuf, outbuf, inlen, outlen, typeform, spacing, outputPos, inputPos);
        free(tableList);
        return 0;
    }

    // Initialize optional arrays
    for (int i = 0; i < inbufLen; i++) {
        typeform[i] = 0;
        spacing[i] = 0;
        outputPos[i] = i;
    }
    for (int i = 0; i < outbufLen; i++) {
        inputPos[i] = i;
    }

    // Test lou_translate
    int mode = 0;
    lou_translate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // Test lou_translateString
    widechar *inbuf2 = malloc(inbufLen * sizeof(widechar));
    widechar *outbuf2 = malloc(outbufLen * sizeof(widechar));
    int inlen2 = inbufLen, outlen2 = outbufLen;
    if (inbuf2 && outbuf2) {
        memcpy(inbuf2, inbuf, inbufLen * sizeof(widechar));
        lou_translateString(tableList, inbuf2, &inlen2, outbuf2, &outlen2, typeform, spacing, mode);
        free(inbuf2);
        free(outbuf2);
    }

    // Test lou_dotsToChar
    widechar *dotsIn = malloc(inbufLen * sizeof(widechar));
    widechar *dotsOut = malloc(inbufLen * sizeof(widechar));
    if (dotsIn && dotsOut) {
        memcpy(dotsIn, inbuf, inbufLen * sizeof(widechar));
        lou_dotsToChar(tableList, dotsIn, dotsOut, inbufLen, mode);
        free(dotsIn);
        free(dotsOut);
    }

    // Test lou_charToDots
    widechar *charIn = malloc(inbufLen * sizeof(widechar));
    widechar *charOut = malloc(inbufLen * sizeof(widechar));
    if (charIn && charOut) {
        memcpy(charIn, inbuf, inbufLen * sizeof(widechar));
        lou_charToDots(tableList, charIn, charOut, inbufLen, mode);
        free(charIn);
        free(charOut);
    }

    // Test lou_getTable
    const void *table = lou_getTable(tableList);
    (void)table; // Suppress unused variable warning

    // Test lou_indexTables
    const char *tables[2];
    tables[0] = tableList;
    tables[1] = NULL;
    lou_indexTables(tables);

    // Cleanup
    cleanup_buffers(inbuf, outbuf, inlen, outlen, typeform, spacing, outputPos, inputPos);
    free(tableList);

    return 0;
}