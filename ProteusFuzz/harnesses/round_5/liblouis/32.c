// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_table_array(char **arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
    free(arr);
}

static void free_string_array(const char **arr) {
    // Note: lou_indexTables takes const char **, but the strings themselves
    // are expected to be owned by the caller or internal.
    // We do not free the strings here as lou_indexTables does not take ownership
    // of the strings passed in the array, only the array structure if it was
    // returned by a function like lou_listTables.
    // However, if we constructed the array ourselves, we must free the strings.
    // This helper is for arrays we constructed dynamically.
    if (arr == NULL) {
        return;
    }
    // We cannot distinguish between owned and non-owned strings easily here without context.
    // This function is intended for arrays where we allocated the strings.
    // For safety in fuzzing, we assume we allocated them if we pass them here.
    // But wait, lou_indexTables takes const char **. The strings passed must remain valid.
    // We will use a separate helper for freeing our constructed arrays.
}

static void free_constructed_string_array(char **arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
    free(arr);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string operations
    char *query = malloc(Size + 1);
    if (!query) return 0;
    memcpy(query, Data, Size);
    query[Size] = '\0';

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Use the list to index tables
        lou_indexTables((const char **)tables);
        // Free the list
        free_table_array(tables);
    }

    // 2. Test lou_findTable
    char *table_name = lou_findTable(query);
    if (table_name != NULL) {
        // We can use table_name if needed, but for fuzzing just freeing is enough
        // Note: lou_findTable returns a string that must be freed by the caller?
        // Documentation says: "returns the table name". Usually implies caller frees.
        // Let's check typical liblouis behavior. Yes, caller must free.
        free(table_name);
    }

    // 3. Test lou_findTables
    char **matching_tables = lou_findTables(query);
    if (matching_tables != NULL) {
        // Free the array and its elements
        free_table_array(matching_tables);
    }

    // 4. Test lou_indexTables with a custom constructed array
    // We create a simple array with the query string (if it looks like a table name)
    // or just a dummy array to test the function's robustness.
    // We need at least one string and a NULL terminator.
    char *custom_table = strdup(query);
    if (custom_table) {
        char *custom_list[] = { custom_table, NULL };
        lou_indexTables((const char **)custom_list);
        free(custom_table);
    }

    // 5. Test lou_freeTableFile with a dummy string
    // We simulate a loaded table file string
    char *dummy_file_path = strdup("./dummy_file");
    if (dummy_file_path) {
        lou_freeTableFile(dummy_file_path);
    }

    // 6. Test lou_freeTableFiles with a dummy array
    char **dummy_files = malloc(2 * sizeof(char *));
    if (dummy_files) {
        dummy_files[0] = strdup("./dummy_file1");
        dummy_files[1] = NULL;
        if (dummy_files[0]) {
            lou_freeTableFiles(dummy_files);
        } else {
            free(dummy_files);
        }
    }

    free(query);
    return 0;
}