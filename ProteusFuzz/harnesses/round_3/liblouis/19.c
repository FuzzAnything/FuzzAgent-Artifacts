// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
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

static void safe_free(char const **ptr) {
    if (ptr) {
        for (int i = 0; ptr[i] != NULL; i++) {
            free((void *)ptr[i]);
        }
        free((void *)ptr);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create a buffer for table list (ensure null termination)
    size_t tableListSize = Size / 2;
    if (tableListSize > 256) tableListSize = 256;
    char tableList[257];
    memcpy(tableList, Data, tableListSize);
    tableList[tableListSize] = '\0';

    // Create input buffer for widechar
    size_t inbufSize = Size - tableListSize;
    if (inbufSize > 1000) inbufSize = 1000;
    widechar *inbuf = (widechar *)malloc(inbufSize * sizeof(widechar));
    if (!inbuf) return 0;
    
    // Convert input bytes to widechar (simple mapping)
    for (size_t i = 0; i < inbufSize; i++) {
        inbuf[i] = Data[tableListSize + i];
    }
    int inlen = inbufSize;

    // Output buffer
    widechar *outbuf = (widechar *)malloc(inbufSize * 2 * sizeof(widechar));
    if (!outbuf) { free(inbuf); return 0; }
    int outlen = inbufSize * 2;

    // Helper arrays
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    int *cursorPosPtr = &cursorPos;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;

    // Allocate optional arrays if size permits
    if (inlen > 0) {
        typeform = (formtype *)malloc(inlen * sizeof(formtype));
        spacing = (char *)malloc(inlen * sizeof(char));
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        inputHyphens = (char *)malloc(inlen * sizeof(char));
        outputHyphens = (char *)malloc(outlen * sizeof(char));

        if (typeform && spacing && outputPos && inputPos && inputHyphens && outputHyphens) {
            memset(typeform, 0, inlen * sizeof(formtype));
            memset(spacing, 0, inlen * sizeof(char));
            memset(outputPos, 0, inlen * sizeof(int));
            memset(inputPos, 0, outlen * sizeof(int));
            memset(inputHyphens, 0, inlen * sizeof(char));
            memset(outputHyphens, 0, outlen * sizeof(char));
        } else {
            free(typeform); free(spacing); free(outputPos); free(inputPos);
            free(inputHyphens); free(outputHyphens);
            typeform = spacing = outputPos = inputPos = inputHyphens = outputHyphens = NULL;
        }
    }

    // Fuzz lou_translate
    if (inbuf && outbuf) {
        int mode = Data[0] & 0xFF;
        lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, cursorPosPtr, mode);
    }

    // Fuzz lou_backTranslate
    if (inbuf && outbuf) {
        int mode = Data[1] & 0xFF;
        // Reset lengths for back translation
        inlen = inbufSize;
        outlen = inbufSize * 2;
        lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, cursorPosPtr, mode);
    }

    // Fuzz lou_translatePrehyphenated
    if (inbuf && outbuf && inputHyphens && outputHyphens) {
        int mode = Data[0] & 0xFF;
        inlen = inbufSize;
        outlen = inbufSize * 2;
        // Ensure inputPos is monotonic-ish by filling with increasing values
        for (int i = 0; i < outlen; i++) inputPos[i] = i;
        lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, cursorPosPtr, inputHyphens, outputHyphens, mode);
    }

    // Fuzz lou_hyphenate
    if (inbuf) {
        char *hyphens = (char *)malloc(inbufSize * sizeof(char));
        if (hyphens) {
            memset(hyphens, 0, inbufSize * sizeof(char));
            int mode = Data[0] & 0xFF;
            lou_hyphenate(tableList, inbuf, inbufSize, hyphens, mode);
            free(hyphens);
        }
    }

    // Fuzz lou_dotsToChar
    if (inbuf && outbuf) {
        int length = inbufSize;
        int mode = Data[0] & 0xFF;
        lou_dotsToChar(tableList, inbuf, outbuf, length, mode);
    }

    // Fuzz lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    safe_free(emphClasses);

    // Cleanup
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