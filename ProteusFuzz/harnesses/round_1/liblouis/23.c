// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Step 1: Prepare environment
    int charSize = lou_charSize();
    if (charSize <= 0) return 0;

    // Allocate buffers based on input size, ensuring minimum sizes
    size_t maxLen = Size;
    if (maxLen > 10000) maxLen = 10000;

    widechar *inbuf = malloc(maxLen * sizeof(widechar));
    widechar *outbuf = malloc(maxLen * sizeof(widechar));
    formtype *typeform = malloc(maxLen * sizeof(formtype));
    char *spacing = malloc(maxLen * sizeof(char));
    int *outputPos = malloc(maxLen * sizeof(int));
    int *inputPos = malloc(maxLen * sizeof(int));
    int cursorPos = 0;

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, maxLen * sizeof(widechar));
    memset(outbuf, 0, maxLen * sizeof(widechar));
    memset(typeform, 0, maxLen * sizeof(formtype));
    memset(spacing, 0, maxLen * sizeof(char));
    memset(outputPos, 0, maxLen * sizeof(int));
    memset(inputPos, 0, maxLen * sizeof(int));

    // Convert input data to widechar for inbuf
    for (size_t i = 0; i < maxLen && i < Size; i++) {
        inbuf[i] = (widechar)Data[i];
    }

    // Prepare table list string from input
    char tableList[256];
    size_t tableLen = (Size > 250) ? 250 : Size;
    for (size_t i = 0; i < tableLen; i++) {
        tableList[i] = Data[i];
    }
    tableList[tableLen] = '\0';

    int inlen = (int)maxLen;
    int outlen = (int)maxLen;

    // Step 2 & 3: Invoke target functions diversely

    // 1. lou_charSize (already called, just use value)
    (void)charSize;

    // 2. lou_backTranslateString
    int res1 = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                       typeform, spacing, 0);
    (void)res1;

    // Reset lengths for next call
    inlen = (int)maxLen;
    outlen = (int)maxLen;

    // 3. lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        // Iterate and potentially use classes
        for (int i = 0; emphClasses[i] != NULL; i++) {
            // 4. lou_getTypeformForEmphClass
            formtype tf = lou_getTypeformForEmphClass(tableList, emphClasses[i]);
            (void)tf;
        }
        // Free the array of strings returned by lou_getEmphClasses
        // Note: liblouis typically manages the internal strings, but the array itself might need freeing depending on version.
        // Standard practice for this API is to free the array if it's dynamically allocated.
        // However, documentation often implies the array is static or managed. 
        // To be safe and avoid double-free if static, we check if it's a known static pattern or just free if we allocated.
        // Since we didn't allocate, we assume it's managed by liblouis or static. 
        // But the doc says "Callers must free the result", so we free it.
        free((void*)emphClasses);
    }

    // 5. lou_backTranslate
    int res2 = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                                 typeform, spacing, outputPos, inputPos, &cursorPos, 0);
    (void)res2;

    // Reset lengths
    inlen = (int)maxLen;
    outlen = (int)maxLen;

    // 6. lou_translate
    int res3 = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                             typeform, spacing, outputPos, inputPos, &cursorPos, 0);
    (void)res3;

    // Additional exploration: Try with NULL optional parameters
    inlen = (int)maxLen;
    outlen = (int)maxLen;
    int res4 = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                                 NULL, NULL, outputPos, inputPos, &cursorPos, 0);
    (void)res4;

    inlen = (int)maxLen;
    outlen = (int)maxLen;
    int res5 = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                             NULL, NULL, outputPos, inputPos, &cursorPos, 0);
    (void)res5;

    // Write dummy file if needed (though not strictly required by these APIs, 
    // we do it to satisfy the requirement if the fuzzer input triggers file logic internally)
    write_dummy_file(Data, Size);

    // Step 2: Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}