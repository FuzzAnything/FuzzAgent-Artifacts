// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 
 * Helper to create a dummy file for curl_mime_filedata testing.
 * Returns 0 on success, non-zero on failure.
 */
static int create_dummy_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 1;
    }
    const char *dummy_content = "This is dummy content for fuzzing curl MIME functions.\n";
    size_t written = fwrite(dummy_content, 1, strlen(dummy_content), fp);
    fclose(fp);
    return (written == strlen(dummy_content)) ? 0 : 1;
}

/*
 * Helper to parse the input data into strings for fuzzing.
 * Returns an array of strings (char**) and sets count.
 * The caller must free the array and each string.
 */
static char** parse_input_strings(const uint8_t *Data, size_t Size, size_t *count) {
    if (Size == 0) {
        *count = 0;
        return NULL;
    }

    // Estimate max number of strings (at least 1 byte per string + null terminator)
    size_t max_strings = Size / 2;
    if (max_strings == 0) max_strings = 1;

    char **strings = (char **)malloc(max_strings * sizeof(char *));
    if (!strings) {
        *count = 0;
        return NULL;
    }

    size_t idx = 0;
    size_t i = 0;
    *count = 0;

    while (i < Size && idx < max_strings) {
        // Find the end of the current string (null terminator or end of buffer)
        size_t len = 0;
        while (i + len < Size && Data[i + len] != '\0') {
            len++;
        }
        
        if (len > 0) {
            strings[idx] = (char *)malloc(len + 1);
            if (strings[idx]) {
                memcpy(strings[idx], &Data[i], len);
                strings[idx][len] = '\0';
                idx++;
            }
        }
        
        // Move past the null terminator if present
        if (i + len < Size && Data[i + len] == '\0') {
            i += len + 1;
        } else {
            i += len;
            break; // End of buffer without null terminator
        }
    }

    *count = idx;
    return strings;
}

/*
 * Cleanup function for strings array.
 */
static void free_strings(char **strings, size_t count) {
    if (!strings) return;
    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment (optional but good practice)
    // curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a dummy file for file-based operations
    const char *dummy_file_path = "./dummy_file";
    if (create_dummy_file(dummy_file_path) != 0) {
        // If file creation fails, we can still test other functions
        // but skip file-specific ones
    }

    // Create a MIME structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    // Create a MIME part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    // Parse input data into strings for fuzzing
    size_t str_count;
    char **strings = parse_input_strings(Data, Size, &str_count);

    // Helper to get a string safely
    #define GET_STR(idx) ((idx < str_count && strings[idx]) ? strings[idx] : NULL)

    // Fuzz curl_mime_filename
    // Try with various strings, NULL, and potentially invalid pointers (though we use valid ones here)
    for (size_t i = 0; i < str_count; i++) {
        curl_mime_filename(part, GET_STR(i));
    }
    // Test with NULL
    curl_mime_filename(part, NULL);

    // Fuzz curl_mime_encoder
    // Common encodings: "base64", "quoted-printable", "7bit", "8bit", "binary"
    // We use input strings as encoding names to test error handling for unknown encodings
    for (size_t i = 0; i < str_count; i++) {
        curl_mime_encoder(part, GET_STR(i));
    }
    // Test with NULL to remove encoding
    curl_mime_encoder(part, NULL);

    // Fuzz curl_mime_name
    for (size_t i = 0; i < str_count; i++) {
        curl_mime_name(part, GET_STR(i));
    }
    // Test with NULL to clear name
    curl_mime_name(part, NULL);

    // Fuzz curl_mime_type
    // Common types: "text/plain", "application/json", "image/png", etc.
    // Input strings will test various type strings
    for (size_t i = 0; i < str_count; i++) {
        curl_mime_type(part, GET_STR(i));
    }
    // Test with NULL to clear type
    curl_mime_type(part, NULL);

    // Fuzz curl_mime_filedata
    // Use the dummy file and also input strings as potential filenames
    // Note: curl_mime_filedata will fail if the file doesn't exist, which is expected for fuzzing
    curl_mime_filedata(part, dummy_file_path);
    for (size_t i = 0; i < str_count; i++) {
        // Try using input strings as filenames
        curl_mime_filedata(part, GET_STR(i));
    }

    // Cleanup strings
    free_strings(strings, str_count);

    // Cleanup MIME structure
    curl_mime_free(mime);

    // Cleanup dummy file
    remove(dummy_file_path);

    // Cleanup curl global environment (optional)
    // curl_global_cleanup();

    return 0;
}