// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
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

static void safe_free_emph_classes(char const **ptr) {
    if (ptr == NULL) return;
    size_t i = 0;
    while (ptr[i] != NULL) {
        free((void *)ptr[i]);
        i++;
    }
    free((void *)ptr);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from the input
    char *tableList = (char *)malloc(Size + 1);
    if (tableList == NULL) return 0;
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // First call to lou_getTable
    const void *table1 = lou_getTable(tableList);

    // Second call to lou_getTable (reusing or different input is fine, here reuse)
    const void *table2 = lou_getTable(tableList);

    // Third call to lou_getTable
    const void *table3 = lou_getTable(tableList);

    // Call lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses != NULL) {
        // Optionally iterate to ensure validity, though not strictly required for fuzzing
        size_t i = 0;
        while (emphClasses[i] != NULL) {
            i++;
        }
        // Free the array and its contents
        safe_free_emph_classes(emphClasses);
    }

    // Cleanup: free liblouis resources
    lou_free();

    // Free the allocated tableList
    free(tableList);

    return 0;
}