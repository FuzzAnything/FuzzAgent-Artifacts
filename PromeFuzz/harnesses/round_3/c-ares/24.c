// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include necessary headers for missing types */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_dns_record(const uint8_t* data, size_t size) {
    // Create a DNS record with values from fuzzer input
    ares_dns_record_t* dnsrec = NULL;
    
    // Extract values from fuzzer input
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (size >= 1) id = data[0];
    if (size >= 2) flags = data[1];
    if (size >= 3) opcode = (ares_dns_opcode_t)(data[2] % 16);  // Assume 16 opcodes
    if (size >= 4) rcode = (ares_dns_rcode_t)(data[3] % 16);   // Assume 16 rcodes
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) return NULL;
    
    return dnsrec;
}

static const char* get_valid_name(const uint8_t* data, size_t size, size_t offset) {
    // Use a fixed valid name for simplicity
    static const char* valid_name = "example.com";
    return valid_name;
}

static ares_dns_rec_type_t get_valid_type(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid record type based on input data
    if (size <= offset) return ARES_REC_TYPE_A;
    return (ares_dns_rec_type_t)(data[offset] % 256);
}

static ares_dns_class_t get_valid_class(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid class based on input data
    if (size <= offset) return ARES_CLASS_IN;
    return (ares_dns_class_t)(data[offset] % 65536);
}

static unsigned int get_valid_ttl(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid TTL based on input data
    if (size < offset + 4) return 300;
    return (unsigned int)((data[offset] << 24) | (data[offset + 1] << 16) | 
                          (data[offset + 2] << 8) | data[offset + 3]);
}

static ares_dns_section_t get_valid_section(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid section based on input data
    if (size <= offset) return ARES_SECTION_ANSWER;
    return (ares_dns_section_t)(data[offset] % 4);
}

static ares_dns_rr_key_t get_valid_key(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid key based on input data
    if (size <= offset) return 0;
    return (ares_dns_rr_key_t)(data[offset] % 1000);
}

static unsigned short get_valid_u16(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid 16-bit value based on input data
    if (size < offset + 2) return 12345;
    return (unsigned short)((data[offset] << 8) | data[offset + 1]);
}

static const char* get_valid_string(const uint8_t* data, size_t size, size_t offset) {
    // Return a valid string based on input data
    static char buffer[256];
    size_t max_len = 255;
    
    if (size <= offset) return "default";
    
    size_t len = size - offset;
    if (len > max_len) len = max_len;
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = data[offset + i];
        // Ensure printable ASCII
        buffer[i] = (c >= 32 && c < 127) ? c : 'a' + (c % 26);
    }
    buffer[len] = '\0';
    
    return buffer;
}

static const unsigned char* get_valid_bin_data(const uint8_t* data, size_t size, size_t offset, size_t* out_len) {
    // Return valid binary data based on input data
    if (size <= offset) {
        *out_len = 0;
        return NULL;
    }
    
    *out_len = size - offset;
    if (*out_len > 1024) *out_len = 1024;
    
    return data + offset;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;  // Need minimum data
    
    // Create DNS record
    ares_dns_record_t* dnsrec = create_dns_record(Data, Size);
    if (!dnsrec) return 0;
    
    ares_dns_rr_t* rr1 = NULL;
    ares_dns_rr_t* rr2 = NULL;
    ares_dns_rr_t* rr3 = NULL;
    
    size_t data_offset = 4;  // Skip first 4 bytes used for DNS record creation
    
    // 1. First ares_dns_rr_set_str
    // Need to create a RR first to set string on it
    // We'll create it in the next step
    
    // 2. First ares_dns_record_rr_add
    ares_status_t status = ares_dns_record_rr_add(
        &rr1, 
        dnsrec, 
        get_valid_section(Data, Size, data_offset),
        get_valid_name(Data, Size, data_offset),
        get_valid_type(Data, Size, data_offset + 1),
        get_valid_class(Data, Size, data_offset + 2),
        get_valid_ttl(Data, Size, data_offset + 3)
    );
    
    data_offset += 7;  // Advance offset for used data
    
    if (status == ARES_SUCCESS && rr1) {
        // 1. Now do the first ares_dns_rr_set_str (as per order)
        ares_dns_rr_set_str(rr1, get_valid_key(Data, Size, data_offset), 
                           get_valid_string(Data, Size, data_offset + 1));
        data_offset += 20;  // Advance offset for next operation
    }
    
    // 3. Second ares_dns_rr_set_str
    if (rr1) {
        ares_dns_rr_set_str(rr1, get_valid_key(Data, Size, data_offset), 
                           get_valid_string(Data, Size, data_offset + 1));
        data_offset += 20;
    }
    
    // 4. Second ares_dns_record_rr_add
    status = ares_dns_record_rr_add(
        &rr2, 
        dnsrec, 
        get_valid_section(Data, Size, data_offset),
        get_valid_name(Data, Size, data_offset + 1),
        get_valid_type(Data, Size, data_offset + 2),
        get_valid_class(Data, Size, data_offset + 3),
        get_valid_ttl(Data, Size, data_offset + 4)
    );
    
    data_offset += 7;
    
    if (status == ARES_SUCCESS && rr2) {
        // 5. First ares_dns_rr_add_abin
        size_t bin_len1 = 0;
        const unsigned char* bin_data1 = get_valid_bin_data(Data, Size, data_offset, &bin_len1);
        if (bin_data1 && bin_len1 > 0) {
            ares_dns_rr_add_abin(rr2, get_valid_key(Data, Size, data_offset), 
                                bin_data1, bin_len1);
            data_offset += bin_len1 + 1;
        }
        
        // 6. Second ares_dns_rr_add_abin
        size_t bin_len2 = 0;
        const unsigned char* bin_data2 = get_valid_bin_data(Data, Size, data_offset, &bin_len2);
        if (bin_data2 && bin_len2 > 0) {
            ares_dns_rr_add_abin(rr2, get_valid_key(Data, Size, data_offset), 
                                bin_data2, bin_len2);
            data_offset += bin_len2 + 1;
        }
    }
    
    // 7. Third ares_dns_record_rr_add
    status = ares_dns_record_rr_add(
        &rr3, 
        dnsrec, 
        get_valid_section(Data, Size, data_offset),
        get_valid_name(Data, Size, data_offset + 1),
        get_valid_type(Data, Size, data_offset + 2),
        get_valid_class(Data, Size, data_offset + 3),
        get_valid_ttl(Data, Size, data_offset + 4)
    );
    
    data_offset += 7;
    
    if (status == ARES_SUCCESS && rr3) {
        // 8. ares_dns_rr_set_u16
        ares_dns_rr_set_u16(rr3, get_valid_key(Data, Size, data_offset), 
                           get_valid_u16(Data, Size, data_offset + 1));
    }
    
    // Cleanup - destroy the parent record which owns all RRs
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}