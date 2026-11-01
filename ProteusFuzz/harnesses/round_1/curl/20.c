// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static void cleanup_mime(curl_mime *mime) {
    if (mime) {
        curl_mime_free(mime);
    }
}

static char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    if (*offset == start) {
        // Skip empty strings to avoid infinite loops if all zeros
        (*offset)++;
        return NULL;
    }
    size_t len = *offset - start;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + start, len);
    str[len] = '\0';
    (*offset)++; // Move past null terminator
    return str;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize curl global if needed
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a MIME structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        curl_global_cleanup();
        return 0;
    }

    // Create a MIME part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        cleanup_mime(mime);
        curl_global_cleanup();
        return 0;
    }

    size_t offset = 0;
    int operation = Data[0] % 7; // 0-6 for 6 functions + 1 extra for NULL tests

    char *str_data = NULL;

    // Extract strings based on operation needs
    if (operation <= 5) {
        str_data = extract_string(Data, Size, &offset);
    }
    if (operation == 4) { // curl_mime_data needs size too
        if (offset + 4 <= Size) {
            // Use next 4 bytes as size hint, but clamp to reasonable limits
            uint32_t size_hint = Data[offset] | (Data[offset+1] << 8) | 
                                 (Data[offset+2] << 16) | (Data[offset+3] << 24);
            offset += 4;
            // Limit size to prevent OOM
            if (size_hint > 1024 * 1024) size_hint = 1024 * 1024;
            // If we have data, use it, otherwise use dummy
            if (!str_data && size_hint > 0) {
                str_data = malloc(size_hint);
                if (str_data) {
                    memcpy(str_data, Data + offset, size_hint);
                    offset += size_hint;
                }
            }
        }
    }

    // Create dummy file for file operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    CURLcode res;

    switch (operation) {
        case 0: // curl_mime_type
            if (str_data) {
                res = curl_mime_type(part, str_data);
            } else {
                res = curl_mime_type(part, NULL);
            }
            break;
        case 1: // curl_mime_encoder
            if (str_data) {
                res = curl_mime_encoder(part, str_data);
            } else {
                res = curl_mime_encoder(part, NULL);
            }
            break;
        case 2: // curl_mime_name
            if (str_data) {
                res = curl_mime_name(part, str_data);
            } else {
                res = curl_mime_name(part, NULL);
            }
            break;
        case 3: // curl_mime_filedata
            // Always use the dummy file
            res = curl_mime_filedata(part, "./dummy_file");
            break;
        case 4: // curl_mime_data
            if (str_data) {
                // Determine size: if we parsed a size hint, use it, else use strlen
                size_t data_size = strlen(str_data);
                // If we had a specific size hint from the input, we might want to use that
                // but for safety with malloc'd strings, strlen is safer unless we track exact allocation
                res = curl_mime_data(part, str_data, data_size);
            } else {
                // Test with NULL data or CURL_ZERO_TERMINATED
                res = curl_mime_data(part, NULL, 0);
            }
            break;
        case 5: // curl_mime_filename
            if (str_data) {
                res = curl_mime_filename(part, str_data);
            } else {
                res = curl_mime_filename(part, NULL);
            }
            break;
        case 6: // Test with NULL part (should fail)
            res = curl_mime_type(NULL, "text/plain");
            break;
        default:
            break;
    }

    // Cleanup strings
    if (str_data) free(str_data);

    // Cleanup MIME structures
    // Note: curl_mime_free cleans up all parts automatically
    cleanup_mime(mime);

    // Cleanup global
    curl_global_cleanup();

    return 0;
}