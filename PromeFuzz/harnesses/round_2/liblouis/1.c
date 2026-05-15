// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void logCallback(logLevels level, const char *message) {
    // Empty callback to suppress logging during fuzzing
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 2) {
        return 0;
    }

    // Register log callback
    lou_registerLogCallback(logCallback);

    // Create table list string from fuzzer input
    // Use first 64 bytes or less for table name
    size_t tableLen = Size > 64 ? 64 : Size;
    char *tableList = (char *)malloc(tableLen + 1);
    if (!tableList) {
        return 0;
    }
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Check table validity
    lou_checkTable(tableList);

    // Free liblouis memory (first call)
    lou_free();

    // Free liblouis memory (second call)
    lou_free();

    // Prepare for back translation
    // Use remaining data for input buffer
    size_t remaining = Size - tableLen;
    if (remaining < 2) {
        free(tableList);
        return 0;
    }

    // Calculate input length (number of widechars)
    size_t inlen = remaining / sizeof(widechar);
    if (inlen == 0) {
        free(tableList);
        return 0;
    }

    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    memcpy(inbuf, Data + tableLen, inlen * sizeof(widechar));

    // Initialize lengths
    int actualInlen = (int)inlen;
    int outlen = actualInlen * 2; // Output can be longer than input

    // Allocate output buffer
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(tableList);
        free(inbuf);
        return 0;
    }

    // Decide whether to use typeform and spacing based on input data
    // Use first byte to determine configuration
    uint8_t config = Data[0];
    
    formtype *typeform = NULL;
    char *spacing = NULL;
    
    if (config & 0x01) {
        // Allocate typeform buffer (size should be at least outlen)
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) {
            memset(typeform, 0, outlen * sizeof(formtype));
        }
    }
    
    if (config & 0x02) {
        // Allocate spacing buffer (size should be at least outlen)
        spacing = (char *)malloc(outlen * sizeof(char));
        if (spacing) {
            memset(spacing, 0, outlen * sizeof(char));
        }
    }

    // Set mode based on input
    int mode = (int)(config & 0xFC);

    // Perform back translation
    lou_backTranslateString(tableList, inbuf, &actualInlen, 
                           outbuf, &outlen, typeform, spacing, mode);

    // Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    if (typeform) {
        free(typeform);
    }
    if (spacing) {
        free(spacing);
    }

    // Free liblouis memory (third call)
    lou_free();

    return 0;
}