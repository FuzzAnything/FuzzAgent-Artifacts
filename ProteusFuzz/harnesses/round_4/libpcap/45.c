// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write at least some data to ensure the file is not empty
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal dummy header if input is empty to avoid immediate EOF
            // This helps test the file opening logic even with empty fuzz input
            const char dummy[] = "PCAP";
            fwrite(dummy, 1, 4, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *fp = NULL;
    struct pcap_pkthdr *header;
    const u_char *packet;
    int ret;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Strategy 1: Test pcap_open_offline with the fuzz data as a filename (if valid)
    // We write the fuzz data to a file first to ensure we have a valid file to open
    write_dummy_file(Data, Size);

    // Test pcap_open_offline
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        // Test pcap_file
        FILE *stream = pcap_file(handle);
        (void)stream; // Suppress unused warning, just checking it doesn't crash

        // Test pcap_next_ex
        ret = pcap_next_ex(handle, &header, &packet);
        // Ret values: 1 (success), -1 (error), -2 (EOF)
        // We don't need to do anything with the packet data, just ensure no crash

        pcap_close(handle);
        handle = NULL;
    }

    // Strategy 2: Test pcap_open_offline_with_tstamp_precision
    // Try different precisions based on input data to explore states
    u_int precision = (Size > 0) ? (Data[0] % 3) : 0; // 0, 1, or 2
    handle = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
    if (handle) {
        FILE *stream = pcap_file(handle);
        (void)stream;

        ret = pcap_next_ex(handle, &header, &packet);
        (void)ret;

        pcap_close(handle);
        handle = NULL;
    }

    // Strategy 3: Test pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline(fp, errbuf);
        if (handle) {
            FILE *stream = pcap_file(handle);
            (void)stream;

            ret = pcap_next_ex(handle, &header, &packet);
            (void)ret;

            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // Strategy 4: Test pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        u_int precision2 = (Size > 1) ? (Data[1] % 3) : 1;
        handle = pcap_fopen_offline_with_tstamp_precision(fp, precision2, errbuf);
        if (handle) {
            FILE *stream = pcap_file(handle);
            (void)stream;

            ret = pcap_next_ex(handle, &header, &packet);
            (void)ret;

            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // Cleanup: Remove the dummy file if it exists
    remove("./dummy_file");

    return 0;
}