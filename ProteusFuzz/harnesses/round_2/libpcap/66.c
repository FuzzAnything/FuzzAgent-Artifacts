// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Dummy file for offline capture operations */
static const char *dummy_file = "./dummy_file";

/* Helper to create a dummy pcap_t for offline mode */
static pcap_t *create_dummy_pcap(void) {
    pcap_t *handle = pcap_open_offline(dummy_file, NULL);
    if (handle) {
        /* Activate the handle to allow compilation */
        pcap_activate(handle);
    }
    return handle;
}

/* Helper to write a minimal pcap file header to dummy_file */
static void write_dummy_pcap_file(void) {
    FILE *f = fopen(dummy_file, "wb");
    if (!f) return;

    /* Minimal pcap global header (magic, version, timezone, snaplen, linktype) */
    uint32_t magic = 0xa1b2c3d4; /* Standard pcap magic */
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t timezone = 0;
    uint32_t snaplen = 65535;
    uint32_t linktype = 1; /* Ethernet */

    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&timezone, sizeof(timezone), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&linktype, sizeof(linktype), 1, f);

    /* Write one dummy packet header and data */
    struct pcap_pkthdr hdr;
    struct timeval ts = {0, 0};
    hdr.ts = ts;
    hdr.caplen = 0;
    hdr.len = 0;
    fwrite(&hdr, sizeof(hdr), 1, f);
    /* No data for this packet */

    fclose(f);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data to work with */
    if (Size < 1) return 0;

    /* Prepare environment: create dummy file if needed */
    write_dummy_pcap_file();

    pcap_t *handle = create_dummy_pcap();
    struct bpf_program prog;
    struct bpf_insn *insns = NULL;
    int num_insns = 0;
    int ret;

    /* Initialize prog */
    memset(&prog, 0, sizeof(prog));

    /* Fuzzing Strategy 1: Compile a filter string from input */
    /* We treat the input as a potential filter string, but limit length to avoid huge allocations */
    size_t filter_len = Size;
    if (filter_len > 1000) filter_len = 1000;
    
    char *filter_str = malloc(filter_len + 1);
    if (!filter_str) return 0;
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    /* Try to compile the filter */
    ret = pcap_compile(handle, &prog, filter_str, 1, 0);
    if (ret == 0 && prog.bf_insns != NULL) {
        /* If compilation succeeded, we have a valid program to test other APIs with */
        insns = prog.bf_insns;
        num_insns = prog.bf_len;

        /* Fuzzing Strategy 2: bpf_dump with various options */
        /* Option <= 1: Human readable */
        bpf_dump(&prog, 0);
        bpf_dump(&prog, 1);
        /* Option > 1: C initializer syntax */
        bpf_dump(&prog, 2);
        /* Option > 2: Raw numeric */
        bpf_dump(&prog, 3);

        /* Fuzzing Strategy 3: bpf_image */
        char *img = bpf_image(insns, num_insns);
        if (img) {
            /* Just accessing the string is enough to exercise the code */
            (void)img[0];
        }

        /* Fuzzing Strategy 4: bpf_validate */
        ret = bpf_validate(insns, num_insns);
        (void)ret;

        /* Fuzzing Strategy 5: pcap_offline_filter */
        /* Create a dummy packet header and data */
        struct pcap_pkthdr pkt_hdr;
        struct timeval ts = {0, 0};
        pkt_hdr.ts = ts;
        pkt_hdr.caplen = 0;
        pkt_hdr.len = 0;
        u_char *pkt_data = NULL;
        
        ret = pcap_offline_filter(&prog, &pkt_hdr, pkt_data);
        (void)ret;

        /* Free the compiled program */
        pcap_freecode(&prog);
        insns = NULL;
    }

    /* Fuzzing Strategy 6: Direct manipulation of bpf_insn for bpf_validate and bpf_image */
    /* If compilation failed or to explore more states, construct raw instructions from input */
    if (Size >= sizeof(struct bpf_insn)) {
        size_t max_insns = Size / sizeof(struct bpf_insn);
        if (max_insns > 1000) max_insns = 1000; /* Limit to prevent excessive allocation */
        
        insns = malloc(max_insns * sizeof(struct bpf_insn));
        if (insns) {
            memcpy(insns, Data, max_insns * sizeof(struct bpf_insn));
            
            /* Test bpf_validate on raw input */
            ret = bpf_validate(insns, max_insns);
            (void)ret;

            /* Test bpf_image on raw input */
            char *img = bpf_image(insns, max_insns);
            if (img) {
                (void)img[0];
            }

            /* Test bpf_dump with a constructed program */
            prog.bf_len = max_insns;
            prog.bf_insns = insns;
            bpf_dump(&prog, 0);
            bpf_dump(&prog, 2);
            bpf_dump(&prog, 3);

            /* Test pcap_offline_filter with raw program */
            struct pcap_pkthdr pkt_hdr;
            pkt_hdr.ts.tv_sec = 0;
            pkt_hdr.ts.tv_usec = 0;
            pkt_hdr.caplen = 0;
            pkt_hdr.len = 0;
            u_char *pkt_data = NULL;
            ret = pcap_offline_filter(&prog, &pkt_hdr, pkt_data);
            (void)ret;

            /* Cleanup raw instructions */
            free(insns);
            insns = NULL;
        }
    }

    /* Cleanup */
    free(filter_str);
    if (handle) {
        pcap_close(handle);
    }

    return 0;
}