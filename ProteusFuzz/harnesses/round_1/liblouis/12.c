// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
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
#include <unistd.h>

static void safe_free_emph_classes(char const **classes) {
    if (classes) {
        for (int i = 0; classes[i] != NULL; i++) {
            free((void *)classes[i]);
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare buffers
    size_t max_buf_size = 4096;
    widechar *inbuf = malloc(max_buf_size * sizeof(widechar));
    widechar *outbuf = malloc(max_buf_size * sizeof(widechar));
    formtype *typeform = malloc(max_buf_size * sizeof(formtype));
    char *spacing = malloc(max_buf_size * sizeof(char));
    int *outputPos = malloc(max_buf_size * sizeof(int));
    int *inputPos = malloc(max_buf_size * sizeof(int));
    int *cursorPos = malloc(sizeof(int));
    char *inputHyphens = malloc(max_buf_size * sizeof(char));
    char *outputHyphens = malloc(max_buf_size * sizeof(char));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || 
        !inputPos || !cursorPos || !inputHyphens || !outputHyphens) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos); free(cursorPos);
        free(inputHyphens); free(outputHyphens);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, max_buf_size * sizeof(widechar));
    memset(outbuf, 0, max_buf_size * sizeof(widechar));
    memset(typeform, 0, max_buf_size * sizeof(formtype));
    memset(spacing, 0, max_buf_size * sizeof(char));
    memset(outputPos, 0, max_buf_size * sizeof(int));
    memset(inputPos, 0, max_buf_size * sizeof(int));
    memset(inputHyphens, 0, max_buf_size * sizeof(char));
    memset(outputHyphens, 0, max_buf_size * sizeof(char));
    *cursorPos = 0;

    // Convert input data to widechar input buffer
    int inlen = (Size < max_buf_size) ? (int)Size : (int)max_buf_size;
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }
    int outlen = max_buf_size;

    // Table list: use a default table or a dummy string
    const char *tableList = "en-gb-8";

    // 1. lou_backTranslateString
    int res1 = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                       typeform, spacing, 0);
    (void)res1;

    // Reset lengths for next calls
    inlen = (Size < max_buf_size) ? (int)Size : (int)max_buf_size;
    outlen = max_buf_size;

    // 2. lou_translatePrehyphenated
    int res2 = lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen,
                                          typeform, spacing, outputPos, inputPos,
                                          cursorPos, inputHyphens, outputHyphens, 0);
    (void)res2;

    // Reset lengths
    inlen = (Size < max_buf_size) ? (int)Size : (int)max_buf_size;
    outlen = max_buf_size;

    // 3. lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    safe_free_emph_classes(emphClasses);

    // 4. lou_getTypeformForEmphClass
    formtype ft = lou_getTypeformForEmphClass(tableList, "italic");
    (void)ft;

    // Reset lengths
    inlen = (Size < max_buf_size) ? (int)Size : (int)max_buf_size;
    outlen = max_buf_size;

    // 5. lou_backTranslate
    int res5 = lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                                 typeform, spacing, outputPos, inputPos, cursorPos, 0);
    (void)res5;

    // Reset lengths
    inlen = (Size < max_buf_size) ? (int)Size : (int)max_buf_size;
    outlen = max_buf_size;

    // 6. lou_translate
    int res6 = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                             typeform, spacing, outputPos, inputPos, cursorPos, 0);
    (void)res6;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(inputHyphens);
    free(outputHyphens);

    return 0;
}