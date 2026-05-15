// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setDataPath at compileTranslationTable.c:59:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_getDataPath at compileTranslationTable.c:70:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input for path/query functions
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char *)malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        // If no data, use a minimal default
        input_str = strdup("");
        if (!input_str) return 0;
    }

    // 1. Test lou_setDataPath
    char *path_result = lou_setDataPath(input_str);
    (void)path_result; // Result may be NULL, ignore for fuzzing

    // 2. Test lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        // Free the table list
        lou_freeTableFiles(tables);
    }

    // 3. Test lou_freeTableFile (needs a table pointer)
    // Create a dummy table pointer for testing
    char *dummy_table = NULL;
    if (Size > 0) {
        // Allocate some memory to simulate a table
        dummy_table = (char *)malloc(Size);
        if (dummy_table) {
            memcpy(dummy_table, Data, Size);
            lou_freeTableFile(dummy_table);
            // Note: lou_freeTableFile frees the memory, so dummy_table is now invalid
        }
    }

    // 4. Test lou_getDataPath
    char *data_path = lou_getDataPath();
    (void)data_path; // Ignore result

    // 5. Test lou_freeTableFiles (already tested with lou_listTables)
    // Additional test with NULL
    lou_freeTableFiles(NULL);

    // 6. Test lou_findTables
    char **found_tables = lou_findTables(input_str);
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }

    // Cleanup
    free(input_str);
    
    return 0;
}