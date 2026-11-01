// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <time.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Prepare a pcap handle for offline analysis using the dummy file */
    write_dummy_file(Data, Size);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("./dummy_file", errbuf);
    
    /* Create a bpf_program structure */
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));

    /* Create a packet header and data buffer for pcap_offline_filter */
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = Size > 0 ? Size : 0;
    hdr.len = Size > 0 ? Size : 0;
    struct timeval now;
    gettimeofday(&now, NULL);
    hdr.ts = now;

    /* Test bpf_validate with empty or malformed instructions (avoid NULL pointer) */
    struct bpf_insn empty_insn = {0};
    bpf_validate(&empty_insn, 0);
    bpf_validate(&empty_insn, 1);
    /* Do not call bpf_validate with NULL pointer as it may crash depending on implementation */

    /* Test bpf_image with empty or malformed instructions (avoid NULL pointer) */
    bpf_image(&empty_insn, 0);
    bpf_image(&empty_insn, 1);
    /* Do not call bpf_image with NULL pointer as it may crash */

    /* Test bpf_dump with empty program */
    struct bpf_program empty_prog = {0, NULL};
    bpf_dump(&empty_prog, 0);
    bpf_dump(&empty_prog, 1);
    bpf_dump(&empty_prog, 2);
    bpf_dump(&empty_prog, 3);

    if (!handle) {
        /* If the file is not a valid pcap, we can only test functions that don't need a handle */
        return 0;
    }

    /* Ensure the handle is activated for pcap_compile */
    pcap_activate(handle);

    /* Attempt 1: Use input data as filter string (might fail, which is fine) */
    int ret = pcap_compile(handle, &prog, (const char *)Data, 0, 0);
    if (ret == 0) {
        /* Compilation successful */
        
        /* Test bpf_dump */
        bpf_dump(&prog, 0);
        bpf_dump(&prog, 1);
        bpf_dump(&prog, 2);
        bpf_dump(&prog, 3);
        
        /* Test bpf_image */
        char *img = bpf_image(prog.bf_insns, prog.bf_len);
        (void)img;
        
        /* Test bpf_validate */
        int valid = bpf_validate(prog.bf_insns, prog.bf_len);
        (void)valid;
        
        /* Test pcap_offline_filter */
        int match = pcap_offline_filter(&prog, &hdr, Data);
        (void)match;
        
        /* Test pcap_freecode */
        pcap_freecode(&prog);
    }
    
    /* Attempt 2: Compile a known valid filter to ensure we exercise the success path */
    const char *valid_filter = "host 127.0.0.1";
    ret = pcap_compile(handle, &prog, valid_filter, 1, 0);
    if (ret == 0) {
        /* Compilation successful */
        
        /* Test bpf_dump */
        bpf_dump(&prog, 0);
        bpf_dump(&prog, 1);
        bpf_dump(&prog, 2);
        bpf_dump(&prog, 3);
        
        /* Test bpf_image */
        char *img = bpf_image(prog.bf_insns, prog.bf_len);
        (void)img;
        
        /* Test bpf_validate */
        int valid = bpf_validate(prog.bf_insns, prog.bf_len);
        (void)valid;
        
        /* Test pcap_offline_filter */
        int match = pcap_offline_filter(&prog, &hdr, Data);
        (void)match;
        
        /* Test pcap_freecode */
        pcap_freecode(&prog);
    }

    /* Attempt 3: Compile with optimize=0 and mask=0 */
    ret = pcap_compile(handle, &prog, valid_filter, 0, 0);
    if (ret == 0) {
        bpf_dump(&prog, 1);
        bpf_image(prog.bf_insns, prog.bf_len);
        bpf_validate(prog.bf_insns, prog.bf_len);
        pcap_offline_filter(&prog, &hdr, Data);
        pcap_freecode(&prog);
    }

    /* Attempt 4: Compile with a different valid filter */
    const char *valid_filter2 = "tcp port 80";
    ret = pcap_compile(handle, &prog, valid_filter2, 1, 0);
    if (ret == 0) {
        bpf_dump(&prog, 2);
        bpf_image(prog.bf_insns, prog.bf_len);
        bpf_validate(prog.bf_insns, prog.bf_len);
        pcap_offline_filter(&prog, &hdr, Data);
        pcap_freecode(&prog);
    }

    /* Cleanup */
    pcap_close(handle);
    
    return 0;
}