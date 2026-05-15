// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_version at compileTranslationTable.c:5419:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine which operations to perform
    uint8_t selector = Data[0];
    Data++;
    Size--;

    // Call lou_version - always safe
    const char *version = lou_version();
    (void)version;

    // Set log level based on input data
    if (Size >= 1) {
        logLevels level = (logLevels)(Data[0] % 8); // Assuming reasonable enum range
        lou_setLogLevel(level);
    }

    // Register or unregister callback based on selector
    if (selector & 0x01) {
        lou_registerLogCallback(dummy_log_callback);
    } else {
        lou_registerLogCallback(NULL);
    }

    // Try to set log file
    if (Size >= 2) {
        // Create dummy file with fuzzer data
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            // Use up to min(Size-1, 1024) bytes for file content
            size_t write_size = Size - 1;
            if (write_size > 1024) write_size = 1024;
            fwrite(Data + 1, 1, write_size, fp);
            fclose(fp);
            
            // Try to use it as log file
            lou_logFile("./dummy_file");
            
            // Clean up
            remove("./dummy_file");
        }
    }

    // Try lou_logPrint with various format strings
    if (Size >= 3) {
        // Create a null-terminated string from fuzzer data for format
        size_t format_len = Data[1] % 64;
        if (format_len > Size - 2) format_len = Size - 2;
        
        char *format = malloc(format_len + 1);
        if (format) {
            memcpy(format, Data + 2, format_len);
            format[format_len] = '\0';
            
            // Try to format with some arguments
            if (format_len > 0) {
                lou_logPrint("%s", format);
            }
            
            free(format);
        }
    }

    // Call lou_logEnd if selector indicates
    if (selector & 0x02) {
        lou_logEnd();
    }

    return 0;
}