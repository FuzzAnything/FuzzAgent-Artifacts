// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

/* Dummy callback functions for the async queries */
static void dummy_callback_dnsrec(void* arg, ares_status_t status,
                                  unsigned long qid, const ares_dns_record_t* dnsrec) {
    /* Do nothing */
}

static void dummy_callback(void* arg, int status, int timeouts,
                           const unsigned char* abuf, int alen) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    int init_result;
    unsigned short qid = 0;
    ares_status_t status;
    
    /* Initialize the original channel */
    init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        /* If initialization fails, return early */
        return 0;
    }
    
    /* 1. ares_dup */
    int dup_result = ares_dup(&dup_channel, channel);
    /* We don't check the result because we want to test error paths too */
    
    /* Use fuzzer data to generate parameters for the functions */
    /* For simplicity, we'll use the first 4 bytes as the local IPv4 address */
    unsigned int local_ip4 = 0;
    if (Size >= 4) {
        local_ip4 = *((unsigned int*)Data);
    }
    
    /* Use next 16 bytes for local IPv6 address if available */
    const unsigned char* local_ip6 = NULL;
    static unsigned char local_ip6_buffer[16] = {0};
    if (Size >= 20) {
        memcpy(local_ip6_buffer, Data + 4, 16);
        local_ip6 = local_ip6_buffer;
    }
    
    /* 2. ares_set_local_ip4 */
    ares_set_local_ip4(channel, local_ip4);
    
    /* 3. ares_set_local_ip6 */
    if (local_ip6 != NULL) {
        ares_set_local_ip6(channel, local_ip6);
    }
    
    /* 4. ares_set_local_dev */
    const char* local_dev_name = NULL;
    if (Size > 20) {
        /* Use up to 31 bytes for device name (leave room for null terminator) */
        size_t dev_name_len = Size - 20;
        if (dev_name_len > 31) dev_name_len = 31;
        
        /* Create a null-terminated string from the data */
        char dev_name[32] = {0};
        memcpy(dev_name, Data + 20, dev_name_len);
        local_dev_name = dev_name;
        
        ares_set_local_dev(channel, local_dev_name);
    } else {
        ares_set_local_dev(channel, NULL);
    }
    
    /* 5. ares_query_dnsrec */
    const char* query_name = "example.com";
    if (Size > 0) {
        /* Use the first byte to determine which query name to use */
        uint8_t query_selector = Data[0] % 3;
        if (query_selector == 0) {
            query_name = "example.com";
        } else if (query_selector == 1) {
            query_name = "localhost";
        } else {
            query_name = "test";
        }
    }
    
    status = ares_query_dnsrec(channel, query_name, ARES_CLASS_IN, 
                               ARES_REC_TYPE_A, dummy_callback_dnsrec, 
                               NULL, &qid);
    
    /* 6. ares_query (deprecated but we need to call it) */
    ares_query(channel, query_name, ARES_CLASS_IN, ARES_REC_TYPE_A, 
               dummy_callback, NULL);
    
    /* Clean up */
    if (dup_channel != NULL) {
        ares_destroy(dup_channel);
    }
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    return 0;
}