// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static const char* get_random_string(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return NULL;
    }
    
    size_t max_len = size - *offset;
    size_t len = data[*offset] % (max_len + 1);
    if (len == 0 && max_len > 0) {
        len = 1;
    }
    
    char* str = (char*)malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    for (size_t i = 0; i < len && (*offset + i) < size; i++) {
        str[i] = data[*offset + i];
    }
    str[len] = '\0';
    
    *offset += len;
    return str;
}

static ares_dns_class_t get_random_class(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return 0;
    }
    ares_dns_class_t qclass = (ares_dns_class_t)(data[*offset] % 65536);
    (*offset)++;
    return qclass;
}

static ares_dns_rec_type_t get_random_type(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return 0;
    }
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)(data[*offset] % 65536);
    (*offset)++;
    return type;
}

static ares_dns_section_t get_random_section(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return 0;
    }
    ares_dns_section_t sect = (ares_dns_section_t)(data[*offset] % 4);
    (*offset)++;
    return sect;
}

static unsigned int get_random_ttl(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 3 >= size) {
        return 0;
    }
    unsigned int ttl = 0;
    for (int i = 0; i < 4 && (*offset + i) < size; i++) {
        ttl = (ttl << 8) | data[*offset + i];
    }
    *offset += 4;
    return ttl;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Test ares_dns_class_tostr
    ares_dns_class_t qclass1 = get_random_class(Data, Size, &offset);
    const char* str1 = ares_dns_class_tostr(qclass1);
    (void)str1;
    
    // Test ares_dns_class_fromstr
    const char* class_str = get_random_string(Data, Size, &offset);
    if (class_str) {
        ares_dns_class_t qclass2;
        ares_bool_t result = ares_dns_class_fromstr(&qclass2, class_str);
        (void)result;
        free((void*)class_str);
    }
    
    // Test ares_dns_parse
    if (offset < Size) {
        size_t remaining = Size - offset;
        const unsigned char* buf = Data + offset;
        ares_dns_record_t* dnsrec = NULL;
        unsigned int flags = 0;
        if (remaining > 0) {
            flags = Data[offset] % 256;
        }
        
        ares_status_t parse_status = ares_dns_parse(buf, remaining, flags, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec) {
            // Test ares_dns_record_query_get
            for (size_t idx = 0; idx < 10; idx++) {
                const char* name = NULL;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                
                ares_status_t query_status = ares_dns_record_query_get(
                    dnsrec, idx, &name, &qtype, &qclass);
                
                if (query_status != ARES_SUCCESS) {
                    break;
                }
            }
            
            // Test ares_dns_record_rr_add
            const char* rr_name = get_random_string(Data, Size, &offset);
            if (rr_name) {
                ares_dns_rr_t* rr_out = NULL;
                ares_dns_rec_type_t type = get_random_type(Data, Size, &offset);
                ares_dns_class_t rclass = get_random_class(Data, Size, &offset);
                ares_dns_section_t sect = get_random_section(Data, Size, &offset);
                unsigned int ttl = get_random_ttl(Data, Size, &offset);
                
                ares_status_t add_status = ares_dns_record_rr_add(
                    &rr_out, dnsrec, sect, rr_name, type, rclass, ttl);
                
                if (add_status == ARES_SUCCESS && rr_out) {
                    // Test ares_dns_rr_get_class
                    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_out);
                    (void)rr_class;
                }
                
                free((void*)rr_name);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    return 0;
}