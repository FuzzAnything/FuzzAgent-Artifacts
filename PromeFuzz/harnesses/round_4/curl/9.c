// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_set_url_part(CURLU *url, CURLUPart part, const uint8_t *data, size_t size, unsigned int flags) {
    if (size == 0) {
        curl_url_set(url, part, NULL, flags);
        return;
    }
    
    char *str = (char*)malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    curl_url_set(url, part, str, flags);
    free(str);
}

static void fuzz_get_url_part(CURLU *url, CURLUPart part, unsigned int flags) {
    char *result = NULL;
    CURLUcode res = curl_url_get(url, part, &result, flags);
    
    if (res == CURLUE_OK && result) {
        curl_free(result);
    }
    
    const char *err_str = curl_url_strerror(res);
    (void)err_str;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLU *url = curl_url();
    if (!url) return 0;
    
    unsigned int flags = 0;
    
    uint8_t part_selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (part_selector) {
        case 0:
            fuzz_set_url_part(url, CURLUPART_SCHEME, Data + offset, Size - offset, flags);
            break;
        case 1:
            fuzz_set_url_part(url, CURLUPART_HOST, Data + offset, Size - offset, flags);
            break;
        case 2:
            fuzz_set_url_part(url, CURLUPART_PATH, Data + offset, Size - offset, flags);
            break;
        case 3:
            fuzz_set_url_part(url, CURLUPART_QUERY, Data + offset, Size - offset, flags);
            break;
        case 4:
            fuzz_set_url_part(url, CURLUPART_FRAGMENT, Data + offset, Size - offset, flags);
            break;
        case 5:
            fuzz_set_url_part(url, CURLUPART_URL, Data + offset, Size - offset, flags);
            break;
    }
    
    fuzz_get_url_part(url, CURLUPART_SCHEME, CURLU_URLDECODE);
    fuzz_get_url_part(url, CURLUPART_HOST, CURLU_URLDECODE);
    fuzz_get_url_part(url, CURLUPART_PATH, CURLU_URLDECODE);
    fuzz_get_url_part(url, CURLUPART_QUERY, CURLU_URLDECODE);
    fuzz_get_url_part(url, CURLUPART_FRAGMENT, CURLU_URLDECODE);
    
    CURLU *dup_url = curl_url_dup(url);
    if (dup_url) {
        fuzz_get_url_part(dup_url, CURLUPART_SCHEME, 0);
        fuzz_get_url_part(dup_url, CURLUPART_HOST, 0);
        curl_url_cleanup(dup_url);
    }
    
    curl_url_cleanup(url);
    return 0;
}