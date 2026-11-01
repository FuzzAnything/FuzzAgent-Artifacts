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

static void safe_free_emph_classes(char const **classes) {
    if (classes) {
        for (int i = 0; classes[i] != NULL; i++) {
            free((void *)classes[i]);
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzz input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // First call: lou_getTable
    const void *table1 = lou_getTable(input_str);
    (void)table1; // Use return value to avoid unused warning

    // Second call: lou_getTable
    const void *table2 = lou_getTable(input_str);
    (void)table2;

    // Third call: lou_getTable
    const void *table3 = lou_getTable(input_str);
    (void)table3;

    // Fourth call: lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(input_str);
    if (emph_classes) {
        // Optionally iterate to exercise logic, but no need to store
        for (int i = 0; emph_classes[i] != NULL; i++) {
            // Just accessing to ensure validity
            (void)emph_classes[i];
        }
        safe_free_emph_classes(emph_classes);
    }

    // Fifth call: lou_free
    lou_free();

    free(input_str);
    return 0;
}