// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
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

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        free(prog->bf_insns);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Parse input to determine which function to fuzz and extract parameters
    uint32_t mode = *(uint32_t *)Data;
    mode %= 6; // 0-5 for the 6 functions

    struct bpf_program prog = {0};
    pcap_t *handle = NULL;
    int ret = 0;
    u_char *packet_data = NULL;
    struct pcap_pkthdr pkt_hdr = {0};

    // Prepare packet data for functions that need it
    if (Size > 4) {
        size_t pkt_len = Size - 4;
        if (pkt_len > 0) {
            packet_data = (u_char *)malloc(pkt_len);
            if (packet_data) {
                memcpy(packet_data, Data + 4, pkt_len);
            }
        }
    }

    // Initialize packet header
    pkt_hdr.ts.tv_sec = 1234567890;
    pkt_hdr.ts.tv_usec = 123456;
    pkt_hdr.caplen = packet_data ? (bpf_u_int32)strlen((char *)packet_data) : 0;
    pkt_hdr.len = pkt_hdr.caplen;

    switch (mode) {
        case 0: {
            // Fuzz bpf_validate
            // We need to construct a valid bpf_insn array from input
            if (Size >= 8) {
                size_t num_insns = (Size - 4) / sizeof(struct bpf_insn);
                if (num_insns > 0) {
                    struct bpf_insn *insns = (struct bpf_insn *)malloc(num_insns * sizeof(struct bpf_insn));
                    if (insns) {
                        memcpy(insns, Data + 4, num_insns * sizeof(struct bpf_insn));
                        ret = bpf_validate(insns, num_insns);
                        free(insns);
                    }
                }
            }
            break;
        }

        case 1: {
            // Fuzz pcap_compile_nopcap
            // Create a dead handle first
            handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (handle) {
                const char *filter_str = "host 127.0.0.1";
                if (Size > 10) {
                    // Use part of input as filter string (null-terminated)
                    size_t str_len = Size - 4;
                    if (str_len > 256) str_len = 256;
                    char *temp_str = (char *)malloc(str_len + 1);
                    if (temp_str) {
                        memcpy(temp_str, Data + 4, str_len);
                        temp_str[str_len] = '\0';
                        filter_str = temp_str;
                        ret = pcap_compile_nopcap(65535, DLT_EN10MB, &prog, filter_str, 1, 0);
                        free(temp_str);
                    }
                } else {
                    ret = pcap_compile_nopcap(65535, DLT_EN10MB, &prog, filter_str, 1, 0);
                }
                cleanup_bpf_program(&prog);
                cleanup_pcap(handle);
            }
            break;
        }

        case 2: {
            // Fuzz pcap_offline_filter
            // First compile a filter to get a bpf_program
            handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (handle) {
                const char *filter_str = "host 127.0.0.1";
                if (Size > 10) {
                    size_t str_len = Size - 4;
                    if (str_len > 256) str_len = 256;
                    char *temp_str = (char *)malloc(str_len + 1);
                    if (temp_str) {
                        memcpy(temp_str, Data + 4, str_len);
                        temp_str[str_len] = '\0';
                        filter_str = temp_str;
                    }
                }
                
                if (pcap_compile(handle, &prog, filter_str, 1, 0) == 0) {
                    ret = pcap_offline_filter(&prog, &pkt_hdr, packet_data);
                    cleanup_bpf_program(&prog);
                }
                cleanup_pcap(handle);
            }
            break;
        }

        case 3: {
            // Fuzz bpf_filter (deprecated)
            if (Size >= 8) {
                size_t num_insns = (Size - 4) / sizeof(struct bpf_insn);
                if (num_insns > 0) {
                    struct bpf_insn *insns = (struct bpf_insn *)malloc(num_insns * sizeof(struct bpf_insn));
                    if (insns) {
                        memcpy(insns, Data + 4, num_insns * sizeof(struct bpf_insn));
                        u_int wire_len = packet_data ? (u_int)strlen((char *)packet_data) : 0;
                        u_int buf_len = wire_len;
                        ret = bpf_filter(insns, packet_data, wire_len, buf_len);
                        free(insns);
                    }
                }
            }
            break;
        }

        case 4: {
            // Fuzz bpf_dump
            // First compile a filter to get a bpf_program
            handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (handle) {
                const char *filter_str = "host 127.0.0.1";
                if (Size > 10) {
                    size_t str_len = Size - 4;
                    if (str_len > 256) str_len = 256;
                    char *temp_str = (char *)malloc(str_len + 1);
                    if (temp_str) {
                        memcpy(temp_str, Data + 4, str_len);
                        temp_str[str_len] = '\0';
                        filter_str = temp_str;
                    }
                }

                if (pcap_compile(handle, &prog, filter_str, 1, 0) == 0) {
                    int dump_opt = (Size > 4) ? (Data[4] % 3) : 0;
                    bpf_dump(&prog, dump_opt);
                    cleanup_bpf_program(&prog);
                }
                cleanup_pcap(handle);
            }
            break;
        }

        case 5: {
            // Fuzz pcap_compile
            handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (handle) {
                // Activate the handle first
                if (pcap_activate(handle) == 0) {
                    const char *filter_str = "host 127.0.0.1";
                    if (Size > 10) {
                        size_t str_len = Size - 4;
                        if (str_len > 256) str_len = 256;
                        char *temp_str = (char *)malloc(str_len + 1);
                        if (temp_str) {
                            memcpy(temp_str, Data + 4, str_len);
                            temp_str[str_len] = '\0';
                            filter_str = temp_str;
                        }
                    }

                    int optimize = (Size > 5) ? (Data[5] % 2) : 1;
                    ret = pcap_compile(handle, &prog, filter_str, optimize, 0);
                    cleanup_bpf_program(&prog);
                }
                cleanup_pcap(handle);
            }
            break;
        }
    }

    if (packet_data) {
        free(packet_data);
    }

    return 0;
}