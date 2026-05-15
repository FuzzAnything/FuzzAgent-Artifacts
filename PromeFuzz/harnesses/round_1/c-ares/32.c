// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_channel_t *channel = NULL;
static int channel_initialized = 0;

static void initialize_channel_once(void) {
    if (!channel_initialized) {
        struct ares_options options;
        int optmask = 0;
        
        options.flags = ARES_FLAG_STAYOPEN;
        optmask |= ARES_OPT_FLAGS;
        
        if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
            channel = NULL;
        }
        channel_initialized = 1;
    }
}

static void cleanup(void) {
    if (channel) {
        ares_destroy(channel);
        channel = NULL;
    }
}

__attribute__((destructor)) static void final_cleanup(void) {
    cleanup();
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    initialize_channel_once();
    if (!channel) return 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    status = ares_dns_parse(Data, Size, 0, &dnsrec);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t s = 0; s < sizeof(sections)/sizeof(sections[0]); s++) {
        size_t cnt = ares_dns_record_rr_cnt(dnsrec, sections[s]);
        
        for (size_t idx = 0; idx < cnt; idx++) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sections[s], idx);
            if (rr != NULL) {
                ares_dns_rec_type_t type = ares_dns_rr_get_type(rr);
                (void)type;
            }
        }
    }
    
    ares_dns_record_destroy(dnsrec);
    return 0;
}