// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define feature macros to disable problematic type-checking macros in curl headers */
#define CURL_DISABLE_TYPECHECK 1
#define CURL_DISABLE_HSTS 1
#define CURL_DISABLE_CRYPTO_AUTH 1
#define CURL_DISABLE_DOH 1
#define CURL_DISABLE_MIME 1
#define CURL_DISABLE_HTTP2 1
#define CURL_DISABLE_HTTP3 1
#define CURL_DISABLE_RTSP 1
#define CURL_DISABLE_SMB 1
#define CURL_DISABLE_GSSAPI 1
#define CURL_DISABLE_NTLM 1
#define CURL_DISABLE_TLS_SRP 1
#define CURL_DISABLE_OSSP 1
#define CURL_DISABLE_COOKIES 1
#define CURL_DISABLE_DICT 1
#define CURL_DISABLE_FILE 1
#define CURL_DISABLE_FTP 1
#define CURL_DISABLE_GOPHER 1
#define CURL_DISABLE_IMAP 1
#define CURL_DISABLE_LDAP 1
#define CURL_DISABLE_LDAPS 1
#define CURL_DISABLE_POP3 1
#define CURL_DISABLE_SMTP 1
#define CURL_DISABLE_TELNET 1
#define CURL_DISABLE_TFTP 1
#define CURL_DISABLE_WEBSOCKETS 1

#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode rc;
    const char *error_str;
    struct curl_waitfd waitfds[10];
    unsigned int fd_count = 0;
    unsigned int notification_flags;

    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle */
    rc = curl_multi_add_handle(multi_handle, easy_handle);
    if (rc != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Extract data for fuzzing */
    unsigned int op = Data[0] % 6;
    unsigned int error_code = 0;
    if (Size > 1) {
        error_code = Data[1];
    }
    if (Size > 2) {
        notification_flags = Data[2];
    }
    unsigned int size_param = 0;
    if (Size > 3) {
        size_param = Data[3] % 10;
    }

    switch (op) {
        case 0:
            /* Fuzz curl_multi_strerror */
            error_str = curl_multi_strerror((CURLMcode)error_code);
            if (error_str) {
                (void)strlen(error_str);
            }
            break;

        case 1:
            /* Fuzz curl_multi_notify_enable */
            rc = curl_multi_notify_enable(multi_handle, notification_flags);
            (void)rc;
            break;

        case 2:
            /* Fuzz curl_multi_remove_handle */
            rc = curl_multi_remove_handle(multi_handle, easy_handle);
            (void)rc;
            break;

        case 3:
            /* Fuzz curl_multi_init (already initialized, but we can try again) */
            {
                CURLM *temp_multi = curl_multi_init();
                if (temp_multi) {
                    curl_multi_cleanup(temp_multi);
                }
            }
            break;

        case 4:
            /* Fuzz curl_multi_waitfds */
            rc = curl_multi_waitfds(multi_handle, waitfds, size_param, &fd_count);
            (void)rc;
            break;

        case 5:
            /* Fuzz curl_multi_notify_disable */
            rc = curl_multi_notify_disable(multi_handle, notification_flags);
            (void)rc;
            break;
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    return 0;
}