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
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static const char *dummy_strings[] = {
    "A", "AAAA", "MX", "TXT", "SRV", "SOA", "PTR", "CNAME", "NS", "ANY",
    "INVALID", "123", "", "A\0B", "TYPE255", "TYPE65535", "SPF", "CAA", "URI"
};

static const size_t num_dummy_strings = sizeof(dummy_strings) / sizeof(dummy_strings[0]);

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to seed deterministic behavior
    unsigned int seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
    } else {
        seed = Size;
    }
    srand(seed);

    // 1. ares_dns_rec_type_fromstr
    ares_dns_rec_type_t qtype;
    const char *str = dummy_strings[seed % num_dummy_strings];
    ares_bool_t fromstr_result = ares_dns_rec_type_fromstr(&qtype, str);
    
    // 2. ares_dns_rec_type_tostr
    const char *tostr_result = ares_dns_rec_type_tostr(qtype);
    
    // 3. ares_dns_rr_get_keys
    size_t cnt = 0;
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(qtype, &cnt);
    
    if (keys != NULL && cnt > 0) {
        // Iterate through a subset of keys (up to 10) to avoid excessive loops
        size_t max_keys = cnt > 10 ? 10 : cnt;
        for (size_t i = 0; i < max_keys; i++) {
            ares_dns_rr_key_t key = keys[i];
            
            // 4. ares_dns_rr_key_tostr
            const char *key_str = ares_dns_rr_key_tostr(key);
            
            // 5. ares_dns_rr_key_to_rec_type
            ares_dns_rec_type_t rec_type = ares_dns_rr_key_to_rec_type(key);
            
            // 6. ares_dns_rr_key_datatype
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            // Also test with some random key values
            ares_dns_rr_key_t random_key = (ares_dns_rr_key_t)(rand() % 10000);
            (void)ares_dns_rr_key_tostr(random_key);
            (void)ares_dns_rr_key_to_rec_type(random_key);
            (void)ares_dns_rr_key_datatype(random_key);
        }
    }
    
    // Test with some random record types
    for (int i = 0; i < 10; i++) {
        ares_dns_rec_type_t random_type = (ares_dns_rec_type_t)(rand() % 65536);
        const char *random_tostr = ares_dns_rec_type_tostr(random_type);
        
        size_t random_cnt = 0;
        const ares_dns_rr_key_t *random_keys = ares_dns_rr_get_keys(random_type, &random_cnt);
        
        if (random_keys != NULL && random_cnt > 0) {
            size_t idx = rand() % random_cnt;
            ares_dns_rr_key_t random_key = random_keys[idx];
            (void)ares_dns_rr_key_tostr(random_key);
            (void)ares_dns_rr_key_to_rec_type(random_key);
            (void)ares_dns_rr_key_datatype(random_key);
        }
    }
    
    return 0;
}