// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/*
 * Fuzz driver for curl_easy_getinfo, curl_easy_perform, and curl_easy_upkeep.
 * Fixed: Ensures correct pointer types are passed to curl_easy_getinfo based on the CURLINFO option.
 */

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Use a dummy URL to avoid actual network dependency */
    const char *dummy_url = "file:///dev/null";
    
    /* Set a basic URL */
    curl_easy_setopt(curl, CURLOPT_URL, dummy_url);
    
    /* Set a timeout to prevent hanging */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    /* Disable SSL verification for local files or test environments */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    struct curl_slist *headers = NULL;

    /* Use input data to set various options to explore different code paths */
    if (Size > 0) {
        /* Use the first byte to determine if we set a POST field */
        if (Data[0] & 0x01) {
            /* Create a small buffer for POST data, ensuring null termination */
            char post_data[256];
            size_t len = (Size > 255) ? 255 : Size;
            memcpy(post_data, Data, len);
            post_data[len] = '\0';
            
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);
        }

        /* Use second byte to set a custom header if available */
        if (Size > 1 && (Data[1] & 0x01)) {
            headers = curl_slist_append(headers, "X-Fuzz-Header: test");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
    }

    /* Perform the transfer */
    curl_easy_perform(curl);

    /*
     * Now fuzz curl_easy_getinfo with various CURLINFO types.
     * We must ensure the pointer passed matches the expected type for the info option.
     */
    
    /* Variables to hold results of getinfo calls */
    char *string_buf = NULL;
    long long_buf = 0;
    double double_buf = 0.0;
    curl_off_t off_t_buf = 0;
    curl_socket_t socket_buf = -1;
    struct curl_slist *slist_buf = NULL;
    struct curl_certinfo *certinfo_buf = NULL;
    struct curl_tlssessioninfo *tlssessioninfo_buf = NULL;

    /* 
     * We use the input data to select which info types to query.
     * We explicitly map the random value to a known CURLINFO enum and the correct variable type.
     */
    for (size_t i = 0; i < Size; i++) {
        uint8_t info_selector = Data[i]; 
        
        CURLINFO info = CURLINFO_NONE;
        int type_category = -1; // 0: string, 1: long, 2: double, 3: off_t, 4: socket, 5: slist, 6: certinfo, 7: tls

        /* Map selector to specific info and type */
        switch (info_selector % 20) {
            case 0: info = CURLINFO_EFFECTIVE_URL; type_category = 0; break;
            case 1: info = CURLINFO_CONTENT_TYPE; type_category = 0; break;
            case 2: info = CURLINFO_HTTP_CODE; type_category = 1; break;
            case 3: info = CURLINFO_RESPONSE_CODE; type_category = 1; break;
            case 4: info = CURLINFO_TOTAL_TIME; type_category = 2; break;
            case 5: info = CURLINFO_NAMELOOKUP_TIME; type_category = 2; break;
            case 6: info = CURLINFO_SIZE_DOWNLOAD; type_category = 3; break;
            case 7: info = CURLINFO_SIZE_UPLOAD; type_category = 3; break;
            case 8: info = CURLINFO_CONTENT_LENGTH_DOWNLOAD; type_category = 3; break;
            case 9: info = CURLINFO_CONTENT_LENGTH_UPLOAD; type_category = 3; break;
            case 10: info = CURLINFO_ACTIVESOCKET; type_category = 4; break;
            case 11: info = CURLINFO_COOKIELIST; type_category = 5; break;
            case 12: info = CURLINFO_CERTINFO; type_category = 6; break;
            case 13: info = CURLINFO_TLS_SESSION; type_category = 7; break;
            case 14: info = CURLINFO_TLS_SSL_PTR; type_category = 7; break;
            case 15: info = CURLINFO_PRIVATE; type_category = 0; break;
            case 16: info = CURLINFO_SPEED_DOWNLOAD; type_category = 2; break;
            case 17: info = CURLINFO_SPEED_UPLOAD; type_category = 2; break;
            case 18: info = CURLINFO_FILETIME; type_category = 1; break;
            case 19: info = CURLINFO_APPCONNECT_TIME; type_category = 2; break;
        }

        /* Perform the getinfo call with the correctly typed pointer */
        switch (type_category) {
            case 0: /* String */
                curl_easy_getinfo(curl, info, &string_buf);
                break;
            case 1: /* Long */
                curl_easy_getinfo(curl, info, &long_buf);
                break;
            case 2: /* Double */
                curl_easy_getinfo(curl, info, &double_buf);
                break;
            case 3: /* Off_t */
                curl_easy_getinfo(curl, info, &off_t_buf);
                break;
            case 4: /* Socket */
                curl_easy_getinfo(curl, info, &socket_buf);
                break;
            case 5: /* Slist */
                curl_easy_getinfo(curl, info, &slist_buf);
                break;
            case 6: /* Certinfo */
                curl_easy_getinfo(curl, info, &certinfo_buf);
                break;
            case 7: /* TlsSessionInfo */
                curl_easy_getinfo(curl, info, &tlssessioninfo_buf);
                break;
            default:
                /* Should not happen, but safe fallback */
                break;
        }
    }

    /* Call curl_easy_upkeep to test that function as well */
    curl_easy_upkeep(curl);

    /* Cleanup */
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    /* Free slist returned by getinfo if any */
    if (slist_buf) {
        curl_slist_free_all(slist_buf);
    }

    curl_easy_cleanup(curl);

    return 0;
}