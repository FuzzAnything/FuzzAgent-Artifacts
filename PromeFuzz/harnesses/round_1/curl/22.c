// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version_info at version.c:596:25 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void *dummy_malloc(size_t size) { return malloc(size); }
static void dummy_free(void *ptr) { free(ptr); }
static void *dummy_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char *dummy_strdup(const char *str) { return strdup(str); }
static void *dummy_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (selector % 7) {
        case 0: {
            curl_sslbackend id = (curl_sslbackend)(fuzz_size > 0 ? fuzz_data[0] : 0);
            const char *name = NULL;
            if (fuzz_size > 1) {
                size_t name_len = fuzz_size - 1 < 255 ? fuzz_size - 1 : 255;
                char *name_buf = (char *)malloc(name_len + 1);
                if (name_buf) {
                    memcpy(name_buf, fuzz_data + 1, name_len);
                    name_buf[name_len] = '\0';
                    name = name_buf;
                }
            }
            const curl_ssl_backend ***avail = NULL;
            curl_global_sslset(id, name, avail);
            if (fuzz_size > 1 && name) free((void *)name);
            break;
        }
        case 1: {
            long flags = 0;
            if (fuzz_size >= sizeof(long)) {
                memcpy(&flags, fuzz_data, sizeof(long));
            } else if (fuzz_size > 0) {
                flags = (long)fuzz_data[0];
            }
            curl_global_init(flags);
            break;
        }
        case 2: {
            long flags = 0;
            if (fuzz_size >= sizeof(long)) {
                memcpy(&flags, fuzz_data, sizeof(long));
            } else if (fuzz_size > 0) {
                flags = (long)fuzz_data[0];
            }
            curl_global_init_mem(flags, dummy_malloc, dummy_free,
                                 dummy_realloc, dummy_strdup, dummy_calloc);
            break;
        }
        case 3: {
            const char *config = NULL;
            if (fuzz_size > 0) {
                size_t config_len = fuzz_size < 255 ? fuzz_size : 255;
                char *config_buf = (char *)malloc(config_len + 1);
                if (config_buf) {
                    memcpy(config_buf, fuzz_data, config_len);
                    config_buf[config_len] = '\0';
                    config = config_buf;
                }
            }
            curl_global_trace(config);
            if (config) free((void *)config);
            break;
        }
        case 4: {
            curl_global_cleanup();
            break;
        }
        case 5: {
            CURLversion stamp = CURLVERSION_NOW;
            if (fuzz_size > 0) {
                stamp = (CURLversion)(fuzz_data[0] % (CURLVERSION_TWELFTH + 1));
            }
            curl_version_info(stamp);
            break;
        }
        case 6: {
            FILE *dummy_file = fopen("./dummy_file", "wb");
            if (dummy_file && fuzz_size > 0) {
                size_t write_size = fuzz_size < 1024 ? fuzz_size : 1024;
                fwrite(fuzz_data, 1, write_size, dummy_file);
                fclose(dummy_file);
            } else if (dummy_file) {
                fclose(dummy_file);
            }
            break;
        }
    }

    return 0;
}