// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    const char *dev_name = "any";
    pcap_t *pcap_handle = NULL;
    int status;
    
    /* Initialize errbuf */
    memset(errbuf, 0, sizeof(errbuf));
    
    /* 1. pcap_findalldevs */
    status = pcap_findalldevs(&alldevs, errbuf);
    if (status == -1) {
        /* Error finding devices - continue anyway with default device */
    } else if (alldevs != NULL && alldevs->name != NULL) {
        /* Use first found device if available */
        dev_name = "any"; /* Always use "any" instead of alldevs->name to avoid use-after-free */
    }
    
    /* 2. pcap_freealldevs */
    pcap_freealldevs(alldevs);
    alldevs = NULL;
    
    /* 3. pcap_create */
    memset(errbuf, 0, sizeof(errbuf));
    pcap_handle = pcap_create(dev_name, errbuf);
    if (pcap_handle == NULL) {
        /* Cannot create handle - return early */
        return 0;
    }
    
    /* Use fuzz data to generate parameters */
    int snaplen = 0;
    int promisc = 0;
    int rfmon = 0;
    
    /* Extract parameters from fuzz data if available */
    if (Size >= 3) {
        snaplen = Data[0] | (Data[1] << 8);
        promisc = Data[2] & 1;
        rfmon = (Data[2] >> 1) & 1;
    } else if (Size >= 1) {
        snaplen = Data[0];
        promisc = 0;
        rfmon = 0;
    }
    
    /* 4. pcap_set_snaplen */
    pcap_set_snaplen(pcap_handle, snaplen);
    
    /* 5. pcap_statustostr */
    /* Use various status codes from fuzz data */
    int status_code1 = 0;
    if (Size >= 4) {
        status_code1 = Data[3];
    }
    const char *status_str1 = pcap_statustostr(status_code1);
    (void)status_str1; /* Avoid unused variable warning */
    
    /* 6. pcap_set_promisc */
    pcap_set_promisc(pcap_handle, promisc);
    
    /* 7. pcap_statustostr */
    /* Use another status code from fuzz data */
    int status_code2 = 0;
    if (Size >= 5) {
        status_code2 = Data[4];
    }
    const char *status_str2 = pcap_statustostr(status_code2);
    (void)status_str2; /* Avoid unused variable warning */
    
    /* 8. pcap_set_rfmon */
    pcap_set_rfmon(pcap_handle, rfmon);
    
    /* Cleanup */
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}