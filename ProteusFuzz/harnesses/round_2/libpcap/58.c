// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>

/* Callback function for pcap_loop and pcap_dispatch */
static void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user_data;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;
    int cnt;
    int snaplen;
    int flags;
    int timeout;
    FILE *dump_file = NULL;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Extract parameters from fuzz input if available */
    snaplen = (Size > 0) ? (int)Data[0] : 65535;
    if (snaplen == 0) snaplen = 65535;
    
    flags = (Size > 1) ? (int)Data[1] : 0;
    timeout = (Size > 2) ? (int)Data[2] : 1000;
    cnt = (Size > 3) ? (int)Data[3] : 1;
    if (cnt == 0) cnt = 1;

    /* Create a dummy file for offline capture testing */
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        /* Write a minimal pcap header to make it a valid pcap file */
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        int32_t tz = 0;
        int32_t sigfigs = 0;
        uint32_t linktype = 1; /* Ethernet */
        
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&linktype, sizeof(linktype), 1, f);
        
        /* Write a dummy packet if we have enough data */
        if (Size > 4) {
            struct pcap_pkthdr hdr;
            hdr.ts.tv_sec = 0;
            hdr.ts.tv_usec = 0;
            hdr.caplen = Size - 4;
            hdr.len = Size - 4;
            fwrite(&hdr, sizeof(hdr), 1, f);
            fwrite(Data + 4, Size - 4, 1, f);
        }
        fclose(f);
    }

    /* Try to open the capture handle using the dummy file */
    char source[256];
    snprintf(source, sizeof(source), "file://%s", dummy_file);
    
    handle = pcap_open(source, snaplen, flags, timeout, NULL, errbuf);
    
    if (handle == NULL) {
        /* If opening the file failed, try opening "any" (might fail in env, but good for coverage) */
        handle = pcap_open("any", snaplen, flags, timeout, NULL, errbuf);
        if (handle == NULL) {
            /* Cleanup and return if we can't open anything */
            return 0;
        }
    }

    /* Test pcap_next */
    struct pcap_pkthdr hdr;
    const u_char *pkt = pcap_next(handle, &hdr);
    (void)pkt;

    /* Test pcap_next_ex */
    ret = pcap_next_ex(handle, &header, &packet);
    (void)ret;

    /* Test pcap_dispatch */
    ret = pcap_dispatch(handle, cnt, packet_handler, NULL);
    (void)ret;

    /* Test pcap_loop with a small count to avoid hanging */
    ret = pcap_loop(handle, cnt, packet_handler, NULL);
    (void)ret;

    /* Test pcap_dump */
    /* pcap_dump is a callback function: void pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *); */
    /* It writes to the file passed as the first argument (user_data). */
    dump_file = fopen(dummy_file, "ab");
    if (dump_file) {
        /* Create a fake packet header and data for testing the dump callback */
        struct pcap_pkthdr test_hdr;
        test_hdr.ts.tv_sec = 1000;
        test_hdr.ts.tv_usec = 0;
        test_hdr.caplen = 10;
        test_hdr.len = 10;
        
        u_char test_data[10] = {0};
        
        /* Call pcap_dump directly with the file pointer as user_data */
        pcap_dump((u_char *)dump_file, &test_hdr, test_data);
        
        fclose(dump_file);
    }

    /* Cleanup */
    if (handle) {
        pcap_close(handle);
    }

    /* Remove dummy file */
    unlink(dummy_file);

    return 0;
}