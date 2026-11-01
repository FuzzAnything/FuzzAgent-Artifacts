// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pcap.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static pcap_t *create_pcap_from_input(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *devices[] = {"lo", "eth0", "any", NULL};
    
    // Attempt to open a live capture on a standard interface
    for (int i = 0; devices[i] != NULL; i++) {
        handle = pcap_open_live(devices[i], 65535, 0, 1000, errbuf);
        if (handle != NULL) {
            break;
        }
    }

    // If live capture failed, try to open the dummy file as a savefile
    if (handle == NULL && Size > 0) {
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    return handle;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pcap_t *handle = create_pcap_from_input(Data, Size);
    
    if (handle == NULL) {
        return 0;
    }

    // Test 1: pcap_snapshot
    int snap = pcap_snapshot(handle);
    (void)snap;

    // Test 2: pcap_minor_version
    int minor = pcap_minor_version(handle);
    (void)minor;

    // Test 3: pcap_inject
    // We need valid data to inject. Use the fuzz input if size is reasonable.
    // pcap_inject requires size > 0 and <= INT_MAX.
    if (Size > 0 && Size <= (size_t)INT_MAX) {
        pcap_inject(handle, Data, Size);
    } else {
        // Inject a small dummy packet if input is empty or too large to test the function
        char dummy_pkt[14] = {0};
        pcap_inject(handle, dummy_pkt, sizeof(dummy_pkt));
    }

    // Test 4: pcap_major_version
    int major = pcap_major_version(handle);
    (void)major;

    // Test 5: pcap_datalink_ext
    int dlt_ext = pcap_datalink_ext(handle);
    (void)dlt_ext;

    // Test 6: pcap_bufsize
    int buf_size = pcap_bufsize(handle);
    (void)buf_size;

    // Cleanup
    pcap_close(handle);

    return 0;
}