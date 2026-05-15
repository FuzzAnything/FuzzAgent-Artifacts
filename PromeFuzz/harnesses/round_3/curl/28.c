// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for potential file-based operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    
    if (curl) {
        // Test curl_easy_option_by_id with various IDs derived from fuzzer input
        if (Size >= sizeof(CURLoption)) {
            CURLoption id;
            memcpy(&id, Data, sizeof(CURLoption));
            const struct curl_easyoption *opt = curl_easy_option_by_id(id);
            (void)opt; // Use result to avoid unused variable warning
        }
        
        // Test curl_easy_option_next iteration
        const struct curl_easyoption *current = NULL;
        int i = 0;
        while ((current = curl_easy_option_next(current)) != NULL && i < 10) {
            i++;
        }
        
        // Test curl_easy_option_by_name with string from fuzzer input
        if (Size > 0) {
            // Create null-terminated string from fuzzer data
            char *name = (char *)malloc(Size + 1);
            if (name) {
                memcpy(name, Data, Size);
                name[Size] = '\0';
                const struct curl_easyoption *named_opt = curl_easy_option_by_name(name);
                (void)named_opt;
                free(name);
            }
        }
        
        // Set up basic curl options for transfer
        curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        
        // Test curl_easy_setopt with various options derived from fuzzer input
        if (Size >= sizeof(CURLoption) + sizeof(long)) {
            CURLoption setopt_id;
            long setopt_value;
            memcpy(&setopt_id, Data, sizeof(CURLoption));
            memcpy(&setopt_value, Data + sizeof(CURLoption), sizeof(long));
            
            // Only set options that are safe for fuzzing
            switch (setopt_id) {
                case CURLOPT_VERBOSE:
                case CURLOPT_NOPROGRESS:
                case CURLOPT_NOBODY:
                case CURLOPT_FAILONERROR:
                case CURLOPT_FOLLOWLOCATION:
                    curl_easy_setopt(curl, setopt_id, setopt_value);
                    break;
                default:
                    break;
            }
        }
        
        // Perform transfer
        CURLcode res = curl_easy_perform(curl);
        (void)res;
        
        // Test curl_easy_getinfo after transfer
        if (Size >= sizeof(CURLINFO)) {
            CURLINFO info;
            double double_val;
            long long_val;
            char *string_val = NULL;
            
            memcpy(&info, Data, sizeof(CURLINFO));
            
            switch (info) {
                case CURLINFO_RESPONSE_CODE:
                    curl_easy_getinfo(curl, info, &long_val);
                    break;
                case CURLINFO_TOTAL_TIME:
                case CURLINFO_SPEED_DOWNLOAD:
                    curl_easy_getinfo(curl, info, &double_val);
                    break;
                case CURLINFO_EFFECTIVE_URL:
                case CURLINFO_CONTENT_TYPE:
                    curl_easy_getinfo(curl, info, &string_val);
                    break;
                default:
                    break;
            }
        }
        
        // Clean up
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}