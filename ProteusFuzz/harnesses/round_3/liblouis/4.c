// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <liblouis.h>

static void safe_free_table_names(char **names) {
    if (names == NULL) return;
    for (int i = 0; names[i] != NULL; i++) {
        free(names[i]);
    }
    free(names);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Step 1: Set log level
    // Since specific enum values are not provided in the prompt, we use 0, 1, 2, 3
    // which typically correspond to ERROR, WARNING, INFO, DEBUG in such libraries.
    logLevels level = 0; 
    if (Size > 0) {
        level = (logLevels)(Data[0] % 4);
    }
    lou_setLogLevel(level);

    // Step 2: Prepare table names for indexing
    const char *tables[10];
    int tableCount = 0;
    
    char tableNames[10][64];
    size_t chunkSize = Size / 10;
    if (chunkSize == 0) chunkSize = 1;
    
    for (int i = 0; i < 10 && i * chunkSize < Size; i++) {
        size_t len = (i + 1) * chunkSize < Size ? chunkSize : Size - i * chunkSize;
        if (len >= 64) len = 63;
        if (len == 0) continue;
        
        memcpy(tableNames[i], Data + i * chunkSize, len);
        tableNames[i][len] = '\0';
        
        // Ensure it looks like a table name (basic sanitization)
        for (size_t j = 0; j < len; j++) {
            if (!isalnum((unsigned char)tableNames[i][j]) && tableNames[i][j] != '-' && tableNames[i][j] != '_') {
                tableNames[i][j] = '_';
            }
        }
        tables[tableCount++] = tableNames[i];
    }
    
    if (tableCount == 0) {
        tables[0] = "en-gb:computer";
        tableCount = 1;
    }
    tables[tableCount] = NULL;

    // Step 3: Index tables
    lou_indexTables(tables);

    // Step 4: Find a single table
    char *table1 = NULL;
    char query1[64];
    size_t q1Len = Size > 63 ? 63 : Size;
    if (q1Len > 0) {
        memcpy(query1, Data, q1Len);
        query1[q1Len] = '\0';
        for (size_t j = 0; j < q1Len; j++) {
            if (!isalnum((unsigned char)query1[j]) && query1[j] != '-' && query1[j] != ':' && query1[j] != '_') {
                query1[j] = '_';
            }
        }
        table1 = lou_findTable(query1);
    } else {
        table1 = lou_findTable("en:grade2");
    }

    // Step 5: Find another table with different query
    char *table2 = NULL;
    char query2[64];
    size_t q2Len = Size > 63 ? 63 : Size;
    if (q2Len > 0) {
        for (size_t j = 0; j < q2Len; j++) {
            query2[j] = Data[(j + 1) % Size] + (j % 26);
            if (!isalnum((unsigned char)query2[j]) && query2[j] != '-' && query2[j] != ':' && query2[j] != '_') {
                query2[j] = '_';
            }
        }
        query2[q2Len] = '\0';
        table2 = lou_findTable(query2);
    } else {
        table2 = lou_findTable("de:computer");
    }

    // Step 6: Find multiple tables
    char **tablesFound = NULL;
    char query3[64];
    size_t q3Len = Size > 63 ? 63 : Size;
    if (q3Len > 0) {
        memcpy(query3, Data, q3Len);
        query3[q3Len] = '\0';
        for (size_t j = 0; j < q3Len; j++) {
            if (!isalnum((unsigned char)query3[j]) && query3[j] != '-' && query3[j] != ':' && query3[j] != '_') {
                query3[j] = '_';
            }
        }
        tablesFound = lou_findTables(query3);
    } else {
        tablesFound = lou_findTables("en");
    }

    // Cleanup: Free allocated resources
    if (table1 != NULL) {
        free(table1);
    }
    if (table2 != NULL) {
        free(table2);
    }
    if (tablesFound != NULL) {
        safe_free_table_names(tablesFound);
    }

    // Step 7: Free liblouis resources
    lou_free();

    return 0;
}