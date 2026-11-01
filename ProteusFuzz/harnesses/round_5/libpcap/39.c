// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *datalinks = NULL;
    int ret;
    pcap_direction_t direction;
    int dlt;
    const char *dlt_name;

    if (Size < 4) {
        return 0;
    }

    // Extract direction from input (0: IN, 1: OUT, 2: INOUT, others: invalid)
    direction = (pcap_direction_t)(Data[0] % 3);
    if (Data[0] >= 3) {
        direction = (pcap_direction_t)(Data[0] % 3 + 1); // Try to trigger invalid cases if possible
    }

    // Extract DLT index from input
    dlt = Data[1];

    // Extract a string for dlt_name_to_val (null-terminated)
    size_t str_len = Size > 4 ? Size - 4 : 0;
    if (str_len > 100) str_len = 100;
    char dlt_name_buf[101];
    memcpy(dlt_name_buf, Data + 2, str_len);
    dlt_name_buf[str_len] = '\0';
    dlt_name = dlt_name_buf;

    // Try to open a live capture on "any" or a dummy interface
    // Note: pcap_open_live may fail on some systems if no interface is available or permissions are missing
    handle = pcap_open_live("any", 65535, 1, 1000, errbuf);
    if (handle == NULL) {
        // Try opening a savefile as an alternative if live capture fails
        // Write some dummy data to a file to simulate a pcap file
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            // Write a minimal pcap header (magic, version, etc.)
            uint32_t magic = 0xa1b2c3d4;
            uint16_t ver_major = 2, ver_minor = 4;
            int32_t thiszone = 0, sigfigs = 0, snaplen = 65535, network = 1;
            fwrite(&magic, sizeof(magic), 1, f);
            fwrite(&ver_major, sizeof(ver_major), 1, f);
            fwrite(&ver_minor, sizeof(ver_minor), 1, f);
            fwrite(&thiszone, sizeof(thiszone), 1, f);
            fwrite(&sigfigs, sizeof(sigfigs), 1, f);
            fwrite(&snaplen, sizeof(snaplen), 1, f);
            fwrite(&network, sizeof(network), 1, f);
            fclose(f);

            handle = pcap_open_offline("./dummy_file", errbuf);
        }
    }

    if (handle == NULL) {
        // If we cannot open any handle, we can still test pcap_datalink_name_to_val
        pcap_datalink_name_to_val(dlt_name);
        return 0;
    }

    // Test pcap_setdirection
    ret = pcap_setdirection(handle, direction);
    (void)ret; // Suppress unused result warning

    // Test pcap_list_datalinks
    int count = pcap_list_datalinks(handle, &datalinks);
    if (count > 0 && datalinks != NULL) {
        // Optionally iterate or just free
        pcap_free_datalinks(datalinks);
        datalinks = NULL;
    }

    // Test pcap_set_datalink with various DLTs
    // Try the extracted dlt and some common ones
    int test_dlts[] = {dlt, 1, 101, 228, -1};
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_datalink(handle, test_dlts[i]);
        (void)ret;
    }

    // Test pcap_datalink
    int current_dlt = pcap_datalink(handle);
    (void)current_dlt;

    // Test pcap_datalink_name_to_val with the extracted string
    int val = pcap_datalink_name_to_val(dlt_name);
    (void)val;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}