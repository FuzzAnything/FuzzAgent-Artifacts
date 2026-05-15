// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

/* Dummy callback for ares_query_dnsrec */
static void dummy_dnsrec_callback(void *arg, ares_status_t status,
                                  unsigned long qid, const struct ares_dns_record *dnsrec) {
    /* Do nothing */
    (void)arg;
    (void)status;
    (void)qid;
    (void)dnsrec;
}

/* Dummy callback for ares_query (deprecated) */
static void dummy_callback(void *arg, int status, int timeouts,
                           const unsigned char *abuf, int alen) {
    /* Do nothing */
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    /* Minimum size needed for basic parameters */
    if (Size < 22) return 0;

    /* Initialize a channel */
    ares_channel_t *channel = NULL;
    
    /* Simple initialization with defaults */
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    /* 1. ares_dup */
    ares_channel_t *dup_channel = NULL;
    int dup_result = ares_dup(&dup_channel, channel);
    (void)dup_result; /* Result unused but function called */

    /* 2. ares_set_local_ip4 - use first 4 bytes as IP address */
    unsigned int local_ip4 = 0;
    if (Size >= 4) {
        memcpy(&local_ip4, Data, 4);
    }
    ares_set_local_ip4(channel, local_ip4);
    if (dup_channel) {
        ares_set_local_ip4(dup_channel, local_ip4);
    }

    /* 3. ares_set_local_ip6 - use next 16 bytes as IPv6 address */
    const unsigned char *local_ip6 = NULL;
    if (Size >= 20) {
        local_ip6 = Data + 4;
        ares_set_local_ip6(channel, local_ip6);
        if (dup_channel) {
            ares_set_local_ip6(dup_channel, local_ip6);
        }
    }

    /* 4. ares_set_local_dev - use remaining bytes as device name */
    const char *local_dev_name = NULL;
    if (Size > 20) {
        /* Ensure null termination for device name */
        size_t dev_name_len = Size - 20;
        if (dev_name_len > 31) dev_name_len = 31; /* Max 31 chars + null */
        
        char dev_buf[32] = {0};
        memcpy(dev_buf, Data + 20, dev_name_len);
        dev_buf[dev_name_len] = '\0';
        
        ares_set_local_dev(channel, dev_buf);
        if (dup_channel) {
            ares_set_local_dev(dup_channel, dev_buf);
        }
    } else {
        /* Clear device name */
        ares_set_local_dev(channel, NULL);
        if (dup_channel) {
            ares_set_local_dev(dup_channel, NULL);
        }
    }

    /* 5. ares_query_dnsrec */
    /* Extract DNS class and type from data */
    ares_dns_class_t dnsclass = ARES_CLASS_IN;
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    
    if (Size >= 22) {
        /* Use bytes 20 and 21 for class and type (modulo enum ranges) */
        dnsclass = (ares_dns_class_t)(Data[20] % 4); /* 0-3 valid classes */
        type = (ares_dns_rec_type_t)(Data[21] % 256); /* 0-255 valid types */
    }
    
    /* Create query name - use a safe domain */
    const char *query_name = "example.com";
    unsigned short qid = 0;
    ares_status_t query_status;
    
    query_status = ares_query_dnsrec(channel, query_name, dnsclass, type,
                                     dummy_dnsrec_callback, NULL, &qid);
    (void)query_status; /* Result unused but function called */

    /* 6. ares_query (deprecated) */
    /* Convert enums to int for deprecated function */
    int legacy_dnsclass = (int)dnsclass;
    int legacy_type = (int)type;
    
    ares_query(channel, query_name, legacy_dnsclass, legacy_type,
               dummy_callback, NULL);

    /* Cleanup */
    if (dup_channel) {
        ares_destroy(dup_channel);
    }
    ares_destroy(channel);
    
    return 0;
}