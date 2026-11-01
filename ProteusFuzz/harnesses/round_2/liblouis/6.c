// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1 || Size > 1024) {
        return 0;
    }

    // Create a null-terminated table list string from input
    char tableList[Size + 1];
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Ensure tableList is valid (non-empty and printable-ish for safety)
    if (tableList[0] == '\0') {
        return 0;
    }

    // Prepare buffers for translation
    size_t maxLen = Size * 2 + 100;
    widechar *inbuf = malloc(maxLen * sizeof(widechar));
    widechar *outbuf = malloc(maxLen * sizeof(widechar));
    int *outputPos = malloc(maxLen * sizeof(int));
    int *inputPos = malloc(maxLen * sizeof(int));
    formtype *typeform = malloc(maxLen * sizeof(formtype));
    char *spacing = malloc(maxLen * sizeof(char));
    char *hyphens = malloc(maxLen * sizeof(char));

    if (!inbuf || !outbuf || !outputPos || !inputPos || !typeform || !spacing || !hyphens) {
        free(inbuf);
        free(outbuf);
        free(outputPos);
        free(inputPos);
        free(typeform);
        free(spacing);
        free(hyphens);
        return 0;
    }

    // Initialize input buffer with fuzz data as widechars
    int inlen = Size;
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }
    inbuf[inlen] = 0;

    int outlen = maxLen;
    int cursorPos = 0;
    int mode = 0;

    // Fuzz lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        free_emph_classes(emphClasses);
    }

    // Fuzz lou_getTable
    const void *table = lou_getTable(tableList);
    // No cleanup needed for lou_getTable as per typical liblouis usage

    // Fuzz lou_translate
    int result = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos,
                               &cursorPos, mode);
    (void)result;

    // Fuzz lou_backTranslate
    inlen = Size;
    outlen = maxLen;
    result = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos,
                               &cursorPos, mode);
    (void)result;

    // Fuzz lou_backTranslateString
    inlen = Size;
    outlen = maxLen;
    result = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                     typeform, spacing, mode);
    (void)result;

    // Fuzz lou_hyphenate (mode 0 for text)
    result = lou_hyphenate(tableList, inbuf, inlen, hyphens, 0);
    (void)result;

    // Fuzz lou_hyphenate (mode 1 for braille)
    result = lou_hyphenate(tableList, inbuf, inlen, hyphens, 1);
    (void)result;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(outputPos);
    free(inputPos);
    free(typeform);
    free(spacing);
    free(hyphens);

    return 0;
}