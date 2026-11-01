// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void dummyLogCallback(logLevels level, const char *format) {
    (void)level;
    (void)format;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Step 1: Prepare the environment
    // Register log callback
    lou_registerLogCallback(dummyLogCallback);

    // Create a table list from input data (ensure it's null-terminated and valid)
    // We limit the table list length to avoid consuming all input for the table name
    // and ensure we have data left for the buffer.
    size_t tableListLen = Size / 2;
    if (tableListLen > 255) {
        tableListLen = 255;
    }
    
    // Ensure we leave at least 1 byte for the input buffer if possible
    if (tableListLen == Size) {
        tableListLen = Size - 1;
    }

    char tableList[256];
    memset(tableList, 0, sizeof(tableList));
    memcpy(tableList, Data, tableListLen);
    tableList[tableListLen] = '\0';

    // Step 2: Invoke lou_checkTable
    int tableExists = lou_checkTable(tableList);
    (void)tableExists;

    // Prepare input and output buffers for backTranslateString
    // Calculate remaining bytes for the input buffer
    size_t remainingBytes = Size - tableListLen;
    if (remainingBytes == 0) {
        remainingBytes = 1; // Ensure at least 1 byte if possible, though Size check handles this mostly
    }
    
    // We treat the remaining bytes as widechar data. 
    // Since widechar is unsigned short (2 bytes), we need to be careful with alignment and size.
    // To avoid buffer overflow, we calculate how many full widechars we can fit.
    size_t inLen = remainingBytes / sizeof(widechar);
    if (inLen == 0) {
        inLen = 1; // Fallback to 1 if no full widechars fit, but we must ensure we don't read out of bounds
        // If remainingBytes < sizeof(widechar), we can't safely copy as widechar without padding.
        // Let's just use a safe small size if input is tiny.
        if (remainingBytes < sizeof(widechar)) {
             // Create a dummy valid widechar
             widechar dummy = 0;
             widechar *inbuf = (widechar *)malloc(sizeof(widechar));
             if (!inbuf) return 0;
             inbuf[0] = dummy;
             int inlen = 1;
             
             widechar *outbuf = (widechar *)malloc(2 * sizeof(widechar));
             if (!outbuf) { free(inbuf); return 0; }
             int outlen = 2;
             
             int result = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, NULL, NULL, 0);
             (void)result;
             
             free(inbuf);
             free(outbuf);
             lou_free();
             lou_free();
             return 0;
        }
    }

    // Ensure we don't exceed the allocated input buffer size
    // We allocate exactly inLen * sizeof(widechar)
    widechar *inbuf = (widechar *)malloc(inLen * sizeof(widechar));
    if (!inbuf) {
        return 0;
    }
    
    // Copy data safely. We copy exactly inLen * sizeof(widechar) bytes.
    // The source is Data + tableListLen.
    // We must ensure (tableListLen + inLen * sizeof(widechar)) <= Size.
    // By definition: inLen = remainingBytes / sizeof(widechar) = (Size - tableListLen) / sizeof(widechar)
    // So inLen * sizeof(widechar) <= Size - tableListLen.
    // Thus, tableListLen + inLen * sizeof(widechar) <= Size. Safe.
    memcpy(inbuf, Data + tableListLen, inLen * sizeof(widechar));
    int inlen = (int)inLen;

    // Allocate output buffer. A safe upper bound is 2x input length.
    widechar *outbuf = (widechar *)malloc(inLen * 2 * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    int outlen = (int)(inLen * 2);

    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    // Step 3: Invoke lou_backTranslateString
    int result = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
    (void)result;

    // Step 4: Cleanup
    free(inbuf);
    free(outbuf);

    // Step 5: Invoke lou_free twice as per the task requirement
    lou_free();
    lou_free();

    return 0;
}