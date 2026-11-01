// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
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
#include <limits.h>

static int fuzz_lou_getEmphClasses(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Ensure null termination
    char *tableList = malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }

    free(tableList);
    return 0;
}

static int fuzz_lou_translate(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    // Split data: first part as tableList, rest as input text
    size_t tableLen = size / 2;
    if (tableLen == 0) tableLen = 1;
    if (tableLen > size) tableLen = size;

    char *tableList = malloc(tableLen + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, tableLen);
    tableList[tableLen] = '\0';

    size_t textLen = size - tableLen;
    if (textLen == 0) textLen = 1;
    if (textLen > size - tableLen) textLen = size - tableLen;

    widechar *inbuf = malloc(textLen * sizeof(widechar));
    if (!inbuf) { free(tableList); return 0; }
    
    // Convert input bytes to widechar
    for (size_t i = 0; i < textLen; i++) {
        inbuf[i] = data[tableLen + i];
    }

    int inlen = textLen;
    int outlen = textLen * 2; // Allocate more space for output
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    if (!outbuf) { free(tableList); free(inbuf); return 0; }

    // Optional parameters
    formtype *typeform = malloc(textLen * sizeof(formtype));
    char *spacing = malloc(textLen * sizeof(char));
    int *outputPos = malloc(textLen * sizeof(int));
    int *inputPos = malloc(outlen * sizeof(int));
    int cursorPos = 0;

    if (!typeform || !spacing || !outputPos || !inputPos) {
        free(tableList); free(inbuf); free(outbuf);
        free(typeform); free(spacing); free(outputPos); free(inputPos);
        return 0;
    }

    // Initialize optional arrays
    memset(typeform, 0, textLen * sizeof(formtype));
    memset(spacing, 0, textLen * sizeof(char));
    memset(outputPos, 0, textLen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));

    // Try different modes
    int mode = data[0] % 4; // 0 to 3

    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}

static int fuzz_lou_getTypeformForEmphClass(const uint8_t *data, size_t size) {
    if (size < 2) return 0;

    size_t tableLen = size / 2;
    if (tableLen == 0) tableLen = 1;
    if (tableLen > size) tableLen = size;

    char *tableList = malloc(tableLen + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, tableLen);
    tableList[tableLen] = '\0';

    size_t classLen = size - tableLen;
    if (classLen == 0) classLen = 1;
    if (classLen > size - tableLen) classLen = size - tableLen;

    char *emphClass = malloc(classLen + 1);
    if (!emphClass) { free(tableList); return 0; }
    memcpy(emphClass, data + tableLen, classLen);
    emphClass[classLen] = '\0';

    formtype result = lou_getTypeformForEmphClass(tableList, emphClass);
    (void)result; // Use result to avoid unused warning

    free(tableList);
    free(emphClass);
    return 0;
}

static int fuzz_lou_compileString(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    char *tableList = malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    // Use same data as inString for simplicity
    int result = lou_compileString(tableList, tableList);
    (void)result;

    free(tableList);
    return 0;
}

static int fuzz_lou_getTable(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    char *tableList = malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    const void *table = lou_getTable(tableList);
    (void)table;

    free(tableList);
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each target function
    fuzz_lou_getEmphClasses(Data, Size);
    fuzz_lou_translate(Data, Size);
    fuzz_lou_getTypeformForEmphClass(Data, Size);
    fuzz_lou_compileString(Data, Size);
    fuzz_lou_getTable(Data, Size);

    return 0;
}