// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
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
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    const char *device_name = NULL;
    char device_name_copy[256];

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // 1. pcap_findalldevs
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // If findalldevs fails, we might not have a device to test with.
        // We can still try pcap_create with NULL or a dummy name.
    }

    // Select a device if available
    if (alldevs != NULL) {
        // Make a local copy of the device name before freeing the list
        if (alldevs->name != NULL) {
            strncpy(device_name_copy, alldevs->name, sizeof(device_name_copy) - 1);
            device_name_copy[sizeof(device_name_copy) - 1] = '\0';
            device_name = device_name_copy;
        }
    }

    // 2. pcap_freealldevs
    // Free the list immediately to avoid use-after-free if we used the pointer directly
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. pcap_create
    // Use the copied device name or NULL
    handle = pcap_create(device_name, errbuf);
    if (handle == NULL) {
        // If creation fails, we can't proceed with other functions.
        return 0;
    }

    // 4. pcap_setnonblock (enable)
    pcap_setnonblock(handle, 1, errbuf);

    // 5. pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 6. pcap_activate
    // If activation fails, handle might be invalid for subsequent calls.
    if (pcap_activate(handle) != 0) {
        // Activation failed. Continue to test error paths.
    }

    // 7. pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 8. pcap_setnonblock (disable)
    pcap_setnonblock(handle, 0, errbuf);

    // 9. pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 10. pcap_setnonblock (enable again)
    pcap_setnonblock(handle, 1, errbuf);

    // 11. pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 12. pcap_loop
    // Use a small count to avoid hanging.
    pcap_loop(handle, 1, dummy_callback, NULL);

    // 13. pcap_geterr
    char *err = pcap_geterr(handle);
    (void)err;

    // 14. pcap_setnonblock (disable)
    pcap_setnonblock(handle, 0, errbuf);

    // 15. pcap_setnonblock (enable)
    pcap_setnonblock(handle, 1, errbuf);

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}