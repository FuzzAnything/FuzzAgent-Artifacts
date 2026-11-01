// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pcap.h>
#include <bpf.h>

/* 
 * Fuzz driver for libpcap API functions:
 * bpf_image, pcap_compile, bpf_validate, pcap_freecode, 
 * pcap_offline_filter, bpf_dump
 */

static void free_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        pcap_freecode(prog);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

static int create_dummy_packet_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    /* Write a minimal pcap global header (24 bytes) */
    uint32_t magic = 0xa1b2c3d4; /* Standard pcap magic */
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; /* Ethernet */

    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&thiszone, sizeof(thiszone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);

    /* Write a packet header and data if available */
    if (size > 0) {
        struct pcap_pkthdr hdr;
        hdr.ts.tv_sec = 1;
        hdr.ts.tv_usec = 0;
        hdr.caplen = size;
        hdr.len = size;

        fwrite(&hdr, sizeof(hdr), 1, f);
        fwrite(data, 1, size, f);
    } else {
        /* Write an empty packet header to ensure valid file */
        struct pcap_pkthdr hdr;
        hdr.ts.tv_sec = 1;
        hdr.ts.tv_usec = 0;
        hdr.caplen = 0;
        hdr.len = 0;
        fwrite(&hdr, sizeof(hdr), 1, f);
    }

    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 1. Prepare environment */
    
    /* Create a dummy pcap file for offline capture */
    if (create_dummy_packet_file(Data, Size) != 0) {
        return 0;
    }

    pcap_t *handle = pcap_open_offline("./dummy_file", NULL);
    if (!handle) {
        return 0;
    }

    /* Activate the handle if necessary (pcap_compile requires activation) */
    /* Note: pcap_open_offline usually activates automatically, but we ensure state */
    /* We don't explicitly call pcap_activate as it's internal, but we assume open_offline works */
    
    struct bpf_program fcode;
    memset(&fcode, 0, sizeof(fcode));
    
    /* 2. Fuzz Logic */

    /* A. Fuzz bpf_validate and bpf_image with raw data interpreted as instructions */
    /* We treat the input data as a stream of bpf_insn structures */
    size_t max_insns = Size / sizeof(struct bpf_insn);
    if (max_insns > 0) {
        const struct bpf_insn *insns = (const struct bpf_insn *)Data;
        
        /* Test bpf_validate */
        int val_result = bpf_validate(insns, (int)max_insns);
        (void)val_result; /* Use result to avoid unused warning, logic is side-effect free */

        /* Test bpf_image */
        char *img = bpf_image(insns, (int)max_insns);
        if (img) {
            /* Just access the string to ensure it's valid */
            size_t len = strlen(img);
            (void)len;
        }
    }

    /* B. Fuzz pcap_compile */
    /* We need a filter string. We'll use the input data as a potential filter string */
    /* Ensure null termination for safety */
    char *filter_str = (char *)malloc(Size + 1);
    if (!filter_str) {
        pcap_close(handle);
        return 0;
    }
    memcpy(filter_str, Data, Size);
    filter_str[Size] = '\0';

    /* Try compiling with different netmasks and optimize flags */
    bpf_u_int32 netmask = 0;
    int optimize = 0;

    /* Extract potential netmask from data if possible, or use 0 */
    if (Size >= 4) {
        netmask = *(const bpf_u_int32 *)(Data + 4);
    }
    if (Size >= 8) {
        optimize = (Data[8] & 1);
    }

    int compile_result = pcap_compile(handle, &fcode, filter_str, optimize, netmask);
    
    if (compile_result == 0 && fcode.bf_insns) {
        /* Compilation successful, test other functions with the generated code */
        
        /* Test bpf_dump with various options */
        bpf_dump(&fcode, 0); /* Human readable */
        bpf_dump(&fcode, 1); /* C initializer */
        bpf_dump(&fcode, 2); /* Raw numeric */
        bpf_dump(&fcode, 3); /* Raw numeric */

        /* Test pcap_offline_filter */
        /* We need a packet header and data. Re-read from file or construct dummy */
        /* Since we opened the file, we can try to read a packet */
        struct pcap_pkthdr *pkt_header;
        const u_char *pkt_data;
        
        /* Try to get a packet from the handle */
        int packet_result = pcap_next_ex(handle, &pkt_header, &pkt_data);
        if (packet_result == 1) {
            int match = pcap_offline_filter(&fcode, pkt_header, pkt_data);
            (void)match;
        } else {
            /* Construct a dummy packet header if read failed */
            struct pcap_pkthdr dummy_hdr;
            dummy_hdr.ts.tv_sec = 0;
            dummy_hdr.ts.tv_usec = 0;
            dummy_hdr.caplen = Size;
            dummy_hdr.len = Size;
            
            /* Use the input data as packet data */
            int match = pcap_offline_filter(&fcode, &dummy_hdr, Data);
            (void)match;
        }

        /* Cleanup the compiled code */
        pcap_freecode(&fcode);
    }

    /* 3. Cleanup */
    free(filter_str);
    pcap_close(handle);
    
    /* Remove dummy file */
    unlink("./dummy_file");

    return 0;
}