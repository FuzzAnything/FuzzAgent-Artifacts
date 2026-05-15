// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to determine function to test
    uint8_t function_selector = Data[0] % 6;
    // Use second byte for mode and flags
    uint8_t mode_flags = Data[1];
    
    // Use remaining data for input string
    size_t input_size = Size - 2;
    const uint8_t *input_data = Data + 2;
    
    // Convert input to widechar
    size_t widechar_len = input_size / sizeof(widechar);
    if (widechar_len == 0) widechar_len = 1;
    
    widechar *inbuf = (widechar *)malloc(widechar_len * sizeof(widechar));
    if (!inbuf) return 0;
    
    // Copy data, padding with zeros if needed
    size_t copy_bytes = input_size < widechar_len * sizeof(widechar) ? 
                        input_size : widechar_len * sizeof(widechar);
    memcpy(inbuf, input_data, copy_bytes);
    if (copy_bytes < widechar_len * sizeof(widechar)) {
        memset((char *)inbuf + copy_bytes, 0, 
               widechar_len * sizeof(widechar) - copy_bytes);
    }
    
    int inlen = (int)widechar_len;
    int outlen = inlen * 4;  // Reasonable output buffer size
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    
    // Common parameters
    const char *tableList = "en-us-g2.ctb";
    int mode = mode_flags & 0x7F;
    
    // Allocate optional arrays based on flags
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;
    char *hyphens = NULL;
    
    // Set up optional parameters based on mode_flags bits
    if (mode_flags & 0x01) {  // Bit 0: allocate typeform
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
    }
    
    if (mode_flags & 0x02) {  // Bit 1: allocate spacing
        spacing = (char *)malloc((outlen + 1) * sizeof(char));
        if (spacing) memset(spacing, 0, (outlen + 1) * sizeof(char));
    }
    
    if (mode_flags & 0x04) {  // Bit 2: allocate position arrays
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
        if (inputPos) memset(inputPos, 0, outlen * sizeof(int));
    }
    
    if (mode_flags & 0x08) {  // Bit 3: allocate hyphen arrays
        inputHyphens = (char *)malloc((inlen + 1) * sizeof(char));
        outputHyphens = (char *)malloc((outlen + 1) * sizeof(char));
        hyphens = (char *)malloc((inlen + 1) * sizeof(char));
        if (inputHyphens) memset(inputHyphens, 0, (inlen + 1) * sizeof(char));
        if (outputHyphens) memset(outputHyphens, 0, (outlen + 1) * sizeof(char));
        if (hyphens) memset(hyphens, 0, (inlen + 1) * sizeof(char));
    }
    
    // Call selected function
    switch (function_selector) {
        case 0:
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                               typeform, spacing, mode);
            break;
        case 1:
            lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen,
                                      typeform, spacing, outputPos, inputPos,
                                      &cursorPos, inputHyphens, outputHyphens, mode);
            break;
        case 2:
            lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
            break;
        case 3:
            lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
            break;
        case 4:
            lou_getTable(tableList);
            break;
        case 5:
            lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                   typeform, spacing, mode);
            break;
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (inputHyphens) free(inputHyphens);
    if (outputHyphens) free(outputHyphens);
    if (hyphens) free(hyphens);
    
    return 0;
}