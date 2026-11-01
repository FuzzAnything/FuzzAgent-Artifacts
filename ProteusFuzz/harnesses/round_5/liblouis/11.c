// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logEnd at logging.c:229:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

// Correct callback signature based on liblouis.h: void (*)(logLevels, const char *)
static void dummy_log_callback(logLevels level, const char *format) {
    (void)level;
    (void)format;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    // Register a custom log callback
    lou_registerLogCallback(dummy_log_callback);

    // Set log level based on input data
    // Using correct enum values: LOU_LOG_ERROR, LOU_LOG_WARN, LOU_LOG_INFO, LOU_LOG_DEBUG
    logLevels level = LOU_LOG_ERROR;
    if (Size > 0) {
        uint8_t level_byte = Data[0];
        if (level_byte == 0) level = LOU_LOG_ERROR;
        else if (level_byte == 1) level = LOU_LOG_WARN;
        else if (level_byte == 2) level = LOU_LOG_INFO;
        else if (level_byte == 3) level = LOU_LOG_DEBUG;
        else level = LOU_LOG_ERROR; // Default fallback
    }
    lou_setLogLevel(level);

    // Set log file based on input data
    if (Size > 1) {
        // Create a safe filename from input data (limit length and sanitize)
        char filename[256];
        size_t len = (Size < 255) ? Size : 255;
        // Copy data, ensuring null termination
        memcpy(filename, Data, len);
        filename[len] = '\0';
        
        // Sanitize: replace any '/' or '\\' to prevent directory traversal
        for (size_t i = 0; i < len; i++) {
            if (filename[i] == '/' || filename[i] == '\\') {
                filename[i] = '_';
            }
        }
        
        // Ensure it's not empty or just whitespace
        if (len > 0 && filename[0] != '\0') {
            lou_logFile(filename);
        }
    } else {
        // Default to dummy file if no specific filename derived
        lou_logFile("./dummy_file");
    }

    // Log a message using input data as format string (carefully)
    // To avoid crashes from invalid format specifiers, we limit the format string usage
    if (Size > 0) {
        // We cannot safely pass user data as format string directly to printf-like functions
        // without validation. Instead, we log a static message to ensure stability.
        lou_logPrint("Fuzz input size: %zu, level: %d", Size, (int)level);
    }

    // End logging
    lou_logEnd();

    // Cleanup
    lou_free();

    return 0;
}