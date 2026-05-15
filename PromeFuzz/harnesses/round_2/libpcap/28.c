// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static char *device_name = NULL;

/* Initialize once at startup */
__attribute__((constructor)) static void init_device(void) {
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (pcap_findalldevs(&alldevs, errbuf) == 0 && alldevs != NULL) {
        device_name = strdup(alldevs->name);
        pcap_freealldevs(alldevs);
    }
    
    if (device_name == NULL) {
        device_name = strdup("any");
    }
}

static void fuzz_pcap_functions(pcap_t *handle, const uint8_t *data, size_t size) {
    if (size < 7) return;
    
    /* Use input bytes to determine function calls and parameters */
    uint8_t selector = data[0] % 8;
    int param1 = (int)data[1];
    int param2 = (int)data[2];
    int param3 = (int)data[3];
    int param4 = (int)data[4];
    int param5 = (int)data[5];
    int param6 = (int)data[6];
    
    /* Try different sequences of API calls */
    switch (selector) {
        case 0:
            pcap_set_snaplen(handle, param1);
            pcap_set_buffer_size(handle, param2);
            pcap_set_rfmon(handle, param3);
            pcap_set_promisc(handle, param4);
            pcap_activate(handle);
            break;
        case 1:
            pcap_set_buffer_size(handle, param1);
            pcap_set_snaplen(handle, param2);
            pcap_set_promisc(handle, param3);
            pcap_activate(handle);
            break;
        case 2:
            pcap_set_rfmon(handle, param1);
            pcap_set_buffer_size(handle, param2);
            pcap_set_snaplen(handle, param3);
            pcap_activate(handle);
            break;
        case 3:
            pcap_set_promisc(handle, param1);
            pcap_activate(handle);
            pcap_set_snaplen(handle, param2); /* Should fail after activation */
            break;
        case 4:
            pcap_activate(handle);
            pcap_set_buffer_size(handle, param1); /* Should fail after activation */
            pcap_set_rfmon(handle, param2); /* Should fail after activation */
            break;
        case 5:
            pcap_set_snaplen(handle, param1);
            pcap_activate(handle);
            pcap_set_promisc(handle, param2); /* Should fail after activation */
            break;
        case 6:
            /* Try with negative/edge case parameters */
            pcap_set_snaplen(handle, -1);
            pcap_set_buffer_size(handle, 0);
            pcap_set_rfmon(handle, 2); /* Non-boolean value */
            pcap_set_promisc(handle, -1);
            pcap_activate(handle);
            break;
        case 7:
            /* Try Linux-specific function if available */
            #ifdef __linux__
            pcap_set_protocol_linux(handle, param1);
            pcap_set_snaplen(handle, param2);
            pcap_activate(handle);
            #endif
            break;
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    
    /* Create a dummy file if needed for any file operations */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
        }
        close(fd);
    }
    
    /* Create pcap handle */
    if (device_name == NULL) {
        return 0;
    }
    
    handle = pcap_create(device_name, errbuf);
    if (handle == NULL) {
        return 0;
    }
    
    /* Fuzz the API functions */
    fuzz_pcap_functions(handle, Data, Size);
    
    /* Cleanup */
    if (pcap_activate(handle) == 0) {
        /* Handle was successfully activated, do some operations */
        pcap_t *temp_handle;
        struct pcap_pkthdr hdr;
        const u_char *packet;
        
        /* Try to read a packet (non-blocking) */
        pcap_setnonblock(handle, 1, errbuf);
        packet = pcap_next(handle, &hdr);
        
        /* Create another handle to test more states */
        temp_handle = pcap_create(device_name, errbuf);
        if (temp_handle != NULL) {
            pcap_set_snaplen(temp_handle, 128);
            pcap_set_buffer_size(temp_handle, 1024);
            pcap_close(temp_handle);
        }
    }
    
    pcap_close(handle);
    
    return 0;
}