// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Helper function to extract a null-terminated string from the fuzz input.
 * Returns a pointer to the extracted string or NULL if not found.
 * Advances the offset to the next byte after the null terminator.
 */
static const char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    size_t start = *offset;
    if (start >= size) return NULL;

    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    if (*offset >= size) return NULL; // No null terminator found

    size_t len = *offset - start;
    if (len == 0) {
        (*offset)++; // Skip the null terminator for empty string
        return "";
    }

    (*offset)++; // Skip the null terminator
    return (const char *)(data + start);
}

/*
 * Helper function to extract an integer from the fuzz input.
 * Reads up to 4 bytes and interprets them as a signed 32-bit integer.
 */
static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        // If not enough data, just return a random-ish value based on remaining bytes
        int val = 0;
        size_t remaining = size - *offset;
        if (remaining > 0) val = data[*offset];
        (*offset) += remaining;
        return val;
    }
    
    int val = (int)data[*offset] | (data[*offset+1] << 8) | 
              (data[*offset+2] << 16) | (data[*offset+3] << 24);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t offset = 0;
    const char *zDatabase = NULL;
    const char *zJournal = NULL;
    const char *zWal = NULL;
    int nParam = 0;
    const char **azParam = NULL;

    // 1. Extract Database, Journal, and WAL filenames
    // We try to extract up to 3 strings for D, J, W
    if (offset < Size) {
        zDatabase = extract_string(Data, Size, &offset);
    }
    if (offset < Size) {
        zJournal = extract_string(Data, Size, &offset);
    }
    if (offset < Size) {
        zWal = extract_string(Data, Size, &offset);
    }

    // Ensure non-NULL for create_filename if we have data, otherwise use empty strings
    // The API requires D, J, W not to be NULL, but they can be empty strings.
    if (zDatabase == NULL) zDatabase = "";
    if (zJournal == NULL) zJournal = "";
    if (zWal == NULL) zWal = "";

    // 2. Extract number of parameters
    if (offset < Size) {
        nParam = extract_int(Data, Size, &offset);
        // Clamp nParam to a reasonable range to avoid allocation issues
        if (nParam < 0) nParam = 0;
        if (nParam > 50) nParam = 50; 
    }

    // 3. Extract parameter array (2*N pointers)
    if (nParam > 0) {
        azParam = (const char **)malloc(nParam * 2 * sizeof(const char *));
        if (azParam == NULL) return 0;

        for (int i = 0; i < nParam * 2; i++) {
            const char *s = extract_string(Data, Size, &offset);
            if (s == NULL) {
                // If we run out of data, use empty string
                azParam[i] = "";
            } else {
                azParam[i] = s;
            }
        }
    }

    // 4. Invoke sqlite3_create_filename
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);

    // 5. Test various API functions with the created filename
    if (filename != NULL) {
        // Test sqlite3_filename_database
        const char *db = sqlite3_filename_database(filename);
        (void)db; // Suppress unused variable warning

        // Test sqlite3_uri_key with various indices
        int test_idx = 0;
        if (offset < Size) test_idx = extract_int(Data, Size, &offset);
        
        // Try positive and negative indices
        for (int i = -2; i <= 5; i++) {
            const char *key = sqlite3_uri_key(filename, i);
            (void)key;
        }

        // Test sqlite3_uri_parameter
        if (offset < Size) {
            const char *param_name = extract_string(Data, Size, &offset);
            if (param_name == NULL) param_name = "";
            const char *val = sqlite3_uri_parameter(filename, param_name);
            (void)val;
        }

        // Test sqlite3_uri_boolean
        if (offset < Size) {
            const char *bool_param = extract_string(Data, Size, &offset);
            if (bool_param == NULL) bool_param = "";
            int default_val = 0;
            if (offset < Size) default_val = extract_int(Data, Size, &offset);
            int result = sqlite3_uri_boolean(filename, bool_param, default_val);
            (void)result;
        }
    }

    // 6. Cleanup
    if (azParam != NULL) {
        free((void *)azParam);
    }
    
    if (filename != NULL) {
        sqlite3_free_filename(filename);
    }

    return 0;
}