// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_logEnd at logging.c:229:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "liblouis.h"

static void dummy_log_callback(logLevels level, const char *message) {
    // Do nothing - just a dummy callback for fuzzing
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Create a dummy file for logging functions
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }

    // Test lou_setLogLevel with various levels
    // Use modulo to get valid enum values (assuming 0-4 are valid levels)
    logLevels level = (logLevels)(Data[0] % 5);
    lou_setLogLevel(level);

    // Test lou_registerLogCallback
    if (Size % 2 == 0) {
        lou_registerLogCallback(dummy_log_callback);
    } else {
        lou_registerLogCallback(NULL);
    }

    // Test lou_logFile with the dummy file
    lou_logFile("./dummy_file");

    // Test lou_logPrint with simple format string
    // Use a safe format to avoid format string vulnerabilities
    lou_logPrint("Fuzzer input: %s", input_str);

    // Test lou_findTable with fuzzer input as query
    // Only call if we have enough data to avoid NULL pointer issues
    if (Size > 1) {
        char *table_result = lou_findTable(input_str);
        if (table_result) {
            free(table_result);
        }
    }

    // Test lou_logEnd
    lou_logEnd();

    // Clean up
    free(input_str);
    
    // Remove dummy file if it exists
    remove("./dummy_file");

    return 0;
}