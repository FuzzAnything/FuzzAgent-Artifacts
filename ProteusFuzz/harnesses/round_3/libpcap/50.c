// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <pcap.h>
#include <bpf.h>

/*
 * Helper to create a dummy pcap file for offline operations.
 * Returns 0 on success, -1 on failure.
 */
static int create_dummy_pcap_file(const char *filename) {
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

    fwrite(&magic, sizeof(uint32_t), 1, f);
    fwrite(&version_major, sizeof(uint16_t), 1, f);
    fwrite(&version_minor, sizeof(uint16_t), 1, f);
    fwrite(&thiszone, sizeof(int32_t), 1, f);
    fwrite(&sigfigs, sizeof(uint32_t), 1, f);
    fwrite(&snaplen, sizeof(uint32_t), 1, f);
    fwrite(&network, sizeof(uint32_t), 1, f);

    /* Write a dummy packet header (16 bytes) */
    struct pcap_pkthdr hdr;
    struct timeval ts = {0, 0};
    hdr.ts = ts;
    hdr.caplen = 0;
    hdr.len = 0;
    fwrite(&hdr, sizeof(struct pcap_pkthdr), 1, f);

    fclose(f);
    return 0;
}

/*
 * Helper to generate random BPF instructions from fuzz input.
 * Returns the number of instructions generated.
 */
static int generate_bpf_instructions(const uint8_t *data, size_t size, struct bpf_insn *insns, int max_len) {
    int count = 0;
    int idx = 0;

    while (count < max_len && idx + 8 <= size) {
        struct bpf_insn *insn = &insns[count];
        
        /* Read code (2 bytes) */
        insn->code = data[idx] | (data[idx+1] << 8);
        idx += 2;
        
        /* Read jt (1 byte) */
        insn->jt = data[idx];
        idx += 1;
        
        /* Read jf (1 byte) */
        insn->jf = data[idx];
        idx += 1;
        
        /* Read k (4 bytes) */
        if (idx + 4 <= size) {
            insn->k = data[idx] | (data[idx+1] << 8) | (data[idx+2] << 16) | ((bpf_u_int32)data[idx+3] << 24);
            idx += 4;
        } else {
            insn->k = 0;
        }

        count++;
    }
    return count;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Prepare environment */
    const char *dummy_file = "./dummy_file";
    struct bpf_program prog;
    struct bpf_insn *insns = NULL;
    int num_insns = 0;
    pcap_t *handle = NULL;
    struct pcap_pkthdr pkt_header;
    u_char *pkt_data = NULL;
    int ret;

    /* Initialize structures */
    memset(&prog, 0, sizeof(prog));
    memset(&pkt_header, 0, sizeof(pkt_header));
    pkt_header.caplen = 0;
    pkt_header.len = 0;

    /* Generate BPF instructions from input */
    /* Limit max instructions to prevent excessive allocation */
    int max_insns = (Size / 8) + 1;
    if (max_insns > 1000) max_insns = 1000;
    
    insns = (struct bpf_insn *)malloc(max_insns * sizeof(struct bpf_insn));
    if (!insns) return 0;

    num_insns = generate_bpf_instructions(Data, Size, insns, max_insns);
    prog.bf_len = num_insns;
    prog.bf_insns = insns;

    /* 1. Test bpf_validate */
    /* Validates the instruction sequence */
    ret = bpf_validate(insns, num_insns);
    (void)ret; /* Ignore return for fuzzing, just ensure no crash */

    /* 2. Test bpf_image */
    /* Converts instructions to string */
    char *img = bpf_image(insns, num_insns);
    (void)img; /* Ignore return, just ensure no crash */

    /* 3. Test bpf_dump */
    /* Prints instructions in various formats */
    bpf_dump(&prog, 0); /* Human readable */
    bpf_dump(&prog, 1); /* C initializer */
    bpf_dump(&prog, 2); /* Raw numeric */
    bpf_dump(&prog, 3); /* Extended raw */

    /* 4. Test pcap_offline_filter */
    /* Requires a valid pcap handle for context, but filter can be tested directly */
    /* We create a dummy file to open a handle for pcap_compile later, 
       but pcap_offline_filter works with just the program and header/data */
    
    /* Create dummy data for packet */
    pkt_data = (u_char *)malloc(100);
    if (pkt_data) {
        memset(pkt_data, 0, 100);
        /* Randomize some bytes if data available */
        if (Size > 0) pkt_data[0] = Data[0];
        
        /* 
         * FIX: pcap_offline_filter implementation accesses fp->bf_insns.
         * Passing NULL causes a segfault. We must pass a valid struct, 
         * even if it has no instructions, or skip the NULL test.
         * We will test with an empty program instead of NULL.
         */
        struct bpf_program empty_prog;
        memset(&empty_prog, 0, sizeof(empty_prog));
        empty_prog.bf_len = 0;
        empty_prog.bf_insns = NULL;
        
        ret = pcap_offline_filter(&empty_prog, &pkt_header, pkt_data);
        (void)ret;
    }

    /* 5. Test pcap_compile and pcap_freecode */
    /* Requires an open pcap handle */
    if (create_dummy_pcap_file(dummy_file) == 0) {
        handle = pcap_open_offline(dummy_file, NULL);
        if (handle) {
            /* Try to compile a filter string derived from input or a default */
            /* We use a simple valid filter string to ensure compilation works, 
               but we could also try to parse parts of the fuzz input if it's printable */
            const char *filter_str = "host 127.0.0.1";
            int optimize = (Size % 2); /* Toggle optimization based on input size */
            bpf_u_int32 netmask = 0xffffff00;

            ret = pcap_compile(handle, &prog, filter_str, optimize, netmask);
            if (ret == 0) {
                /* Compilation successful, test the compiled program */
                if (pkt_data) {
                    ret = pcap_offline_filter(&prog, &pkt_header, pkt_data);
                    (void)ret;
                }
                
                /* Dump the compiled program */
                bpf_dump(&prog, 0);
                
                /* Free the compiled code */
                pcap_freecode(&prog);
            } else {
                /* Compilation failed, ensure we don't leak if partial init happened */
                pcap_freecode(&prog);
            }
            
            pcap_close(handle);
            handle = NULL;
        }
    }

    /* Cleanup */
    if (insns) {
        free(insns);
    }
    if (pkt_data) {
        free(pkt_data);
    }
    if (handle) {
        pcap_close(handle);
    }
    
    /* Remove dummy file */
    unlink(dummy_file);

    return 0;
}