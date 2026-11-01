// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Helper function to extract a null-terminated string from the fuzz input.
 * Returns a pointer to a string within the input buffer or a static empty string.
 * Ensures we don't read past the buffer bounds by explicitly null-terminating.
 */
static const char *extract_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        return "";
    }
    const char *start = (const char *)(Data + *offset);
    size_t len = 0;
    while (*offset + len < Size && Data[*offset + len] != 0) {
        len++;
    }
    
    // If we hit the end of the buffer without finding a null terminator,
    // we must ensure the string is valid for strlen by returning a safe copy
    // or handling the unterminated case. However, since we cannot modify
    // the input buffer directly to add a null terminator safely without
    // copying, we will copy the data to a static buffer if it's unterminated.
    // A simpler approach for fuzzing is to ensure the returned pointer
    // points to a null-terminated string.
    
    if (*offset + len < Size) {
        // Null terminator found within bounds
        *offset += len + 1;
        return start;
    } else {
        // No null terminator found; we must create a safe, null-terminated string.
        // We use a static buffer for small strings or malloc for larger ones.
        // To keep it simple and safe for fuzzing, we'll use a static buffer
        // if the length is reasonable, otherwise return empty string to avoid
        // complex allocation logic in the helper.
        static char static_buf[256];
        if (len < sizeof(static_buf)) {
            memcpy(static_buf, start, len);
            static_buf[len] = '\0';
            *offset += len; // Consume the data up to the end
            return static_buf;
        } else {
            // If too long, treat as empty to prevent huge allocations
            *offset += len;
            return "";
        }
    }
}

/*
 * Helper function to extract a non-empty string for URI keys.
 * Ensures the key is not empty to satisfy sqlite3_create_filename constraints.
 */
static const char *extract_key(const uint8_t *Data, size_t Size, size_t *offset) {
    const char *s = extract_string(Data, Size, offset);
    if (s[0] == '\0') {
        // Return a dummy valid key if the extracted one is empty
        return "dummy";
    }
    return s;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;

    // Extract components for sqlite3_create_filename
    // zDatabase, zJournal, zWal
    const char *zDatabase = extract_string(Data, Size, &offset);
    const char *zJournal = extract_string(Data, Size, &offset);
    const char *zWal = extract_string(Data, Size, &offset);

    // Extract number of parameters (nParam)
    // We limit this to a reasonable number to avoid excessive allocation
    int nParam = 0;
    if (offset < Size) {
        nParam = (int)Data[offset] % 10; // Limit to 0-9 pairs
        offset++;
    }

    // Prepare parameter array
    // We need an array of 2*nParam pointers
    const char **azParam = NULL;
    if (nParam > 0) {
        azParam = (const char **)malloc(sizeof(const char *) * 2 * nParam);
        if (!azParam) {
            return 0;
        }
        for (int i = 0; i < nParam; i++) {
            // Key
            azParam[2 * i] = extract_key(Data, Size, &offset);
            // Value
            azParam[2 * i + 1] = extract_string(Data, Size, &offset);
        }
    }

    // Create the filename object
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);

    if (filename) {
        // Test sqlite3_filename_database
        const char *db_name = sqlite3_filename_database(filename);
        (void)db_name; // Suppress unused variable warning

        // Test sqlite3_filename_journal
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;

        // Test sqlite3_filename_wal
        const char *wal_name = sqlite3_filename_wal(filename);
        (void)wal_name;

        // Test sqlite3_uri_parameter with various parameter names
        // We try to extract a parameter name from the fuzz input
        const char *param_name = extract_string(Data, Size, &offset);
        const char *param_value = sqlite3_uri_parameter(filename, param_name);
        (void)param_value;

        // Test with a hardcoded parameter name to ensure coverage
        const char *hardcoded_param = "mode";
        const char *hardcoded_value = sqlite3_uri_parameter(filename, hardcoded_param);
        (void)hardcoded_value;
    }

    // Cleanup
    if (azParam) {
        free((void *)azParam);
    }
    if (filename) {
        sqlite3_free_filename(filename);
    }

    return 0;
}