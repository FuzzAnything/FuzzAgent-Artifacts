// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void init_buffers(widechar *buf, int *len, size_t max_size) {
    if (buf && len) {
        *len = max_size;
        memset(buf, 0, max_size * sizeof(widechar));
    }
}

static void init_int_arrays(int *arr, int size) {
    if (arr) {
        memset(arr, 0, size * sizeof(int));
    }
}

static void init_char_arrays(char *arr, int size) {
    if (arr) {
        memset(arr, 0, size * sizeof(char));
    }
}

static void init_formtype_arrays(formtype *arr, int size) {
    if (arr) {
        memset(arr, 0, size * sizeof(formtype));
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Limit buffer sizes to prevent excessive memory usage
    size_t max_len = (Size > 1024) ? 1024 : Size;
    
    // Allocate buffers
    widechar *inbuf = (widechar *)malloc(max_len * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(max_len * sizeof(widechar));
    int *inlen = (int *)malloc(sizeof(int));
    int *outlen = (int *)malloc(sizeof(int));
    int *outputPos = (int *)malloc(max_len * sizeof(int));
    int *inputPos = (int *)malloc(max_len * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));
    formtype *typeform = (formtype *)malloc(max_len * sizeof(formtype));
    char *spacing = (char *)malloc(max_len * sizeof(char));
    char *inputHyphens = (char *)malloc(max_len * sizeof(char));
    char *outputHyphens = (char *)malloc(max_len * sizeof(char));
    char *hyphens = (char *)malloc(max_len * sizeof(char));

    if (!inbuf || !outbuf || !inlen || !outlen || !outputPos || !inputPos || 
        !cursorPos || !typeform || !spacing || !inputHyphens || !outputHyphens || !hyphens) {
        free(inbuf); free(outbuf); free(inlen); free(outlen);
        free(outputPos); free(inputPos); free(cursorPos);
        free(typeform); free(spacing); free(inputHyphens); free(outputHyphens); free(hyphens);
        return 0;
    }

    // Initialize buffers
    init_buffers(inbuf, inlen, max_len);
    init_buffers(outbuf, outlen, max_len);
    init_int_arrays(outputPos, max_len);
    init_int_arrays(inputPos, max_len);
    *cursorPos = 0;
    init_formtype_arrays(typeform, max_len);
    init_char_arrays(spacing, max_len);
    init_char_arrays(inputHyphens, max_len);
    init_char_arrays(outputHyphens, max_len);
    init_char_arrays(hyphens, max_len);

    // Copy input data to inbuf safely
    // Data is uint8_t, inbuf is widechar (unsigned short). 
    // We copy byte-by-byte or cast carefully. Since widechar is 2 bytes, 
    // we treat the input as raw bytes filling the widechar buffer.
    // To avoid overflow, we copy min(Size, max_len * sizeof(widechar)) bytes.
    size_t copy_size = (Size < max_len * sizeof(widechar)) ? Size : max_len * sizeof(widechar);
    memcpy(inbuf, Data, copy_size);
    
    // Update inlen to reflect the number of widechars actually filled
    // If Size is odd, the last widechar might be partially filled, but liblouis expects valid widechars.
    // We set inlen to the number of complete widechars we can safely use.
    *inlen = (int)(copy_size / sizeof(widechar));

    // Define table list (use a common default table)
    const char *tableList = "en-gb-8";

    // Fuzz lou_translate
    lou_translate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing, 
                  outputPos, inputPos, cursorPos, 0);

    // Fuzz lou_backTranslate
    lou_backTranslate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing, 
                      outputPos, inputPos, cursorPos, 0);

    // Fuzz lou_translatePrehyphenated
    lou_translatePrehyphenated(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing, 
                               outputPos, inputPos, cursorPos, inputHyphens, outputHyphens, 0);

    // Fuzz lou_hyphenate (text mode)
    lou_hyphenate(tableList, inbuf, *inlen, hyphens, 0);
    // Fuzz lou_hyphenate (braille mode)
    lou_hyphenate(tableList, inbuf, *inlen, hyphens, 1);

    // Fuzz lou_dotsToChar
    lou_dotsToChar(tableList, inbuf, outbuf, *inlen, 0);

    // Fuzz lou_charToDots
    lou_charToDots(tableList, inbuf, outbuf, *inlen, 0);

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(inlen);
    free(outlen);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(typeform);
    free(spacing);
    free(inputHyphens);
    free(outputHyphens);
    free(hyphens);

    return 0;
}