// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logEnd at logging.c:229:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

/* Custom log callback for fuzzing matching the expected signature: void (*)(logLevels, const char *) */
static void
fuzz_log_callback(logLevels level, const char *message) {
    (void)level;
    (void)message;
    /* Silently discard log output during fuzzing to avoid side effects */
}

int
LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. lou_getDataPath: Test deprecated path retrieval */
    char *dataPath = lou_getDataPath();
    if (dataPath != NULL) {
        /* No cleanup needed for dataPath as it is static/internal */
        (void)dataPath;
    }

    /* 2. lou_setLogLevel: Set logging level using input data */
    /* Map input byte to a valid log level enum if possible, else default */
    logLevels level = (logLevels)(input_str[0] % 10); 
    lou_setLogLevel(level);

    /* 3. lou_logFile: Set log file to a dummy file */
    lou_logFile("./dummy_file");

    /* 4. lou_registerLogCallback: Register custom callback */
    lou_registerLogCallback(fuzz_log_callback);

    /* 5. lou_logPrint: Log formatted message using input data */
    /* Use a safe format string to avoid format string vulnerabilities */
    lou_logPrint("Fuzz input length: %zu", Size);
    /* Try to log the input string safely by treating it as data, not format */
    lou_logPrint("Input data: %s", input_str);

    /* 6. lou_logEnd: Clean up logging */
    lou_logEnd();

    /* Cleanup */
    free(input_str);

    return 0;
}