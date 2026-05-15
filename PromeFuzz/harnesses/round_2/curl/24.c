// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/urlapi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_url_set(CURLU *handle, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine which part to set
    CURLUPart parts[] = {
        CURLUPART_URL,
        CURLUPART_SCHEME,
        CURLUPART_USER,
        CURLUPART_PASSWORD,
        CURLUPART_HOST,
        CURLUPART_PORT,
        CURLUPART_PATH,
        CURLUPART_QUERY,
        CURLUPART_FRAGMENT
    };
    
    CURLUPart part = parts[data[0] % (sizeof(parts)/sizeof(parts[0]))];
    unsigned int flags = 0;
    
    // Use second byte for flags if available
    if (size > 1) {
        flags = data[1] & (CURLU_URLDECODE | CURLU_URLENCODE | CURLU_APPENDQUERY);
    }
    
    // Create a null-terminated string from remaining data
    size_t str_len = size > 2 ? size - 2 : 0;
    char *str = NULL;
    if (str_len > 0) {
        str = (char*)malloc(str_len + 1);
        if (str) {
            memcpy(str, data + 2, str_len);
            str[str_len] = '\0';
        }
    }
    
    CURLUcode res = curl_url_set(handle, part, str, flags);
    const char *error_str = curl_url_strerror(res);
    
    if (str) free(str);
}

static void fuzz_url_get(CURLU *handle, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to determine which part to get
    CURLUPart parts[] = {
        CURLUPART_URL,
        CURLUPART_SCHEME,
        CURLUPART_USER,
        CURLUPART_PASSWORD,
        CURLUPART_HOST,
        CURLUPART_PORT,
        CURLUPART_PATH,
        CURLUPART_QUERY,
        CURLUPART_FRAGMENT
    };
    
    CURLUPart part = parts[data[0] % (sizeof(parts)/sizeof(parts[0]))];
    unsigned int flags = 0;
    
    // Use second byte for flags if available
    if (size > 1) {
        flags = data[1] & (CURLU_URLDECODE | CURLU_URLENCODE | CURLU_APPENDQUERY);
    }
    
    char *part_str = NULL;
    CURLUcode res = curl_url_get(handle, part, &part_str, flags);
    const char *error_str = curl_url_strerror(res);
    
    if (part_str) {
        curl_free(part_str);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create initial handle
    CURLU *url = curl_url();
    if (!url) return 0;
    
    // Set some URL parts using fuzz data
    fuzz_url_set(url, Data, Size);
    
    // Duplicate the handle
    CURLU *dup_url = curl_url_dup(url);
    if (dup_url) {
        // Get URL parts from both handles
        fuzz_url_get(url, Data, Size);
        fuzz_url_get(dup_url, Data, Size);
        
        // Clean up duplicate
        curl_url_cleanup(dup_url);
    }
    
    // Try getting URL parts from original handle again
    fuzz_url_get(url, Data, Size);
    
    // Clean up original handle
    curl_url_cleanup(url);
    
    return 0;
}