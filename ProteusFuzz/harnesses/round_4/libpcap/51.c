// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <time.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal valid pcap header if data is empty or too short
        if (size < 24) {
            // Magic number, version, etc.
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
        } else {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *f = NULL;
    FILE *stream = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;

    // Create a dummy file from fuzz input
    write_dummy_file("./dummy_file", Data, Size);

    // Test 1: pcap_open_offline
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        // Test pcap_file
        stream = pcap_file(handle);
        if (stream) {
            // Try to read a bit to ensure stream is valid
            fseek(stream, 0, SEEK_END);
            long end = ftell(stream);
            fseek(stream, 0, SEEK_SET);
            // Try reading some bytes if file is not empty
            if (end > 0) {
                char buf[10];
                fread(buf, 1, sizeof(buf), stream);
            }
        }

        // Test pcap_next_ex
        ret = pcap_next_ex(handle, &header, &packet);
        if (ret == 1) {
            // Packet read successfully, do nothing or inspect header
            (void)header;
            (void)packet;
        }

        pcap_close(handle);
        handle = NULL;
    }

    // Test 2: pcap_open_offline_with_tstamp_precision
    // Use a precision based on input data (modulo to keep valid range)
    u_int precision = (Size > 0) ? (Data[0] % 2) + 1 : 1; // 1 or 2 (MICROSECONDS or NANOSECONDS)
    handle = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
    if (handle) {
        stream = pcap_file(handle);
        if (stream) {
            fseek(stream, 0, SEEK_END);
            ftell(stream);
            fseek(stream, 0, SEEK_SET);
        }
        ret = pcap_next_ex(handle, &header, &packet);
        (void)ret;
        pcap_close(handle);
        handle = NULL;
    }

    // Test 3: pcap_fopen_offline
    f = fopen("./dummy_file", "rb");
    if (f) {
        handle = pcap_fopen_offline(f, errbuf);
        if (handle) {
            stream = pcap_file(handle);
            if (stream) {
                fseek(stream, 0, SEEK_END);
                ftell(stream);
                fseek(stream, 0, SEEK_SET);
            }
            ret = pcap_next_ex(handle, &header, &packet);
            (void)ret;
            pcap_close(handle);
            handle = NULL;
            // pcap_close closes the underlying FILE if opened via pcap_fopen_offline
        } else {
            // If pcap_fopen_offline failed, we must close the FILE ourselves.
            fclose(f);
            f = NULL;
        }
    }

    // Test 4: pcap_fopen_offline_with_tstamp_precision
    f = fopen("./dummy_file", "rb");
    if (f) {
        handle = pcap_fopen_offline_with_tstamp_precision(f, precision, errbuf);
        if (handle) {
            stream = pcap_file(handle);
            if (stream) {
                fseek(stream, 0, SEEK_END);
                ftell(stream);
                fseek(stream, 0, SEEK_SET);
            }
            ret = pcap_next_ex(handle, &header, &packet);
            (void)ret;
            pcap_close(handle);
            handle = NULL;
            // pcap_close closes the underlying FILE if opened via pcap_fopen_offline_with_tstamp_precision
        } else {
            // If pcap_fopen_offline_with_tstamp_precision failed, we must close the FILE ourselves.
            fclose(f);
            f = NULL;
        }
    }

    return 0;
}