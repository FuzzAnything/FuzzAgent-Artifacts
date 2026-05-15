// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy callback functions for ares_query and ares_query_dnsrec
static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing - just a placeholder for the callback
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing - just a placeholder for the callback
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;  // Need at least some data for initialization
    }

    // Initialize source channel
    ares_channel_t *src_channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&src_channel, &options, optmask) != ARES_SUCCESS) {
        return 0;
    }

    // 1. Test ares_dup
    ares_channel_t *dest_channel = NULL;
    int dup_result = ares_dup(&dest_channel, src_channel);
    
    if (dup_result != ARES_SUCCESS) {
        ares_destroy(src_channel);
        return 0;
    }

    // 2. Test ares_set_local_ip4
    // Extract 4 bytes for local_ip4
    unsigned int local_ip4 = 0;
    if (Size >= 4) {
        memcpy(&local_ip4, Data, 4);
    }
    ares_set_local_ip4(dest_channel, local_ip4);

    // 3. Test ares_set_local_ip6
    // Extract 16 bytes for local_ip6
    unsigned char local_ip6[16] = {0};
    size_t ip6_offset = 4;
    if (Size >= ip6_offset + 16) {
        memcpy(local_ip6, Data + ip6_offset, 16);
    } else if (Size > ip6_offset) {
        memcpy(local_ip6, Data + ip6_offset, Size - ip6_offset);
    }
    ares_set_local_ip6(dest_channel, local_ip6);

    // 4. Test ares_set_local_dev
    // Use part of input as device name
    size_t dev_offset = ip6_offset + 16;
    if (dev_offset < Size) {
        size_t dev_len = Size - dev_offset;
        if (dev_len > 31) {
            dev_len = 31;  // Max length for local_dev_name
        }
        
        char local_dev_name[32] = {0};
        memcpy(local_dev_name, Data + dev_offset, dev_len);
        local_dev_name[dev_len] = '\0';
        ares_set_local_dev(dest_channel, local_dev_name);
    } else {
        ares_set_local_dev(dest_channel, NULL);
    }

    // 5. Test ares_query_dnsrec
    // Prepare query parameters
    const char *name = "example.com";
    ares_dns_class_t dnsclass = ARES_CLASS_IN;
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    unsigned short qid = 0;
    
    // Determine query name from input if possible
    char query_name[256] = {0};
    size_t query_offset = dev_offset + (Size > dev_offset ? (Size - dev_offset) : 0);
    if (query_offset < Size && (Size - query_offset) > 0) {
        size_t name_len = Size - query_offset;
        if (name_len > 255) {
            name_len = 255;
        }
        memcpy(query_name, Data + query_offset, name_len);
        query_name[name_len] = '\0';
        name = query_name;
    }

    ares_query_dnsrec(dest_channel, name, dnsclass, type, 
                      dummy_callback_dnsrec, NULL, &qid);

    // 6. Test ares_query (deprecated wrapper)
    // Use different parameters for variety
    ares_query(dest_channel, name, ARES_CLASS_IN, ARES_REC_TYPE_AAAA, 
               dummy_callback, NULL);

    // Cleanup
    if (dest_channel) {
        ares_destroy(dest_channel);
    }
    if (src_channel) {
        ares_destroy(src_channel);
    }

    return 0;
}