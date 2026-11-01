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
#include <liblouis.h>

static void log_callback(logLevels level, const char *msg) {
    // No-op logging callback for fuzzing
    (void)level;
    (void)msg;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Step 1: Register log callback
    lou_registerLogCallback(log_callback);

    // Prepare table list from input data (ensure null termination)
    char tableList[256];
    size_t tableLen = (Size < 255) ? Size : 255;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Step 2: Check table
    int tableExists = lou_checkTable(tableList);
    (void)tableExists; // Suppress unused variable warning

    // Step 3: Free resources (as per required order)
    lou_free();

    // Step 4: Free again (as per required order, though likely no-op after first)
    lou_free();

    // Prepare input and output buffers for translation
    // Re-initialize minimal state if needed after free, but liblouis may need tables loaded
    // Since we freed, we try to translate anyway to test error handling
    widechar inbuf[256];
    int inlen = (Size < 256) ? Size : 256;
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)Data[i];
    }

    widechar outbuf[512];
    int outlen = sizeof(outbuf) / sizeof(outbuf[0]);

    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    // Step 5: Translate string
    int result = lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
    (void)result; // Result used for path exploration

    // Step 6: Final cleanup
    lou_free();

    return 0;
}