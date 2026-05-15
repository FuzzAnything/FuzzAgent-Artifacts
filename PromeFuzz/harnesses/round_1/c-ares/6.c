// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Step 1: Create null-terminated string from fuzzer input for ares_dns_rec_type_fromstr
    char *str = NULL;
    if (Size > 0) {
        str = (char *)malloc(Size + 1);
        if (!str) return 0;
        memcpy(str, Data, Size);
        str[Size] = '\0';
    } else {
        // Use empty string if no data
        str = strdup("");
        if (!str) return 0;
    }
    
    // Step 2: Call ares_dns_rec_type_fromstr
    ares_dns_rec_type_t rec_type;
    ares_bool_t fromstr_result = ares_dns_rec_type_fromstr(&rec_type, str);
    
    // Step 3: Call ares_dns_rec_type_tostr with various record types
    // Use the record type from fromstr if successful, otherwise try different values
    const char *tostr_result = NULL;
    if (fromstr_result == ARES_TRUE) {
        tostr_result = ares_dns_rec_type_tostr(rec_type);
    } else {
        // Try some specific record types as fallback
        static const ares_dns_rec_type_t fallback_types[] = {
            ARES_REC_TYPE_A,
            ARES_REC_TYPE_NS,
            ARES_REC_TYPE_CNAME,
            ARES_REC_TYPE_SOA,
            ARES_REC_TYPE_PTR,
            ARES_REC_TYPE_MX,
            ARES_REC_TYPE_TXT,
            ARES_REC_TYPE_AAAA,
            ARES_REC_TYPE_SRV,
            ARES_REC_TYPE_ANY
        };
        for (size_t i = 0; i < sizeof(fallback_types)/sizeof(fallback_types[0]); i++) {
            if (i < Size) {
                tostr_result = ares_dns_rec_type_tostr(fallback_types[i]);
            }
        }
    }
    
    // Step 4: Call ares_dns_rr_get_keys
    size_t key_count = 0;
    const ares_dns_rr_key_t *keys = NULL;
    
    // Try with the record type from fromstr if available
    if (fromstr_result == ARES_TRUE) {
        keys = ares_dns_rr_get_keys(rec_type, &key_count);
    }
    
    // If that didn't work or returned NULL, try with some fallback record types
    if (!keys) {
        static const ares_dns_rec_type_t fallback_types[] = {
            ARES_REC_TYPE_A,
            ARES_REC_TYPE_NS,
            ARES_REC_TYPE_CNAME,
            ARES_REC_TYPE_SOA,
            ARES_REC_TYPE_PTR,
            ARES_REC_TYPE_MX,
            ARES_REC_TYPE_TXT,
            ARES_REC_TYPE_AAAA,
            ARES_REC_TYPE_SRV,
            ARES_REC_TYPE_ANY
        };
        for (size_t i = 0; i < sizeof(fallback_types)/sizeof(fallback_types[0]); i++) {
            if (i < Size) {
                keys = ares_dns_rr_get_keys(fallback_types[i], &key_count);
                if (keys && key_count > 0) break;
            }
        }
    }
    
    // Steps 5-7: Call functions for each key if available
    if (keys && key_count > 0) {
        // Limit the number of keys we process to avoid timeouts
        size_t max_keys = key_count;
        if (max_keys > 10) max_keys = 10;
        
        for (size_t i = 0; i < max_keys; i++) {
            ares_dns_rr_key_t key = keys[i];
            
            // Step 5: Call ares_dns_rr_key_tostr
            const char *key_str = ares_dns_rr_key_tostr(key);
            
            // Step 6: Call ares_dns_rr_key_to_rec_type
            ares_dns_rec_type_t key_rec_type = ares_dns_rr_key_to_rec_type(key);
            
            // Step 7: Call ares_dns_rr_key_datatype
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
        }
    }
    
    // Also try calling the key functions with some raw values from the input
    // to explore more program states
    if (Size >= sizeof(ares_dns_rr_key_t)) {
        for (size_t offset = 0; offset + sizeof(ares_dns_rr_key_t) <= Size; offset += sizeof(ares_dns_rr_key_t)) {
            ares_dns_rr_key_t raw_key;
            memcpy(&raw_key, Data + offset, sizeof(ares_dns_rr_key_t));
            
            const char *key_str = ares_dns_rr_key_tostr(raw_key);
            ares_dns_rec_type_t key_rec_type = ares_dns_rr_key_to_rec_type(raw_key);
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(raw_key);
        }
    }
    
    // Cleanup
    free(str);
    
    return 0;
}