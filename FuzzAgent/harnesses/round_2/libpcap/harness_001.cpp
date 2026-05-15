#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least linktype (1 byte), snaplen (2 bytes), 
    // filter length (1 byte), netmask (4 bytes) = 8 bytes
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    // Link type: reasonable range 1-255 (0 is reserved)
    int linktype = fdp.ConsumeIntegralInRange<int>(1, 255);
    
    // Snaplen: reasonable range 64-65535
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);
    
    // Netmask for filter compilation
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume filter string length and filter string
    size_t filter_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_len);
    
    // Optimize flag: 0 or 1
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Remaining bytes are for packet data to test bpf_filter
    std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    struct bpf_program bpf;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Step 1: Compile BPF filter using pcap_compile_nopcap (standalone compilation)
    int compile_result = pcap_compile_nopcap(snaplen, linktype, &bpf, 
                                            filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Step 2: Validate the compiled BPF program
        int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
        
        // Step 3: Display the BPF program using bpf_dump
        // bpf_dump prints to stdout, but in fuzzing context this is fine
        bpf_dump(&bpf, 0);  // option 0: human-readable format
        bpf_dump(&bpf, 1);  // option 1: C array format  
        bpf_dump(&bpf, 2);  // option 2: raw numbers format
        
        // Step 4: Test bpf_image for each instruction
        if (bpf.bf_len > 0 && bpf.bf_insns != NULL) {
            for (int i = 0; i < bpf.bf_len; i++) {
                char* image = bpf_image(&bpf.bf_insns[i], i);
                // bpf_image returns a static buffer, no need to free
                if (image != NULL) {
                    // Use the image string (could be checked or ignored in fuzzing)
                }
            }
        }
        
        // Step 5: Test bpf_filter with packet data if we have enough
        if (packet_data.size() >= 14) {  // Minimum Ethernet header size
            // Need to provide proper parameters to bpf_filter:
            // const struct bpf_insn *pc, const u_char *p, u_int wirelen, u_int buflen
            u_int wirelen = packet_data.size();
            u_int buflen = packet_data.size();
            
            // Limit to reasonable sizes for performance
            if (wirelen > 65535) wirelen = 65535;
            if (buflen > 65535) buflen = 65535;
            
            u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(), 
                                            wirelen, buflen);
            // filter_result is the return value from the BPF program
        }
        
        // Step 6: Cleanup
        pcap_freecode(&bpf);
    } else {
        // Compilation failed - test error handling
        // pcap_compile_nopcap doesn't set errbuf, but we can test other error paths
    }
    
    // Test edge cases: NULL filter string
    if (filter_str.empty()) {
        struct bpf_program bpf2;
        // This should fail but we test the API call
        pcap_compile_nopcap(snaplen, linktype, &bpf2, NULL, optimize, netmask);
        // Don't free if compilation failed (bf_insns would be NULL)
    }
    
    // Test with minimal packet data
    if (packet_data.size() > 0) {
        // Create a minimal BPF program that accepts all packets
        struct bpf_insn insns[] = {
            BPF_STMT(BPF_RET | BPF_K, 0xFFFFFFFF),  // Return all packets
        };
        struct bpf_program minimal_prog = {
            .bf_len = 1,
            .bf_insns = insns
        };
        
        // Validate it
        bpf_validate(minimal_prog.bf_insns, minimal_prog.bf_len);
        
        // Test bpf_image on it
        char* image = bpf_image(minimal_prog.bf_insns, 0);
        
        // Test bpf_dump on it
        bpf_dump(&minimal_prog, 0);
        
        // Test bpf_filter with it
        if (packet_data.size() >= 14) {
            u_int wirelen = packet_data.size() > 65535 ? 65535 : packet_data.size();
            u_int buflen = wirelen;
            bpf_filter(minimal_prog.bf_insns, packet_data.data(), wirelen, buflen);
        }
    }
    
    return 0;
}

} // namespace harness_001
