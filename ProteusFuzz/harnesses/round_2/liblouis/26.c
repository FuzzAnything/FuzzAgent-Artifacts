// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
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

static void fuzz_lou_getEmphClasses(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    char const **classes = lou_getEmphClasses(tableList);
    if (classes) {
        lou_freeEmphClasses(classes);
    }
    free(tableList);
}

static void fuzz_lou_getTypeformForEmphClass(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    size_t split = Size / 2;
    char *tableList = (char *)malloc(split + 1);
    char *emphClass = (char *)malloc(Size - split + 1);
    if (!tableList || !emphClass) {
        free(tableList);
        free(emphClass);
        return;
    }
    memcpy(tableList, Data, split);
    tableList[split] = '\0';
    memcpy(emphClass, Data + split, Size - split);
    emphClass[Size - split] = '\0';

    formtype result = lou_getTypeformForEmphClass(tableList, emphClass);
    (void)result;

    free(tableList);
    free(emphClass);
}

static void fuzz_lou_translate(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;

    size_t tableSize = Size / 4;
    size_t inSize = Size / 2;
    if (tableSize == 0) tableSize = 1;
    if (inSize == 0) inSize = 1;

    char *tableList = (char *)malloc(tableSize + 1);
    widechar *inbuf = (widechar *)malloc(inSize * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc((Size + 100) * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(inSize * sizeof(formtype));
    char *spacing = (char *)malloc(inSize * sizeof(char));
    int *outputPos = (int *)malloc(inSize * sizeof(int));
    int *inputPos = (int *)malloc((Size + 100) * sizeof(int));

    if (!tableList || !inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        return;
    }

    memcpy(tableList, Data, tableSize);
    tableList[tableSize] = '\0';

    for (size_t i = 0; i < inSize; i++) {
        inbuf[i] = (widechar)Data[tableSize + i];
        typeform[i] = (formtype)Data[tableSize + i];
        spacing[i] = (char)Data[tableSize + i];
        outputPos[i] = (int)Data[tableSize + i];
    }

    int inlen = (int)inSize;
    int outlen = (int)(Size + 100);
    int cursorPos = (int)Data[tableSize];
    int mode = (int)Data[tableSize + 1];

    int result = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos,
                               &cursorPos, mode);
    (void)result;

    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
}

static void fuzz_lou_backTranslate(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;

    size_t tableSize = Size / 4;
    size_t inSize = Size / 2;
    if (tableSize == 0) tableSize = 1;
    if (inSize == 0) inSize = 1;

    char *tableList = (char *)malloc(tableSize + 1);
    widechar *inbuf = (widechar *)malloc(inSize * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc((Size + 100) * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(inSize * sizeof(formtype));
    char *spacing = (char *)malloc(inSize * sizeof(char));
    int *outputPos = (int *)malloc(inSize * sizeof(int));
    int *inputPos = (int *)malloc((Size + 100) * sizeof(int));

    if (!tableList || !inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        return;
    }

    memcpy(tableList, Data, tableSize);
    tableList[tableSize] = '\0';

    for (size_t i = 0; i < inSize; i++) {
        inbuf[i] = (widechar)Data[tableSize + i];
        typeform[i] = (formtype)Data[tableSize + i];
        spacing[i] = (char)Data[tableSize + i];
        outputPos[i] = (int)Data[tableSize + i];
    }

    int inlen = (int)inSize;
    int outlen = (int)(Size + 100);
    int cursorPos = (int)Data[tableSize];
    int mode = (int)Data[tableSize + 1];

    int result = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                                   typeform, spacing, outputPos, inputPos,
                                   &cursorPos, mode);
    (void)result;

    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
}

static void fuzz_lou_getTable(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    const void *table = lou_getTable(tableList);
    (void)table;

    free(tableList);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_lou_getEmphClasses(Data, Size);
    fuzz_lou_getTypeformForEmphClass(Data, Size);
    fuzz_lou_translate(Data, Size);
    fuzz_lou_backTranslate(Data, Size);
    fuzz_lou_getTable(Data, Size);

    return 0;
}