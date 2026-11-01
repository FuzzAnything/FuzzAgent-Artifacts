// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
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

static void free_emph_classes(char const **classes) {
    if (classes == NULL) return;
    int i = 0;
    while (classes[i] != NULL) {
        free((void *)classes[i]);
        i++;
    }
    free((void *)classes);
}

static void free_widechar_buffer(widechar *buf) {
    if (buf != NULL) {
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from input for table list
    char tableList[256];
    size_t tableListLen = (Size < sizeof(tableList) - 1) ? Size : sizeof(tableList) - 1;
    memcpy(tableList, Data, tableListLen);
    tableList[tableListLen] = '\0';

    // Fuzz lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses != NULL) {
        free_emph_classes(emphClasses);
    }

    // Fuzz lou_getTable
    const void *table = lou_getTable(tableList);
    // No cleanup needed for lou_getTable as it returns a pointer to internal data

    // Prepare buffers for translation functions
    // Use a fixed size for simplicity, but scale based on input if needed
    int maxLen = 1024;
    if (Size > 0 && Size < 100) maxLen = Size * 10;

    widechar *inbuf = (widechar *)malloc(maxLen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(maxLen * sizeof(widechar));
    int *outputPos = (int *)malloc(maxLen * sizeof(int));
    int *inputPos = (int *)malloc(maxLen * sizeof(int));
    formtype *typeform = (formtype *)malloc(maxLen * sizeof(formtype));
    char *spacing = (char *)malloc(maxLen * sizeof(char));

    if (!inbuf || !outbuf || !outputPos || !inputPos || !typeform || !spacing) {
        free(inbuf);
        free(outbuf);
        free(outputPos);
        free(inputPos);
        free(typeform);
        free(spacing);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, maxLen * sizeof(widechar));
    memset(outbuf, 0, maxLen * sizeof(widechar));
    memset(outputPos, 0, maxLen * sizeof(int));
    memset(inputPos, 0, maxLen * sizeof(int));
    memset(typeform, 0, maxLen * sizeof(formtype));
    memset(spacing, 0, maxLen * sizeof(char));

    // Convert input data to widechar for inbuf
    int inlen = 0;
    for (size_t i = 0; i < Size && i < maxLen; i++) {
        inbuf[i] = (widechar)Data[i];
        inlen++;
    }

    int outlen = maxLen;
    int cursorPos = 0;

    // Fuzz lou_translate
    int mode = (Size % 4) - 2; // Generate modes: -2, -1, 0, 1
    int ret = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    (void)ret; // Ignore return value for fuzzing purposes

    // Fuzz lou_backTranslate
    // Swap inbuf and outbuf to use translated output as input for back translation
    widechar *tempBuf = inbuf;
    inbuf = outbuf;
    outbuf = tempBuf;
    
    // Reset lengths
    inlen = outlen; // Previous outlen becomes new inlen
    outlen = maxLen;
    cursorPos = 0;

    ret = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    (void)ret;

    // Fuzz lou_backTranslateString
    // Reuse buffers
    tempBuf = inbuf;
    inbuf = outbuf;
    outbuf = tempBuf;
    
    inlen = outlen;
    outlen = maxLen;

    ret = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
    (void)ret;

    // Fuzz lou_charSize
    int charSize = lou_charSize();
    (void)charSize; // Just call it

    // Cleanup
    free_widechar_buffer(inbuf);
    free_widechar_buffer(outbuf);
    free(outputPos);
    free(inputPos);
    free(typeform);
    free(spacing);

    return 0;
}