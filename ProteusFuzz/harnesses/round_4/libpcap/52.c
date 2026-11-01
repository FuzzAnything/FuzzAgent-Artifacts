// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf jump_env;
static volatile sig_atomic_t can_jump = 0;

static void signal_handler(int sig) {
    if (can_jump) {
        longjmp(jump_env, 1);
    }
}

static void setup_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        pcap_freecode(prog);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    setup_signal_handler();

    // Create a dead handle for pcap_compile
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!handle) {
        return 0;
    }

    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    struct bpf_program prog2;
    memset(&prog2, 0, sizeof(prog2));

    // Extract filter string from input (null-terminated or truncated)
    char filter[256];
    size_t filter_len = 0;
    
    // Find null terminator or limit to buffer size
    if (Size > 255) {
        filter_len = 255;
    } else {
        filter_len = Size;
    }
    
    // Ensure we don't read past the input buffer
    if (filter_len > Size) {
        filter_len = Size;
    }

    memcpy(filter, Data, filter_len);
    filter[filter_len] = '\0';

    // Calculate remaining data size for packet
    size_t remaining_data = Size - filter_len;
    
    // Try to compile the filter
    int compile_result = pcap_compile(handle, &prog, filter, 0, 0);
    if (compile_result == -1) {
        // Try pcap_compile_nopcap as alternative
        compile_result = pcap_compile_nopcap(DLT_EN10MB, 65535, &prog2, filter, 0, 0);
    }

    // If we have a valid program, test various functions
    if (compile_result == 0 && (prog.bf_insns || prog2.bf_insns)) {
        struct bpf_program *current_prog = (prog.bf_insns) ? &prog : &prog2;
        
        // Test bpf_validate
        bpf_validate(current_prog->bf_insns, current_prog->bf_len);

        // Test bpf_dump with different options
        bpf_dump(current_prog, 0);
        bpf_dump(current_prog, 1);
        bpf_dump(current_prog, 2);
        bpf_dump(current_prog, 3);

        // Prepare packet data for filtering
        struct pcap_pkthdr pkthdr;
        memset(&pkthdr, 0, sizeof(pkthdr));
        
        // Limit packet size to remaining data
        bpf_u_int32 pkt_len = (remaining_data > 1000) ? 1000 : remaining_data;
        pkthdr.caplen = pkt_len;
        pkthdr.len = pkt_len;
        
        // Create dummy packet data
        u_char *pkt_data = NULL;
        if (pkt_len > 0) {
            pkt_data = (u_char *)malloc(pkt_len);
            if (pkt_data) {
                // Only copy if we have enough data
                if (remaining_data >= pkt_len) {
                    memcpy(pkt_data, Data + filter_len, pkt_len);
                } else {
                    // Fill with zeros if not enough data
                    memset(pkt_data, 0, pkt_len);
                }
                
                // Test pcap_offline_filter
                pcap_offline_filter(current_prog, &pkthdr, pkt_data);

                // Test bpf_filter with signal handling
                can_jump = 1;
                if (setjmp(jump_env) == 0) {
                    bpf_filter(current_prog->bf_insns, pkt_data, pkthdr.caplen, pkthdr.caplen);
                }
                can_jump = 0;
            }
        }
        
        if (pkt_data) {
            free(pkt_data);
        }
    }

    // Cleanup
    cleanup_bpf_program(&prog);
    cleanup_bpf_program(&prog2);
    cleanup_pcap(handle);

    return 0;
}