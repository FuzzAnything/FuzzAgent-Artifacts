// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static void dummy_log_callback(logLevels level, const char *message) {
    // Do nothing - just a dummy callback for fuzzing
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
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

    // Create a shorter string for some functions
    size_t half_size = Size / 2;
    char *short_str = (char *)malloc(half_size + 1);
    if (short_str) {
        memcpy(short_str, Data, half_size);
        short_str[half_size] = '\0';
    }

    // Create dummy file for lou_logFile
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Use first byte to determine log level (modulo to keep within enum range)
    logLevels level = (logLevels)(Data[0] % 8);

    // Call target functions in various combinations
    // 1. lou_setDataPath
    char *path_result = lou_setDataPath(input_str);
    
    // 2. lou_setLogLevel
    lou_setLogLevel(level);
    
    // 3. lou_logPrint with various format strings
    if (Size > 10) {
        lou_logPrint("Test message: %s", input_str);
        lou_logPrint("Short: %s", short_str ? short_str : "");
        lou_logPrint("Size: %zu", Size);
    }
    
    // 4. lou_registerLogCallback
    lou_registerLogCallback(dummy_log_callback);
    lou_registerLogCallback(NULL); // Reset to default
    
    // 5. lou_getDataPath
    char *get_path = lou_getDataPath();
    
    // 6. lou_logFile
    lou_logFile("./dummy_file");
    if (short_str) {
        lou_logFile(short_str); // Try with fuzzer input as filename
    }
    
    // Clean up
    free(input_str);
    if (short_str) {
        free(short_str);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}