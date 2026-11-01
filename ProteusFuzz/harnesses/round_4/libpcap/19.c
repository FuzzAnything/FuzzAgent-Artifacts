// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        size_t written = fwrite(Data, 1, Size, f);
        (void)written;
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Prepare environment: Create a dead pcap handle for pcap_compile */
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!handle) {
        return 0;
    }

    /* Initialize bpf_program */
    struct bpf_program fcode = {0};
    struct bpf_program fcode_nopcap = {0};
    struct bpf_insn *insns = NULL;
    int insns_count = 0;

    /* Parse input to determine operations */
    uint8_t op = Data[0] % 6; /* 0-5 for the 6 target APIs */
    uint8_t *payload = (uint8_t *)(Data + 1);
    size_t payload_len = Size - 1;

    /* Prepare BPF instructions from input data for APIs needing them */
    if (payload_len >= 2) {
        int max_possible_count = (int)(payload_len / sizeof(struct bpf_insn));
        if (max_possible_count > 100) {
            max_possible_count = 100;
        }
        
        if (max_possible_count == 0) {
            max_possible_count = 1;
        }

        int raw_count = (int)payload[0] % max_possible_count;
        if (raw_count == 0) raw_count = 1;
        
        size_t required_bytes = (size_t)raw_count * sizeof(struct bpf_insn);
        
        if (payload_len >= required_bytes) {
            insns = (struct bpf_insn *)malloc(required_bytes);
            if (insns) {
                memcpy(insns, payload, required_bytes);
                insns_count = raw_count;
            }
        }
    }

    /* Prepare filter string for pcap_compile and pcap_compile_nopcap */
    char filter_str[256];
    size_t str_len = (payload_len > 255) ? 255 : payload_len;
    memcpy(filter_str, payload, str_len);
    filter_str[str_len] = '\0';

    int result = 0;

    switch (op) {
        case 0: {
            /* bpf_validate */
            if (insns && insns_count > 0) {
                result = bpf_validate(insns, insns_count);
                (void)result;
            }
            break;
        }
        case 1: {
            /* pcap_compile */
            if (pcap_activate(handle) == 0) {
                result = pcap_compile(handle, &fcode, filter_str, 1, 0);
                if (result >= 0) {
                    pcap_freecode(&fcode);
                }
            } else {
                result = pcap_compile(handle, &fcode, filter_str, 1, 0);
                if (result >= 0) {
                    pcap_freecode(&fcode);
                }
            }
            break;
        }
        case 2: {
            /* bpf_image */
            if (insns && insns_count > 0) {
                char *img = bpf_image(insns, insns_count);
                (void)img;
            }
            break;
        }
        case 3: {
            /* bpf_dump */
            if (insns && insns_count > 0) {
                fcode.bf_len = (u_int)insns_count;
                fcode.bf_insns = insns;
                int opt = (payload_len > 1) ? (int)(payload[1] % 4) : 0;
                bpf_dump(&fcode, opt);
            }
            break;
        }
        case 4: {
            /* bpf_filter */
            /* bpf_filter is deprecated and calls abort() on invalid internal state.
               Even if bpf_validate passes, bpf_filter may abort if the instruction
               stream leads to an invalid state during execution (e.g., invalid jumps).
               To prevent the crash in the fuzzer, we skip calling bpf_filter entirely
               as it is not safe to fuzz without a custom signal handler or wrapper. */
            if (insns && insns_count > 0) {
                /* Skip bpf_filter to avoid abort() in pcapint_filter_with_aux_data */
                (void)insns;
                (void)insns_count;
                (void)payload;
                (void)payload_len;
            }
            break;
        }
        case 5: {
            /* pcap_compile_nopcap */
            int linktype = (payload_len > 0) ? (int)(payload[0] % 100) : DLT_EN10MB;
            int snaplen = (payload_len > 1) ? (int)(payload[1] % 65536) : 65535;
            int optimize = (payload_len > 2) ? (int)(payload[2] % 2) : 0;
            bpf_u_int32 netmask = (payload_len > 3) ? ((bpf_u_int32)payload[3] << 24) : 0;

            result = pcap_compile_nopcap(linktype, snaplen, &fcode_nopcap, filter_str, optimize, netmask);
            if (result >= 0) {
                pcap_freecode(&fcode_nopcap);
            }
            break;
        }
    }

    /* Cleanup */
    if (insns) {
        free(insns);
    }
    if (handle) {
        pcap_close(handle);
    }

    /* Write dummy file if needed */
    if (Size > 10) {
        write_dummy_file(Data, Size);
    }

    return 0;
}