// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
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

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    
    // Use a static buffer for device name to avoid stack-use-after-scope
    // if the library stores a pointer to it.
    static char device_name[256];
    const char *device = "lo";

    // If input starts with a printable string that looks like a device name, use it
    if (Size >= 2 && Data[Size-1] == 0) {
        size_t copy_len = (Size < 256) ? Size : 256;
        memcpy(device_name, Data, copy_len);
        device_name[copy_len - 1] = '\0';
        device = device_name;
    }

    // Attempt to open live capture
    handle = pcap_open_live(device, 65535, 1, 1000, errbuf);
    
    if (handle == NULL) {
        // If live capture fails, try to create a dummy savefile
        const char *dummy_file = "./dummy_file";
        FILE *f = fopen(dummy_file, "wb");
        if (f) {
            // Write a minimal pcap header
            uint32_t magic = 0xa1b2c3d4;
            fwrite(&magic, sizeof(magic), 1, f);
            uint16_t ver_major = 2;
            uint16_t ver_minor = 4;
            fwrite(&ver_major, sizeof(ver_major), 1, f);
            fwrite(&ver_minor, sizeof(ver_minor), 1, f);
            int32_t tz = 0;
            uint32_t sigfigs = 0;
            uint32_t snaplen = 65535;
            uint32_t net = 1; // Ethernet
            fwrite(&tz, sizeof(tz), 1, f);
            fwrite(&sigfigs, sizeof(sigfigs), 1, f);
            fwrite(&snaplen, sizeof(snaplen), 1, f);
            fwrite(&net, sizeof(net), 1, f);
            fclose(f);

            handle = pcap_open_offline(dummy_file, errbuf);
            
            // Cleanup the dummy file
            remove(dummy_file);
        }
    }

    if (handle == NULL) {
        return 0;
    }

    // Invoke target functions
    int major = pcap_major_version(handle);
    int swapped = pcap_is_swapped(handle);
    int snap = pcap_snapshot(handle);
    int minor = pcap_minor_version(handle);
    int buf = pcap_bufsize(handle);
    int dlt_ext = pcap_datalink_ext(handle);

    // Use results to prevent unused variable warnings
    volatile int dummy = major + swapped + snap + minor + buf + dlt_ext;
    (void)dummy;

    cleanup_pcap(handle);

    return 0;
}