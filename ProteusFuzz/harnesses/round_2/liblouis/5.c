// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void free_emph_classes(char const **classes) {
    if (classes) {
        for (int i = 0; classes[i] != NULL; i++) {
            free((void *)classes[i]);
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a null-terminated string from fuzz data for tableList
    char *tableList = malloc(Size + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Create a null-terminated string for emphasis class name (subset of data)
    size_t classSize = Size / 2;
    if (classSize == 0) classSize = 1;
    char *emphClass = malloc(classSize + 1);
    if (!emphClass) { free(tableList); return 0; }
    memcpy(emphClass, Data, classSize);
    emphClass[classSize] = '\0';

    // Prepare buffers for translation
    size_t bufSize = Size * 2;
    if (bufSize > 10000) bufSize = 10000; // Limit buffer size for safety

    widechar *inbuf = (widechar *)malloc(bufSize * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(bufSize * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(bufSize * sizeof(formtype));
    char *spacing = (char *)malloc(bufSize * sizeof(char));
    int *outputPos = (int *)malloc(bufSize * sizeof(int));
    int *inputPos = (int *)malloc(bufSize * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
        free(emphClass);
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, bufSize * sizeof(widechar));
    memset(outbuf, 0, bufSize * sizeof(widechar));
    memset(typeform, 0, bufSize * sizeof(formtype));
    memset(spacing, 0, bufSize * sizeof(char));
    memset(outputPos, 0, bufSize * sizeof(int));
    memset(inputPos, 0, bufSize * sizeof(int));

    // Fill inbuf with fuzz data interpreted as widechars
    int inlen = 0;
    for (size_t i = 0; i < Size && inlen < bufSize - 1; i++) {
        inbuf[inlen++] = Data[i];
    }

    int outlen = bufSize;
    int cursorPos = 0;
    int mode = 0;

    // 1. Test lou_getEmphClasses
    char const **classes = lou_getEmphClasses(tableList);
    if (classes) {
        // Just iterate to ensure no crash, then free
        for (int i = 0; classes[i] != NULL; i++) {
            // Access string to trigger potential issues
            size_t len = strlen(classes[i]);
            if (len > 0) classes[i][0];
        }
        free_emph_classes(classes);
    }

    // 2. Test lou_getTypeformForEmphClass
    formtype typeformVal = lou_getTypeformForEmphClass(tableList, emphClass);
    (void)typeformVal; // Suppress unused warning

    // 3. Test lou_translate
    int result = lou_translate(
        tableList,
        inbuf, &inlen,
        outbuf, &outlen,
        typeform, spacing,
        outputPos, inputPos,
        &cursorPos, mode
    );
    (void)result;

    // 4. Test lou_backTranslate
    // Reset lengths for back translation test
    inlen = bufSize; // Assume full buffer is input for back translation
    outlen = bufSize;
    // Fill outbuf with fuzz data as input braille
    for (size_t i = 0; i < Size && i < bufSize - 1; i++) {
        outbuf[i] = Data[i];
    }
    outlen = (int)Size;

    result = lou_backTranslate(
        tableList,
        outbuf, &outlen,
        inbuf, &inlen,
        typeform, spacing,
        outputPos, inputPos,
        &cursorPos, mode
    );
    (void)result;

    // 5. Test lou_getTable
    const void *table = lou_getTable(tableList);
    (void)table; // Suppress unused warning

    // 6. Test lou_backTranslateString
    inlen = bufSize;
    outlen = bufSize;
    result = lou_backTranslateString(
        tableList,
        outbuf, &outlen,
        inbuf, &inlen,
        typeform, spacing, mode
    );
    (void)result;

    // Cleanup
    free(tableList);
    free(emphClass);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}