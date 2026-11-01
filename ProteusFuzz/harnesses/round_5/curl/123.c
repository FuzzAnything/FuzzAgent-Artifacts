// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void setup_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Prepare dummy file if needed
    setup_dummy_file("./dummy_file", Data, Size);

    // Fuzz various options to explore different states
    // Use data from input to set options where appropriate
    
    // Set URL from input if it looks like a valid string (basic check)
    if (Size > 10) {
        char url[256];
        size_t url_len = (Size < 255) ? Size : 255;
        memcpy(url, Data, url_len);
        url[url_len] = '\0';
        // Ensure it starts with http or https for safety, or use a dummy
        if (strncmp(url, "http", 4) != 0) {
            snprintf(url, sizeof(url), "http://example.com/%s", Data);
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/dummy");
    }

    // Fuzz CURLOPT_POSTFIELDS if data is large enough
    if (Size > 100) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)Data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, Size);
    }

    // Fuzz CURLOPT_TIMEOUT
    if (Size > 2) {
        long timeout = (long)(Data[0] + Data[1]);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }

    // Fuzz CURLOPT_FOLLOWLOCATION
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[0] % 2));

    // Fuzz CURLOPT_USERAGENT
    if (Size > 5) {
        char ua[64];
        size_t ua_len = (Size < 63) ? Size : 63;
        memcpy(ua, Data, ua_len);
        ua[ua_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_USERAGENT, ua);
    }

    // Perform the request (this will likely fail due to network, but exercises the code)
    // We use a dummy file for reading if needed, but here we just test the setup
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    
    // Execute
    CURLcode res = curl_easy_perform(curl);

    // Test curl_easy_getinfo with various types to trigger the warning macros logic
    // The macros themselves are compile-time checks, but calling the function
    // with correct types exercises the runtime logic.

    // Test double info
    double download_speed;
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &download_speed);

    // Test long info
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    // Test string info
    char *effective_url;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);

    // Test slist info (CURLINFO_COOKIELIST)
    struct curl_slist *cookies = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (cookies) {
        curl_slist_free_all(cookies);
    }

    // Test certinfo
    struct curl_certinfo *certinfo = NULL;
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
    // Note: curl_certinfo contains a list of curl_certinfo, usually freed by curl_slist_free_all or similar logic in newer versions,
    // but curl_easy_cleanup handles the handle. The structure itself is managed by curl.

    // Test socket info
    curl_socket_t socket_fd;
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_fd);

    // Test off_t info
    curl_off_t total_download;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &total_download);

    // Test tls session info
    struct curl_tlssessioninfo *tls_info = NULL;
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);

    // Cleanup
    curl_easy_cleanup(curl);

    return 0;
}