// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version_info at version.c:596:25 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/*
 * Fuzz driver for curl_version_info.
 * Note: The other listed items (Wcurl_easy_setopt_err_*, etc.) are compile-time
 * warning macros and cannot be invoked directly at runtime.
 * Therefore, this driver focuses on fuzzing the valid runtime API: curl_version_info.
 */

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Prepare the environment.
     * curl_version_info takes a CURLversion enum.
     * We derive the enum value from the input data to explore different states.
     * Valid values are typically 0 to CURLVERSION_NOW.
     */
    CURLversion stamp = (CURLversion)(Data[0] % (CURLVERSION_NOW + 1));

    /*
     * Invoke the target function.
     * curl_version_info returns a pointer to a static struct.
     * It is thread-safe for reading but the returned pointer should not be modified.
     */
    curl_version_info_data *ver_info = curl_version_info(stamp);

    /*
     * Explore program states.
     * If the function returns a valid pointer, we read the fields to ensure
     * they are accessible and to trigger any internal logic related to
     * feature availability or string formatting.
     */
    if (ver_info != NULL) {
        /* Access basic fields */
        (void)ver_info->age;
        (void)ver_info->version;
        (void)ver_info->version_num;
        (void)ver_info->host;
        (void)ver_info->features;
        (void)ver_info->ssl_version;
        (void)ver_info->ssl_version_num;
        (void)ver_info->libz_version;

        /* Access protocols array (terminated by NULL) */
        if (ver_info->protocols != NULL) {
            for (int i = 0; ver_info->protocols[i] != NULL; i++) {
                /* Just read to ensure no segfaults */
                const char *proto = ver_info->protocols[i];
                (void)proto;
            }
        }

        /* Access fields added in CURLVERSION_SECOND */
        (void)ver_info->ares;
        (void)ver_info->ares_num;

        /* Access fields added in CURLVERSION_THIRD */
        (void)ver_info->libidn;

        /* Access fields added in CURLVERSION_FOURTH */
        (void)ver_info->iconv_ver_num;
        (void)ver_info->libssh_version;

        /* Access fields added in CURLVERSION_FIFTH */
        (void)ver_info->brotli_ver_num;
        (void)ver_info->brotli_version;

        /* Access fields added in CURLVERSION_SIXTH */
        (void)ver_info->nghttp2_ver_num;
        (void)ver_info->nghttp2_version;
        (void)ver_info->quic_version;

        /* Access fields added in CURLVERSION_SEVENTH */
        (void)ver_info->cainfo;
        (void)ver_info->capath;

        /* Access fields added in CURLVERSION_EIGHTH */
        (void)ver_info->zstd_ver_num;
        (void)ver_info->zstd_version;

        /* Access fields added in CURLVERSION_NINTH */
        (void)ver_info->hyper_version;

        /* Access fields added in CURLVERSION_TENTH */
        (void)ver_info->gsasl_version;

        /* Access fields added in CURLVERSION_ELEVENTH */
        if (ver_info->feature_names != NULL) {
            for (int i = 0; ver_info->feature_names[i] != NULL; i++) {
                const char *feat = ver_info->feature_names[i];
                (void)feat;
            }
        }

        /* Access fields added in CURLVERSION_TWELFTH */
        (void)ver_info->rtmp_version;
    }

    /*
     * Cleanup.
     * curl_version_info returns a static struct, so no memory needs to be freed.
     * No exceptions are thrown by this function.
     */

    return 0;
}