// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/urlapi.h>

static void fuzz_url_components(CURLU *handle, const uint8_t *data, size_t size) {
    if (!handle || !data || size == 0) {
        return;
    }

    char *part = NULL;
    CURLUcode res;

    // Fuzz curl_url_get with various parts
    for (int i = 0; i < 10 && i < (int)size; i++) {
        CURLUPart what = (CURLUPart)(data[i] % 10); // Limit to valid range roughly
        unsigned int flags = (data[(i + 1) % size] & 0x03); // Limit flags
        
        res = curl_url_get(handle, what, &part, flags);
        if (res == CURLUE_OK && part) {
            // Use the part briefly to ensure it's valid
            size_t len = strlen(part);
            if (len > 0) {
                // Do nothing, just ensure no crash
            }
            curl_free(part);
            part = NULL;
        }
    }

    // Fuzz curl_url_set with various parts
    for (int i = 0; i < 10 && i < (int)size; i++) {
        CURLUPart what = (CURLUPart)(data[i] % 10);
        unsigned int flags = (data[(i + 1) % size] & 0x03);
        
        // Create a small buffer from input data for the part string
        // Ensure we have enough data before calculating len
        if (i + 3 >= size) break;
        
        size_t len = (data[(i + 2) % size] % 100) + 1;
        // Ensure len does not exceed remaining data
        if (len > size - (i + 3)) {
            len = size - (i + 3);
        }
        
        if (len == 0) continue;

        char *buf = malloc(len + 1);
        if (!buf) continue;
        memcpy(buf, &data[i + 3], len);
        buf[len] = '\0';

        res = curl_url_set(handle, what, buf, flags);
        
        // Also test setting NULL to clear
        res = curl_url_set(handle, what, NULL, flags);
        
        free(buf);
    }
}

static void fuzz_url_dup_and_cleanup(const uint8_t *data, size_t size) {
    CURLU *handle = curl_url();
    if (!handle) return;

    // Set some initial data to duplicate
    if (size > 0) {
        char buf[64];
        size_t len = (data[0] % 60) + 1;
        if (len > size) len = size;
        if (len > sizeof(buf) - 1) len = sizeof(buf) - 1;
        memcpy(buf, data, len);
        buf[len] = '\0';
        curl_url_set(handle, CURLUPART_URL, buf, 0);
    }

    CURLU *dup = curl_url_dup(handle);
    if (dup) {
        // Fuzz the duplicate handle
        fuzz_url_components(dup, data, size);
        curl_url_cleanup(dup);
    }

    // Fuzz the original handle
    fuzz_url_components(handle, data, size);
    curl_url_cleanup(handle);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test curl_url_strerror with various codes
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        CURLUcode code = (CURLUcode)(Data[i]);
        const char *str = curl_url_strerror(code);
        (void)str; // Avoid unused variable warning
    }

    // Main fuzzing logic
    fuzz_url_dup_and_cleanup(Data, Size);

    // Additional test: create handle, set invalid data, cleanup
    CURLU *h = curl_url();
    if (h) {
        // Try to set with potentially malformed data
        if (Size > 0) {
            char *test_str = malloc(Size + 1);
            if (test_str) {
                memcpy(test_str, Data, Size);
                test_str[Size] = '\0';
                curl_url_set(h, CURLUPART_SCHEME, test_str, 0);
                curl_url_set(h, CURLUPART_HOST, test_str, 0);
                curl_url_set(h, CURLUPART_PATH, test_str, 0);
                curl_url_set(h, CURLUPART_QUERY, test_str, 0);
                curl_url_set(h, CURLUPART_FRAGMENT, test_str, 0);
                free(test_str);
            }
        }
        curl_url_cleanup(h);
    }

    return 0;
}