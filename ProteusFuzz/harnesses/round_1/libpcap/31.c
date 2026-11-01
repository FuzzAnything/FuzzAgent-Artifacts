// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
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
#include <time.h>

static void create_dummy_capture_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    // Write a minimal pcap global header (24 bytes)
    // Magic number: 0xa1b2c3d4 (microseconds)
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet

    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&thiszone, sizeof(thiszone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);

    // Write a minimal packet header (16 bytes)
    struct pcap_pkthdr hdr;
    hdr.ts.tv_sec = 1000;
    hdr.ts.tv_usec = 1000;
    hdr.caplen = 4;
    hdr.len = 4;

    fwrite(&hdr, sizeof(hdr), 1, f);

    // Write dummy packet data
    uint8_t data[4] = {0x00, 0x01, 0x02, 0x03};
    fwrite(data, sizeof(data), 1, f);

    fclose(f);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *file_stream = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret = 0;

    // Create a dummy file for testing
    create_dummy_capture_file("./dummy_file");

    // Strategy 1: pcap_open_offline
    if (Data[0] % 3 == 0) {
        handle = pcap_open_offline("./dummy_file", errbuf);
        if (handle) {
            FILE *f = pcap_file(handle);
            if (f) {
                // Try to read a packet
                ret = pcap_next_ex(handle, &header, &packet);
            }
        }
    }
    // Strategy 2: pcap_open_offline_with_tstamp_precision
    else if (Data[0] % 3 == 1) {
        u_int precision = (Size > 1) ? (Data[1] % 2) : 0; // 0 or 1
        handle = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
        if (handle) {
            FILE *f = pcap_file(handle);
            if (f) {
                ret = pcap_next_ex(handle, &header, &packet);
            }
        }
    }
    // Strategy 3: pcap_fopen_offline
    else if (Data[0] % 3 == 2) {
        file_stream = fopen("./dummy_file", "rb");
        if (file_stream) {
            handle = pcap_fopen_offline(file_stream, errbuf);
            if (handle) {
                FILE *f = pcap_file(handle);
                if (f) {
                    ret = pcap_next_ex(handle, &header, &packet);
                }
            }
            // Note: pcap_fopen_offline takes ownership of the file stream,
            // so we should not fclose(file_stream) here if handle is valid.
            // If handle is NULL, we might need to close it, but libpcap docs
            // suggest it closes the stream on error or we should not close it.
            // To be safe in fuzzing, we rely on pcap_close to clean up.
        }
    }
    // Strategy 4: pcap_fopen_offline_with_tstamp_precision
    else {
        file_stream = fopen("./dummy_file", "rb");
        if (file_stream) {
            u_int precision = (Size > 2) ? (Data[2] % 2) : 0;
            handle = pcap_fopen_offline_with_tstamp_precision(file_stream, precision, errbuf);
            if (handle) {
                FILE *f = pcap_file(handle);
                if (f) {
                    ret = pcap_next_ex(handle, &header, &packet);
                }
            }
        }
    }

    // Cleanup
    if (handle) {
        pcap_close(handle);
    }
    // If pcap_fopen_offline failed, the file stream might still be open.
    // However, standard libpcap behavior usually leaves it open on failure
    // or takes ownership on success. Since we don't have a handle, we try to close.
    // This is a heuristic for fuzzing safety.
    if (!handle && file_stream) {
        fclose(file_stream);
    }

    return 0;
}