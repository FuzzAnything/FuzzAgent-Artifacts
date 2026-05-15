// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create a null-terminated query string from fuzz input
    char *query = NULL;
    if (Size > 0) {
        query = (char *)malloc(Size + 1);
        if (!query) return 0;
        memcpy(query, Data, Size);
        query[Size] = '\0';
    } else {
        query = strdup("");
    }

    // Clear any existing index first
    const char *empty_tables[] = {NULL};
    lou_indexTables(empty_tables);

    // Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list) {
        lou_freeTableFiles(table_list);
    }

    // Index with a dummy table file
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        const char *tables[] = {"./dummy_file", NULL};
        lou_indexTables(tables);
        remove("./dummy_file");
    }

    // Test lou_findTable with the query
    char *found_table = lou_findTable(query);
    if (found_table) {
        lou_freeTableFile(found_table);
    }

    // Test lou_findTables with the query
    char **found_tables = lou_findTables(query);
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }

    // Clean up
    free(query);
    
    // Clear index again
    lou_indexTables(empty_tables);

    return 0;
}