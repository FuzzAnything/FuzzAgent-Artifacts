// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/urlapi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_url_operations(const uint8_t *data, size_t size) {
    if (size < 1) return;

    // Create a new CURLU handle
    CURLU *handle = curl_url();
    if (!handle) return;

    // Use fuzzing data to set URL parts
    // We'll try to set different parts based on the data
    if (size > 0) {
        // Try to set scheme (first byte determines which part to set)
        unsigned int part_type = data[0] % 10; // 0-9 for different parts
        unsigned int flags = (size > 1) ? (data[1] % 4) : 0;
        
        // Prepare a string from fuzzing data for setting
        size_t str_len = (size > 2) ? (data[2] % (size - 2)) : 0;
        if (str_len > 0 && str_len < size) {
            char *part_str = (char *)malloc(str_len + 1);
            if (part_str) {
                memcpy(part_str, data + 3, str_len);
                part_str[str_len] = '\0';
                
                // Try to set different URL parts
                CURLUPart part = (CURLUPart)part_type;
                curl_url_set(handle, part, part_str, flags);
                
                free(part_str);
            }
        }
    }

    // Try to duplicate the handle
    CURLU *dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        // Try to modify the duplicated handle
        if (size > 0) {
            unsigned int dup_part_type = (data[0] + 1) % 10;
            unsigned int dup_flags = (size > 1) ? (data[1] % 4) : 0;
            
            size_t dup_str_len = (size > 2) ? (data[2] % (size - 2)) : 0;
            if (dup_str_len > 0 && dup_str_len < size) {
                char *dup_part_str = (char *)malloc(dup_str_len + 1);
                if (dup_part_str) {
                    memcpy(dup_part_str, data + 3, dup_str_len);
                    dup_part_str[dup_str_len] = '\0';
                    
                    curl_url_set(dup_handle, (CURLUPart)dup_part_type, dup_part_str, dup_flags);
                    free(dup_part_str);
                }
            }
        }
        
        // Try to get parts from duplicated handle
        char *get_part = NULL;
        CURLUcode get_result = curl_url_get(dup_handle, (CURLUPart)(data[0] % 10), &get_part, (size > 1) ? (data[1] % 4) : 0);
        if (get_result == CURLUE_OK && get_part) {
            curl_free(get_part);
        }
        
        curl_url_cleanup(dup_handle);
    }

    // Try to get parts from original handle
    char *get_part = NULL;
    for (int i = 0; i < 10; i++) {
        CURLUcode get_result = curl_url_get(handle, (CURLUPart)i, &get_part, (size > 1) ? (data[1] % 4) : 0);
        if (get_result == CURLUE_OK && get_part) {
            curl_free(get_part);
        }
    }

    // Test error string conversion
    for (int i = 0; i < 20; i++) {
        const char *error_str = curl_url_strerror((CURLUcode)i);
        (void)error_str; // Use the error string
    }

    // Cleanup
    curl_url_cleanup(handle);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    fuzz_url_operations(Data, Size);
    return 0;
}