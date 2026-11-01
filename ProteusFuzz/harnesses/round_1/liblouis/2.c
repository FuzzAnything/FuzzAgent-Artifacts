// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void log_handler(logLevels level, const char *format) {
    (void)level;
    (void)format;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Register log callback
    lou_registerLogCallback(log_handler);

    // Prepare a table list from input data (ensure null termination)
    char tableList[256];
    size_t tableLen = (Size < sizeof(tableList) - 1) ? Size : sizeof(tableList) - 1;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Step 2: Check if the table exists
    int tableExists = lou_checkTable(tableList);

    // Prepare input buffer for back translation
    widechar *inbuf = NULL;
    int inlen = 0;
    widechar *outbuf = NULL;
    int outlen = 0;

    if (tableExists) {
        // Allocate input buffer
        inlen = (Size > 0) ? (int)Size : 1;
        inbuf = (widechar *)malloc(inlen * sizeof(widechar));
        if (!inbuf) {
            return 0;
        }

        // Fill input buffer with fuzz data (cast to widechar)
        for (int i = 0; i < inlen; i++) {
            inbuf[i] = (widechar)Data[i % Size];
        }

        // Allocate output buffer
        outlen = inlen + 10;
        outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        if (!outbuf) {
            free(inbuf);
            return 0;
        }

        // Prepare typeform and spacing buffers (optional, set to NULL for simplicity)
        formtype *typeform = NULL;
        char *spacing = NULL;

        // Step 3: Perform back translation
        int mode = 0; // Default mode
        int result = lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
        (void)result; // Result is not used for fuzzing logic

        // Step 4: Free allocated buffers
        free(inbuf);
        free(outbuf);
    }

    // Step 5: Cleanup liblouis resources
    lou_free();

    return 0;
}