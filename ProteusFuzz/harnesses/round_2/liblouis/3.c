// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void free_table_array(char **tables) {
    if (tables == NULL) {
        return;
    }
    for (int i = 0; tables[i] != NULL; i++) {
        free(tables[i]);
    }
    free(tables);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for string operations
    char *input_str = malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Set log level
    // We try different log levels based on the first byte of input.
    // Since specific enum values (LL_LOG_LEVEL_*) are not guaranteed in the provided header info,
    // we cast integers directly to logLevels to ensure compilation.
    // Typically: 0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR, 4=FATAL
    logLevels level = (logLevels)0;
    if (Size > 0) {
        int val = Data[0] % 5;
        level = (logLevels)val;
    }
    lou_setLogLevel(level);

    // 2. Index tables
    // We create a dummy table name array. 
    // We use the input string as a potential table name and a fallback.
    const char *dummy_tables[3];
    dummy_tables[0] = input_str; 
    dummy_tables[1] = "en-gb:computer"; 
    dummy_tables[2] = NULL;
    
    lou_indexTables(dummy_tables);

    // 3. Find Table (First call)
    char *table1 = lou_findTable(input_str);
    if (table1 != NULL) {
        free(table1);
    }

    // 4. Find Table (Second call)
    char *table2 = lou_findTable(input_str);
    if (table2 != NULL) {
        free(table2);
    }

    // 5. Find Tables
    char **tables = lou_findTables(input_str);
    if (tables != NULL) {
        free_table_array(tables);
    }

    // 6. Cleanup
    lou_free();

    free(input_str);
    return 0;
}