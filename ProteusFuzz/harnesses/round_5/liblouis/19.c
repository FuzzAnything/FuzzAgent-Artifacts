// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
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
        size_t i = 0;
        while (classes[i] != NULL) {
            free((void *)classes[i]);
            i++;
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a safe string from input data, ensuring null termination
    size_t max_len = Size < 1000 ? Size : 1000;
    char *input_str = malloc(max_len + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, max_len);
    input_str[max_len] = '\0';

    // Prepare table list (use a common default or part of input)
    // We try to use a known table name if possible, otherwise a safe fallback
    const char *tableList = "en-gb-8.dot";
    if (Size > 10 && input_str[0] == 'e' && input_str[1] == 'n') {
        tableList = input_str;
    }

    // Prepare widechar input buffer
    int inlen = (int)strlen(input_str);
    if (inlen == 0) {
        free(input_str);
        return 0;
    }

    widechar *inbuf = malloc((inlen + 1) * sizeof(widechar));
    if (!inbuf) {
        free(input_str);
        return 0;
    }
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)input_str[i];
    }
    inbuf[inlen] = 0;

    // Prepare output buffers
    int outlen = inlen * 2 + 10; // Heuristic for output size
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        free(input_str);
        return 0;
    }

    // Allocate auxiliary buffers for lou_translate and lou_translatePrehyphenated
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;

    // Allocate if we want to test with these parameters (diversity)
    if (Size > 50) {
        typeform = calloc(inlen, sizeof(formtype));
        spacing = calloc(inlen, sizeof(char));
        outputPos = calloc(inlen, sizeof(int));
        inputPos = calloc(outlen, sizeof(int));
        inputHyphens = calloc(inlen, sizeof(char));
        outputHyphens = calloc(outlen, sizeof(char));
    }

    int ret = 0;

    // 1. Test lou_compileString
    ret = lou_compileString(tableList, input_str);
    (void)ret; // Ignore return for fuzzing, just exercise

    // 2. Test lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(tableList);
    cleanup_emph_classes(emph_classes);

    // 3. Test lou_translate
    int inlen_copy = inlen;
    int outlen_copy = outlen;
    ret = lou_translate(
        tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
        typeform, spacing, outputPos, inputPos, &cursorPos, 0
    );
    (void)ret;

    // 4. Test lou_translateString
    inlen_copy = inlen;
    outlen_copy = outlen;
    ret = lou_translateString(
        tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
        typeform, spacing, 0
    );
    (void)ret;

    // 5. Test lou_hyphenate
    char *hyphens = malloc(inlen + 1);
    if (hyphens) {
        ret = lou_hyphenate(tableList, inbuf, inlen, hyphens, 0); // Text mode
        (void)ret;
        ret = lou_hyphenate(tableList, inbuf, inlen, hyphens, 1); // Braille mode
        (void)ret;
        free(hyphens);
    }

    // 6. Test lou_translatePrehyphenated
    int inlen_copy2 = inlen;
    int outlen_copy2 = outlen;
    if (inputHyphens && outputHyphens && inputPos && outputPos) {
        // Ensure monotonic positions for valid test
        for (int i = 0; i < inlen; i++) inputPos[i] = i;
        for (int i = 0; i < outlen; i++) outputPos[i] = i;
        
        ret = lou_translatePrehyphenated(
            tableList, inbuf, &inlen_copy2, outbuf, &outlen_copy2,
            typeform, spacing, outputPos, inputPos, &cursorPos,
            inputHyphens, outputHyphens, 0
        );
        (void)ret;
    }

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(input_str);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(inputHyphens);
    free(outputHyphens);

    return 0;
}