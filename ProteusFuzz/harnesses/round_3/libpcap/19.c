// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pcap.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    const char *version;
    int major_ver;
    
    // Always safe to call
    version = pcap_lib_version();
    (void)version;

    // Ensure dummy file exists for pcap_open tests
    write_dummy_file(Data, Size);

    // Test pcap_open with various sources
    // Source: "any", file, or NULL (defaults to any)
    const char *sources[] = {"any", "./dummy_file", NULL};
    int snaplen = 65535;
    int flags = 0;
    int timeout = 1000;
    struct pcap_rmtauth auth = {0};
    
    // Fuzz source selection
    int src_idx = 0;
    if (Size > 0) {
        src_idx = Data[0] % 3;
    }
    const char *source = sources[src_idx];

    // Fuzz snaplen
    if (Size > 1) {
        snaplen = (int)Data[1];
        if (snaplen < 0) snaplen = 0;
    }

    // Fuzz flags
    if (Size > 2) {
        flags = (int)Data[2];
    }

    // Fuzz timeout
    if (Size > 3) {
        timeout = (int)Data[3];
        if (timeout < 0) timeout = 0;
    }

    // Test pcap_open
    handle = pcap_open(source, snaplen, flags, timeout, &auth, errbuf);

    if (handle != NULL) {
        // Test pcap_set_promisc (must be before activate)
        if (Size > 4) {
            ret = pcap_set_promisc(handle, Data[4] % 2);
            (void)ret;
        }

        // Test pcap_set_rfmon (must be before activate)
        if (Size > 5) {
            ret = pcap_set_rfmon(handle, Data[5] % 2);
            (void)ret;
        }

        // Try to activate the handle to enable pcap_major_version
        // Note: pcap_activate is not in the target list, but pcap_major_version requires it.
        // We cannot call pcap_activate directly as it's not a target, but we can try to 
        // invoke pcap_major_version on an inactive handle to test the error path.
        
        // Test pcap_major_version on INACTIVE handle (should return PCAP_ERROR_NOT_ACTIVATED)
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        // Attempt to set options after activation would fail, but we haven't activated yet.
        // If we could activate, we would test pcap_major_version again.
        // Since we can't call pcap_activate, we rely on the inactive state test.
        
        // Test setting options again after the first call (idempotent check)
        if (Size > 6) {
            ret = pcap_set_promisc(handle, Data[6] % 2);
            (void)ret;
        }
        if (Size > 7) {
            ret = pcap_set_rfmon(handle, Data[7] % 2);
            (void)ret;
        }

        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_open_live
    // Fuzz device name
    const char *devices[] = {"any", "lo", NULL};
    int dev_idx = 0;
    if (Size > 8) {
        dev_idx = Data[8] % 3;
    }
    const char *device = devices[dev_idx];

    int promisc = 0;
    if (Size > 9) {
        promisc = Data[9] % 2;
    }

    handle = pcap_open_live(device, snaplen, promisc, timeout, errbuf);

    if (handle != NULL) {
        // Test pcap_major_version on INACTIVE handle (pcap_open_live does not auto-activate in newer versions usually, or we test before activate)
        // Actually, pcap_open_live usually activates immediately in older versions, but in newer libpcap it might not.
        // Let's try to set options which should fail if activated.
        
        if (Size > 10) {
            ret = pcap_set_promisc(handle, Data[10] % 2);
            (void)ret;
        }
        if (Size > 11) {
            ret = pcap_set_rfmon(handle, Data[11] % 2);
            (void)ret;
        }

        // Test pcap_major_version
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        pcap_close(handle);
        handle = NULL;
    }

    return 0;
}