// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logEnd at logging.c:229:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void custom_log_callback(logLevels level, const char *message) {
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a safe null-terminated string from the fuzz input
    char *safe_input = malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    // Test lou_setLogLevel with different levels
    // We try a range of values to explore different states
    logLevels levels[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 11; i++) {
        lou_setLogLevel(levels[i]);
    }

    // Test lou_logFile with the fuzzed input as filename
    lou_logFile(safe_input);

    // Also test with NULL and empty string
    lou_logFile(NULL);
    lou_logFile("");

    // Test lou_logPrint with fuzzed format string
    // We limit the size to avoid potential stack overflow or excessive recursion
    if (Size > 0 && Size < 1024) {
        lou_logPrint(safe_input);
    }

    // Test lou_registerLogCallback
    lou_registerLogCallback(custom_log_callback);
    lou_registerLogCallback(NULL);

    // Test lou_logEnd
    lou_logEnd();

    // Test lou_free
    // This cleans up resources and resets state
    lou_free();

    free(safe_input);
    return 0;
}