// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/urlapi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_url_operations(const uint8_t *Data, size_t Size) {
    CURLU *handle = NULL;
    CURLU *dup_handle = NULL;
    char *part = NULL;
    CURLUcode res;
    int i;

    // 1. Create a new handle
    handle = curl_url();
    if (!handle) {
        return;
    }

    // 2. Set various URL parts using fuzz data
    // We try to set different parts to trigger different code paths
    // Use a simple loop to iterate through parts if data is available
    // Note: CURLUPart enum values are typically 0-10, we try a few common ones
    const char *test_strings[] = {
        "http", "https", "ftp", "user", "pass", "example.com", "8080", 
        "/path", "query=1", "fragment"
    };
    int num_strings = sizeof(test_strings) / sizeof(test_strings[0]);

    for (i = 0; i < num_strings && Size > 0; i++) {
        // Use fuzz data to decide flags (0 or 1)
        unsigned int flags = (Size > 0) ? (Data[0] & 1) : 0;
        
        // Try setting different parts
        // We map i to a specific CURLUPart. 
        // CURLUPART_SCHEME=0, USER=1, PASSWORD=2, OPTIONS=3, HOST=4, 
        // ZONEID=5, PORT=6, PATH=7, QUERY=8, FRAGMENT=9
        res = curl_url_set(handle, (CURLUPart)(i % 10), test_strings[i], flags);
        if (res != CURLUE_OK) {
            // Continue even on error to explore more states
        }
        
        // Consume some data to vary input for next iteration
        if (Size > 0) {
            Data++;
            Size--;
        }
    }

    // 3. Try to duplicate the handle
    dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        // Try to get parts from the duplicate
        for (i = 0; i < 10; i++) {
            res = curl_url_get(dup_handle, (CURLUPart)i, &part, 0);
            if (res == CURLUE_OK && part) {
                // part is allocated by curl, must be freed
                curl_free(part);
                part = NULL;
            }
        }
        
        // Try to set parts on the duplicate
        for (i = 0; i < 5 && Size > 0; i++) {
            unsigned int flags = (Data[0] & 1);
            res = curl_url_set(dup_handle, (CURLUPart)(i % 10), test_strings[i % num_strings], flags);
            if (Size > 0) { Data++; Size--; }
        }
        
        curl_url_cleanup(dup_handle);
        dup_handle = NULL;
    }

    // 4. Try to get parts from the original handle
    for (i = 0; i < 10; i++) {
        res = curl_url_get(handle, (CURLUPart)i, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        }
    }

    // 5. Test error string conversion
    // Try a range of error codes
    for (i = 0; i < 20; i++) {
        const char *str = curl_url_strerror((CURLUcode)i);
        (void)str; // Use the string to avoid unused warning
    }

    // 6. Cleanup
    curl_url_cleanup(handle);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_url_operations(Data, Size);

    return 0;
}