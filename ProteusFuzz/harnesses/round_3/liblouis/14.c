// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void free_string_array(char **array) {
    if (array == NULL) return;
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for string operations
    char *query = (char *)malloc(Size + 1);
    if (query == NULL) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // 1. Test lou_findTables
    char **tables = lou_findTables(query);
    if (tables != NULL) {
        // Optionally iterate to test internal consistency
        for (int i = 0; tables[i] != NULL; i++) {
            // Just accessing to ensure validity
            if (tables[i][0] == '\0') break;
        }
        free_string_array(tables);
    }

    // 2. Test lou_findTable
    char *bestTable = lou_findTable(query);
    if (bestTable != NULL) {
        free(bestTable);
    }

    // 3. Test lou_listTables
    char **allTables = lou_listTables();
    if (allTables != NULL) {
        for (int i = 0; allTables[i] != NULL; i++) {
            // Test lou_getTableInfo with each table and a few keys
            char *info = lou_getTableInfo(allTables[i], "language");
            if (info != NULL) free(info);

            info = lou_getTableInfo(allTables[i], "description");
            if (info != NULL) free(info);

            info = lou_getTableInfo(allTables[i], query); // Use fuzz input as key
            if (info != NULL) free(info);
        }
        free_string_array(allTables);
    }

    // 4. Test lou_indexTables
    // Prepare a static array of table names to index
    // We reuse 'allTables' logic or just use a dummy list if we want to test empty/indexing behavior
    // However, lou_indexTables expects const char **. Let's create a small array from fuzz data if possible
    // or just pass NULL or a single element array to test edge cases.
    // Since we don't have a list of valid tables easily without calling lou_listTables again,
    // we will construct a temporary array from the fuzz input split by nulls or just pass a single string.
    
    // Let's try to index the table found by lou_findTable if it exists, or the query itself
    char *tempTable = lou_findTable(query);
    const char *tablesToIndex[2];
    tablesToIndex[0] = NULL;
    tablesToIndex[1] = NULL;

    if (tempTable != NULL) {
        tablesToIndex[0] = tempTable;
        tablesToIndex[1] = NULL; // NULL terminated
        lou_indexTables(tablesToIndex);
        free(tempTable);
    } else {
        // Try indexing with the query string directly as a potential table name (might fail, which is fine for fuzzing)
        tablesToIndex[0] = query;
        tablesToIndex[1] = NULL;
        lou_indexTables(tablesToIndex);
    }

    // 5. Test lou_freeTableFiles
    // This function expects an array of file paths returned by lou_loadTableFiles.
    // Since lou_loadTableFiles is not in the target list, we cannot generate a valid array to free.
    // However, we can safely call it with NULL to ensure it handles that case.
    lou_freeTableFiles(NULL);

    // We can also create a dummy array to test if it crashes on invalid input (though it expects valid pointers)
    // For safety in fuzzing, we only pass NULL or valid pointers we allocated.
    // If we wanted to test invalid pointers, we would risk crashes, but liblouis should handle NULL gracefully.
    // Let's skip passing invalid non-NULL pointers to avoid undefined behavior unless the API guarantees safety.
    // The API description says: "If the pointer is NULL, it safely returns without action."
    // It does not explicitly guarantee safety for non-NULL invalid pointers, so we stick to NULL.

    free(query);
    return 0;
}