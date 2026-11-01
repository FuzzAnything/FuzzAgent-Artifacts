// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
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

static void free_emph_classes(char const **classes) {
    if (classes) {
        for (int i = 0; classes[i] != NULL; i++) {
            free((void *)classes[i]);
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create a table list string from input
    char tableList[256];
    size_t tableLen = (Size > 255) ? 255 : Size;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Prepare buffers for translation
    size_t maxBufSize = 4096;
    widechar *inbuf = (widechar *)malloc(maxBufSize * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(maxBufSize * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(maxBufSize * sizeof(formtype));
    char *spacing = (char *)malloc(maxBufSize * sizeof(char));
    int *outputPos = (int *)malloc(maxBufSize * sizeof(int));
    int *inputPos = (int *)malloc(maxBufSize * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing); free(outputPos); free(inputPos);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, maxBufSize * sizeof(widechar));
    memset(outbuf, 0, maxBufSize * sizeof(widechar));
    memset(typeform, 0, maxBufSize * sizeof(formtype));
    memset(spacing, 0, maxBufSize * sizeof(char));
    memset(outputPos, 0, maxBufSize * sizeof(int));
    memset(inputPos, 0, maxBufSize * sizeof(int));

    // Fill inbuf with fuzzed data (cast to widechar)
    size_t inlen = (Size > maxBufSize) ? maxBufSize : Size;
    for (size_t i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }

    int inLen = (int)inlen;
    int outLen = maxBufSize;
    int cursorPos = (Size > 0) ? (int)Data[0] % inlen : 0;

    // Test lou_getTable
    const void *table = lou_getTable(tableList);
    // No cleanup needed for lou_getTable as per typical liblouis usage (internal cache)

    // Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    free_emph_classes(emphClasses);

    // Test lou_getTypeformForEmphClass with various class names
    const char *testClasses[] = {"italic", "bold", "computer", "none", NULL};
    for (int i = 0; testClasses[i]; i++) {
        formtype ft = lou_getTypeformForEmphClass(tableList, testClasses[i]);
        (void)ft; // Suppress unused warning
    }

    // Test lou_translate
    int ret = lou_translate(tableList, inbuf, &inLen, outbuf, &outLen, typeform, spacing, outputPos, inputPos, &cursorPos, 0);
    (void)ret;

    // Test lou_backTranslate
    // Prepare braille input (reuse inbuf as braille)
    inLen = (int)inlen;
    outLen = maxBufSize;
    ret = lou_backTranslate(tableList, inbuf, &inLen, outbuf, &outLen, typeform, spacing, outputPos, inputPos, &cursorPos, 0);
    (void)ret;

    // Test lou_backTranslateString
    inLen = (int)inlen;
    outLen = maxBufSize;
    ret = lou_backTranslateString(tableList, inbuf, &inLen, outbuf, &outLen, typeform, spacing, 0);
    (void)ret;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}