// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include the required headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

static void fuzz_api_functions(const uint8_t *data, size_t size) {
    // Limit string length to avoid excessive memory usage
    size_t str_len = size > 100 ? 100 : size;
    char *str = (char*)malloc(str_len + 1);
    if (!str) return;
    
    // Copy data to string, ensuring null termination
    memcpy(str, data, str_len);
    str[str_len] = '\0';
    
    // 1. ares_dns_rec_type_fromstr
    ares_dns_rec_type_t rec_type;
    ares_bool_t success = ares_dns_rec_type_fromstr(&rec_type, str);
    
    // 2. ares_dns_rec_type_tostr
    const char *type_str = ares_dns_rec_type_tostr(rec_type);
    (void)type_str; // Use result to avoid compiler warnings
    
    // 3. ares_dns_rr_get_keys
    size_t cnt = 0;
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rec_type, &cnt);
    
    if (keys && cnt > 0) {
        // Iterate through keys to exercise more code paths
        for (size_t i = 0; i < cnt; i++) {
            // 4. ares_dns_rr_key_tostr
            const char *key_str = ares_dns_rr_key_tostr(keys[i]);
            (void)key_str;
            
            // 5. ares_dns_rr_key_to_rec_type
            ares_dns_rec_type_t key_rec_type = ares_dns_rr_key_to_rec_type(keys[i]);
            (void)key_rec_type;
            
            // 6. ares_dns_rr_key_datatype
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[i]);
            (void)datatype;
        }
    }
    
    // Also test with some random key values from fuzzing data
    if (size >= sizeof(ares_dns_rr_key_t)) {
        ares_dns_rr_key_t random_key;
        memcpy(&random_key, data, sizeof(ares_dns_rr_key_t));
        
        // Test the key functions with random input
        const char *random_key_str = ares_dns_rr_key_tostr(random_key);
        (void)random_key_str;
        
        ares_dns_rec_type_t random_rec_type = ares_dns_rr_key_to_rec_type(random_key);
        (void)random_rec_type;
        
        ares_dns_datatype_t random_datatype = ares_dns_rr_key_datatype(random_key);
        (void)random_datatype;
    }
    
    free(str);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Test with empty input
        uint8_t dummy = 0;
        fuzz_api_functions(&dummy, 0);
        return 0;
    }
    
    fuzz_api_functions(Data, Size);
    return 0;
}