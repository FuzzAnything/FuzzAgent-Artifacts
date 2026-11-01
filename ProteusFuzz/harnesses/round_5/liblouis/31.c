// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_freeTableFiles at compileTranslationTable.c:4933:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_findTables at metadata.c:1110:1 in liblouis.h
// lou_freeTableFile at metadata.c:1089:1 in liblouis.h
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

static void free_table_list(char **list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

static int create_dummy_table_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return -1;
    }
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy table file to ensure liblouis has something to index
    if (create_dummy_table_file("./dummy_file", Data, Size) != 0) {
        return 0;
    }

    // 1. Test lou_listTables
    char **tables = lou_listTables();
    if (tables != NULL) {
        // Use the list to build a query for findTables
        for (int i = 0; tables[i] != NULL; i++) {
            // Try to find tables using the name as a query
            char *found = lou_findTable(tables[i]);
            if (found != NULL) {
                free(found);
            }

            char **found_list = lou_findTables(tables[i]);
            if (found_list != NULL) {
                free_table_list(found_list);
            }
        }
        // Index the tables
        lou_indexTables((const char **)tables);
        free_table_list(tables);
    }

    // 2. Test lou_findTable with fuzzed input as query
    // Ensure the input is null-terminated for string functions
    char *query = malloc(Size + 1);
    if (query == NULL) {
        return 0;
    }
    memcpy(query, Data, Size);
    query[Size] = '\0';

    char *found_table = lou_findTable(query);
    if (found_table != NULL) {
        free(found_table);
    }

    // 3. Test lou_findTables with fuzzed input as query
    char **found_tables = lou_findTables(query);
    if (found_tables != NULL) {
        free_table_list(found_tables);
    }

    free(query);

    // 4. Test lou_freeTableFile with a dummy string (simulating loaded file content)
    // We cannot easily get a valid pointer from lou_loadTableFile without a real table,
    // but we can test the free function with a malloc'd string to ensure it doesn't crash
    // if the library's internal logic were to pass such a pointer.
    // Note: The API docs say "Do not use on stack-allocated strings" and "Passing NULL is safe".
    // We will test with NULL and a valid malloc'd string.
    char *dummy_file_content = malloc(100);
    if (dummy_file_content != NULL) {
        memset(dummy_file_content, 0, 100);
        // This simulates freeing a table file string. 
        // In a real scenario, this would be a pointer returned by lou_loadTableFile.
        // Since we don't have a real table loaded, we just test the free mechanism.
        lou_freeTableFile(dummy_file_content);
    }

    // 5. Test lou_freeTableFiles with NULL and a dummy array
    lou_freeTableFiles(NULL);
    
    char **dummy_array = malloc(3 * sizeof(char *));
    if (dummy_array != NULL) {
        dummy_array[0] = malloc(10);
        dummy_array[1] = malloc(10);
        dummy_array[2] = NULL;
        if (dummy_array[0] != NULL) memset(dummy_array[0], 0, 10);
        if (dummy_array[1] != NULL) memset(dummy_array[1], 0, 10);
        
        lou_freeTableFiles(dummy_array);
        // Note: lou_freeTableFiles frees the array itself, so we don't free dummy_array again.
    }

    return 0;
}