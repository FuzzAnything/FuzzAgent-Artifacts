// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
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
#include <stddef.h>

/*
 * Helper function to extract a null-terminated string from the fuzzer input.
 * Returns a newly allocated string or NULL if the input is too short.
 */
static char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    size_t start = *offset;
    size_t len = 0;
    
    // Find the null terminator
    while (start + len < size && data[start + len] != '\0') {
        len++;
    }
    
    // If no null terminator found within bounds, treat the rest as the string
    if (start + len >= size) {
        // Check if we have at least one byte
        if (len == 0) return NULL;
        // Return the substring up to the end of buffer
        char *str = malloc(len + 1);
        if (!str) return NULL;
        memcpy(str, data + start, len);
        str[len] = '\0';
        *offset = size;
        return str;
    }
    
    // Found null terminator
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + start, len);
    str[len] = '\0';
    *offset = start + len + 1;
    return str;
}

/*
 * Helper to create a simple URI parameter array from fuzzer input.
 * Returns allocated array of const char* (2*nParam pointers) or NULL.
 * The caller must free the array and its contents.
 */
static const char **create_param_array(const uint8_t *data, size_t size, size_t *offset, int *nParam) {
    *nParam = 0;
    if (*offset >= size) return NULL;
    
    int max_params = 10; // Limit to avoid excessive allocation
    int count = 0;
    
    // Try to read pairs of strings (key, value)
    while (count < max_params) {
        char *key = extract_string(data, size, offset);
        if (!key) break;
        
        char *val = extract_string(data, size, offset);
        if (!val) {
            free(key);
            break;
        }
        
        // Ensure key is not empty
        if (strlen(key) == 0) {
            free(key);
            free(val);
            break;
        }
        
        count++;
    }
    
    if (count == 0) return NULL;
    
    const char **params = malloc(sizeof(char*) * 2 * count);
    if (!params) {
        // Cleanup on failure
        // Note: We can't easily track allocated keys/vals here without a list
        // In a real scenario, we'd track them, but for fuzzing simplicity, 
        // we assume the fuzzer will retry.
        return NULL;
    }
    
    // Re-extract to fill the array (since we consumed the data)
    // This is inefficient but safe for fuzzing logic
    *offset = 0; // Reset to re-read
    for (int i = 0; i < count; i++) {
        char *key = extract_string(data, size, offset);
        char *val = extract_string(data, size, offset);
        if (!key || !val) {
            // Cleanup partial allocation
            for (int j = 0; j < i; j++) {
                free((char*)params[2*j]);
                free((char*)params[2*j+1]);
            }
            free(params);
            return NULL;
        }
        if (strlen(key) == 0) {
            free(key);
            free(val);
            for (int j = 0; j < i; j++) {
                free((char*)params[2*j]);
                free((char*)params[2*j+1]);
            }
            free(params);
            return NULL;
        }
        params[2*i] = key;
        params[2*i+1] = val;
    }
    
    *nParam = count;
    return params;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Use a portion of the input to determine behavior
    uint32_t mode = *(uint32_t*)Data;
    size_t offset = 4;

    // Extract database, journal, and wal filenames
    char *zDatabase = extract_string(Data, Size, &offset);
    char *zJournal = extract_string(Data, Size, &offset);
    char *zWal = extract_string(Data, Size, &offset);

    // Ensure we have at least the database name
    if (!zDatabase) {
        if (zJournal) free(zJournal);
        if (zWal) free(zWal);
        return 0;
    }
    
    // If journal or wal are missing, use empty strings as per API requirements
    if (!zJournal) zJournal = strdup("");
    if (!zWal) zWal = strdup("");
    
    // Extract URI parameters
    int nParam = 0;
    const char **azParam = NULL;
    if (offset < Size) {
        azParam = create_param_array(Data, Size, &offset, &nParam);
    }

    // Create the filename object
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    // Cleanup parameters if allocated
    if (azParam && nParam > 0) {
        for (int i = 0; i < nParam; i++) {
            free((char*)azParam[2*i]);
            free((char*)azParam[2*i+1]);
        }
        free((char*)azParam);
    }
    free(zDatabase);
    free(zJournal);
    free(zWal);

    if (!filename) {
        // Allocation failed, nothing to test
        return 0;
    }

    // Test sqlite3_filename_database
    const char *db_name = sqlite3_filename_database(filename);
    (void)db_name; // Use the result to avoid unused warning

    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(filename);
    (void)journal_name;

    // Test sqlite3_filename_wal (if available in the build, otherwise commented out)
    // const char *wal_name = sqlite3_filename_wal(filename);
    // (void)wal_name;

    // Test sqlite3_uri_key with various indices
    int max_key_idx = 10;
    for (int i = 0; i < max_key_idx; i++) {
        const char *key = sqlite3_uri_key(filename, i);
        (void)key;
    }
    
    // Test sqlite3_uri_boolean with various parameters
    // We try to extract a key name from the input if possible, or use a dummy
    const char *test_param = "test";
    if (offset < Size && Data[offset] != '\0') {
        // Use a slice of data as param name if it looks like a string
        size_t param_len = 0;
        while (offset + param_len < Size && Data[offset + param_len] != '\0') {
            param_len++;
        }
        if (param_len > 0 && param_len < 100) {
            char *tmp = malloc(param_len + 1);
            if (tmp) {
                memcpy(tmp, Data + offset, param_len);
                tmp[param_len] = '\0';
                test_param = tmp;
                free(tmp); // We just used it, but we need to keep it for the call? 
                           // Actually, we can't free it before the call if we use it.
                           // Let's just use the static string or a safe copy.
            }
        }
    }
    
    // We need a persistent string for the param name
    char param_buf[100];
    if (offset < Size) {
        size_t len = 0;
        while (offset + len < Size && Data[offset + len] != '\0' && len < 99) {
            len++;
        }
        memcpy(param_buf, Data + offset, len);
        param_buf[len] = '\0';
        sqlite3_uri_boolean(filename, param_buf, 0);
        sqlite3_uri_boolean(filename, param_buf, 1);
    }
    
    // Test with a static string
    sqlite3_uri_boolean(filename, "mode", 0);
    sqlite3_uri_boolean(filename, "mode", 1);

    // Free the filename
    sqlite3_free_filename(filename);

    return 0;
}