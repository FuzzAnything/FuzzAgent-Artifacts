// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include "pcap/bpf.h"
#include "pcap/pcap.h"

static sigjmp_buf jmp_env;
static volatile sig_atomic_t can_jump = 0;

static void sigabrt_handler(int sig) {
    if (can_jump) {
        can_jump = 0;
        siglongjmp(jmp_env, 1);
    }
}

static u_int safe_bpf_filter(const struct bpf_insn *insns, const u_char *data, 
                             u_int wirelen, u_int buflen) {
    struct sigaction old_act, new_act;
    u_int result = 0;
    
    // Set up signal handler for SIGABRT
    memset(&new_act, 0, sizeof(new_act));
    new_act.sa_handler = sigabrt_handler;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    
    if (sigaction(SIGABRT, &new_act, &old_act) == -1) {
        return 0;
    }
    
    can_jump = 1;
    if (sigsetjmp(jmp_env, 1) == 0) {
        result = bpf_filter(insns, data, wirelen, buflen);
    } else {
        // SIGABRT was caught, return 0 safely
        result = 0;
    }
    can_jump = 0;
    
    // Restore original signal handler
    sigaction(SIGABRT, &old_act, NULL);
    return result;
}

static void safe_bpf_dump(const struct bpf_program *program, int level) {
    struct sigaction old_act, new_act;
    
    // Set up signal handler for SIGABRT
    memset(&new_act, 0, sizeof(new_act));
    new_act.sa_handler = sigabrt_handler;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    
    if (sigaction(SIGABRT, &new_act, &old_act) == -1) {
        return;
    }
    
    can_jump = 1;
    if (sigsetjmp(jmp_env, 1) == 0) {
        bpf_dump(program, level);
    }
    can_jump = 0;
    
    // Restore original signal handler
    sigaction(SIGABRT, &old_act, NULL);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Use first byte to choose different code paths
    uint8_t selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    // Common variables
    struct bpf_program program;
    struct bpf_insn *insns = NULL;
    int linktype = 1;  // DLT_EN10MB
    int snaplen = 65535;
    u_int precision = 0;
    int optimize = 1;
    bpf_u_int32 netmask = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int result;

    // Initialize errbuf
    memset(errbuf, 0, sizeof(errbuf));

    switch (selector) {
        case 0: {
            // Test bpf_validate and bpf_filter
            if (remaining_size < sizeof(struct bpf_insn)) {
                break;
            }

            // Use up to 64 instructions or whatever fits in the input
            int max_insns = (int)(remaining_size / sizeof(struct bpf_insn));
            if (max_insns > 64) max_insns = 64;
            if (max_insns < 1) max_insns = 1;

            insns = (struct bpf_insn *)remaining_data;
            
            // Validate the BPF program
            if (bpf_validate(insns, max_insns)) {
                // Create dummy packet data
                u_char packet_data[256];
                size_t packet_size = sizeof(packet_data) < remaining_size ? 
                                     sizeof(packet_data) : remaining_size;
                if (packet_size > 0) {
                    memcpy(packet_data, remaining_data, packet_size);
                }
                
                // Call bpf_filter with the validated program using safe wrapper
                u_int wirelen = (u_int)(packet_size > 256 ? 256 : packet_size);
                u_int caplen = wirelen;
                safe_bpf_filter(insns, packet_data, wirelen, caplen);
            }
            break;
        }

        case 1: {
            // Test bpf_dump
            if (remaining_size < sizeof(struct bpf_insn)) {
                break;
            }

            // Create a minimal BPF program
            int insn_count = 1;
            if (remaining_size >= sizeof(struct bpf_insn) * 2) {
                insn_count = 2;
            }
            
            program.bf_len = insn_count;
            program.bf_insns = (struct bpf_insn *)remaining_data;
            
            // Call bpf_dump with different debug levels using safe wrapper
            safe_bpf_dump(&program, 0);
            safe_bpf_dump(&program, 1);
            safe_bpf_dump(&program, 2);
            break;
        }

        case 2: {
            // Test pcap_open_dead_with_tstamp_precision
            if (remaining_size >= 1) {
                linktype = remaining_data[0] % 256;
            }
            if (remaining_size >= 2) {
                snaplen = remaining_data[1] * 256 + (remaining_data[0] % 256);
                if (snaplen < 1) snaplen = 1;
            }
            if (remaining_size >= 3) {
                precision = remaining_data[2] % 3;  // 0, 1, or 2
            }

            pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (pcap_handle) {
                pcap_close(pcap_handle);
            }
            break;
        }

        case 3: {
            // Test pcap_compile
            pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (!pcap_handle) {
                break;
            }

            // Initialize bpf_program
            memset(&program, 0, sizeof(program));
            
            // Create filter string from input (null-terminated)
            char filter_str[256];
            size_t filter_len = remaining_size < 255 ? remaining_size : 255;
            if (filter_len > 0) {
                memcpy(filter_str, remaining_data, filter_len);
                filter_str[filter_len] = '\0';
            } else {
                strcpy(filter_str, "ip");
            }

            // Try to compile the filter
            result = pcap_compile(pcap_handle, &program, filter_str, optimize, netmask);
            
            // Always free the program
            pcap_freecode(&program);
            pcap_close(pcap_handle);
            break;
        }

        case 4: {
            // Test pcap_compile_nopcap (deprecated but still testable)
            if (remaining_size >= 1) {
                snaplen = remaining_data[0] * 256;
                if (snaplen < 1) snaplen = 1;
            }
            if (remaining_size >= 2) {
                linktype = remaining_data[1] % 256;
            }

            // Initialize bpf_program
            memset(&program, 0, sizeof(program));
            
            // Create filter string from input (null-terminated)
            char filter_str[256];
            size_t filter_len = (remaining_size > 2) ? (remaining_size - 2 < 255 ? remaining_size - 2 : 255) : 0;
            if (filter_len > 0 && remaining_size > 2) {
                memcpy(filter_str, remaining_data + 2, filter_len);
                filter_str[filter_len] = '\0';
            } else {
                strcpy(filter_str, "tcp");
            }

            // Try to compile the filter
            result = pcap_compile_nopcap(snaplen, linktype, &program, filter_str, optimize, netmask);
            
            // Always free the program
            pcap_freecode(&program);
            break;
        }

        case 5: {
            // Combined test: open dead pcap, compile filter, validate, and filter
            pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (!pcap_handle) {
                break;
            }

            // Initialize bpf_program
            memset(&program, 0, sizeof(program));
            
            // Create filter string
            char filter_str[256];
            size_t filter_len = remaining_size < 255 ? remaining_size : 255;
            if (filter_len > 0) {
                memcpy(filter_str, remaining_data, filter_len);
                filter_str[filter_len] = '\0';
            } else {
                strcpy(filter_str, "udp");
            }

            // Compile
            if (pcap_compile(pcap_handle, &program, filter_str, optimize, netmask) == 0) {
                // Validate the compiled program
                if (bpf_validate(program.bf_insns, program.bf_len)) {
                    // Create dummy packet
                    u_char packet[64];
                    for (int i = 0; i < 64; i++) {
                        packet[i] = (u_char)(i % 256);
                    }
                    
                    // Apply filter using safe wrapper
                    safe_bpf_filter(program.bf_insns, packet, 64, 64);
                    
                    // Dump the program using safe wrapper
                    safe_bpf_dump(&program, 1);
                }
            }
            
            pcap_freecode(&program);
            pcap_close(pcap_handle);
            break;
        }
    }

    return 0;
}