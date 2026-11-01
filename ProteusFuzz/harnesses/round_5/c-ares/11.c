// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN before including c-ares headers to resolve compilation errors */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types to resolve compilation errors */
typedef enum { ARES_FALSE = 0, ARES_TRUE = 1 } ares_bool_t;
typedef enum { ARES_SUCCESS = 0, ARES_EBADQUERY = 1, ARES_EBADNAME = 2, ARES_EBADFAMILY = 3, ARES_ENODATA = 4, ARES_EBADRESP = 5, ARES_ETIMEOUT = 6, ARES_EOF = 7, ARES_EFILE = 8, ARES_ENOMEM = 9, ARES_EDESTRUCTION = 10, ARES_EBADSTR = 11, ARES_EBADFLAGS = 12, ARES_ENOTFOUND = 13, ARES_ECONNREFUSED = 14, ARES_EINVAL = 15, ARES_EBADHINTS = 16, ARES_EBADID = 17, ARES_ENOTIMP = 18, ARES_EDEPRECATED = 19, ARES_ECANCELLED = 20 } ares_status_t;

#include <ares_dns_record.h>

/* Helper to extract a string from the fuzzer input */
static const char *extract_string(const uint8_t *Data, size_t Size, size_t *len) {
    if (Size == 0) {
        *len = 0;
        return NULL;
    }
    
    size_t i = 0;
    while (i < Size && Data[i] != '\0') {
        i++;
    }
    
    if (i >= Size) {
        *len = 0;
        return NULL;
    }
    
    *len = i;
    return (const char *)Data;
}

/* Helper to extract a numeric value from fuzzer input */
static ares_dns_rec_type_t extract_rec_type(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return ARES_REC_TYPE_A;
    }
    
    /* Use the first byte as a seed, but limit to reasonable range */
    uint8_t val = Data[0];
    
    /* Map to some known types to ensure valid testing */
    switch (val % 10) {
        case 0: return ARES_REC_TYPE_A;
        case 1: return ARES_REC_TYPE_NS;
        case 2: return ARES_REC_TYPE_CNAME;
        case 3: return ARES_REC_TYPE_SOA;
        case 4: return ARES_REC_TYPE_PTR;
        case 5: return ARES_REC_TYPE_MX;
        case 6: return ARES_REC_TYPE_TXT;
        case 7: return ARES_REC_TYPE_AAAA;
        case 8: return ARES_REC_TYPE_SRV;
        case 9: return ARES_REC_TYPE_ANY;
        default: return ARES_REC_TYPE_A;
    }
}

/* Helper to extract an RR key from fuzzer input */
static ares_dns_rr_key_t extract_rr_key(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return ARES_RR_A_ADDR;
    }
    
    /* Use the first byte as a seed */
    uint8_t val = Data[0];
    
    /* Map to some known keys to ensure valid testing */
    switch (val % 5) {
        case 0: return ARES_RR_A_ADDR;
        case 1: return ARES_RR_NS_NSDNAME;
        case 2: return ARES_RR_CNAME_CNAME;
        case 3: return ARES_RR_SOA_MNAME;
        case 4: return ARES_RR_SOA_RNAME;
        default: return ARES_RR_A_ADDR;
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    /* Step 1: ares_dns_rec_type_fromstr */
    size_t str_len;
    const char *str = extract_string(Data, Size, &str_len);
    ares_dns_rec_type_t qtype;
    
    if (str != NULL && str_len > 0) {
        ares_dns_rec_type_fromstr(&qtype, str);
        
        /* Step 2: ares_dns_rec_type_tostr */
        const char *type_str = ares_dns_rec_type_tostr(qtype);
        (void)type_str; /* Suppress unused variable warning */
        
        /* Step 3: ares_dns_rr_get_keys */
        size_t cnt = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(qtype, &cnt);
        
        if (keys != NULL) {
            /* Step 4: ares_dns_rr_key_tostr for each key */
            for (size_t i = 0; i < cnt; i++) {
                const char *key_str = ares_dns_rr_key_tostr(keys[i]);
                (void)key_str; /* Suppress unused variable warning */
                
                /* Step 5: ares_dns_rr_key_to_rec_type */
                ares_dns_rec_type_t rec_type = ares_dns_rr_key_to_rec_type(keys[i]);
                (void)rec_type; /* Suppress unused variable warning */
                
                /* Step 6: ares_dns_rr_key_datatype */
                ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[i]);
                (void)datatype; /* Suppress unused variable warning */
            }
        }
    }
    
    /* Additional exploration: Test with extracted types and keys */
    ares_dns_rec_type_t test_type = extract_rec_type(Data, Size);
    ares_dns_rr_key_t test_key = extract_rr_key(Data, Size);
    
    /* Test ares_dns_rec_type_tostr with extracted type */
    const char *test_type_str = ares_dns_rec_type_tostr(test_type);
    (void)test_type_str;
    
    /* Test ares_dns_rr_get_keys with extracted type */
    size_t test_cnt = 0;
    const ares_dns_rr_key_t *test_keys = ares_dns_rr_get_keys(test_type, &test_cnt);
    
    if (test_keys != NULL) {
        for (size_t i = 0; i < test_cnt; i++) {
            const char *test_key_str = ares_dns_rr_key_tostr(test_keys[i]);
            (void)test_key_str;
            
            ares_dns_rec_type_t test_rec_type = ares_dns_rr_key_to_rec_type(test_keys[i]);
            (void)test_rec_type;
            
            ares_dns_datatype_t test_datatype = ares_dns_rr_key_datatype(test_keys[i]);
            (void)test_datatype;
        }
    }
    
    /* Test with extracted key directly */
    const char *direct_key_str = ares_dns_rr_key_tostr(test_key);
    (void)direct_key_str;
    
    ares_dns_rec_type_t direct_rec_type = ares_dns_rr_key_to_rec_type(test_key);
    (void)direct_rec_type;
    
    ares_dns_datatype_t direct_datatype = ares_dns_rr_key_datatype(test_key);
    (void)direct_datatype;
    
    return 0;
}