// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test pcap_lib_version (no arguments, always succeeds)
    const char *version = pcap_lib_version();
    if (version != NULL) {
        (void)version;
    }

    // Prepare error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Extract parameters from fuzz input safely
    int snaplen = 0;
    int promisc = 0;
    int timeout = 0;
    int rfmon = 0;
    int flags = 0;
    int read_timeout = 0;
    const char *source = NULL;
    struct pcap_rmtauth auth = {0};

    if (Size >= 4) {
        snaplen = *(const int32_t *)(Data);
    }
    if (Size >= 8) {
        promisc = *(const int32_t *)(Data + 4) % 2;
    }
    if (Size >= 12) {
        timeout = *(const int32_t *)(Data + 8);
    }
    if (Size >= 16) {
        rfmon = *(const int32_t *)(Data + 12) % 2;
    }
    if (Size >= 20) {
        flags = *(const int32_t *)(Data + 16);
    }
    if (Size >= 24) {
        read_timeout = *(const int32_t *)(Data + 20);
    }

    // Limit values to reasonable ranges to avoid crashes
    if (snaplen < 0) snaplen = 0;
    if (snaplen > 65535) snaplen = 65535;
    if (timeout < 0) timeout = 0;
    if (timeout > 10000) timeout = 10000;
    if (read_timeout < 0) read_timeout = 0;
    if (read_timeout > 10000) read_timeout = 10000;

    // Create a dummy source string from input if possible
    char source_buf[256];
    size_t src_len = (Size > 255) ? 255 : Size;
    memcpy(source_buf, Data, src_len);
    source_buf[src_len] = '\0';
    source = source_buf;

    // Test pcap_open_live
    pcap_t *handle_live = pcap_open_live(source, snaplen, promisc, timeout, errbuf);
    if (handle_live != NULL) {
        // Test pcap_set_rfmon (only if not activated yet, but pcap_open_live activates immediately)
        // This should fail with PCAP_ERROR_ACTIVATED, which is expected
        (void)pcap_set_rfmon(handle_live, rfmon);

        // Test pcap_set_promisc (should also fail as it's activated)
        (void)pcap_set_promisc(handle_live, rfmon);

        // Test pcap_major_version (should work as handle is activated)
        (void)pcap_major_version(handle_live);

        cleanup_pcap(handle_live);
    }

    // Test pcap_open
    // Prepare auth structure if we have enough data
    if (Size >= 28) {
        auth.type = *(const int32_t *)(Data + 24);
        // We don't set username/password from fuzz data to avoid complexity, keep as NULL
        auth.username = NULL;
        auth.password = NULL;
    } else {
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
    }

    pcap_t *handle_open = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (handle_open != NULL) {
        // Try to set rfmon before activation (pcap_open might activate immediately depending on source)
        // If it's a file, it might not be activated in the same way
        int ret = pcap_set_rfmon(handle_open, rfmon);
        (void)ret;

        // Try to set promisc
        ret = pcap_set_promisc(handle_open, rfmon);
        (void)ret;

        // Try to get major version
        int major = pcap_major_version(handle_open);
        (void)major;

        cleanup_pcap(handle_open);
    }

    // Test with NULL source for pcap_open_live (defaults to "any")
    pcap_t *handle_null = pcap_open_live(NULL, snaplen, promisc, timeout, errbuf);
    if (handle_null != NULL) {
        (void)pcap_set_rfmon(handle_null, rfmon);
        (void)pcap_set_promisc(handle_null, rfmon);
        (void)pcap_major_version(handle_null);
        cleanup_pcap(handle_null);
    }

    // Test with NULL source for pcap_open
    pcap_t *handle_null_open = pcap_open(NULL, snaplen, flags, read_timeout, &auth, errbuf);
    if (handle_null_open != NULL) {
        (void)pcap_set_rfmon(handle_null_open, rfmon);
        (void)pcap_set_promisc(handle_null_open, rfmon);
        (void)pcap_major_version(handle_null_open);
        cleanup_pcap(handle_null_open);
    }

    return 0;
}