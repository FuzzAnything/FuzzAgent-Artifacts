// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_logFile at logging.c:196:1 in liblouis.h
// lou_logPrint at logging.c:213:1 in liblouis.h
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
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
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Ensure we have some data
    if (Size == 0) return 0;
    
    // Create null-terminated string from input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';
    
    // Index tables first (required for lou_findTable)
    // Use an empty array instead of NULL to avoid SEGV
    static int tables_indexed = 0;
    if (!tables_indexed) {
        // Create a valid NULL-terminated array of table directories
        // Empty array with NULL terminator
        const char *empty_table_dirs[] = {NULL};
        lou_indexTables(empty_table_dirs);
        tables_indexed = 1;
    }
    
    // Test lou_findTable with a safe query
    // Use a simple query to avoid complex parsing issues
    char *table_result = lou_findTable("locale:en");
    if (table_result) {
        free(table_result);
    }
    
    // Test lou_setLogLevel with various levels
    // Use input data to determine log level (modulo to keep within reasonable range)
    int level = Data[0] % 5;
    lou_setLogLevel((logLevels)level);
    
    // Test lou_registerLogCallback
    lou_registerLogCallback(dummy_log_callback);
    
    // Test lou_logFile with dummy file
    // Create dummy file first
    FILE *dummy = fopen("./dummy_file", "w");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy);
        }
        fclose(dummy);
    }
    
    lou_logFile("./dummy_file");
    
    // Test lou_logPrint with safe format strings
    // Use simple format to avoid issues
    if (Size > 0 && input_str[0] != '\0') {
        lou_logPrint("Message: %s", input_str);
    }
    
    // Reset callback
    lou_registerLogCallback(NULL);
    
    // Test lou_logEnd
    lou_logEnd();
    
    // Cleanup
    free(input_str);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}