// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_local_ip4 at ares_init.c:546:6 in ares.h
// ares_set_local_ip6 at ares_init.c:557:6 in ares.h
// ares_set_local_dev at ares_init.c:568:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
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

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing
}

static void dummy_dnsrec_callback(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize channels
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    
    // Create initial channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    // 1. ares_dup
    int dup_result = ares_dup(&dup_channel, channel);
    if (dup_result != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }

    // 2. ares_set_local_ip4
    unsigned int local_ip4 = 0;
    if (Size >= 4) {
        memcpy(&local_ip4, Data, 4);
    }
    ares_set_local_ip4(dup_channel, local_ip4);

    // 3. ares_set_local_ip6
    unsigned char local_ip6[16];
    size_t ip6_size = Size < 16 ? Size : 16;
    memcpy(local_ip6, Data, ip6_size);
    ares_set_local_ip6(dup_channel, local_ip6);

    // 4. ares_set_local_dev
    const char *local_dev_name = NULL;
    if (Size > 0) {
        // Use first byte as device name character or empty string
        local_dev_name = "";
    }
    ares_set_local_dev(dup_channel, local_dev_name);

    // 5. ares_query_dnsrec
    const char *name = "example.com";
    ares_dns_class_t dnsclass = ARES_CLASS_IN;
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    unsigned short qid = 0;
    
    ares_query_dnsrec(dup_channel, name, dnsclass, type, dummy_dnsrec_callback, NULL, &qid);

    // 6. ares_query
    ares_query(dup_channel, name, dnsclass, type, dummy_callback, NULL);

    // Cleanup
    ares_destroy(dup_channel);
    ares_destroy(channel);
    
    return 0;
}