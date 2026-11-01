// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <time.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

static int create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return -1;
    }
    
    // Write a minimal pcap header if data is too small
    if (Size < 24) {
        // Magic number
        uint32_t magic = 0xa1b2c3d4;
        // Version major
        uint16_t ver_major = 2;
        // Version minor
        uint16_t ver_minor = 4;
        // Timezone
        int32_t tz = 0;
        // Sigfigs
        uint32_t sigfigs = 0;
        // Snaplen
        uint32_t snaplen = 65535;
        // Linktype
        uint32_t linktype = 1; // Ethernet
        
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&linktype, sizeof(linktype), 1, f);
        
        // Write some dummy packet data if available
        if (Size > 0) {
            uint32_t ts_sec = 1000;
            uint32_t ts_usec = 0;
            uint32_t caplen = Size;
            uint32_t len = Size;
            
            fwrite(&ts_sec, sizeof(ts_sec), 1, f);
            fwrite(&ts_usec, sizeof(ts_usec), 1, f);
            fwrite(&caplen, sizeof(caplen), 1, f);
            fwrite(&len, sizeof(len), 1, f);
            fwrite(Data, 1, Size, f);
        }
    } else {
        fwrite(Data, 1, Size, f);
    }
    
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_pkthdr *hdr = NULL;
    const u_char *packet = NULL;
    int ret;

    // Ensure minimum size for safety
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for file-based operations
    if (create_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Try opening a file source
    handle = pcap_open("./dummy_file", 65535, 0, 1000, NULL, errbuf);
    
    if (handle) {
        // Test pcap_next
        hdr = malloc(sizeof(struct pcap_pkthdr));
        if (hdr) {
            packet = pcap_next(handle, hdr);
            (void)packet; // Suppress unused warning
            free(hdr);
        }

        // Test pcap_next_ex
        hdr = malloc(sizeof(struct pcap_pkthdr));
        if (hdr) {
            ret = pcap_next_ex(handle, &hdr, &packet);
            (void)ret;
            (void)packet;
            free(hdr);
        }

        // Test pcap_dispatch
        ret = pcap_dispatch(handle, 10, dummy_callback, NULL);
        (void)ret;

        // Test pcap_loop with a small count to avoid infinite loops
        ret = pcap_loop(handle, 5, dummy_callback, NULL);
        (void)ret;

        // Test pcap_dump
        FILE *dump_file = fopen("./dump_output", "wb");
        if (dump_file) {
            pcap_dumper_t *dump = pcap_dump_open(handle, dump_file);
            if (dump) {
                // We cannot easily invoke pcap_dump directly without a packet loop,
                // but opening and closing exercises the allocation paths.
                pcap_dump_close(dump);
            }
            fclose(dump_file);
        }

        pcap_close(handle);
    }

    // Try opening a live source (might fail, which is fine for fuzzing)
    // Using a dummy interface name from input data
    char source[256];
    size_t src_len = (Size > 255) ? 255 : Size;
    memcpy(source, Data, src_len);
    source[src_len] = '\0';
    
    // Ensure source is valid
    if (src_len == 0 || source[0] == '\0') {
        strcpy(source, "any");
    }

    handle = pcap_open(source, 65535, 0, 1000, NULL, errbuf);
    
    if (handle) {
        // Test pcap_next
        hdr = malloc(sizeof(struct pcap_pkthdr));
        if (hdr) {
            packet = pcap_next(handle, hdr);
            (void)packet;
            free(hdr);
        }

        // Test pcap_next_ex
        hdr = malloc(sizeof(struct pcap_pkthdr));
        if (hdr) {
            ret = pcap_next_ex(handle, &hdr, &packet);
            (void)ret;
            (void)packet;
            free(hdr);
        }

        // Test pcap_dispatch
        ret = pcap_dispatch(handle, 10, dummy_callback, NULL);
        (void)ret;

        // Test pcap_loop
        ret = pcap_loop(handle, 5, dummy_callback, NULL);
        (void)ret;

        pcap_close(handle);
    }

    // Clean up dummy files
    remove("./dummy_file");
    remove("./dump_output");

    return 0;
}