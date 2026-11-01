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

static void cleanup_emph_classes(char const **emph_classes) {
    if (emph_classes == NULL) {
        return;
    }
    // The documentation says callers must free the result.
    // It returns a NULL-terminated array of strings.
    // We free each string and then the array itself.
    for (int i = 0; emph_classes[i] != NULL; i++) {
        free((void *)emph_classes[i]);
    }
    free((void *)emph_classes);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for use as a C string
    char *tableList = (char *)malloc(Size + 1);
    if (tableList == NULL) {
        return 0;
    }
    memcpy(tableList, Data, Size);
    tableList[Size] = '\0';

    // Invoke lou_getTable three times as requested
    const void *table1 = lou_getTable(tableList);
    (void)table1; // Suppress unused variable warning if not used further

    const void *table2 = lou_getTable(tableList);
    (void)table2;

    const void *table3 = lou_getTable(tableList);
    (void)table3;

    // Invoke lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(tableList);
    
    // Cleanup emph_classes if allocated
    cleanup_emph_classes(emph_classes);

    // Cleanup input buffer
    free(tableList);

    // Invoke lou_free to clean up liblouis resources
    // Note: The documentation says "Do not call mid-session", but for fuzzing
    // we call it at the end of each input to reset state and prevent leaks.
    lou_free();

    return 0;
}