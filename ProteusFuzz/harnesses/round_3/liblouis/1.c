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
        i++;
    }
    free(emph_classes);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for use as a string
    char *tableList = (char *)malloc(Size + 1);
    if (tableList == NULL) {
        return 0;
    }
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Invoke lou_getTable three times as requested
    const void *table1 = lou_getTable(tableList);
    const void *table2 = lou_getTable(tableList);
    const void *table3 = lou_getTable(tableList);

    // Invoke lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(tableList);

    // Cleanup emphasis classes if allocated
    safe_free_emph_classes(emph_classes);

    // Free resources before exit
    lou_free();

    free(tableList);
    return 0;
}