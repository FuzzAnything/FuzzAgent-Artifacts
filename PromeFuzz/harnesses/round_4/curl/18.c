// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use first byte to determine which API to test
    uint8_t api_selector = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (api_selector) {
        case 0: { // Test curl_easy_setopt
            if (Size - data_offset < sizeof(int)) break;
            
            // Extract option ID from data
            int opt_id;
            memcpy(&opt_id, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
            
            // Get option details to know the type
            const struct curl_easyoption *opt_info = curl_easy_option_by_id((CURLoption)opt_id);
            if (opt_info) {
                switch (opt_info->type) {
                    case CURLOT_LONG: {
                        if (Size - data_offset < sizeof(long)) break;
                        long val;
                        memcpy(&val, Data + data_offset, sizeof(long));
                        curl_easy_setopt(curl, (CURLoption)opt_id, val);
                        break;
                    }
                    case CURLOT_STRING: {
                        // Use a fixed string or extract from data
                        const char *str = "fuzz_test";
                        if (Size - data_offset > 0) {
                            // Use data as string (null-terminated)
                            size_t str_len = Size - data_offset;
                            if (str_len > 256) str_len = 256;
                            char *temp_str = (char *)malloc(str_len + 1);
                            if (temp_str) {
                                memcpy(temp_str, Data + data_offset, str_len);
                                temp_str[str_len] = '\0';
                                curl_easy_setopt(curl, (CURLoption)opt_id, temp_str);
                                free(temp_str);
                            }
                        } else {
                            curl_easy_setopt(curl, (CURLoption)opt_id, str);
                        }
                        break;
                    }
                    case CURLOT_VALUES: {
                        if (Size - data_offset < sizeof(long)) break;
                        long val;
                        memcpy(&val, Data + data_offset, sizeof(long));
                        curl_easy_setopt(curl, (CURLoption)opt_id, val);
                        break;
                    }
                    case CURLOT_OFF_T: {
                        if (Size - data_offset < sizeof(curl_off_t)) break;
                        curl_off_t val;
                        memcpy(&val, Data + data_offset, sizeof(curl_off_t));
                        curl_easy_setopt(curl, (CURLoption)opt_id, val);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        
        case 1: { // Test curl_easy_option_by_id
            if (Size - data_offset < sizeof(int)) break;
            int opt_id;
            memcpy(&opt_id, Data + data_offset, sizeof(int));
            curl_easy_option_by_id((CURLoption)opt_id);
            break;
        }
        
        case 2: { // Test curl_easy_option_by_name
            if (Size - data_offset < 1) break;
            
            // Extract name from data (null-terminated)
            size_t name_len = Size - data_offset;
            if (name_len > 256) name_len = 256;
            char *name = (char *)malloc(name_len + 1);
            if (name) {
                memcpy(name, Data + data_offset, name_len);
                name[name_len] = '\0';
                curl_easy_option_by_name(name);
                free(name);
            }
            break;
        }
        
        case 3: { // Test curl_easy_perform
            // Create a dummy file for upload if needed
            if (Size > data_offset) {
                write_dummy_file(Data + data_offset, Size - data_offset);
            }
            
            // Set up a simple URL and perform
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/fuzz");
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_READDATA, fopen("./dummy_file", "rb"));
            curl_easy_perform(curl);
            break;
        }
        
        case 4: { // Test curl_easy_option_next
            const struct curl_easyoption *opt = NULL;
            int iterations = (Size - data_offset > 0) ? (Data[data_offset] % 10) : 3;
            for (int i = 0; i < iterations; i++) {
                opt = curl_easy_option_next(opt);
                if (!opt) break;
            }
            break;
        }
        
        case 5: { // Test curl_easy_getinfo
            if (Size - data_offset < sizeof(int)) break;
            
            int info_type;
            memcpy(&info_type, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
            
            // Based on info type, pass appropriate pointer
            switch (info_type) {
                case CURLINFO_RESPONSE_CODE: {
                    long response_code;
                    curl_easy_getinfo(curl, (CURLINFO)info_type, &response_code);
                    break;
                }
                case CURLINFO_TOTAL_TIME: {
                    double total_time;
                    curl_easy_getinfo(curl, (CURLINFO)info_type, &total_time);
                    break;
                }
                case CURLINFO_SIZE_DOWNLOAD: {
                    curl_off_t download_size;
                    curl_easy_getinfo(curl, (CURLINFO)info_type, &download_size);
                    break;
                }
                case CURLINFO_CONTENT_TYPE: {
                    char *content_type = NULL;
                    curl_easy_getinfo(curl, (CURLINFO)info_type, &content_type);
                    break;
                }
                default: {
                    // For unknown types, try with long
                    long dummy;
                    curl_easy_getinfo(curl, (CURLINFO)info_type, &dummy);
                    break;
                }
            }
            break;
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}