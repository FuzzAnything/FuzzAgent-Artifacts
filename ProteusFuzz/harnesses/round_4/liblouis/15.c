// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
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

#define MAX_BUFFER_SIZE 4096

static void fuzz_lou_getEmphClasses(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Create a null-terminated string from input data
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';
    
    char const **result = lou_getEmphClasses(tableList);
    
    // Free the result if not NULL
    if (result != NULL) {
        size_t i = 0;
        while (result[i] != NULL) {
            free((char *)result[i]);
            i++;
        }
        free((char *)result);
    }
    
    free(tableList);
}

static void fuzz_lou_translate(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Split input: first byte for table list length, rest for data
    size_t tableLen = Data[0] % (Size / 2 + 1);
    if (tableLen == 0) tableLen = 1;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data + 1, tableLen);
    tableList[tableLen] = '\0';
    
    size_t dataLen = Size - 1 - tableLen;
    if (dataLen == 0) {
        free(tableList);
        return;
    }
    
    // Prepare input buffer
    widechar *inbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    int inlen = dataLen;
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    for (size_t i = 0; i < dataLen; i++) {
        inbuf[i] = (widechar)(Data[1 + tableLen + i] & 0xFF);
    }
    
    // Prepare output buffer
    widechar *outbuf = (widechar *)malloc(MAX_BUFFER_SIZE * sizeof(widechar));
    int outlen = MAX_BUFFER_SIZE;
    if (!outbuf) {
        free(inbuf);
        free(tableList);
        return;
    }
    
    // Prepare auxiliary buffers
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int *inputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int cursorPos = 0;
    
    if (!outputPos || !inputPos) {
        free(outputPos);
        free(inputPos);
        free(outbuf);
        free(inbuf);
        free(tableList);
        return;
    }
    
    int mode = Data[dataLen] % 4; // Use last byte for mode
    
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, 
                  outputPos, inputPos, &cursorPos, mode);
    
    free(outputPos);
    free(inputPos);
    free(outbuf);
    free(inbuf);
    free(tableList);
}

static void fuzz_lou_hyphenate(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    size_t tableLen = Data[0] % (Size / 2 + 1);
    if (tableLen == 0) tableLen = 1;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data + 1, tableLen);
    tableList[tableLen] = '\0';
    
    size_t dataLen = Size - 1 - tableLen;
    if (dataLen == 0) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    for (size_t i = 0; i < dataLen; i++) {
        inbuf[i] = (widechar)(Data[1 + tableLen + i] & 0xFF);
    }
    
    char *hyphens = (char *)malloc(dataLen + 1);
    if (!hyphens) {
        free(inbuf);
        free(tableList);
        return;
    }
    
    int mode = Data[dataLen] % 2;
    
    lou_hyphenate(tableList, inbuf, dataLen, hyphens, mode);
    
    free(hyphens);
    free(inbuf);
    free(tableList);
}

static void fuzz_lou_dotsToChar(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    size_t tableLen = Data[0] % (Size / 2 + 1);
    if (tableLen == 0) tableLen = 1;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data + 1, tableLen);
    tableList[tableLen] = '\0';
    
    size_t dataLen = Size - 1 - tableLen;
    if (dataLen == 0) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        free(tableList);
        return;
    }
    
    for (size_t i = 0; i < dataLen; i++) {
        inbuf[i] = (widechar)(Data[1 + tableLen + i] & 0xFF);
    }
    
    int mode = Data[dataLen] % 2;
    
    lou_dotsToChar(tableList, inbuf, outbuf, dataLen, mode);
    
    free(outbuf);
    free(inbuf);
    free(tableList);
}

static void fuzz_lou_translatePrehyphenated(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    size_t tableLen = Data[0] % (Size / 2 + 1);
    if (tableLen == 0) tableLen = 1;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data + 1, tableLen);
    tableList[tableLen] = '\0';
    
    size_t dataLen = Size - 1 - tableLen;
    if (dataLen == 0) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    int inlen = dataLen;
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    for (size_t i = 0; i < dataLen; i++) {
        inbuf[i] = (widechar)(Data[1 + tableLen + i] & 0xFF);
    }
    
    widechar *outbuf = (widechar *)malloc(MAX_BUFFER_SIZE * sizeof(widechar));
    int outlen = MAX_BUFFER_SIZE;
    if (!outbuf) {
        free(inbuf);
        free(tableList);
        return;
    }
    
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int *inputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int cursorPos = 0;
    char *inputHyphens = (char *)malloc(dataLen + 1);
    char *outputHyphens = (char *)malloc(MAX_BUFFER_SIZE + 1);
    
    if (!outputPos || !inputPos || !inputHyphens || !outputHyphens) {
        free(outputPos);
        free(inputPos);
        free(inputHyphens);
        free(outputHyphens);
        free(outbuf);
        free(inbuf);
        free(tableList);
        return;
    }
    
    memset(inputHyphens, 0, dataLen + 1);
    memset(outputHyphens, 0, MAX_BUFFER_SIZE + 1);
    
    int mode = Data[dataLen] % 4;
    
    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, 
                               outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, mode);
    
    free(outputHyphens);
    free(inputHyphens);
    free(inputPos);
    free(outputPos);
    free(outbuf);
    free(inbuf);
    free(tableList);
}

static void fuzz_lou_backTranslate(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    size_t tableLen = Data[0] % (Size / 2 + 1);
    if (tableLen == 0) tableLen = 1;
    
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) return;
    
    memcpy(tableList, Data + 1, tableLen);
    tableList[tableLen] = '\0';
    
    size_t dataLen = Size - 1 - tableLen;
    if (dataLen == 0) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(dataLen * sizeof(widechar));
    int inlen = dataLen;
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    for (size_t i = 0; i < dataLen; i++) {
        inbuf[i] = (widechar)(Data[1 + tableLen + i] & 0xFF);
    }
    
    widechar *outbuf = (widechar *)malloc(MAX_BUFFER_SIZE * sizeof(widechar));
    int outlen = MAX_BUFFER_SIZE;
    if (!outbuf) {
        free(inbuf);
        free(tableList);
        return;
    }
    
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int *inputPos = (int *)malloc(MAX_BUFFER_SIZE * sizeof(int));
    int cursorPos = 0;
    
    if (!outputPos || !inputPos) {
        free(outputPos);
        free(inputPos);
        free(outbuf);
        free(inbuf);
        free(tableList);
        return;
    }
    
    int mode = Data[dataLen] % 4;
    
    lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, 
                      outputPos, inputPos, &cursorPos, mode);
    
    free(inputPos);
    free(outputPos);
    free(outbuf);
    free(inbuf);
    free(tableList);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Rotate through different API functions based on input
    int funcIndex = Data[0] % 6;
    
    switch (funcIndex) {
        case 0:
            fuzz_lou_getEmphClasses(Data, Size);
            break;
        case 1:
            fuzz_lou_translate(Data, Size);
            break;
        case 2:
            fuzz_lou_hyphenate(Data, Size);
            break;
        case 3:
            fuzz_lou_dotsToChar(Data, Size);
            break;
        case 4:
            fuzz_lou_translatePrehyphenated(Data, Size);
            break;
        case 5:
            fuzz_lou_backTranslate(Data, Size);
            break;
    }
    
    return 0;
}