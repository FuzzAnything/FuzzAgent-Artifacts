// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
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
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    const char *device_name = NULL;
    char device_buf[256];
    int ret;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));
    memset(device_buf, 0, sizeof(device_buf));

    // Step 1: pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    
    // Step 2: pcap_freealldevs (if we got a list)
    // We must free the list immediately after extracting the name to avoid use-after-free.
    if (alldevs != NULL) {
        if (alldevs->name != NULL) {
            // Copy the name to a local buffer to keep it valid after freeing the list
            size_t len = strlen(alldevs->name);
            if (len < sizeof(device_buf) - 1) {
                strncpy(device_buf, alldevs->name, sizeof(device_buf) - 1);
                device_name = device_buf;
            }
        }
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // If no device was found or name was invalid, use a dummy file path
    if (device_name == NULL) {
        device_name = "./dummy_file";
    }

    // Step 3: pcap_create
    handle = pcap_create(device_name, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Step 4: pcap_setnonblock (1)
    ret = pcap_setnonblock(handle, 1, errbuf);
    (void)ret;

    // Step 5: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 6: pcap_activate
    ret = pcap_activate(handle);
    // Continue even if activation fails to test error handling paths

    // Step 7: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 8: pcap_setnonblock (0)
    ret = pcap_setnonblock(handle, 0, errbuf);
    (void)ret;

    // Step 9: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 10: pcap_setnonblock (1)
    ret = pcap_setnonblock(handle, 1, errbuf);
    (void)ret;

    // Step 11: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 12: pcap_loop
    int loop_count = 0;
    if (Size > 0) {
        loop_count = (int)(Data[0] % 100);
    }
    if (loop_count < 0) loop_count = 0;
    
    ret = pcap_loop(handle, loop_count, dummy_callback, NULL);
    (void)ret;

    // Step 13: pcap_geterr
    const char *err = pcap_geterr(handle);
    (void)err;

    // Step 14: pcap_setnonblock (0)
    ret = pcap_setnonblock(handle, 0, errbuf);
    (void)ret;

    // Step 15: pcap_setnonblock (1)
    ret = pcap_setnonblock(handle, 1, errbuf);
    (void)ret;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}