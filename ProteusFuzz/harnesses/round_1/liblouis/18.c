// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static int fuzz_compile_string(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    // Ensure the input is null-terminated
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    // Use a valid, simple input string for compilation
    const char *inString = "test";
    
    // Call the function
    int result = lou_compileString(tableList, inString);

    free(tableList);
    return result;
}

static int fuzz_get_emph_classes(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    char *tableList = (char *)malloc(size + 1);
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

static int fuzz_get_typeform_for_emph_class(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    // Use a simple class name
    const char *emphClass = "bold";
    formtype typeform = lou_getTypeformForEmphClass(tableList, emphClass);

    free(tableList);
    return typeform;
}

static int fuzz_translate(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    // Prepare input buffer
    // Ensure we have at least one character to avoid empty buffer issues
    size_t safeSize = (size > 0) ? size : 1;
    widechar *inbuf = (widechar *)malloc(safeSize * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    for (size_t i = 0; i < safeSize; i++) {
        inbuf[i] = (widechar)data[i % size];
    }
    int inlen = safeSize;

    // Prepare output buffer
    widechar *outbuf = (widechar *)malloc(safeSize * sizeof(widechar));
    if (!outbuf) {
        free(tableList);
        free(inbuf);
        return 0;
    }
    int outlen = safeSize;

    // Prepare other parameters
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = (int *)malloc(safeSize * sizeof(int));
    int *inputPos = (int *)malloc(safeSize * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));

    if (!outputPos || !inputPos || !cursorPos) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        free(outputPos);
        free(inputPos);
        free(cursorPos);
        return 0;
    }

    *cursorPos = 0;

    int result = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos, cursorPos, 0);

    free(tableList);
    free(inbuf);
    free(outbuf);
    free(outputPos);
    free(inputPos);
    free(cursorPos);

    return result;
}

static int fuzz_get_table(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    char *tableList = (char *)malloc(size + 1);
    if (!tableList) return 0;
    memcpy(tableList, data, size);
    tableList[size] = '\0';

    const void *table = lou_getTable(tableList);

    free(tableList);
    return table != NULL;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each function
    fuzz_compile_string(Data, Size);
    fuzz_get_emph_classes(Data, Size);
    fuzz_get_typeform_for_emph_class(Data, Size);
    fuzz_translate(Data, Size);
    fuzz_get_table(Data, Size);

    return 0;
}