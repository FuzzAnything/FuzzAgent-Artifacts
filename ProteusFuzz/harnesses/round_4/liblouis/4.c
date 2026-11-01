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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_free_string_array(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment: Set log level
    // Use the first byte to determine log level, ensuring valid enum range
    logLevels level = (logLevels)(Data[0] % 10);
    lou_setLogLevel(level);

    // Prepare table names array for lou_indexTables
    // We create a small array of table names derived from input to test parsing
    // Ensure we have at least one entry and a NULL terminator
    const char *table_names[4];
    int table_count = 0;

    // Generate up to 3 table name candidates from input data
    for (int i = 0; i < 3 && table_count < 3; i++) {
        size_t offset = (i * Size) / 3;
        size_t len = Size / 3;
        if (offset + len > Size) len = Size - offset;
        if (len == 0) continue;

        // Create a safe string (replace nulls to avoid truncation issues in logic)
        char *temp = malloc(len + 1);
        if (temp == NULL) continue;
        memcpy(temp, Data + offset, len);
        temp[len] = '\0';

        // Basic sanitization: ensure it looks somewhat like a table name
        // (liblouis will handle invalid names gracefully usually)
        table_names[table_count++] = temp;
    }
    table_names[table_count] = NULL;

    // 2. Invoke lou_indexTables
    // This builds the internal index
    lou_indexTables(table_names);

    // 3. Invoke lou_findTable (first call)
    // Use a portion of input as query
    char query1[256];
    size_t q1_len = Size < 255 ? Size : 255;
    memcpy(query1, Data, q1_len);
    query1[q1_len] = '\0';
    
    char *table1 = lou_findTable(query1);
    if (table1 != NULL) {
        free(table1);
    }

    // 4. Invoke lou_findTable (second call)
    // Use a different portion or modified query
    char query2[256];
    size_t q2_len = Size < 255 ? Size : 255;
    // XOR with a constant to vary the state
    for (size_t i = 0; i < q2_len; i++) {
        query2[i] = Data[i] ^ 0x42;
    }
    query2[q2_len] = '\0';

    char *table2 = lou_findTable(query2);
    if (table2 != NULL) {
        free(table2);
    }

    // 5. Invoke lou_findTables
    char query3[256];
    size_t q3_len = Size < 255 ? Size : 255;
    memcpy(query3, Data, q3_len);
    query3[q3_len] = '\0';

    char **tables = lou_findTables(query3);
    if (tables != NULL) {
        safe_free_string_array(tables);
    }

    // 6. Cleanup
    lou_free();

    // Free the temporary table name strings created for indexing
    for (int i = 0; i < table_count; i++) {
        free((char *)table_names[i]);
    }

    return 0;
}