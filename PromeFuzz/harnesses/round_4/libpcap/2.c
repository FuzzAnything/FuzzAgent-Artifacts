// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_getnonblock at pcap.c:3555:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    /* Dummy packet handler that does nothing */
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int nonblock_ret;
    int loop_ret;
    const char *err_msg;
    
    /* Initialize errbuf */
    memset(errbuf, 0, sizeof(errbuf));
    
    /* 1. pcap_findalldevs */
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        /* Error finding devices, continue with other operations */
    }
    
    /* 2. pcap_freealldevs */
    pcap_freealldevs(alldevs);
    
    /* Create device name from fuzzer input if available, otherwise use NULL */
    char *device_name = NULL;
    if (Size > 0) {
        /* Use first min(Size, PCAP_ERRBUF_SIZE-1) bytes as device name */
        size_t name_len = Size < PCAP_ERRBUF_SIZE ? Size : PCAP_ERRBUF_SIZE - 1;
        device_name = (char *)malloc(name_len + 1);
        if (device_name) {
            memcpy(device_name, Data, name_len);
            device_name[name_len] = '\0';
        }
    }
    
    /* 3. pcap_create */
    memset(errbuf, 0, sizeof(errbuf));
    pcap_handle = pcap_create(device_name, errbuf);
    
    if (device_name) {
        free(device_name);
    }
    
    if (!pcap_handle) {
        /* pcap_create failed, nothing more to do */
        return 0;
    }
    
    /* 4. pcap_setnonblock - try to set non-blocking mode before activation */
    memset(errbuf, 0, sizeof(errbuf));
    int nonblock_mode = (Size > 0) ? (Data[0] & 1) : 0;
    pcap_setnonblock(pcap_handle, nonblock_mode, errbuf);
    
    /* 5. pcap_getnonblock - get non-blocking mode before activation */
    memset(errbuf, 0, sizeof(errbuf));
    pcap_getnonblock(pcap_handle, errbuf);
    
    /* 6. pcap_activate */
    int activate_ret = pcap_activate(pcap_handle);
    
    /* 7. pcap_getnonblock - get non-blocking mode after activation */
    memset(errbuf, 0, sizeof(errbuf));
    nonblock_ret = pcap_getnonblock(pcap_handle, errbuf);
    
    /* 8. pcap_setnonblock - set non-blocking mode after activation */
    memset(errbuf, 0, sizeof(errbuf));
    if (Size > 1) {
        nonblock_mode = (Data[1] & 1);
    }
    pcap_setnonblock(pcap_handle, nonblock_mode, errbuf);
    
    /* 9. pcap_getnonblock */
    memset(errbuf, 0, sizeof(errbuf));
    nonblock_ret = pcap_getnonblock(pcap_handle, errbuf);
    
    /* 10. pcap_setnonblock */
    memset(errbuf, 0, sizeof(errbuf));
    if (Size > 2) {
        nonblock_mode = (Data[2] & 1);
    }
    pcap_setnonblock(pcap_handle, nonblock_mode, errbuf);
    
    /* 11. pcap_getnonblock */
    memset(errbuf, 0, sizeof(errbuf));
    nonblock_ret = pcap_getnonblock(pcap_handle, errbuf);
    
    /* 12. pcap_loop - try to process some packets */
    int loop_count = 0;
    if (Size > 3) {
        loop_count = Data[3] & 0x0F; /* Limit to small count for fuzzing */
    }
    loop_ret = pcap_loop(pcap_handle, loop_count, packet_handler, NULL);
    
    /* 13. pcap_geterr - get any error message */
    err_msg = pcap_geterr(pcap_handle);
    (void)err_msg; /* Use variable to avoid unused warning */
    
    /* 14. pcap_setnonblock */
    memset(errbuf, 0, sizeof(errbuf));
    if (Size > 4) {
        nonblock_mode = (Data[4] & 1);
    }
    pcap_setnonblock(pcap_handle, nonblock_mode, errbuf);
    
    /* 15. pcap_setnonblock */
    memset(errbuf, 0, sizeof(errbuf));
    if (Size > 5) {
        nonblock_mode = (Data[5] & 1);
    }
    pcap_setnonblock(pcap_handle, nonblock_mode, errbuf);
    
    /* Cleanup */
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}