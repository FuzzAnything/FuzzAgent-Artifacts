// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
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
    // Do nothing
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Use first byte to determine log level (mod 8 for safety)
    int log_level = 0;
    if (Size > 0) {
        log_level = Data[0] % 8;
    }
    
    // Test lou_setLogLevel with different levels
    lou_setLogLevel(log_level);
    
    // Test lou_version - always safe to call
    const char *version = lou_version();
    if (version) {
        // Just to use the return value
        volatile const char *v = version;
        (void)v;
    }
    
    // Test lou_registerLogCallback with both NULL and dummy callback
    static int callback_selector = 0;
    callback_selector = (callback_selector + 1) % 3;
    
    if (callback_selector == 0) {
        lou_registerLogCallback(NULL);
    } else if (callback_selector == 1) {
        lou_registerLogCallback(dummy_log_callback);
    } else {
        lou_registerLogCallback(NULL);
    }
    
    // Test lou_logFile with dummy file
    FILE *dummy = fopen("./dummy_file", "w");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy);
        }
        fclose(dummy);
    }
    
    lou_logFile("./dummy_file");
    
    // Test lou_logPrint with various format strings
    if (Size > 0) {
        // Create null-terminated strings from input data
        char format[256];
        char message[256];
        
        size_t format_len = Size > 255 ? 255 : Size;
        memcpy(format, Data, format_len);
        format[format_len] = '\0';
        
        // Use second half of data for message if available
        size_t message_start = Size / 2;
        size_t message_len = Size - message_start;
        if (message_len > 255) message_len = 255;
        
        if (message_len > 0) {
            memcpy(message, Data + message_start, message_len);
            message[message_len] = '\0';
            
            // Try different format specifiers
            lou_logPrint("%s", message);
            lou_logPrint("Test: %s", message);
            
            // Use the format string from input (be careful with user-controlled format)
            // We'll use a safe subset
            if (strchr(format, '%') == NULL) {
                lou_logPrint("%s", format);
            }
        } else {
            lou_logPrint("No message data");
        }
    } else {
        lou_logPrint("Empty input");
    }
    
    // Test lou_logEnd (deprecated but still callable)
    lou_logEnd();
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}