// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <time.h>
#include <sys/time.h>

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

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Parse input to determine which API to fuzz
    uint32_t mode = *(const uint32_t *)Data;
    Data += 4;
    Size -= 4;

    struct bpf_program prog = {0};
    pcap_t *handle = NULL;
    int ret = 0;

    switch (mode % 6) {
        case 0: {
            // Fuzz bpf_validate
            if (Size < sizeof(struct bpf_insn)) {
                break;
            }
            size_t num_insns = Size / sizeof(struct bpf_insn);
            if (num_insns == 0) break;
            
            prog.bf_insns = (struct bpf_insn *)Data;
            prog.bf_len = num_insns;
            
            ret = bpf_validate(prog.bf_insns, prog.bf_len);
            prog.bf_insns = NULL;
            break;
        }
        case 1: {
            // Fuzz pcap_compile_nopcap
            if (Size < 1) break;
            
            char *filter_str = (char *)malloc(Size + 1);
            if (!filter_str) break;
            memcpy(filter_str, Data, Size);
            filter_str[Size] = '\0';
            
            ret = pcap_compile_nopcap(0, DLT_RAW, &prog, filter_str, 1, 0);
            
            if (ret == 0) {
                cleanup_bpf_program(&prog);
            }
            free(filter_str);
            break;
        }
        case 2: {
            // Fuzz pcap_offline_filter
            handle = pcap_open_dead(DLT_RAW, 65535);
            if (!handle) break;
            
            size_t str_len = 0;
            const char *start = (const char *)Data;
            while (str_len < Size && start[str_len] != '\0') {
                str_len++;
            }
            if (str_len == 0) {
                cleanup_pcap(handle);
                break;
            }
            
            char *filter_str = malloc(str_len + 1);
            if (!filter_str) {
                cleanup_pcap(handle);
                break;
            }
            memcpy(filter_str, start, str_len);
            filter_str[str_len] = '\0';
            
            ret = pcap_compile(handle, &prog, filter_str, 1, 0);
            free(filter_str);
            
            if (ret == 0) {
                struct pcap_pkthdr hdr;
                memset(&hdr, 0, sizeof(hdr));
                gettimeofday(&hdr.ts, NULL);
                
                size_t payload_size = Size - str_len;
                const u_char *payload = (const u_char *)(Data + str_len);
                
                if (payload_size > 0) {
                    hdr.caplen = payload_size;
                    hdr.len = payload_size;
                    ret = pcap_offline_filter(&prog, &hdr, payload);
                } else {
                    hdr.caplen = 0;
                    hdr.len = 0;
                    ret = pcap_offline_filter(&prog, &hdr, NULL);
                }
                cleanup_bpf_program(&prog);
            }
            cleanup_pcap(handle);
            break;
        }
        case 3: {
            // Fuzz bpf_filter
            // Skip this case as it triggers an abort() in libpcap on invalid input
            // The library explicitly checks for invalid BPF instructions and aborts.
            // We rely on bpf_validate (case 0) and pcap_compile (case 5) to test validity.
            break;
        }
        case 4: {
            // Fuzz bpf_dump
            handle = pcap_open_dead(DLT_RAW, 65535);
            if (!handle) break;
            
            size_t str_len = 0;
            const char *start = (const char *)Data;
            while (str_len < Size && start[str_len] != '\0') {
                str_len++;
            }
            if (str_len == 0) {
                cleanup_pcap(handle);
                break;
            }
            
            char *filter_str = malloc(str_len + 1);
            if (!filter_str) {
                cleanup_pcap(handle);
                break;
            }
            memcpy(filter_str, start, str_len);
            filter_str[str_len] = '\0';
            
            ret = pcap_compile(handle, &prog, filter_str, 1, 0);
            free(filter_str);
            
            if (ret == 0) {
                int dump_opt = 1;
                if (str_len + 1 < Size) {
                    dump_opt = (Data[str_len] % 3) + 1;
                }
                bpf_dump(&prog, dump_opt);
                cleanup_bpf_program(&prog);
            }
            cleanup_pcap(handle);
            break;
        }
        case 5: {
            // Fuzz pcap_compile
            handle = pcap_open_dead(DLT_RAW, 65535);
            if (!handle) break;
            
            size_t str_len = 0;
            const char *start = (const char *)Data;
            while (str_len < Size && start[str_len] != '\0') {
                str_len++;
            }
            if (str_len == 0) {
                cleanup_pcap(handle);
                break;
            }
            
            char *filter_str = malloc(str_len + 1);
            if (!filter_str) {
                cleanup_pcap(handle);
                break;
            }
            memcpy(filter_str, start, str_len);
            filter_str[str_len] = '\0';
            
            int optimize = 0;
            if (str_len + 1 < Size) {
                optimize = (Data[str_len] & 1);
            }
            
            bpf_u_int32 netmask = 0;
            if (str_len + 1 + 4 <= Size) {
                memcpy(&netmask, Data + str_len + 1, 4);
            }
            
            ret = pcap_compile(handle, &prog, filter_str, optimize, netmask);
            free(filter_str);
            
            if (ret == 0) {
                cleanup_bpf_program(&prog);
            }
            cleanup_pcap(handle);
            break;
        }
    }

    return 0;
}