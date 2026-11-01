// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
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
// pcap_loop at pcap.c:2899:1 in pcap.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    pcap_if_t *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    int nonblock_val;
    const char *device_name = NULL;
    char *device_name_copy = NULL;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Step 1: pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == -1) {
        // On failure, alldevs is NULL
        goto cleanup;
    }

    // Extract device name before freeing the list
    if (alldevs != NULL) {
        dev = alldevs;
        if (dev != NULL && dev->name != NULL) {
            // strdup to keep the name valid after freeing the list
            device_name_copy = strdup(dev->name);
            if (device_name_copy != NULL) {
                device_name = device_name_copy;
            }
        }
    }

    // Step 2: pcap_freealldevs
    // Free the list immediately as per the requested sequence
    pcap_freealldevs(alldevs);
    alldevs = NULL;

    // Step 3: pcap_create
    // Use the saved device name copy
    handle = pcap_create(device_name, errbuf);
    if (handle == NULL) {
        goto cleanup;
    }

    // Step 4: pcap_setnonblock
    int mode = (Size > 0) ? (Data[0] % 2) : 1;
    ret = pcap_setnonblock(handle, mode, errbuf);
    (void)ret;

    // Step 5: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 6: pcap_activate
    ret = pcap_activate(handle);
    if (ret != 0) {
        // Handle might be in error state, but we continue to test error paths
    }

    // Step 7: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 8: pcap_setnonblock
    mode = (Size > 1) ? (Data[1] % 2) : 0;
    ret = pcap_setnonblock(handle, mode, errbuf);
    (void)ret;

    // Step 9: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 10: pcap_setnonblock
    mode = (Size > 2) ? (Data[2] % 2) : 1;
    ret = pcap_setnonblock(handle, mode, errbuf);
    (void)ret;

    // Step 11: pcap_getnonblock
    ret = pcap_getnonblock(handle, errbuf);
    (void)ret;

    // Step 12: pcap_loop
    int count = (Size > 3) ? (Data[3] % 100) : 0;
    ret = pcap_loop(handle, count, dummy_callback, NULL);
    (void)ret;

    // Step 13: pcap_geterr
    const char *err = pcap_geterr(handle);
    (void)err;

    // Step 14: pcap_setnonblock
    mode = (Size > 4) ? (Data[4] % 2) : 0;
    ret = pcap_setnonblock(handle, mode, errbuf);
    (void)ret;

    // Step 15: pcap_setnonblock
    mode = (Size > 5) ? (Data[5] % 2) : 1;
    ret = pcap_setnonblock(handle, mode, errbuf);
    (void)ret;

cleanup:
    // Cleanup: Close handle if it exists
    if (handle != NULL) {
        pcap_close(handle);
    }
    
    // Free the duplicated device name
    if (device_name_copy != NULL) {
        free(device_name_copy);
    }
    
    return 0;
}