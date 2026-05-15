// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/urlapi.h"
#include <stdlib.h>
#include <string.h>

static void fuzz_url_set_get(CURLU *handle, const uint8_t *data, size_t size, CURLUPart part, unsigned int flags) {
    char *str = NULL;
    CURLUcode res;
    
    if (size > 0) {
        str = (char *)malloc(size + 1);
        if (!str) return;
        memcpy(str, data, size);
        str[size] = '\0';
        
        res = curl_url_set(handle, part, str, flags);
        const char *err_str = curl_url_strerror(res);
        (void)err_str;
        
        free(str);
    } else {
        res = curl_url_set(handle, part, NULL, flags);
        const char *err_str = curl_url_strerror(res);
        (void)err_str;
    }
    
    char *part_str = NULL;
    res = curl_url_get(handle, part, &part_str, flags);
    const char *err_str2 = curl_url_strerror(res);
    (void)err_str2;
    
    if (part_str) {
        curl_free(part_str);
    }
}

static void fuzz_with_different_parts(CURLU *handle, const uint8_t *data, size_t size) {
    unsigned int flags[] = {0, CURLU_URLDECODE, CURLU_URLENCODE, CURLU_DEFAULT_SCHEME, 
                           CURLU_NON_SUPPORT_SCHEME, CURLU_PATH_AS_IS, CURLU_DISALLOW_USER,
                           CURLU_GUESS_SCHEME, CURLU_NO_AUTHORITY};
    size_t flags_count = sizeof(flags) / sizeof(flags[0]);
    
    CURLUPart parts[] = {CURLUPART_URL, CURLUPART_SCHEME, CURLUPART_USER, CURLUPART_PASSWORD,
                        CURLUPART_OPTIONS, CURLUPART_HOST, CURLUPART_PORT, CURLUPART_PATH,
                        CURLUPART_QUERY, CURLUPART_FRAGMENT};
    size_t parts_count = sizeof(parts) / sizeof(parts[0]);
    
    for (size_t i = 0; i < parts_count; i++) {
        for (size_t j = 0; j < flags_count; j++) {
            fuzz_url_set_get(handle, data, size, parts[i], flags[j]);
        }
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    CURLU *handle = curl_url();
    if (!handle) return 0;
    
    fuzz_with_different_parts(handle, Data, Size);
    
    CURLU *dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        fuzz_with_different_parts(dup_handle, Data, Size);
        curl_url_cleanup(dup_handle);
    }
    
    curl_url_cleanup(handle);
    return 0;
}