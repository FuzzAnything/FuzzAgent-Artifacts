// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl globally
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    // Parse input to determine behavior
    // Use first byte to select function to test
    uint8_t func_selector = Data[0] % 4;

    // Prepare a safe config string for curl_global_trace
    char config_buf[256];
    size_t config_len = (Size > 256) ? 255 : Size - 1;
    if (config_len > 0) {
        memcpy(config_buf, Data + 1, config_len);
        config_buf[config_len] = '\0';
    } else {
        config_buf[0] = '\0';
    }

    // Create a dummy file if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        if (Size > 1) {
            fwrite(Data + 1, 1, Size - 1, f);
        }
        fclose(f);
    }

    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Set a basic URL to avoid immediate failures in perform
    // Use a safe, non-existent URL for fuzzing
    if (Size > 20) {
        char url_buf[21];
        memcpy(url_buf, Data + 1, 20);
        url_buf[20] = '\0';
        // Ensure it looks like a URL to avoid early rejection
        if (url_buf[0] != 'h' || url_buf[1] != 't' || url_buf[2] != 't' || url_buf[3] != 'p') {
            strcpy(url_buf, "http://fuzz.invalid/");
        }
        curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://fuzz.invalid/");
    }

    // Set a timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);

    // Execute selected function(s)
    switch (func_selector) {
        case 0:
            // Test curl_global_trace
            curl_global_trace(config_buf);
            break;

        case 1:
            // Test curl_easy_upkeep
            curl_easy_upkeep(curl);
            break;

        case 2:
            // Test curl_easy_perform and curl_easy_getinfo
            res = curl_easy_perform(curl);
            
            // Try to get various info types
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            
            double total_time = 0.0;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            
            char *effective_url = NULL;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
            if (effective_url) {
                // No need to free, curl manages this
            }
            
            curl_off_t size_download = 0;
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &size_download);
            break;

        case 3:
            // Test with a file handle option
            f = fopen(dummy_file, "r");
            if (f) {
                curl_easy_setopt(curl, CURLOPT_READDATA, f);
                res = curl_easy_perform(curl);
                fclose(f);
            }
            
            long http_code2 = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code2);
            break;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}