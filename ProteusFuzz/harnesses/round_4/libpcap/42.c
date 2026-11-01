// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *version;
    int ret;
    int major_ver;
    int snaplen = 65535;
    int flags = 0;
    int timeout = 1000;
    int promisc = 0;
    int rfmon = 0;

    // 1. Test pcap_lib_version (always succeeds, no args)
    version = pcap_lib_version();
    if (version != NULL) {
        if (strlen(version) > 0) {
            (void)version[0];
        }
    }

    // Prepare parameters based on input data with bounds checking
    if (Size > 0) {
        snaplen = (Data[0] % 65536) + 1;
    }
    if (Size > 1) {
        flags = Data[1] % 4;
    }
    if (Size > 2) {
        timeout = (Data[2] % 1000) + 1;
    }
    if (Size > 3) {
        promisc = Data[3] % 2;
    }
    if (Size > 4) {
        rfmon = Data[4] % 2;
    }

    // Create a temporary file for testing pcap_open with file source
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        size_t write_size = (Size > 1000) ? 1000 : Size;
        if (write_size > 0) {
            fwrite(Data, 1, write_size, f);
        }
        fclose(f);
    }

    const char *source = "./dummy_file";

    // 2. Test pcap_open
    handle = pcap_open(source, snaplen, flags, timeout, NULL, errbuf);
    if (handle != NULL) {
        // Try pcap_set_promisc on an already activated handle
        ret = pcap_set_promisc(handle, promisc);
        (void)ret;

        // Try pcap_set_rfmon on an already activated handle
        ret = pcap_set_rfmon(handle, rfmon);
        (void)ret;

        // Try pcap_major_version on an activated handle
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // 3. Test pcap_open_live
    handle = pcap_open_live(source, snaplen, promisc, timeout, errbuf);
    if (handle != NULL) {
        // Try pcap_set_promisc on an already activated handle
        ret = pcap_set_promisc(handle, !promisc);
        (void)ret;

        // Try pcap_set_rfmon on an already activated handle
        ret = pcap_set_rfmon(handle, !rfmon);
        (void)ret;

        // Try pcap_major_version
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}