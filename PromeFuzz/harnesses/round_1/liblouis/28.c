// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logEnd at logging.c:229:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void dummy_log_callback(logLevels level, const char *message) {
    // Do nothing - just a dummy callback for fuzzing
    (void)level;
    (void)message;
}

static void cleanup_files(void) {
    // Clean up any created files
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data for string parameters
    size_t str_len = Size > 1024 ? 1024 : Size;
    char *str_data = (char *)malloc(str_len + 1);
    if (!str_data) {
        return 0;
    }
    memcpy(str_data, Data, str_len);
    str_data[str_len] = '\0';

    // Create dummy file for lou_logFile
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy);
        }
        fclose(dummy);
    }

    // Use first byte to determine which log level to use
    int level_choice = Data[0] % 6;  // Assuming 6 log levels (0-5)
    
    // Call lou_setLogLevel with different levels
    lou_setLogLevel((logLevels)level_choice);
    
    // Call lou_version - always safe
    const char *version = lou_version();
    (void)version;  // Avoid unused variable warning
    
    // Register and unregister log callback
    lou_registerLogCallback(dummy_log_callback);
    lou_registerLogCallback(NULL);
    
    // Call lou_logFile with the dummy file
    lou_logFile("./dummy_file");
    
    // Call lou_logPrint with various format strings
    // Use safe format strings to avoid format string vulnerabilities
    if (str_len > 0) {
        // Use a safe format with limited string
        lou_logPrint("Message: %s", str_data);
        
        // Try with different format specifiers
        lou_logPrint("Test log message");
        lou_logPrint("Size: %zu", Size);
    }
    
    // Call lou_logEnd to clean up
    lou_logEnd();
    
    // Clean up
    free(str_data);
    cleanup_files();
    
    return 0;
}