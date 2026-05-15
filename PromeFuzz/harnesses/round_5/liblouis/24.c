// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Write fuzz data to dummy file for potential table loading
    dummy_file_write(Data, Size);

    // Split input for various parameters
    size_t split = Size / 2;
    const char *tableList = (const char *)Data;
    // Ensure null termination for tableList
    char *tableListCopy = malloc(split + 1);
    if (!tableListCopy) return 0;
    memcpy(tableListCopy, Data, split);
    tableListCopy[split] = 0;
    
    // Prepare widechar buffers
    size_t widechar_len = (Size - split) / sizeof(widechar);
    if (widechar_len < 1) {
        free(tableListCopy);
        return 0;
    }
    
    widechar *inbuf = malloc(widechar_len * sizeof(widechar));
    widechar *outbuf = malloc(widechar_len * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(tableListCopy); free(inbuf); free(outbuf);
        return 0;
    }
    memcpy(inbuf, Data + split, widechar_len * sizeof(widechar));
    
    // Additional buffers for translate functions
    formtype *typeform = malloc(widechar_len * sizeof(formtype));
    char *spacing = malloc(widechar_len + 1); // +1 for null terminator
    int *outputPos = malloc(widechar_len * sizeof(int));
    int *inputPos = malloc(widechar_len * sizeof(int));
    int cursorPos = 0;
    int inlen = (int)widechar_len;
    int outlen = (int)widechar_len;
    
    if (!typeform || !spacing || !outputPos || !inputPos) {
        free(tableListCopy); free(inbuf); free(outbuf);
        free(typeform); free(spacing); free(outputPos); free(inputPos);
        return 0;
    }
    memset(spacing, 0, widechar_len + 1);
    
    // Mode selection from input data
    int mode = Data[0] % 4;
    
    // 1. lou_getTable - validate table first
    const void *table = lou_getTable(tableListCopy);
    
    // 2. lou_compileString - only if table exists or use dummy
    if (table) {
        lou_compileString(tableListCopy, (const char *)(Data + split));
    }
    
    // 3. lou_charToDots
    lou_charToDots(tableListCopy, inbuf, outbuf, inlen, mode);
    
    // 4. lou_dotsToChar
    lou_dotsToChar(tableListCopy, inbuf, outbuf, inlen, mode);
    
    // 5. lou_translateString
    lou_translateString(tableListCopy, inbuf, &inlen, outbuf, &outlen, 
                       typeform, spacing, mode);
    
    // 6. lou_backTranslate
    lou_backTranslate(tableListCopy, inbuf, &inlen, outbuf, &outlen,
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    // Cleanup
    free(tableListCopy);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    
    return 0;
}