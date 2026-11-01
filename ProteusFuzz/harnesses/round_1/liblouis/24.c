// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
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

static void cleanup_emph_classes(char const **classes) {
    if (classes) {
        lou_freeEmphClasses(classes);
    }
}

static void cleanup_table(const void *table) {
    // liblouis does not provide a public API to free the table returned by lou_getTable
    // in the provided API list. Typically, tables are cached internally.
    // We do not free 'table' here as no free function is exposed in the target API list.
    (void)table;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string-based inputs
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // 1. Test lou_version (No args, safe)
    const char *version = lou_version();
    (void)version; // Use version if needed, here just to ensure no crash

    // 2. Test lou_getTable
    const void *table = lou_getTable(tableList);
    // Table is typically cached, no explicit free needed based on provided API list

    // 3. Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    // Cleanup handled later
    (void)emphClasses; // Just triggering the logic

    // 4. Test lou_getTypeformForEmphClass
    // We need a class name. Let's try the first one from emphClasses if available,
    // or a dummy string from input.
    const char *testClass = "dummy";
    if (emphClasses && emphClasses[0]) {
        testClass = emphClasses[0];
    } else {
        // Fallback to a substring of input if no classes found
        if (Size > 0) {
            testClass = tableList; 
        }
    }
    formtype typeform = lou_getTypeformForEmphClass(tableList, testClass);
    (void)typeform;

    // 5. Test lou_translate
    // Prepare buffers
    size_t inlen = Size;
    size_t outlen = Size * 2; // Braille output can be larger
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeformBuf = (formtype *)malloc(inlen * sizeof(formtype));
    char *spacingBuf = (char *)malloc(inlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;

    if (!inbuf || !outbuf || !typeformBuf || !spacingBuf || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeformBuf); free(spacingBuf);
        free(outputPos); free(inputPos); free(tableList);
        cleanup_emph_classes(emphClasses);
        return 0;
    }

    // Initialize input buffer
    for (size_t i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
        typeformBuf[i] = 0; // No typeform by default
        spacingBuf[i] = 0;
        outputPos[i] = 0;
    }
    for (size_t i = 0; i < outlen; i++) {
        inputPos[i] = 0;
    }

    int result = lou_translate(
        tableList,
        inbuf,
        (int *)&inlen,
        outbuf,
        (int *)&outlen,
        typeformBuf,
        spacingBuf,
        outputPos,
        inputPos,
        &cursorPos,
        0 // mode
    );
    (void)result;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeformBuf);
    free(spacingBuf);
    free(outputPos);
    free(inputPos);
    free(tableList);
    cleanup_emph_classes(emphClasses);

    return 0;
}