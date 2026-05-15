// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // Cleanup function if needed
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    // Create dummy file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size > 1024 ? 1024 : Size);
        close(fd);
    }

    // Initialize structures for ares_dns_pton
    struct ares_addr addr;
    size_t out_len;
    
    // Prepare IP address string from fuzz data
    char ip_str[256];
    size_t ip_len = Size > 255 ? 255 : Size;
    memcpy(ip_str, Data, ip_len);
    ip_str[ip_len] = '\0';
    
    // Call ares_dns_pton with different family values
    addr.family = AF_UNSPEC;
    const void *result = ares_dns_pton(ip_str, &addr, &out_len);
    
    addr.family = AF_INET;
    result = ares_dns_pton(ip_str, &addr, &out_len);
    
    addr.family = AF_INET6;
    result = ares_dns_pton(ip_str, &addr, &out_len);

    // Create DNS record with proper parameters
    ares_dns_record_t *dnsrec = NULL;
    
    // Extract parameters from fuzz data for ares_dns_record_create
    unsigned short id = *(unsigned short*)Data;
    unsigned short flags = *(unsigned short*)(Data + 2);
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(Data[4] % 16);
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(Data[5] % 16);
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        // Cleanup and return if we can't create a DNS record
        unlink("./dummy_file");
        return 0;
    }
    
    // Create a resource record handle
    ares_dns_rr_t *rr_out = NULL;
    
    // Extract parameters from fuzz data for ares_dns_record_rr_add
    unsigned int ttl = *(unsigned int*)(Data + 6);
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)(Data[10] % 100);
    ares_dns_class_t rclass = (ares_dns_class_t)(Data[11] % 4);
    ares_dns_section_t sect = (ares_dns_section_t)(Data[12] % 4);
    
    char name[256];
    size_t name_len = (Size - 13) > 255 ? 255 : (Size - 13);
    if (name_len > 0) {
        memcpy(name, Data + 13, name_len);
        name[name_len] = '\0';
    } else {
        name[0] = '\0';
    }
    
    // Call ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, name, type, rclass, ttl);
    
    if (status == ARES_SUCCESS && rr_out != NULL) {
        // Prepare string value for ares_dns_rr_set_str
        char str_val[256];
        size_t str_len = Size > 200 ? 200 : Size;
        memcpy(str_val, Data, str_len);
        str_val[str_len] = '\0';
        
        // Call ares_dns_rr_set_str
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[13] % 100);
        status = ares_dns_rr_set_str(rr_out, key, str_val);
        
        // Prepare data for ares_dns_rr_set_opt calls
        unsigned short opt1 = *(unsigned short*)(Data + 14);
        unsigned short opt2 = *(unsigned short*)(Data + 16);
        
        ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[18] % 100);
        ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[19] % 100);
        
        unsigned char val_data[256];
        size_t val_len = (Size - 20) > 256 ? 256 : (Size - 20);
        if (val_len > 0) {
            memcpy(val_data, Data + 20, val_len);
        } else {
            val_len = 0;
        }
        
        // First call to ares_dns_rr_set_opt
        if (val_len > 0) {
            status = ares_dns_rr_set_opt(rr_out, key1, opt1, val_data, val_len);
        }
        
        // Second call to ares_dns_rr_set_opt
        if (val_len > 0) {
            status = ares_dns_rr_set_opt(rr_out, key2, opt2, val_data, val_len % 128);
        }
    }
    
    // Cleanup - destroy the DNS record
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    
    dummy_cleanup();
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}