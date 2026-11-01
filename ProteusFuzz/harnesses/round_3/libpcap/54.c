// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void cleanup_pcap(pcap_t *handle, char *errbuf) {
    if (handle != NULL) {
        pcap_close(handle);
    }
    if (errbuf != NULL) {
        free(errbuf);
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = "any";
    pcap_t *handle = NULL;

    // Extract parameters from fuzz input
    int param1 = (int)(Data[0] | (Data[1] << 8));
    int param2 = (int)(Data[2] | (Data[3] << 8));
    int param3 = (int)(Data[0] ^ Data[1]);
    int param4 = (int)(Data[2] ^ Data[3]);

    // Create a pcap handle
    handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Fuzz pcap_can_set_rfmon
    // No parameters needed, just check return value
    int ret1 = pcap_can_set_rfmon(handle);
    (void)ret1;

    // Fuzz pcap_set_snaplen
    // Use param1 as snaplen, ensure it's positive for valid test
    int snaplen = param1 > 0 ? param1 : 65535;
    if (snaplen < 0) snaplen = 65535;
    int ret2 = pcap_set_snaplen(handle, snaplen);
    (void)ret2;

    // Fuzz pcap_set_timeout
    // Use param2 as timeout in ms
    int timeout = param2;
    if (timeout < 0) timeout = 0;
    int ret3 = pcap_set_timeout(handle, timeout);
    (void)ret3;

    // Fuzz pcap_set_protocol_linux
    // Use param3 as protocol (AF_PACKET variants)
    // Only meaningful on Linux, but safe to call on other platforms (returns error)
    int protocol = param3;
    int ret4 = pcap_set_protocol_linux(handle, protocol);
    (void)ret4;

    // Fuzz pcap_set_buffer_size
    // Use param4 as buffer size
    int bufsize = param4;
    if (bufsize <= 0) bufsize = 1024;
    int ret5 = pcap_set_buffer_size(handle, bufsize);
    (void)ret5;

    // Fuzz pcap_set_immediate_mode
    // Use Data[0] % 2 to get 0 or 1
    int immediate = (Data[0] % 2);
    int ret6 = pcap_set_immediate_mode(handle, immediate);
    (void)ret6;

    // Cleanup
    cleanup_pcap(handle, NULL);

    return 0;
}