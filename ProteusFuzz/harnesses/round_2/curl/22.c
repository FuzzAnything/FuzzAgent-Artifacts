// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fprintf(fp, "dummy content for fuzzing");
        fclose(fp);
    }
}

/* Static callback for SSL export matching the expected signature */
/* Note: The signature includes an extra 'unsigned long' parameter at the end compared to the previous attempt */
static CURLcode ssl_export_callback(void *userptr, void *ssl_session,
                                    const char *hostname,
                                    const unsigned char *server_cert,
                                    unsigned long server_cert_len,
                                    const unsigned char *server_cert_chain,
                                    unsigned long server_cert_chain_len,
                                    long server_cert_version,
                                    int server_cert_valid,
                                    const char *server_cert_subject,
                                    unsigned long server_cert_subject_len) {
    (void)userptr;
    (void)ssl_session;
    (void)hostname;
    (void)server_cert;
    (void)server_cert_len;
    (void)server_cert_chain;
    (void)server_cert_chain_len;
    (void)server_cert_version;
    (void)server_cert_valid;
    (void)server_cert_subject;
    (void)server_cert_subject_len;
    return CURLE_OK;
}

/* Static function to parse input and fuzz curl_easy_upkeep */
static void fuzz_upkeep(const uint8_t *data, size_t size) {
    if (size < 1) return;
    CURL *handle = curl_easy_init();
    if (!handle) return;

    /* Optionally set a URL to make the handle more valid */
    if (size > 10) {
        char url[20];
        size_t url_len = size < 20 ? size : 20;
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(handle, CURLOPT_URL, url);
    }

    curl_easy_upkeep(handle);
    curl_easy_cleanup(handle);
}

/* Static function to parse input and fuzz curl_easy_pause */
static void fuzz_pause(const uint8_t *data, size_t size) {
    if (size < 1) return;
    CURL *handle = curl_easy_init();
    if (!handle) return;

    /* Set a dummy URL */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");

    int bitmask = (size > 0) ? (int)data[0] : 0;
    /* Ensure bitmask is within reasonable range for testing */
    bitmask &= 0xFF; 

    curl_easy_pause(handle, bitmask);
    curl_easy_cleanup(handle);
}

/* Static function to parse input and fuzz curl_easy_ssls_export */
static void fuzz_ssls_export(const uint8_t *data, size_t size) {
    if (size < 1) return;
    CURL *handle = curl_easy_init();
    if (!handle) return;

    /* Set a dummy URL */
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com");

    /* Call with valid callback and userptr */
    curl_easy_ssls_export(handle, ssl_export_callback, (void *)(size_t)data[0]);
    curl_easy_cleanup(handle);
}

/* Static function to parse input and fuzz curl_easy_perform */
static void fuzz_perform(const uint8_t *data, size_t size) {
    if (size < 1) return;
    CURL *handle = curl_easy_init();
    if (!handle) return;

    /* Construct a URL from input data */
    char url[256];
    size_t url_len = size < 255 ? size : 255;
    memcpy(url, data, url_len);
    url[url_len] = '\0';

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100); /* Prevent hanging */
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
}

/* Static function to parse input and fuzz curl_global_trace */
static void fuzz_global_trace(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    /* Create a null-terminated string from input */
    char config[256];
    size_t config_len = size < 255 ? size : 255;
    memcpy(config, data, config_len);
    config[config_len] = '\0';

    curl_global_trace(config);
}

/* Static function to parse input and fuzz curl_mime_headers */
static void fuzz_mime_headers(const uint8_t *data, size_t size) {
    if (size < 1) return;

    CURL *handle = curl_easy_init();
    if (!handle) return;

    /* Create a mime structure */
    curl_mime *mime = curl_mime_init(handle);
    if (!mime) {
        curl_easy_cleanup(handle);
        return;
    }

    /* Add a part */
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(handle);
        return;
    }

    /* Create headers list */
    struct curl_slist *headers = NULL;
    if (size > 10) {
        char header[64];
        size_t h_len = size < 63 ? size : 63;
        memcpy(header, data, h_len);
        header[h_len] = '\0';
        headers = curl_slist_append(headers, header);
    }

    int take_ownership = (size > 0) ? (int)data[0] % 2 : 0;

    curl_mime_headers(part, headers, take_ownership);

    /* Cleanup */
    if (headers && take_ownership == 0) {
        curl_slist_free_all(headers);
    }
    curl_mime_free(mime);
    curl_easy_cleanup(handle);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    /* Ensure we have some data to work with */
    if (Size == 0) return 0;

    /* Create dummy file if needed for file-based operations */
    create_dummy_file();

    /* Fuzz different API functions based on the first byte */
    uint8_t selector = Data[0];

    switch (selector % 6) {
        case 0:
            fuzz_upkeep(Data, Size);
            break;
        case 1:
            fuzz_pause(Data, Size);
            break;
        case 2:
            fuzz_ssls_export(Data, Size);
            break;
        case 3:
            fuzz_perform(Data, Size);
            break;
        case 4:
            fuzz_global_trace(Data, Size);
            break;
        case 5:
            fuzz_mime_headers(Data, Size);
            break;
    }

    return 0;
}