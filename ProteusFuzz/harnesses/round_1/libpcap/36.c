// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pcap.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *device = "any";
    int ret;

    /* Attempt to create a pcap handle using pcap_create */
    handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        /* If creation fails, try to open a savefile if data is provided */
        if (Size > 0) {
            write_dummy_file(Data, Size);
            handle = pcap_open_offline("./dummy_file", errbuf);
        }
        if (handle == NULL) {
            return 0;
        }
    } else {
        /* Try to activate the handle */
        ret = pcap_activate(handle);
        if (ret != 0) {
            /* Activation failed, try to open as offline if data exists */
            pcap_close(handle);
            handle = NULL;
            if (Size > 0) {
                write_dummy_file(Data, Size);
                handle = pcap_open_offline("./dummy_file", errbuf);
            }
            if (handle == NULL) {
                return 0;
            }
        }
    }

    if (handle == NULL) {
        return 0;
    }

    /*
     * Test pcap_snapshot
     */
    int snapshot = pcap_snapshot(handle);
    (void)snapshot;

    /*
     * Test pcap_datalink
     */
    int linktype = pcap_datalink(handle);
    (void)linktype;

    /*
     * Test pcap_datalink_ext
     */
    int linktype_ext = pcap_datalink_ext(handle);
    (void)linktype_ext;

    /*
     * Test pcap_major_version
     */
    int major_version = pcap_major_version(handle);
    (void)major_version;

    /*
     * Test pcap_minor_version
     */
    int minor_version = pcap_minor_version(handle);
    (void)minor_version;

    /*
     * Test pcap_inject
     * Only attempt if we have data and the handle supports injection.
     */
    if (Size > 0) {
        /* Try injecting the entire buffer if size is valid */
        if (Size <= (size_t)INT_MAX) {
            ret = pcap_inject(handle, Data, Size);
            (void)ret;
        }

        /* Try injecting a single byte */
        ret = pcap_inject(handle, Data, 1);
        (void)ret;

        /* Try injecting a subset of the data */
        if (Size > 10) {
            ret = pcap_inject(handle, Data, 10);
            (void)ret;
        }

        /* Try injecting from the end of the buffer */
        if (Size > 5) {
            ret = pcap_inject(handle, Data + Size - 5, 5);
            (void)ret;
        }
    }

    /* Cleanup */
    pcap_close(handle);
    return 0;
}