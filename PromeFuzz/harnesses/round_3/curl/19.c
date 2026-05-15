// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_version_info at version.c:596:25 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void fuzz_curl_version_info(void) {
    const curl_version_info_data *info = curl_version_info(CURLVERSION_NOW);
    if (info) {
        volatile int sink;
        sink = info->age;
        if (info->version) sink = *info->version;
        sink = info->version_num;
        if (info->host) sink = *info->host;
        sink = info->features;
        if (info->ssl_version) sink = *info->ssl_version;
        sink = info->ssl_version_num;
        if (info->libz_version) sink = *info->libz_version;
        if (info->protocols) {
            for (int i = 0; info->protocols[i] != NULL; i++) {
                sink = *info->protocols[i];
            }
        }
        if (info->ares) sink = *info->ares;
        sink = info->ares_num;
        if (info->libidn) sink = *info->libidn;
        sink = info->iconv_ver_num;
        if (info->libssh_version) sink = *info->libssh_version;
        sink = info->brotli_ver_num;
        if (info->brotli_version) sink = *info->brotli_version;
        sink = info->nghttp2_ver_num;
        if (info->nghttp2_version) sink = *info->nghttp2_version;
        if (info->quic_version) sink = *info->quic_version;
        if (info->cainfo) sink = *info->cainfo;
        if (info->capath) sink = *info->capath;
        sink = info->zstd_ver_num;
        if (info->zstd_version) sink = *info->zstd_version;
        if (info->hyper_version) sink = *info->hyper_version;
        if (info->gsasl_version) sink = *info->gsasl_version;
        if (info->feature_names) {
            for (int i = 0; info->feature_names[i] != NULL; i++) {
                sink = *info->feature_names[i];
            }
        }
        if (info->rtmp_version) sink = *info->rtmp_version;
    }
}

static void fuzz_curl_version(void) {
    const char *version = curl_version();
    if (version) {
        volatile int sink;
        sink = *version;
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    if (Size > 0) {
        char *null_terminated_data = (char *)malloc(Size + 1);
        if (null_terminated_data) {
            memcpy(null_terminated_data, Data, Size);
            null_terminated_data[Size] = '\0';
            
            curl_global_trace(null_terminated_data);
            
            time_t result = curl_getdate(null_terminated_data, NULL);
            volatile time_t sink = result;
            
            free(null_terminated_data);
        }
    }
    
    fuzz_curl_version_info();
    fuzz_curl_version();
    
    return 0;
}