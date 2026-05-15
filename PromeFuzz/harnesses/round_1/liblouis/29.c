// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
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
#include "liblouis.h"

static void custom_log_callback(logLevels level, const char *message) {
    // Custom logging callback that does nothing
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
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

    // Create dummy file and write fuzzer data to it
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Initialize liblouis tables - use an empty table list instead of NULL
    const char *table_list[] = {NULL};
    lou_indexTables(table_list);

    // Test lou_setLogLevel with different levels
    // Use modulo to get valid enum values (assuming 0-4 are valid levels)
    int log_level = Data[0] % 5;
    lou_setLogLevel((logLevels)log_level);

    // Test lou_registerLogCallback with both custom callback and NULL
    if (Data[0] % 2 == 0) {
        lou_registerLogCallback(custom_log_callback);
    } else {
        lou_registerLogCallback(NULL);
    }

    // Test lou_logFile with dummy file
    lou_logFile("./dummy_file");

    // Test lou_logPrint with various format strings
    // Use a subset of the input as format string to avoid format string vulnerabilities
    size_t format_len = Size > 100 ? 100 : Size;
    char *format_str = (char *)malloc(format_len + 1);
    if (format_str) {
        memcpy(format_str, Data, format_len);
        format_str[format_len] = '\0';
        lou_logPrint("%s", format_str);
        free(format_str);
    }

    // Test lou_findTable with query from fuzzer input
    char *table_result = lou_findTable(input_str);
    if (table_result) {
        free(table_result);
    }

    // Test lou_logEnd
    lou_logEnd();

    // Clean up
    free(input_str);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}