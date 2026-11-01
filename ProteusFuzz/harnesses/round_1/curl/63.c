// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/header.h>
#include <curl/mprintf.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Fuzz curl_easy_nextheader
    struct curl_header *prev_header = NULL;
    struct curl_header *next_header = curl_easy_nextheader(handle, 0, 0, prev_header);
    (void)next_header; // Suppress unused warning

    // Fuzz curl_easy_unescape
    int outlength = 0;
    const char *input_str = (const char *)Data;
    char *unescaped = curl_easy_unescape(handle, input_str, (int)Size, &outlength);
    if (unescaped) {
        free(unescaped);
    }

    // Fuzz curl_strequal safely
    if (Size > 1) {
        char s1_buf[256];
        char s2_buf[256];
        size_t s1_len = Size > 255 ? 255 : Size;
        size_t s2_len = Size > 255 ? 255 : Size - 1;
        
        memcpy(s1_buf, Data, s1_len);
        s1_buf[s1_len] = '\0';
        
        memcpy(s2_buf, Data + 1, s2_len);
        s2_buf[s2_len] = '\0';

        int result = curl_strequal(s1_buf, s2_buf);
        (void)result; // Suppress unused warning
    }

    // Fuzz curl_mprintf
    // Create a safe, null-terminated string from fuzz input to avoid format string issues
    // and buffer overflows. Limit length to prevent large allocations.
    char safe_data[256];
    size_t safe_len = Size > 255 ? 255 : Size;
    memcpy(safe_data, Data, safe_len);
    safe_data[safe_len] = '\0';

    // Use a fixed format string to prevent format string vulnerabilities
    int mprintf_result = curl_mprintf("%s", safe_data);
    (void)mprintf_result; // Suppress unused warning

    // Fuzz curl_easy_header
    struct curl_header *hout = NULL;
    // Ensure header name is null-terminated
    char header_name[256];
    size_t name_len = Size > 255 ? 255 : Size;
    memcpy(header_name, Data, name_len);
    header_name[name_len] = '\0';
    
    CURLHcode header_result = curl_easy_header(handle, header_name, 0, 0, 0, &hout);
    (void)header_result; // Suppress unused warning

    // Fuzz curl_msprintf
    char buffer[256];
    // Use a fixed format string to prevent format string vulnerabilities
    int msprintf_result = curl_msprintf(buffer, "%s", safe_data);
    (void)msprintf_result; // Suppress unused warning

    curl_easy_cleanup(handle);
    return 0;
}