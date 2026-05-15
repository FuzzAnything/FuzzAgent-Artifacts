// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_set_ttl at ares_dns_record.c:537:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_dns_record(void) {
    // Create a basic DNS record with some initial data
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 1234, 0, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    // Add some resource records to different sections for testing
    const char* name = "example.com";
    
    // Add a few records to ANSWER section
    ares_dns_rr_t* rr1 = NULL;
    ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, 
                           name, ARES_REC_TYPE_A, 
                           ARES_CLASS_IN, 300);
    
    ares_dns_rr_t* rr2 = NULL;
    ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_ANSWER, 
                           name, ARES_REC_TYPE_AAAA, 
                           ARES_CLASS_IN, 300);
    
    // Add a record to AUTHORITY section
    ares_dns_rr_t* rr3 = NULL;
    ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_AUTHORITY, 
                           "ns1.example.com", ARES_REC_TYPE_NS, 
                           ARES_CLASS_IN, 86400);
    
    // Add a record to ADDITIONAL section
    ares_dns_rr_t* rr4 = NULL;
    ares_dns_record_rr_add(&rr4, dnsrec, ARES_SECTION_ADDITIONAL, 
                           "example.com", ARES_REC_TYPE_TXT, 
                           ARES_CLASS_IN, 300);
    
    return dnsrec;
}

static void test_ares_dns_record_rr_del(ares_dns_record_t* dnsrec, 
                                       const uint8_t* data, size_t size) {
    if (!dnsrec || size < 2) return;
    
    // Use first byte for section, second for index
    ares_dns_section_t sect = (ares_dns_section_t)(data[0] % 4);
    size_t idx = data[1];
    
    // Try to delete the resource record
    ares_dns_record_rr_del(dnsrec, sect, idx);
}

static void test_ares_dns_rr_add_abin(ares_dns_record_t* dnsrec, 
                                     const uint8_t* data, size_t size) {
    if (!dnsrec || size < 3) return;
    
    // Get a resource record from ANSWER section (index 0 or 1)
    size_t rr_idx = data[0] % 2;
    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, rr_idx);
    if (!rr) return;
    
    // Try different keys that might be ABINP type
    // Use second byte to select key
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(data[1] % 100);
    
    // Use remaining data as binary data
    const unsigned char* val = data + 2;
    size_t len = size - 2;
    if (len > 0) {
        ares_dns_rr_add_abin(rr, key, val, len);
    }
}

static void test_ares_dns_rr_set_u16(ares_dns_record_t* dnsrec, 
                                    const uint8_t* data, size_t size) {
    if (!dnsrec || size < 3) return;
    
    // Get a resource record from ANSWER section
    size_t rr_idx = data[0] % 2;
    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, rr_idx);
    if (!rr) return;
    
    // Try different keys that might be U16 type
    // Use second byte to select key
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(data[1] % 100);
    
    // Use third byte for value
    unsigned short val = data[2];
    
    ares_dns_rr_set_u16(rr, key, val);
}

static void test_ares_dns_rr_set_ttl(ares_dns_record_t* dnsrec, 
                                    const uint8_t* data, size_t size) {
    if (!dnsrec || size < 1) return;
    
    // Get a resource record from different sections
    size_t section_idx = data[0] % 3;
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    ares_dns_section_t sect = sections[section_idx];
    size_t rr_idx = 0;
    
    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, sect, rr_idx);
    if (!rr) return;
    
    // Use data to create TTL value
    unsigned int ttl = 0;
    for (size_t i = 0; i < size && i < 4; i++) {
        ttl = (ttl << 8) | data[i];
    }
    
    ares_dns_rr_set_ttl(rr, ttl);
}

static void test_ares_dns_rr_get_ttl(ares_dns_record_t* dnsrec, 
                                    const uint8_t* data, size_t size) {
    if (!dnsrec || size < 1) return;
    
    // Try getting TTL from different sections and indices
    size_t section_idx = data[0] % 3;
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    ares_dns_section_t sect = sections[section_idx];
    size_t rr_idx = 0;
    
    if (size > 1) {
        rr_idx = data[1];
    }
    
    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, sect, rr_idx);
    if (rr) {
        unsigned int ttl = ares_dns_rr_get_ttl(rr);
        (void)ttl; // Use ttl to avoid unused variable warning
    }
}

static void test_ares_dns_record_rr_get(ares_dns_record_t* dnsrec, 
                                       const uint8_t* data, size_t size) {
    if (!dnsrec || size < 2) return;
    
    // Try getting resource records from different sections and indices
    ares_dns_section_t sect = (ares_dns_section_t)(data[0] % 4);
    size_t idx = data[1];
    
    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, sect, idx);
    if (rr) {
        // If we got a valid RR, try to get its TTL
        unsigned int ttl = ares_dns_rr_get_ttl(rr);
        (void)ttl; // Use ttl to avoid unused variable warning
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a test DNS record
    ares_dns_record_t* dnsrec = create_test_dns_record();
    if (!dnsrec) {
        return 0;
    }
    
    // Use first byte to decide which function(s) to test
    uint8_t func_selector = Data[0];
    
    // Test different functions based on the selector
    switch (func_selector % 6) {
        case 0:
            test_ares_dns_record_rr_del(dnsrec, Data + 1, Size - 1);
            break;
        case 1:
            test_ares_dns_rr_add_abin(dnsrec, Data + 1, Size - 1);
            break;
        case 2:
            test_ares_dns_rr_set_u16(dnsrec, Data + 1, Size - 1);
            break;
        case 3:
            test_ares_dns_rr_set_ttl(dnsrec, Data + 1, Size - 1);
            break;
        case 4:
            test_ares_dns_rr_get_ttl(dnsrec, Data + 1, Size - 1);
            break;
        case 5:
            test_ares_dns_record_rr_get(dnsrec, Data + 1, Size - 1);
            break;
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}