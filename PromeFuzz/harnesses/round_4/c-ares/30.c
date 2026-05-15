// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void test_ares_dns_record_get_id(const uint8_t *data, size_t size) {
    if (size < sizeof(unsigned short)) return;
    
    unsigned short id = *((unsigned short*)data);
    ares_dns_record_t *dnsrec = NULL;
    
    // Create a DNS record with the ID from fuzzer input
    ares_status_t status = ares_dns_record_create(&dnsrec, id, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        // Test the function
        unsigned short retrieved_id = ares_dns_record_get_id(dnsrec);
        (void)retrieved_id; // Use result to avoid unused variable warning
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
    }
}

static void test_ares_mkquery(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte as name length, but ensure null termination
    size_t name_len = data[0] % 64; // Limit to reasonable size
    if (name_len == 0) name_len = 1;
    if (name_len > size - 1) name_len = size - 1;
    
    char name[65] = {0};
    if (name_len > 64) name_len = 64;
    for (size_t i = 0; i < name_len && i + 1 < size; i++) {
        name[i] = data[i + 1];
    }
    name[name_len] = '\0';
    
    // Extract parameters from remaining data
    size_t offset = 1 + name_len;
    if (offset + 4 >= size) return;
    
    int dnsclass = data[offset] % 4; // Common classes: 0-3
    int type = data[offset + 1] % 256; // DNS types
    unsigned short id = *((unsigned short*)(data + offset + 2));
    int rd = data[offset + 4] & 1; // 0 or 1
    
    unsigned char *buf = NULL;
    int buflen = 0;
    
    // Test the function
    int result = ares_mkquery(name, dnsclass, type, id, rd, &buf, &buflen);
    (void)result; // Use result to avoid unused variable warning
    
    // Clean up
    if (buf != NULL) {
        ares_free_string(buf);
    }
}

static void test_ares_dns_write(const uint8_t *data, size_t size) {
    if (size < sizeof(unsigned short)) return;
    
    unsigned short id = *((unsigned short*)data);
    ares_dns_record_t *dnsrec = NULL;
    
    // Create a DNS record
    ares_status_t status = ares_dns_record_create(&dnsrec, id, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        unsigned char *buf = NULL;
        size_t buf_len = 0;
        
        // Test the function
        ares_status_t write_status = ares_dns_write(dnsrec, &buf, &buf_len);
        (void)write_status; // Use result to avoid unused variable warning
        
        // Clean up
        if (buf != NULL) {
            ares_free_string(buf);
        }
        ares_dns_record_destroy(dnsrec);
    }
}

static void test_ares_dns_record_create(const uint8_t *data, size_t size) {
    if (size < 6) return; // Need at least 6 bytes: 2 for id, 2 for flags, 1 for opcode, 1 for rcode
    
    // Extract parameters from fuzzer input with bounds checking
    unsigned short id = 0;
    unsigned short flags = 0;
    
    // Use memcpy to avoid potential alignment issues
    memcpy(&id, data, sizeof(unsigned short));
    memcpy(&flags, data + 2, sizeof(unsigned short));
    
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(data[4] % 16); // Valid opcodes are 0-15
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(data[5] % 16); // Valid rcodes are 0-15
    
    ares_dns_record_t *dnsrec = NULL;
    
    // Test the function
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    (void)status; // Use result to avoid unused variable warning
    
    // Clean up - IMPORTANT: Avoid memory leak
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
}

static void test_ares_dns_parse(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use fuzzer input as the buffer to parse
    unsigned int flags = data[0] % 4; // Common flags: 0-3
    ares_dns_record_t *dnsrec = NULL;
    
    // Test the function
    ares_status_t status = ares_dns_parse(data, size, flags, &dnsrec);
    (void)status; // Use result to avoid unused variable warning
    
    // Clean up
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
}

static void test_ares_create_query(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte as name length, but ensure null termination
    size_t name_len = data[0] % 64; // Limit to reasonable size
    if (name_len == 0) name_len = 1;
    if (name_len > size - 1) name_len = size - 1;
    
    char name[65] = {0};
    if (name_len > 64) name_len = 64;
    for (size_t i = 0; i < name_len && i + 1 < size; i++) {
        name[i] = data[i + 1];
    }
    name[name_len] = '\0';
    
    // Extract parameters from remaining data
    size_t offset = 1 + name_len;
    if (offset + 5 >= size) return;
    
    int dnsclass = data[offset] % 4; // Common classes: 0-3
    int type = data[offset + 1] % 256; // DNS types
    unsigned short id = 0;
    memcpy(&id, data + offset + 2, sizeof(unsigned short));
    int rd = data[offset + 4] & 1; // 0 or 1
    int max_udp_size = data[offset + 5] * 256; // Scale to reasonable UDP size
    
    unsigned char *buf = NULL;
    int buflen = 0;
    
    // Test the function
    int result = ares_create_query(name, dnsclass, type, id, rd, &buf, &buflen, max_udp_size);
    (void)result; // Use result to avoid unused variable warning
    
    // Clean up
    if (buf != NULL) {
        ares_free_string(buf);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to test
    unsigned char selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector % 6) {
        case 0:
            test_ares_dns_record_get_id(remaining_data, remaining_size);
            break;
        case 1:
            test_ares_mkquery(remaining_data, remaining_size);
            break;
        case 2:
            test_ares_dns_write(remaining_data, remaining_size);
            break;
        case 3:
            test_ares_dns_record_create(remaining_data, remaining_size);
            break;
        case 4:
            test_ares_dns_parse(remaining_data, remaining_size);
            break;
        case 5:
            test_ares_create_query(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}