// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static const struct curl_easyoption* get_random_option(const uint8_t *data, size_t size, size_t *offset) {
    const struct curl_easyoption *opt = NULL;
    if (*offset + sizeof(CURLoption) <= size) {
        CURLoption id;
        memcpy(&id, data + *offset, sizeof(CURLoption));
        *offset += sizeof(CURLoption);
        opt = curl_easy_option_by_id(id);
    }
    return opt;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    size_t offset = 0;
    
    // Test curl_easy_option_next iteration
    const struct curl_easyoption *opt = NULL;
    int iter_count = 0;
    while ((opt = curl_easy_option_next(opt)) != NULL && iter_count < 10) {
        iter_count++;
    }
    
    // Test curl_easy_option_by_name with fuzzed name
    if (Size > 0) {
        char name[256];
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        const struct curl_easyoption *named_opt = curl_easy_option_by_name(name);
        (void)named_opt;
    }
    
    // Test curl_easy_option_by_id with fuzzed IDs
    offset = 0;
    while (offset + sizeof(CURLoption) <= Size && offset < 100) {
        CURLoption id;
        memcpy(&id, Data + offset, sizeof(CURLoption));
        offset += sizeof(CURLoption);
        const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(id);
        (void)opt_by_id;
    }
    
    // Test curl_easy_setopt with various options
    offset = 0;
    int setopt_count = 0;
    while (offset < Size && setopt_count < 20) {
        const struct curl_easyoption *opt_info = get_random_option(Data, Size, &offset);
        if (!opt_info) break;
        
        CURLcode res = CURLE_OK;
        
        // Handle different option types based on type field
        if (opt_info->type == 0) { // CURLOT_LONG
            if (offset + sizeof(long) <= Size) {
                long value;
                memcpy(&value, Data + offset, sizeof(long));
                offset += sizeof(long);
                res = curl_easy_setopt(curl, opt_info->id, value);
            }
        } else if (opt_info->type == 1) { // CURLOT_OBJECTPOINT (void*)
            if (offset + sizeof(void*) <= Size) {
                void *value;
                memcpy(&value, Data + offset, sizeof(void*));
                offset += sizeof(void*);
                res = curl_easy_setopt(curl, opt_info->id, value);
            }
        } else if (opt_info->type == 2) { // CURLOT_STRING
            if (offset + 1 <= Size) {
                size_t str_len = Data[offset] % 100;
                offset++;
                if (str_len > 0 && offset + str_len <= Size) {
                    char *str = (char*)malloc(str_len + 1);
                    if (str) {
                        memcpy(str, Data + offset, str_len);
                        str[str_len] = '\0';
                        offset += str_len;
                        res = curl_easy_setopt(curl, opt_info->id, str);
                        free(str);
                    }
                }
            }
        } else if (opt_info->type == 3) { // CURLOT_SLIST
            // Skip SLIST for simplicity
        } else if (opt_info->type == 4) { // CURLOT_CBPTR
            if (offset + sizeof(void*) <= Size) {
                void *value;
                memcpy(&value, Data + offset, sizeof(void*));
                offset += sizeof(void*);
                res = curl_easy_setopt(curl, opt_info->id, value);
            }
        } else if (opt_info->type == 5) { // CURLOT_BLOB
            if (offset + sizeof(void*) <= Size) {
                void *value;
                memcpy(&value, Data + offset, sizeof(void*));
                offset += sizeof(void*);
                res = curl_easy_setopt(curl, opt_info->id, value);
            }
        }
        setopt_count++;
    }
    
    // Set up a dummy file for transfer
    write_dummy_file(Data, Size > 1024 ? 1024 : Size);
    
    // Set URL to local file
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    
    // Test curl_easy_perform
    CURLcode perform_res = curl_easy_perform(curl);
    (void)perform_res;
    
    // Test curl_easy_getinfo
    if (perform_res == CURLE_OK) {
        double total_time;
        CURLcode info_res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        (void)info_res;
        
        long response_code;
        info_res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        (void)info_res;
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}