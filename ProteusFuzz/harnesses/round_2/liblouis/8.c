// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_emph_classes(char const **classes) {
    if (classes == NULL) return;
    for (int i = 0; classes[i] != NULL; i++) {
        free((void *)classes[i]);
    }
    free((void *)classes);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from the input data for tableList
    char *tableList = (char *)malloc(Size + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Allocate buffers for translation
    // Use a reasonable size based on input, but ensure minimums
    size_t bufSize = Size * 2 + 100;
    if (bufSize < 1024) bufSize = 1024;

    widechar *inbuf = (widechar *)malloc(bufSize * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(bufSize * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(bufSize * sizeof(formtype));
    char *spacing = (char *)malloc(bufSize * sizeof(char));
    int *outputPos = (int *)malloc(bufSize * sizeof(int));
    int *inputPos = (int *)malloc(bufSize * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
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

    // Fill inbuf with fuzzed data (cast to widechar)
    // Ensure we don't exceed buffer bounds
    size_t inlen = (Size < bufSize) ? Size : bufSize - 1;
    for (size_t i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }
    inbuf[inlen] = 0; // Null terminate

    int inLen = (int)inlen;
    int outLen = (int)bufSize;
    int cursorPos = 0;

    // 1. Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        free_emph_classes(emphClasses);
    }

    // 2. Test lou_translate
    lou_translate(tableList, inbuf, &inLen, outbuf, &outLen,
                  typeform, spacing, outputPos, inputPos, &cursorPos, 0);

    // 3. Test lou_backTranslate
    // Prepare input for back translate (using the output from translate or fuzzed data)
    // Reuse inbuf as source for back translate, but treat it as braille input
    int backInLen = inLen;
    int backOutLen = (int)bufSize;
    int backCursorPos = 0;
    lou_backTranslate(tableList, inbuf, &backInLen, outbuf, &backOutLen,
                      typeform, spacing, outputPos, inputPos, &backCursorPos, 0);

    // 4. Test lou_charSize
    int charSize = lou_charSize();
    (void)charSize; // Use the value to avoid unused warning

    // 5. Test lou_getTable
    const void *table = lou_getTable(tableList);
    (void)table; // Use the value

    // 6. Test lou_backTranslateString
    int backInLenStr = inLen;
    int backOutLenStr = (int)bufSize;
    lou_backTranslateString(tableList, inbuf, &backInLenStr, outbuf, &backOutLenStr,
                            typeform, spacing, 0);

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