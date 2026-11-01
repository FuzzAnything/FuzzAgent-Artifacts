// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/mprintf.h>

static void safe_curl_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

/* Helper function to test curl_mvsprintf with va_list */
static int test_mvsprintf_helper(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLU *handle = NULL;
    CURLUcode res;
    char *part = NULL;
    char buffer[4096];
    char *safe_input = NULL;
    int i;
    int ret;

    // Initialize a safe string from fuzz input for string operations
    safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    // 1. Test curl_url_set
    // Create a handle first
    handle = curl_url();
    if (handle) {
        // Try setting various parts with fuzz data
        // We use a subset of CURLUPart values to avoid undefined behavior
        // 0: Scheme, 1: User, 2: Password, 3: Options, 4: Host, 5: Zoneid, 
        // 6: Port, 7: Path, 8: Query, 9: Fragment
        for (i = 0; i <= 9; i++) {
            res = curl_url_set(handle, (CURLUPart)i, safe_input, 0);
            (void)res; // Ignore errors, just exercise the code path
        }

        // Try setting with NULL to clear
        for (i = 0; i <= 9; i++) {
            res = curl_url_set(handle, (CURLUPart)i, NULL, 0);
            (void)res;
        }

        // Try with flags (CURLU_URLENCODE is typically 1)
        res = curl_url_set(handle, CURLUPART_SCHEME, safe_input, 1);
        (void)res;
    }

    // 2. Test curl_url_get
    if (handle) {
        for (i = 0; i <= 9; i++) {
            res = curl_url_get(handle, (CURLUPart)i, &part, 0);
            if (res == CURLUE_OK && part) {
                safe_curl_free(part);
                part = NULL;
            }
        }
        // Try with flags
        res = curl_url_get(handle, CURLUPART_HOST, &part, 1);
        if (res == CURLUE_OK && part) {
            safe_curl_free(part);
            part = NULL;
        }
    }

    // 3. Test curl_url_strerror
    // Iterate through a range of error codes
    for (i = 0; i < 20; i++) {
        const char *str = curl_url_strerror((CURLUcode)i);
        (void)str; // Use the string to avoid compiler warnings
    }

    // 4. Test curl_maprintf
    // Ensure safe_input is used as %s
    char *mapped_str = curl_maprintf("prefix-%s-suffix", safe_input);
    if (mapped_str) {
        safe_curl_free(mapped_str);
    }
    
    // Test with integer format
    mapped_str = curl_maprintf("num:%d", (int)(Size % 1000));
    if (mapped_str) {
        safe_curl_free(mapped_str);
    }

    // 5. Test curl_msnprintf
    ret = curl_msnprintf(buffer, sizeof(buffer), "msg:%s", safe_input);
    (void)ret;

    // 6. Test curl_mvsprintf using the helper function
    ret = test_mvsprintf_helper(buffer, "va:%s", safe_input);
    (void)ret;

    // Cleanup
    if (handle) {
        curl_url_cleanup(handle);
    }
    if (part) {
        safe_curl_free(part);
    }
    if (safe_input) {
        free(safe_input);
    }

    return 0;
}