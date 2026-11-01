// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
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

static void safe_free_emph_classes(char const **classes) {
    if (classes == NULL) return;
    for (int i = 0; classes[i] != NULL; i++) {
        free((void *)classes[i]);
    }
    free((void *)classes);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a null-terminated string from input for table list
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Prepare buffers for translation
    size_t maxLen = 1024;
    widechar *inbuf = (widechar *)malloc(maxLen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(maxLen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(maxLen * sizeof(formtype));
    char *spacing = (char *)malloc(maxLen * sizeof(char));
    int *outputPos = (int *)malloc(maxLen * sizeof(int));
    int *inputPos = (int *)malloc(maxLen * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
        free(inbuf); free(outbuf); free(typeform); free(spacing); free(outputPos); free(inputPos);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, maxLen * sizeof(widechar));
    memset(outbuf, 0, maxLen * sizeof(widechar));
    memset(typeform, 0, maxLen * sizeof(formtype));
    memset(spacing, 0, maxLen * sizeof(char));
    memset(outputPos, 0, maxLen * sizeof(int));
    memset(inputPos, 0, maxLen * sizeof(int));

    int inlen = (Size < maxLen) ? (int)Size : (int)maxLen;
    int outlen = maxLen;
    int cursorPos = 0;

    // Convert input bytes to widechar for inbuf
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }

    // Fuzz lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        safe_free_emph_classes(emphClasses);
    }

    // Fuzz lou_translate
    int mode = (Data[0] % 10); // Vary mode
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // Fuzz lou_getTypeformForEmphClass
    // Use first part of input as class name if it looks like a string
    char *classStr = (char *)malloc(Size + 1);
    if (classStr) {
        memcpy(classStr, Data, Size);
        classStr[Size] = '\0';
        formtype typeformVal = lou_getTypeformForEmphClass(tableList, classStr);
        (void)typeformVal; // Use result to avoid unused warning
        free(classStr);
    }

    // Fuzz lou_backTranslateString
    int inlenBT = inlen;
    int outlenBT = maxLen;
    lou_backTranslateString(tableList, inbuf, &inlenBT, outbuf, &outlenBT, typeform, spacing, mode);

    // Fuzz lou_getTable
    const void *table = lou_getTable(tableList);
    if (table) {
        // Table loaded successfully, no cleanup needed for this API
    }

    // Fuzz lou_backTranslate
    int inlenBT2 = inlen;
    int outlenBT2 = maxLen;
    int cursorPosBT = 0;
    lou_backTranslate(tableList, inbuf, &inlenBT2, outbuf, &outlenBT2, typeform, spacing, outputPos, inputPos, &cursorPosBT, mode);

    // Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}