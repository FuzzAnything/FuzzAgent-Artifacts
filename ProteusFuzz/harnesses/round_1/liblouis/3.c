// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void logCallback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Register log callback
    lou_registerLogCallback(logCallback);

    // Prepare table list from input data
    char tableList[256];
    size_t tableLen = Size < 255 ? Size : 255;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Step 2: Check if table exists
    (void)lou_checkTable(tableList);

    // Step 3: First call to lou_free to reset state (as per task order)
    lou_free();

    // Step 4: Second call to lou_free (as per task order)
    lou_free();

    // Prepare input buffer for translation
    // We reuse the input data as the widechar input buffer
    // Ensure we have at least 2 bytes for a valid widechar
    if (Size < 2) {
        return 0;
    }

    widechar *inbuf = malloc(Size * sizeof(widechar));
    if (!inbuf) {
        return 0;
    }
    
    // Convert uint8_t to widechar (unsigned short)
    for (size_t i = 0; i < Size; i++) {
        inbuf[i] = Data[i];
    }

    int inlen = (int)Size;
    int outlen = (int)Size * 2; // Allocate more space for output

    widechar *outbuf = malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }

    // Optional parameters: typeform and spacing
    formtype *typeform = NULL;
    char *spacing = NULL;

    // Step 5: Translate string
    int mode = 0; // Default mode
    (void)lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);

    // Step 6: Free translation resources
    lou_free();

    // Cleanup allocated buffers
    free(inbuf);
    free(outbuf);

    return 0;
}