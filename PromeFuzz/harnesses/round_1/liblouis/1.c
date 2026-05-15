// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_setLogLevel at logging.c:143:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for table indexing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Set log level (use a valid level from enum)
    lou_setLogLevel(3); // INFO level

    // Create proper NULL-terminated array for lou_indexTables
    const char *tables[] = {"./dummy_file", NULL};
    lou_indexTables(tables);

    // Create null-terminated query string from fuzzer input
    char *query = malloc(Size + 1);
    if (!query) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // First lou_findTable call
    char *table1 = lou_findTable(query);
    if (table1) free(table1);

    // Second lou_findTable call (same query)
    char *table2 = lou_findTable(query);
    if (table2) free(table2);

    // lou_findTables call
    char **tables_list = lou_findTables(query);
    if (tables_list) {
        for (char **ptr = tables_list; *ptr; ptr++) {
            free(*ptr);
        }
        free(tables_list);
    }

    // Cleanup
    free(query);
    lou_free();

    return 0;
}