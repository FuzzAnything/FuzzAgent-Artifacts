// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_version_info at version.c:596:25 in curl.h
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

static void *dummy_malloc(size_t size) { return malloc(size); }
static void dummy_free(void *ptr) { free(ptr); }
static void *dummy_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char *dummy_strdup(const char *str) { return strdup(str); }
static void *dummy_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    switch (Data[0] % 6) {
        case 0: {
            curl_global_cleanup();
            break;
        }
        case 1: {
            curl_sslbackend id = (curl_sslbackend)Data[1];
            const char *name = NULL;
            const curl_ssl_backend ***avail = NULL;
            if (Size > 2) {
                char *name_buf = (char *)malloc(Size - 1);
                if (name_buf) {
                    memcpy(name_buf, Data + 2, Size - 2);
                    name_buf[Size - 2] = '\0';
                    name = name_buf;
                    curl_global_sslset(id, name, avail);
                    free(name_buf);
                }
            } else {
                curl_global_sslset(id, name, avail);
            }
            break;
        }
        case 2: {
            long flags = 0;
            if (Size >= 5) {
                memcpy(&flags, Data + 1, 4);
            } else if (Size >= 2) {
                flags = Data[1];
            }
            curl_global_init(flags);
            curl_global_cleanup();
            break;
        }
        case 3: {
            if (Size > 1) {
                char *config = (char *)malloc(Size);
                if (config) {
                    memcpy(config, Data + 1, Size - 1);
                    config[Size - 1] = '\0';
                    curl_global_trace(config);
                    free(config);
                }
            }
            break;
        }
        case 4: {
            CURLversion stamp = CURLVERSION_FIRST;
            if (Size >= 5) {
                unsigned int ver;
                memcpy(&ver, Data + 1, 4);
                stamp = (CURLversion)ver;
            } else if (Size >= 2) {
                stamp = (CURLversion)Data[1];
            }
            curl_version_info(stamp);
            break;
        }
        case 5: {
            long flags = 0;
            if (Size >= 5) {
                memcpy(&flags, Data + 1, 4);
            } else if (Size >= 2) {
                flags = Data[1];
            }
            curl_global_init_mem(flags, dummy_malloc, dummy_free,
                                 dummy_realloc, dummy_strdup, dummy_calloc);
            curl_global_cleanup();
            break;
        }
    }

    return 0;
}