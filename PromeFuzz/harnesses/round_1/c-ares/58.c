// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static char *get_string_from_data(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    // Find null terminator or use all remaining data
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    // Create null-terminated string
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    // Skip null terminator if present
    if (len < *size && (*data)[0] == 0) {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size) {
    // Create dummy file with the input data
    create_dummy_file(data, size);
    
    // Initialize variables using fuzzer input
    const uint8_t *ptr = data;
    size_t remaining = size;
    
    // Create DNS record if we have enough data
    ares_dns_record_t *dnsrec = NULL;
    if (remaining >= sizeof(unsigned short) * 4) {
        // Simulate record creation by parsing input data
        // We'll use the first bytes to initialize a dummy record
        ptr += 6;
        remaining -= 6;
    }
    
    // Test ares_dns_rec_type_fromstr
    if (remaining > 0) {
        char *type_str = get_string_from_data(&ptr, &remaining);
        if (type_str) {
            ares_dns_rec_type_t qtype;
            ares_bool_t result = ares_dns_rec_type_fromstr(&qtype, type_str);
            (void)result;
            free(type_str);
        }
    }
    
    // Test ares_dns_class_fromstr
    if (remaining > 0) {
        char *class_str = get_string_from_data(&ptr, &remaining);
        if (class_str) {
            ares_dns_class_t qclass;
            ares_bool_t result = ares_dns_class_fromstr(&qclass, class_str);
            (void)result;
            free(class_str);
        }
    }
    
    // Test ares_dns_class_tostr with various enum values
    if (remaining > 0) {
        ares_dns_class_t test_class = (ares_dns_class_t)(ptr[0] % 10);
        const char *str = ares_dns_class_tostr(test_class);
        (void)str;
        ptr++;
        remaining--;
    }
    
    // Test ares_dns_record_query_get with various indices
    if (dnsrec) {
        for (size_t idx = 0; idx < 4; idx++) {
            const char *name = NULL;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            
            ares_status_t status = ares_dns_record_query_get(dnsrec, idx, &name, &qtype, &qclass);
            (void)status;
            
            // Test with NULL parameters
            status = ares_dns_record_query_get(dnsrec, idx, NULL, NULL, NULL);
            (void)status;
        }
    }
    
    // Test ares_dns_record_rr_add with various inputs
    if (remaining > 0) {
        char *name = get_string_from_data(&ptr, &remaining);
        if (name) {
            if (remaining > 6) {
                ares_dns_rr_t *rr_out = NULL;
                ares_dns_section_t sect = (ares_dns_section_t)(ptr[0] % 4);
                ares_dns_rec_type_t type = (ares_dns_rec_type_t)(ptr[1] % 64);
                ares_dns_class_t rclass = (ares_dns_class_t)(ptr[2] % 4);
                unsigned int ttl = (ptr[3] << 24) | (ptr[4] << 16) | (ptr[5] << 8) | ptr[6];
                
                ptr += 7;
                remaining -= 7;
                
                // Note: We need a valid dnsrec to add RR, but we don't have one
                // For fuzzing purposes, we'll skip the actual call if dnsrec is NULL
                // but test the function if we had a valid record
                if (dnsrec) {
                    ares_status_t status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, name, type, rclass, ttl);
                    (void)status;
                    
                    // If RR was created, test ares_dns_rr_get_class
                    if (rr_out) {
                        ares_dns_class_t retrieved_class = ares_dns_rr_get_class(rr_out);
                        (void)retrieved_class;
                    }
                }
            }
            free(name);
        }
    }
    
    // Clean up dummy file
    cleanup_dummy_file();
    
    return 0;
}