// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
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
#include <sys/time.h>
#include <errno.h>

/* Callback for pcap_loop and pcap_dispatch */
static void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user_data;
    (void)header;
    (void)packet;
}

/* Helper to create a dummy pcap file for offline testing */
static int create_dummy_pcap_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return -1;
    }

    /* Write a minimal pcap global header (24 bytes) */
    uint32_t magic = 0xa1b2c3d4;
    uint16_t ver_major = 2;
    uint16_t ver_minor = 4;
    int32_t tz = 0;
    int32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1;

    fwrite(&magic, sizeof(magic), 1, fp);
    fwrite(&ver_major, sizeof(ver_major), 1, fp);
    fwrite(&ver_minor, sizeof(ver_minor), 1, fp);
    fwrite(&tz, sizeof(tz), 1, fp);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
    fwrite(&snaplen, sizeof(snaplen), 1, fp);
    fwrite(&network, sizeof(network), 1, fp);

    /* Write one dummy packet */
    uint32_t ts_sec = 1;
    uint32_t ts_usec = 0;
    uint32_t incl_len = 4;
    uint32_t orig_len = 4;
    uint8_t data[4] = {0x00, 0x01, 0x02, 0x03};

    fwrite(&ts_sec, sizeof(ts_sec), 1, fp);
    fwrite(&ts_usec, sizeof(ts_usec), 1, fp);
    fwrite(&incl_len, sizeof(incl_len), 1, fp);
    fwrite(&orig_len, sizeof(orig_len), 1, fp);
    fwrite(data, sizeof(data), 1, fp);

    fclose(fp);
    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;
    int loop_count = 0;
    int dispatch_count = 0;
    int next_ex_ret;
    
    if (Size == 0) {
        return 0;
    }

    if (create_dummy_pcap_file("./dummy_file") != 0) {
        return 0;
    }

    int flags = (Data[0] % 2);
    int snaplen = 65535;
    int read_timeout = 1000;
    
    handle = pcap_open("./dummy_file", snaplen, flags, read_timeout, NULL, errbuf);

    if (handle == NULL) {
        return 0;
    }

    /* 1. Test pcap_next */
    header = malloc(sizeof(struct pcap_pkthdr));
    if (header) {
        packet = pcap_next(handle, header);
        free(header);
        header = NULL;
    }

    /* 2. Test pcap_next_ex */
    packet = NULL;
    next_ex_ret = pcap_next_ex(handle, &header, &packet);
    if (header) {
        (void)header;
    }

    /* 3. Test pcap_dispatch */
    dispatch_count = (Size > 1 ? Data[1] : 0) % 10 + 1;
    ret = pcap_dispatch(handle, dispatch_count, packet_handler, NULL);

    /* 4. Test pcap_loop */
    loop_count = (Size > 2 ? Data[2] : 0) % 5 + 1;
    ret = pcap_loop(handle, loop_count, packet_handler, NULL);

    /* 5. Test pcap_dump */
    FILE *dump_fp = fopen("./dump_output.pcap", "wb");
    if (dump_fp) {
        header = malloc(sizeof(struct pcap_pkthdr));
        if (header) {
            packet = pcap_next(handle, header);
            if (packet && header) {
                /* Call pcap_dump with the FILE pointer cast to u_char * as per prompt signature */
                pcap_dump((u_char *)dump_fp, header, packet);
            }
            free(header);
        }
        fclose(dump_fp);
    }

    pcap_close(handle);
    
    remove("./dummy_file");
    remove("./dump_output.pcap");

    return 0;
}