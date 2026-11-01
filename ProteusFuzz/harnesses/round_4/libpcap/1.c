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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    const char *device_name = NULL;
    char *device_name_copy = NULL;

    // 1. Prepare environment: Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        if (alldevs != NULL) {
            // We need to copy the device name because pcap_freealldevs will free the original list
            // including the name strings.
            device_name = alldevs->name;
            if (device_name != NULL) {
                device_name_copy = strdup(device_name);
            }
        }
    }

    // 2. Cleanup devices list immediately after extracting the name
    pcap_freealldevs(alldevs);

    // 3. Create handle using the copied name
    // If device_name_copy is NULL, we pass NULL to pcap_create
    handle = pcap_create(device_name_copy, errbuf);

    // Free the copy since pcap_create makes its own internal copy if needed
    free(device_name_copy);

    if (handle == NULL) {
        return 0;
    }

    // 4. Invoke pcap_setnonblock (First call)
    int nonblock_mode = (Size > 0) ? (Data[0] % 2) : 1;
    pcap_setnonblock(handle, nonblock_mode, errbuf);

    // 5. Invoke pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 6. Invoke pcap_activate
    pcap_activate(handle);

    // 7. Invoke pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 8. Invoke pcap_setnonblock
    nonblock_mode = (Size > 1) ? (Data[1] % 2) : 0;
    pcap_setnonblock(handle, nonblock_mode, errbuf);

    // 9. Invoke pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 10. Invoke pcap_setnonblock
    nonblock_mode = (Size > 2) ? (Data[2] % 2) : 1;
    pcap_setnonblock(handle, nonblock_mode, errbuf);

    // 11. Invoke pcap_getnonblock
    pcap_getnonblock(handle, errbuf);

    // 12. Invoke pcap_loop
    int loop_count = 0;
    if (Size > 3) {
        loop_count = Data[3];
        if (loop_count > 10) loop_count = 10;
    }
    pcap_loop(handle, loop_count, dummy_callback, NULL);

    // 13. Invoke pcap_geterr
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 14. Invoke pcap_setnonblock
    nonblock_mode = (Size > 4) ? (Data[4] % 2) : 1;
    pcap_setnonblock(handle, nonblock_mode, errbuf);

    // 15. Invoke pcap_setnonblock
    nonblock_mode = (Size > 5) ? (Data[5] % 2) : 0;
    pcap_setnonblock(handle, nonblock_mode, errbuf);

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}