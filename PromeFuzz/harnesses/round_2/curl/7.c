// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *dummy_malloc(size_t size) {
    return malloc(size);
}

static void dummy_free(void *ptr) {
    free(ptr);
}

static void *dummy_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *dummy_strdup(const char *str) {
    return strdup(str);
}

static void *dummy_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    unsigned char selector = Data[0];
    Data++;
    Size--;

    long flags = 0;
    if (Size >= sizeof(long)) {
        memcpy(&flags, Data, sizeof(long));
        Data += sizeof(long);
        Size -= sizeof(long);
    }

    CURLversion version_stamp = CURLVERSION_NOW;
    if (Size >= sizeof(CURLversion)) {
        memcpy(&version_stamp, Data, sizeof(CURLversion));
        Data += sizeof(CURLversion);
        Size -= sizeof(CURLversion);
    }

    char config[256];
    if (Size > 0) {
        size_t config_len = Size < 255 ? Size : 255;
        memcpy(config, Data, config_len);
        config[config_len] = '\0';
    } else {
        config[0] = '\0';
    }

    CURLcode init_result = curl_global_init(flags);
    if (init_result != CURLE_OK) {
        curl_global_cleanup();
        return 0;
    }

    switch (selector % 6) {
        case 0:
            curl_global_cleanup();
            break;
        case 1: {
            char *version = curl_version();
            (void)version;
            curl_global_cleanup();
            break;
        }
        case 2:
            curl_global_cleanup();
            break;
        case 3:
            curl_global_trace(config);
            curl_global_cleanup();
            break;
        case 4: {
            curl_version_info_data *info = curl_version_info(version_stamp);
            (void)info;
            curl_global_cleanup();
            break;
        }
        case 5:
            curl_global_init_mem(flags, dummy_malloc, dummy_free,
                                 dummy_realloc, dummy_strdup, dummy_calloc);
            curl_global_cleanup();
            break;
    }

    return 0;
}