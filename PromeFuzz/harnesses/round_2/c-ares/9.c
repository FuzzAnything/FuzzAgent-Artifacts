// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
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

static char* create_null_terminated_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_9(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. ares_dns_rec_type_fromstr
    char* str_input = create_null_terminated_string(Data, Size);
    if (!str_input) {
        return 0;
    }
    ares_dns_rec_type_t qtype;
    ares_bool_t fromstr_result = ares_dns_rec_type_fromstr(&qtype, str_input);
    free(str_input);

    // 2. ares_dns_rec_type_tostr
    // Use both the result from fromstr and a value derived from input
    const char* tostr_result1 = ares_dns_rec_type_tostr(qtype);
    // Also test with a value derived from input data
    ares_dns_rec_type_t derived_type = (ares_dns_rec_type_t)(Data[0] % 256);
    const char* tostr_result2 = ares_dns_rec_type_tostr(derived_type);

    // 3. ares_dns_rr_get_keys
    size_t cnt = 0;
    const ares_dns_rr_key_t* keys = NULL;
    
    // Try with the type from fromstr if valid, otherwise use derived type
    ares_dns_rec_type_t type_for_keys = fromstr_result ? qtype : derived_type;
    keys = ares_dns_rr_get_keys(type_for_keys, &cnt);
    
    // 4. ares_dns_rr_key_tostr, 5. ares_dns_rr_key_to_rec_type, 6. ares_dns_rr_key_datatype
    if (keys && cnt > 0) {
        // Test with the first key
        ares_dns_rr_key_t key = keys[0];
        const char* key_tostr_result = ares_dns_rr_key_tostr(key);
        ares_dns_rec_type_t key_to_rec_type_result = ares_dns_rr_key_to_rec_type(key);
        ares_dns_datatype_t key_datatype_result = ares_dns_rr_key_datatype(key);
        
        // Also test with a key derived from input data
        ares_dns_rr_key_t derived_key = (ares_dns_rr_key_t)(Data[0] % 10000);
        const char* derived_key_tostr = ares_dns_rr_key_tostr(derived_key);
        ares_dns_rec_type_t derived_key_to_rec_type = ares_dns_rr_key_to_rec_type(derived_key);
        ares_dns_datatype_t derived_key_datatype = ares_dns_rr_key_datatype(derived_key);
    }

    // Try with different derived types to explore more states
    for (int i = 0; i < 3 && i < (int)Size; i++) {
        ares_dns_rec_type_t alt_type = (ares_dns_rec_type_t)(Data[i] % 256);
        size_t alt_cnt = 0;
        const ares_dns_rr_key_t* alt_keys = ares_dns_rr_get_keys(alt_type, &alt_cnt);
        
        if (alt_keys && alt_cnt > 0) {
            // Test a few keys from this type
            for (int j = 0; j < 2 && j < (int)alt_cnt; j++) {
                ares_dns_rr_key_t test_key = alt_keys[j];
                const char* test_str = ares_dns_rr_key_tostr(test_key);
                ares_dns_rec_type_t test_rec_type = ares_dns_rr_key_to_rec_type(test_key);
                ares_dns_datatype_t test_datatype = ares_dns_rr_key_datatype(test_key);
            }
        }
    }

    return 0;
}