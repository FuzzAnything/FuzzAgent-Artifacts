// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void cleanup_table_info(char *info) {
    if (info != NULL) {
        lou_freeTableInfo(info);
    }
}

static void cleanup_table_file(char *table_file) {
    if (table_file != NULL) {
        lou_freeTableFile(table_file);
    }
}

static void cleanup_table_files(char **table_files) {
    if (table_files != NULL) {
        lou_freeTableFiles(table_files);
    }
}

static void cleanup_table_list(char **table_list) {
    if (table_list != NULL) {
        for (int i = 0; table_list[i] != NULL; i++) {
            free(table_list[i]);
        }
        free(table_list);
    }
}

static void cleanup_find_tables(char **found_tables) {
    if (found_tables != NULL) {
        for (int i = 0; found_tables[i] != NULL; i++) {
            free(found_tables[i]);
        }
        free(found_tables);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test lou_findTables with the input string as a query
    char **found_tables = lou_findTables(input_str);
    cleanup_find_tables(found_tables);

    // Test lou_listTables
    char **table_list = lou_listTables();
    if (table_list != NULL) {
        // Iterate through the list and test lou_getTableInfo for each table
        for (int i = 0; table_list[i] != NULL; i++) {
            char *table_name = table_list[i];
            
            // Test lou_getTableInfo with a fixed key
            char *info = lou_getTableInfo(table_name, "language");
            if (info != NULL) {
                lou_freeTableInfo(info);
            }

            // Test lou_getTableInfo with the input string as a key (if it's a valid key)
            char *info2 = lou_getTableInfo(table_name, input_str);
            if (info2 != NULL) {
                lou_freeTableInfo(info2);
            }
        }
        cleanup_table_list(table_list);
    }

    // Test lou_freeTableInfo with NULL (should be safe)
    lou_freeTableInfo(NULL);

    // Test lou_freeTableFile with NULL (should be safe)
    lou_freeTableFile(NULL);

    // Test lou_freeTableFiles with NULL (should be safe)
    lou_freeTableFiles(NULL);

    free(input_str);
    return 0;
}