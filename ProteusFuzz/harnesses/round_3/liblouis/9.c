// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void init_widechar_buffer(widechar *buf, const uint8_t *data, size_t size, size_t buf_size) {
    size_t i;
    for (i = 0; i < size && i < buf_size - 1; i++) {
        buf[i] = (widechar)(data[i] & 0x7F);
    }
    // Ensure null termination within bounds
    if (i < buf_size) {
        buf[i] = 0;
    }
}

static void init_formtype_buffer(formtype *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

static void init_spacing_buffer(char *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = ' ';
    }
    if (size > 0) buf[size - 1] = '\0';
}

static void init_pos_buffer(int *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

static void init_hyphens_buffer(char *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = '-';
    }
    if (size > 0) buf[size - 1] = '\0';
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate buffers with extra space for null termination
    size_t max_len = Size > 1000 ? 1000 : Size;
    // Add 1 to ensure we can always null-terminate
    size_t alloc_len = max_len + 1;

    widechar *inbuf = (widechar *)malloc(alloc_len * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(alloc_len * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(alloc_len * sizeof(formtype));
    char *spacing = (char *)malloc(alloc_len * sizeof(char));
    int *outputPos = (int *)malloc(alloc_len * sizeof(int));
    int *inputPos = (int *)malloc(alloc_len * sizeof(int));
    char *inputHyphens = (char *)malloc(alloc_len * sizeof(char));
    char *outputHyphens = (char *)malloc(alloc_len * sizeof(char));
    char *hyphens = (char *)malloc(alloc_len * sizeof(char));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos ||
        !inputHyphens || !outputHyphens || !hyphens) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos); free(inputHyphens);
        free(outputHyphens); free(hyphens);
        return 0;
    }

    // Initialize buffers
    init_widechar_buffer(inbuf, Data, Size, alloc_len);
    init_widechar_buffer(outbuf, Data, Size, alloc_len);
    init_formtype_buffer(typeform, alloc_len);
    init_spacing_buffer(spacing, alloc_len);
    init_pos_buffer(outputPos, alloc_len);
    init_pos_buffer(inputPos, alloc_len);
    init_hyphens_buffer(inputHyphens, alloc_len);
    init_hyphens_buffer(outputHyphens, alloc_len);
    init_hyphens_buffer(hyphens, alloc_len);

    int inlen = max_len;
    int outlen = max_len;
    int cursorPos = 0;
    const char *tableList = "en-gb-g";
    int mode = 0;

    // Fuzz lou_translate
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing,
                  outputPos, inputPos, &cursorPos, mode);

    // Fuzz lou_backTranslate
    inlen = max_len;
    outlen = max_len;
    cursorPos = 0;
    lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing,
                      outputPos, inputPos, &cursorPos, mode);

    // Fuzz lou_dotsToChar
    inlen = max_len;
    lou_dotsToChar(tableList, inbuf, outbuf, inlen, mode);

    // Fuzz lou_hyphenate
    inlen = max_len;
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    lou_hyphenate(tableList, inbuf, inlen, hyphens, 1);

    // Fuzz lou_translatePrehyphenated
    inlen = max_len;
    outlen = max_len;
    cursorPos = 0;
    for (int i = 0; i < max_len; i++) {
        inputPos[i] = i;
    }
    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, typeform,
                               spacing, outputPos, inputPos, &cursorPos,
                               inputHyphens, outputHyphens, mode);

    // Fuzz lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        // Attempt to free if dynamically allocated. 
        // Note: liblouis often returns static arrays for this, so freeing might be unsafe 
        // depending on the specific build/version. However, the API docs say "Callers must free".
        // We cast to void* to suppress warnings if the type is const**.
        free((void *)emphClasses);
    }

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(inputHyphens);
    free(outputHyphens);
    free(hyphens);

    return 0;
}