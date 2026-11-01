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

static void log_callback(logLevels level, const char *msg) {
    // Custom log handler, can be empty for fuzzing
    (void)level;
    (void)msg;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Register log callback with correct signature
    lou_registerLogCallback(log_callback);

    // Prepare table list from input (ensure null termination)
    char tableList[256];
    size_t tableLen = (Size < sizeof(tableList) - 1) ? Size : sizeof(tableList) - 1;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // 2. Invoke lou_checkTable
    int checkResult = lou_checkTable(tableList);
    (void)checkResult; // Use result to avoid unused warning

    // 3. Invoke lou_free (first time)
    lou_free();

    // 4. Invoke lou_free (second time, as per requirement)
    lou_free();

    // Re-initialize state for translation if needed, though lou_free clears everything.
    // We attempt translation with the input data.

    // Prepare input buffer for translation
    // We reuse the input data as the widechar input buffer
    // Ensure we have at least 2 bytes for a valid widechar if Size >= 2
    if (Size < 2) {
        return 0;
    }

    widechar *inbuf = (widechar *)malloc(Size);
    if (!inbuf) return 0;

    // Convert input bytes to widechar (simple cast for fuzzing)
    memcpy(inbuf, Data, Size);

    int inlen = (int)(Size / sizeof(widechar));
    // Ensure inlen is positive
    if (inlen <= 0) {
        free(inbuf);
        return 0;
    }

    // Prepare output buffer
    int outlen = inlen * 2; // Heuristic for output size
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }

    // Prepare typeform and spacing (NULL as per hint for no checking)
    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    // 5. Invoke lou_translateString
    // Note: Since we called lou_free twice, tables might be unloaded. 
    // This is intentional to test error handling or re-initialization behavior.
    int translateResult = lou_translateString(
        tableList, 
        inbuf, &inlen, 
        outbuf, &outlen, 
        typeform, spacing, mode
    );
    (void)translateResult;

    // 6. Cleanup
    lou_free();

    free(inbuf);
    free(outbuf);

    return 0;
}