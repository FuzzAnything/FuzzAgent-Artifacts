// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Empty callback for fuzzing
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, unsigned long qid, const ares_dns_record_t *dnsrec) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Initialize source channel
    ares_channel_t *src_channel = NULL;
    int init_result = ares_init(&src_channel);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // 1. ares_dup
    ares_channel_t *dest_channel = NULL;
    int dup_result = ares_dup(&dest_channel, src_channel);
    
    // Only proceed if duplication was successful
    if (dup_result == ARES_SUCCESS && dest_channel != NULL) {
        // Use fuzzer data for parameters
        size_t offset = 0;
        
        // 2. ares_set_local_ip4 - use 4 bytes from fuzzer input
        unsigned int local_ip4 = 0;
        if (Size >= offset + 4) {
            memcpy(&local_ip4, Data + offset, 4);
            offset += 4;
        }
        ares_set_local_ip4(dest_channel, local_ip4);
        
        // 3. ares_set_local_ip6 - use 16 bytes from fuzzer input
        unsigned char local_ip6[16] = {0};
        if (Size >= offset + 16) {
            memcpy(local_ip6, Data + offset, 16);
            offset += 16;
        }
        ares_set_local_ip6(dest_channel, local_ip6);
        
        // 4. ares_set_local_dev - use next bytes as device name (null-terminated)
        char local_dev_name[32] = {0};
        size_t dev_name_len = 0;
        if (offset < Size) {
            dev_name_len = Size - offset;
            if (dev_name_len > 31) {
                dev_name_len = 31;
            }
            memcpy(local_dev_name, Data + offset, dev_name_len);
            local_dev_name[dev_name_len] = '\0';
        }
        ares_set_local_dev(dest_channel, local_dev_name);
        
        // 5. ares_query_dnsrec - use remaining data for query name
        const char *query_name = "example.com";
        if (offset < Size) {
            // Create a null-terminated string from remaining data
            size_t name_len = Size - offset;
            if (name_len > 255) {
                name_len = 255;
            }
            char *dynamic_name = (char*)malloc(name_len + 1);
            if (dynamic_name) {
                memcpy(dynamic_name, Data + offset, name_len);
                dynamic_name[name_len] = '\0';
                query_name = dynamic_name;
            }
        }
        
        unsigned short qid = 0;
        ares_query_dnsrec(dest_channel, query_name, 
                         ARES_CLASS_IN, ARES_REC_TYPE_A,
                         dummy_callback_dnsrec, NULL, &qid);
        
        // 6. ares_query (deprecated wrapper)
        ares_query(dest_channel, query_name, 
                  ARES_CLASS_IN, ARES_REC_TYPE_A,
                  dummy_callback, NULL);
        
        // Clean up dynamically allocated query name if used
        if (offset < Size && query_name != "example.com") {
            free((void*)query_name);
        }
        
        // Clean up duplicated channel
        ares_destroy(dest_channel);
    }
    
    // Clean up source channel
    ares_destroy(src_channel);
    
    return 0;
}