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

static void safe_free_emph_classes(char const **emph_classes) {
    if (emph_classes == NULL) {
        return;
    }
    size_t i = 0;
    while (emph_classes[i] != NULL) {
        free((void *)emph_classes[i]);
        i++;
    }
    free((void *)emph_classes);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the fuzz input
    char *table_name = (char *)malloc(Size + 1);
    if (table_name == NULL) {
        return 0;
    }
    memcpy(table_name, Data, Size);
    table_name[Size] = '\0';

    // First call to lou_getTable
    const void *table1 = lou_getTable(table_name);
    (void)table1; // Suppress unused variable warning if not used further

    // Second call to lou_getTable
    const void *table2 = lou_getTable(table_name);
    (void)table2;

    // Third call to lou_getTable
    const void *table3 = lou_getTable(table_name);
    (void)table3;

    // Call to lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(table_name);
    if (emph_classes != NULL) {
        // Optionally, we could iterate and process the classes here
        // For fuzzing, we just ensure we can free them safely
        safe_free_emph_classes(emph_classes);
    }

    // Cleanup
    lou_free();

    free(table_name);
    return 0;
}