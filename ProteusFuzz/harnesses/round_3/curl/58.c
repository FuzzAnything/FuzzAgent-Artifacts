// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_global_init at easy.c:197:10 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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

/* Fuzz driver for curl API functions */

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    /* Ensure we have some data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    CURLcode rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc != CURLE_OK) {
        return 0;
    }

    /* Create a curl handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Test curl_version - no arguments needed */
    const char *version = curl_version();
    if (version) {
        /* Just use the string to prevent unused variable warnings */
        (void)strlen(version);
    }

    /* Test curl_getenv with fuzzed input as variable name */
    /* Ensure null termination */
    char *env_var = malloc(Size + 1);
    if (env_var) {
        memcpy(env_var, Data, Size);
        env_var[Size] = '\0';
        
        char *result = curl_getenv(env_var);
        if (result) {
            /* Use the result to prevent unused variable warnings */
            (void)strlen(result);
            curl_free(result);
        }
        free(env_var);
    }

    /* Test curl_global_sslset */
    curl_ssl_backend **avail = NULL;
    /* Use fuzzed data to determine SSL backend ID and name */
    curl_sslbackend backend_id = (Size > 0) ? (curl_sslbackend)Data[0] : CURLSSLBACKEND_NONE;
    
    /* Create a name string from fuzzed data */
    char *ssl_name = NULL;
    if (Size > 1) {
        ssl_name = malloc(Size);
        if (ssl_name) {
            memcpy(ssl_name, Data, Size);
            ssl_name[Size - 1] = '\0'; /* Ensure null termination */
        }
    }
    
    CURLsslset sslset_result = curl_global_sslset(backend_id, ssl_name, &avail);
    (void)sslset_result; /* Suppress unused variable warning */
    
    if (ssl_name) {
        free(ssl_name);
    }

    /* Test curl_easy_setopt with various options using fuzzed data */
    if (Size >= 2) {
        /* Use first byte as option type indicator */
        int option_type = Data[0] % 10;
        
        switch (option_type) {
            case 0:
                /* String option - URL */
                if (Size > 1) {
                    char *url = malloc(Size);
                    if (url) {
                        memcpy(url, Data, Size);
                        url[Size - 1] = '\0';
                        curl_easy_setopt(handle, CURLOPT_URL, url);
                        free(url);
                    }
                }
                break;
                
            case 1:
                /* Long option - Timeout */
                long timeout = (long)(Data[0] + Data[1]);
                curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
                break;
                
            case 2:
                /* String option - User-Agent */
                if (Size > 1) {
                    char *ua = malloc(Size);
                    if (ua) {
                        memcpy(ua, Data, Size);
                        ua[Size - 1] = '\0';
                        curl_easy_setopt(handle, CURLOPT_USERAGENT, ua);
                        free(ua);
                    }
                }
                break;
                
            case 3:
                /* POST data */
                if (Size > 1) {
                    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (char *)Data);
                    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, Size - 1);
                }
                break;
                
            case 4:
                /* String option - Proxy */
                if (Size > 1) {
                    char *proxy = malloc(Size);
                    if (proxy) {
                        memcpy(proxy, Data, Size);
                        proxy[Size - 1] = '\0';
                        curl_easy_setopt(handle, CURLOPT_PROXY, proxy);
                        free(proxy);
                    }
                }
                break;
                
            case 5:
                /* Long option - Max Redirs */
                long max_redirs = (long)(Data[0] + Data[1]);
                curl_easy_setopt(handle, CURLOPT_MAXREDIRS, max_redirs);
                break;
                
            case 6:
                /* String option - Cookie */
                if (Size > 1) {
                    char *cookie = malloc(Size);
                    if (cookie) {
                        memcpy(cookie, Data, Size);
                        cookie[Size - 1] = '\0';
                        curl_easy_setopt(handle, CURLOPT_COOKIE, cookie);
                        free(cookie);
                    }
                }
                break;
                
            case 7:
                /* Long option - Follow Location */
                long follow = (Data[0] % 2);
                curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, follow);
                break;
                
            case 8:
                /* String option - Referer */
                if (Size > 1) {
                    char *referer = malloc(Size);
                    if (referer) {
                        memcpy(referer, Data, Size);
                        referer[Size - 1] = '\0';
                        curl_easy_setopt(handle, CURLOPT_REFERER, referer);
                        free(referer);
                    }
                }
                break;
                
            case 9:
                /* Long option - Connect Timeout */
                long connect_timeout = (long)(Data[0] + Data[1]);
                curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
                break;
                
            default:
                break;
        }
    }

    /* Test curl_easy_getinfo with various info types */
    if (Size >= 1) {
        int info_type = Data[0] % 20;
        
        switch (info_type) {
            case 0: {
                long response_code;
                curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response_code);
                break;
            }
            case 1: {
                long http_code;
                curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &http_code);
                break;
            }
            case 2: {
                double total_time;
                curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);
                break;
            }
            case 3: {
                double namelookup_time;
                curl_easy_getinfo(handle, CURLINFO_NAMELOOKUP_TIME, &namelookup_time);
                break;
            }
            case 4: {
                double connect_time;
                curl_easy_getinfo(handle, CURLINFO_CONNECT_TIME, &connect_time);
                break;
            }
            case 5: {
                double pretransfer_time;
                curl_easy_getinfo(handle, CURLINFO_PRETRANSFER_TIME, &pretransfer_time);
                break;
            }
            case 6: {
                double starttransfer_time;
                curl_easy_getinfo(handle, CURLINFO_STARTTRANSFER_TIME, &starttransfer_time);
                break;
            }
            case 7: {
                long size_download;
                curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &size_download);
                break;
            }
            case 8: {
                long size_upload;
                curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD, &size_upload);
                break;
            }
            case 9: {
                long speed_download;
                curl_easy_getinfo(handle, CURLINFO_SPEED_DOWNLOAD, &speed_download);
                break;
            }
            case 10: {
                long speed_upload;
                curl_easy_getinfo(handle, CURLINFO_SPEED_UPLOAD, &speed_upload);
                break;
            }
            case 11: {
                char *url;
                curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
                if (url) {
                    (void)strlen(url);
                }
                break;
            }
            case 12: {
                long content_length_download;
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length_download);
                break;
            }
            case 13: {
                long content_length_upload;
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_UPLOAD, &content_length_upload);
                break;
            }
            case 14: {
                long content_type;
                curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &content_type);
                break;
            }
            case 15: {
                long redirect_count;
                curl_easy_getinfo(handle, CURLINFO_REDIRECT_COUNT, &redirect_count);
                break;
            }
            case 16: {
                long redirect_time;
                curl_easy_getinfo(handle, CURLINFO_REDIRECT_TIME, &redirect_time);
                break;
            }
            case 17: {
                long ssl_verify_result;
                curl_easy_getinfo(handle, CURLINFO_SSL_VERIFYRESULT, &ssl_verify_result);
                break;
            }
            case 18: {
                curl_socket_t socket;
                curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &socket);
                break;
            }
            case 19: {
                curl_off_t download_size;
                curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
                break;
            }
            default:
                break;
        }
    }

    /* Cleanup curl handle */
    curl_easy_cleanup(handle);
    
    /* Cleanup global environment */
    curl_global_cleanup();
    
    return 0;
}