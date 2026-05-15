// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/bpf.h"
#include "pcap/pcap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(struct bpf_insn)) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    struct bpf_program bpf_prog;
    int ret;
    
    // 1. First pcap_geterr call (on NULL handle to trigger error path)
    pcap_geterr(NULL);
    
    // 2. bpf_validate - validate BPF instructions from fuzzer input
    size_t num_insns = Size / sizeof(struct bpf_insn);
    if (num_insns > 0) {
        const struct bpf_insn *insns = (const struct bpf_insn *)Data;
        bpf_validate(insns, (int)num_insns);
    }
    
    // 3. bpf_dump - dump BPF program (use dummy program)
    bpf_prog.bf_len = 0;
    bpf_prog.bf_insns = NULL;
    if (num_insns > 0 && num_insns <= 1024) {
        bpf_prog.bf_len = (u_int)num_insns;
        bpf_prog.bf_insns = (struct bpf_insn *)Data;
        bpf_dump(&bpf_prog, 2);  // Level 2 for verbose output
    }
    
    // Create a pcap handle for offline reading
    write_dummy_file(Data, Size);
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap_handle) {
        // 4. pcap_next_ex - try to read packets
        struct pcap_pkthdr *pkt_header = NULL;
        const u_char *pkt_data = NULL;
        
        // Try reading multiple packets if available
        for (int i = 0; i < 10; i++) {
            ret = pcap_next_ex(pcap_handle, &pkt_header, &pkt_data);
            if (ret <= 0) {
                break;
            }
        }
        
        // 5. Second pcap_geterr call (on actual handle)
        pcap_geterr(pcap_handle);
        
        // 6. pcap_offline_filter - test filter matching
        // First compile a simple filter
        struct bpf_program filter_prog;
        const char *filter_str = "ip";
        
        if (pcap_compile(pcap_handle, &filter_prog, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
            // Set filter to validate the program
            if (pcap_setfilter(pcap_handle, &filter_prog) == 0) {
                // Create a dummy packet for testing
                struct pcap_pkthdr dummy_hdr;
                const u_char dummy_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x08, 0x00, 0x45, 0x00};
                
                dummy_hdr.ts.tv_sec = 0;
                dummy_hdr.ts.tv_usec = 0;
                dummy_hdr.caplen = sizeof(dummy_data);
                dummy_hdr.len = sizeof(dummy_data);
                
                // Use fuzzer data as packet if we have enough
                const u_char *test_data = dummy_data;
                size_t test_len = sizeof(dummy_data);
                
                if (Size >= 16) {
                    test_data = Data;
                    test_len = Size > 65535 ? 65535 : Size;
                    dummy_hdr.caplen = (bpf_u_int32)test_len;
                    dummy_hdr.len = (bpf_u_int32)test_len;
                }
                
                pcap_offline_filter(&filter_prog, &dummy_hdr, test_data);
            }
            pcap_freecode(&filter_prog);
        }
        
        pcap_close(pcap_handle);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    // Also test with pcap_open_dead for additional coverage
    pcap_t *dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (dead_handle) {
        pcap_geterr(dead_handle);
        
        // Try to compile a filter with fuzzer data as filter string
        if (Size > 0 && Size < 256) {
            struct bpf_program dead_filter;
            char filter_buf[256];
            memcpy(filter_buf, Data, Size > 255 ? 255 : Size);
            filter_buf[Size > 255 ? 255 : Size] = '\0';
            
            pcap_compile(dead_handle, &dead_filter, filter_buf, 1, PCAP_NETMASK_UNKNOWN);
            pcap_freecode(&dead_filter);
        }
        
        pcap_close(dead_handle);
    }
    
    return 0;
}