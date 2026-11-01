// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    if (size == 0) return;
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_rmtauth auth = {0};
    int ret;
    int major_ver;
    const char *ver_str;

    // Always safe to call
    ver_str = pcap_lib_version();
    (void)ver_str;

    if (Size < 10) {
        return 0;
    }

    // Prepare dummy file for file-based capture
    write_dummy_file(Data, Size);

    // Fuzz pcap_open
    // Source: try "any", "lo", "./dummy_file", or data from input
    const char *source = NULL;
    if (Data[0] == 'f') {
        source = "./dummy_file";
    } else if (Data[0] == 'l') {
        source = "lo";
    } else {
        source = "any";
    }

    int snaplen = (Data[1] % 65536) + 1;
    int flags = (Data[2] % 10);
    int timeout = (Data[3] % 10000);

    // Setup auth if needed (simplified)
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    handle = pcap_open(source, snaplen, flags, timeout, &auth, errbuf);

    if (handle != NULL) {
        // Fuzz pcap_set_rfmon (must be before activate)
        ret = pcap_set_rfmon(handle, Data[4] % 2);
        (void)ret;

        // Fuzz pcap_set_promisc (must be before activate)
        ret = pcap_set_promisc(handle, Data[5] % 2);
        (void)ret;

        // Activate the handle to test pcap_major_version
        ret = pcap_activate(handle);
        if (ret == 0) {
            // Now handle is active, pcap_major_version should work
            major_ver = pcap_major_version(handle);
            (void)major_ver;
        } else {
            // If activation failed, pcap_major_version should return PCAP_ERROR_NOT_ACTIVATED
            major_ver = pcap_major_version(handle);
            (void)major_ver;
        }

        cleanup_pcap(handle);
    }

    // Fuzz pcap_open_live
    // Reuse source logic
    const char *dev = (Data[0] == 'l') ? "lo" : "any";
    int promisc = Data[6] % 2;
    int live_timeout = (Data[7] % 10000);

    handle = pcap_open_live(dev, snaplen, promisc, live_timeout, errbuf);

    if (handle != NULL) {
        // Try to set rfmon/promisc after opening (should fail as it's likely active or close to it)
        // pcap_open_live usually activates immediately or sets up for activation.
        // Attempting to set these might return PCAP_ERROR_ACTIVATED.
        ret = pcap_set_rfmon(handle, Data[8] % 2);
        (void)ret;
        ret = pcap_set_promisc(handle, Data[9] % 2);
        (void)ret;

        // Try to get major version
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        cleanup_pcap(handle);
    }

    return 0;
}