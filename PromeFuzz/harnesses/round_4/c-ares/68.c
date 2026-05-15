// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_rr_t* create_dummy_rr(const uint8_t *Data, size_t Size) {
    // Create a DNS record structure with proper arguments
    ares_dns_record_t* dns_rec = NULL;
    ares_status_t status;
    
    // Use fuzzer data to generate some parameters
    unsigned short id = (Size > 0) ? Data[0] : 0;
    unsigned short flags = (Size > 1) ? Data[1] : 0;
    ares_dns_opcode_t opcode = (Size > 2) ? (ares_dns_opcode_t)(Data[2] % 16) : 0;
    ares_dns_rcode_t rcode = (Size > 3) ? (ares_dns_rcode_t)(Data[3] % 16) : 0;
    
    status = ares_dns_record_create(&dns_rec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dns_rec) {
        return NULL;
    }
    
    // Create an OPT RR by adding it to the record
    ares_dns_rr_t* rr = NULL;
    const char* name = ".";
    ares_dns_rec_type_t type = 41; // ARES_REC_TYPE_OPT
    ares_dns_class_t rclass = 0; // OPT uses class 0
    unsigned int ttl = 0;
    
    // Use fuzzer data for section (0-3)
    ares_dns_section_t sect = (Size > 4) ? (ares_dns_section_t)(Data[4] % 4) : 3;
    
    status = ares_dns_record_rr_add(&rr, dns_rec, sect, name, type, rclass, ttl);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dns_rec);
        return NULL;
    }
    
    // Store the parent record pointer in the RR for later cleanup
    // We'll use a global variable to track it
    static ares_dns_record_t* parent_record = NULL;
    if (parent_record) {
        ares_dns_record_destroy(parent_record);
    }
    parent_record = dns_rec;
    
    return rr;
}

static void test_ares_dns_rr_get_str(const ares_dns_rr_t* dns_rr, ares_dns_rr_key_t key) {
    const char* str = ares_dns_rr_get_str(dns_rr, key);
    (void)str; // Use result to avoid unused variable warning
}

static void test_ares_dns_rr_key_tostr(ares_dns_rr_key_t key) {
    const char* str = ares_dns_rr_key_tostr(key);
    (void)str;
}

static void test_ares_dns_rr_set_opt(ares_dns_rr_t* dns_rr, ares_dns_rr_key_t key, 
                                     unsigned short opt, const unsigned char* val, 
                                     size_t val_len) {
    ares_status_t status = ares_dns_rr_set_opt(dns_rr, key, opt, val, val_len);
    (void)status;
}

static void test_ares_dns_rr_key_datatype(ares_dns_rr_key_t key) {
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype;
}

static void test_ares_dns_rr_get_opt(const ares_dns_rr_t* dns_rr, ares_dns_rr_key_t key, 
                                     size_t idx, const unsigned char** val, 
                                     size_t* val_len) {
    unsigned short opt = ares_dns_rr_get_opt(dns_rr, key, idx, val, val_len);
    (void)opt;
}

static void test_ares_dns_opt_get_name(ares_dns_rr_key_t key, unsigned short opt) {
    const char* name = ares_dns_opt_get_name(key, opt);
    (void)name;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy RR for testing
    ares_dns_rr_t* rr = create_dummy_rr(Data, Size);
    if (!rr) return 0;
    
    // Extract some values from fuzzer input
    unsigned short opt_key = 0;
    size_t idx = 0;
    ares_dns_rr_key_t key = 100; // ARES_RR_OPT_OPTIONS = 100
    
    if (Size >= 2) {
        opt_key = (Data[0] << 8) | Data[1];
        idx = Data[0] % 10; // Keep index small
        key = (ares_dns_rr_key_t)(Data[1] % 200); // Generate valid-ish key
    }
    
    // Prepare dummy value for set_opt
    const unsigned char* val = (Size > 0) ? Data : NULL;
    size_t val_len = (Size > 0) ? (Size % 256) : 0;
    
    // Test all target functions
    test_ares_dns_rr_get_str(rr, key);
    test_ares_dns_rr_key_tostr(key);
    test_ares_dns_rr_set_opt(rr, key, opt_key, val, val_len);
    test_ares_dns_rr_key_datatype(key);
    
    // Test get_opt with dummy pointers
    const unsigned char* out_val = NULL;
    size_t out_val_len = 0;
    test_ares_dns_rr_get_opt(rr, key, idx, &out_val, &out_val_len);
    
    test_ares_dns_opt_get_name(key, opt_key);
    
    // Note: We don't need to explicitly destroy rr since it's owned by the parent record
    // The parent record will be destroyed in the next call to create_dummy_rr
    
    return 0;
}