// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <liblouis.h>

static void init_buffer_from_fuzz(widechar *buf, int *len, const uint8_t *data, size_t size) {
    *len = 0;
    if (size == 0) return;
    
    // Limit length to avoid overflow and respect API constraints
    int max_len = (size < 100) ? (int)size : 100;
    if (max_len > 1000) max_len = 1000; // Safety cap
    
    for (int i = 0; i < max_len; i++) {
        // Create valid widechar values from fuzz data
        buf[i] = (widechar)(data[i] & 0x7FFF); 
        (*len)++;
    }
}

static void init_char_buffer_from_fuzz(char *buf, size_t buf_size, const uint8_t *data, size_t data_size) {
    if (buf_size == 0 || data_size == 0) {
        if (buf_size > 0) buf[0] = '\0';
        return;
    }
    
    // Ensure we don't copy more than the buffer can hold (minus 1 for null terminator)
    size_t copy_len = (data_size < buf_size - 1) ? data_size : (buf_size - 1);
    memcpy(buf, data, copy_len);
    buf[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate buffers
    const int MAX_BUF = 2048;
    widechar *inbuf = (widechar *)malloc(MAX_BUF * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(MAX_BUF * sizeof(widechar));
    char *hyphens = (char *)malloc(MAX_BUF);
    char *inputHyphens = (char *)malloc(MAX_BUF);
    char *outputHyphens = (char *)malloc(MAX_BUF);
    formtype *typeform = (formtype *)malloc(MAX_BUF * sizeof(formtype));
    char *spacing = (char *)malloc(MAX_BUF);
    int *inputPos = (int *)malloc(MAX_BUF * sizeof(int));
    int *outputPos = (int *)malloc(MAX_BUF * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));
    
    if (!inbuf || !outbuf || !hyphens || !inputHyphens || !outputHyphens || 
        !typeform || !spacing || !inputPos || !outputPos || !cursorPos) {
        free(inbuf); free(outbuf); free(hyphens); free(inputHyphens); free(outputHyphens);
        free(typeform); free(spacing); free(inputPos); free(outputPos); free(cursorPos);
        return 0;
    }

    // Initialize buffers
    int inlen = 0, outlen = 0;
    init_buffer_from_fuzz(inbuf, &inlen, Data, Size);
    
    // Initialize other buffers with safe defaults
    memset(hyphens, 0, MAX_BUF);
    memset(inputHyphens, 0, MAX_BUF);
    memset(outputHyphens, 0, MAX_BUF);
    memset(typeform, 0, MAX_BUF * sizeof(formtype));
    memset(spacing, 0, MAX_BUF);
    memset(inputPos, 0, MAX_BUF * sizeof(int));
    memset(outputPos, 0, MAX_BUF * sizeof(int));
    *cursorPos = 0;

    // Create a table list string from fuzz data (ensure it's null-terminated and safe)
    char tableList[MAX_BUF];
    init_char_buffer_from_fuzz(tableList, MAX_BUF, Data, Size);
    
    // Ensure tableList is not empty for functions that require it
    if (tableList[0] == '\0') {
        strcpy(tableList, "en-gb-8"); // Default fallback table
    }

    // 1. Fuzz lou_compileString
    // This validates the table and compiles a string
    char compileString[MAX_BUF];
    init_char_buffer_from_fuzz(compileString, MAX_BUF, Data, Size);
    lou_compileString(tableList, compileString);

    // 2. Fuzz lou_getEmphClasses and lou_freeEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        // Optional: iterate to ensure no crash, though not strictly needed for fuzzing
        lou_freeEmphClasses(emphClasses);
    }

    // 3. Fuzz lou_hyphenate
    // Mode 0: text, non-zero: braille
    lou_hyphenate(tableList, inbuf, inlen, hyphens, 0);
    lou_hyphenate(tableList, inbuf, inlen, hyphens, 1);

    // 4. Fuzz lou_translateString
    // Reset output length
    outlen = 0;
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, 0);
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, 1);

    // 5. Fuzz lou_translatePrehyphenated
    // Prepare input positions (monotonic)
    for (int i = 0; i < inlen; i++) {
        inputPos[i] = i;
        inputHyphens[i] = (i % 5 == 0) ? '-' : ' ';
    }
    inputHyphens[inlen] = '\0';
    
    // Reset output length
    outlen = 0;
    int mode = (Data[0] % 2); // 0 or 1
    
    lou_translatePrehyphenated(
        tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing,
        outputPos, inputPos, cursorPos, inputHyphens, outputHyphens, mode
    );

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(hyphens);
    free(inputHyphens);
    free(outputHyphens);
    free(typeform);
    free(spacing);
    free(inputPos);
    free(outputPos);
    free(cursorPos);

    return 0;
}