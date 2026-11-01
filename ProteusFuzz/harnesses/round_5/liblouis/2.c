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

static void free_findTables_result(char **result) {
    if (result == NULL) return;
    for (int i = 0; result[i] != NULL; i++) {
        free(result[i]);
    }
    free(result);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Ensure the input is null-terminated for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // 1. Prepare environment: Set log level
    // Using a safe default range for log levels (0-5)
    int logLevelVal = (Data[0] % 6); 
    lou_setLogLevel((logLevels)logLevelVal);

    // 2. Index tables
    const char *tables[2];
    tables[0] = query;
    tables[1] = NULL;
    lou_indexTables(tables);

    // 3. Invoke lou_findTable (first call)
    // lou_findTable returns a dynamically allocated string (strdup) that must be freed.
    char *table1 = lou_findTable(query);
    
    // 4. Invoke lou_findTable (second call)
    char *table2 = lou_findTable(query);

    // 5. Invoke lou_findTables
    char **tablesResult = lou_findTables(query);
    // Caller must free the array and its strings.
    free_findTables_result(tablesResult);

    // 6. Cleanup: Free resources allocated by lou_findTable before calling lou_free
    // The leak report indicates table1 and table2 are strdup'd and not freed.
    if (table1 != NULL) {
        free(table1);
    }
    if (table2 != NULL) {
        free(table2);
    }

    // Invoke lou_free to clean up global liblouis resources
    lou_free();

    // Cleanup local allocations
    free(query);

    return 0;
}