#include <fuzzer/FuzzedDataProvider.h>
#include <pcap.h>
#include <pcap/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace harness_017 {
/*
 * Harness for testing libpcap filter compilation and BPF inspection
 * Targets: pcap_compile, pcap_compile_nopcap, bpf_image, bpf_dump
 * Coverage goal: 100+ points of undiscovered complexity in filter compilation subsystem
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need at least some bytes for basic operation
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume operation type: 0 = pcap_compile with handle, 1 = pcap_compile_nopcap
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume DLT (Data Link Type) - limited to reasonable range
    int dlt = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Consume optimization flag (0 or 1)
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume netmask (0 for PCAP_NETMASK_UNKNOWN or valid netmask)
    bpf_u_int32 netmask;
    if (fdp.ConsumeBool()) {
        netmask = PCAP_NETMASK_UNKNOWN;
    } else {
        netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    }
    
    // Consume snaplen (snapshot length)
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 262144); // MAXIMUM_SNAPLEN
    
    // Consume filter expression string
    std::string filter_expr = fdp.ConsumeRandomLengthString(512);
    
    // Structure to hold compiled BPF program
    struct bpf_program fcode;
    memset(&fcode, 0, sizeof(fcode));

    if (operation == 0) {
        // Test sequence 1: pcap_compile with pcap_open_dead handle
        
        // Create a pcap handle for filter compilation
        pcap_t *pd = pcap_open_dead(dlt, snaplen);
        if (pd == NULL) {
            return 0;
        }

        char errbuf[PCAP_ERRBUF_SIZE];
        
        // Try compiling the filter expression
        int compile_result = pcap_compile(pd, &fcode, filter_expr.c_str(), optimize, netmask);
        
        if (compile_result == 0) {
            // Compilation successful - test BPF inspection functions
            
            // Test bpf_image to convert BPF program to human-readable string
            if (fcode.bf_len > 0 && fcode.bf_insns != NULL) {
                for (int i = 0; i < fcode.bf_len && i < 100; i++) { // Limit to 100 instructions
                    const char *image = bpf_image(&fcode.bf_insns[i], 0);
                    if (image != NULL) {
                        // Just access the image string to exercise the code
                        volatile char c = image[0];
                        (void)c;
                    }
                }
            }
            
            // Test bpf_dump to dump BPF program structure
            // Use different debug levels based on fuzzed input
            int debug_level = fdp.ConsumeIntegralInRange<int>(0, 3);
            bpf_dump(&fcode, debug_level);
            
            // Also test pcap_setfilter (if compilation succeeded)
            if (pcap_setfilter(pd, &fcode) == -1) {
                // Error is expected for some filters, continue
            }
        }
        
        // Cleanup
        pcap_freecode(&fcode);
        pcap_close(pd);
        
    } else {
        // Test sequence 2: pcap_compile_nopcap without handle
        
        char errbuf[PCAP_ERRBUF_SIZE];
        
        // Try compiling the filter expression directly
        int compile_result = pcap_compile_nopcap(dlt, snaplen, &fcode, 
                                                  filter_expr.c_str(), optimize, netmask);
        
        if (compile_result == 0) {
            // Compilation successful - test BPF inspection functions
            
            // Test bpf_image to convert BPF program to human-readable string
            if (fcode.bf_len > 0 && fcode.bf_insns != NULL) {
                for (int i = 0; i < fcode.bf_len && i < 100; i++) { // Limit to 100 instructions
                    const char *image = bpf_image(&fcode.bf_insns[i], 0);
                    if (image != NULL) {
                        // Just access the image string to exercise the code
                        volatile char c = image[0];
                        (void)c;
                    }
                }
            }
            
            // Test bpf_dump to dump BPF program structure
            // Use different debug levels based on fuzzed input
            int debug_level = fdp.ConsumeIntegralInRange<int>(0, 3);
            bpf_dump(&fcode, debug_level);
            
            // Test bpf_validate if we have instructions
            if (fcode.bf_len > 0 && fcode.bf_insns != NULL) {
                int valid = bpf_validate(fcode.bf_insns, fcode.bf_len);
                (void)valid; // Use result to avoid compiler warning
            }
        }
        
        // Cleanup
        pcap_freecode(&fcode);
    }

    return 0;
}

} // namespace harness_017
