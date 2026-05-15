#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>

namespace harness_002 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT value
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    int dump_option = fdp.ConsumeIntegralInRange<int>(0, 5);  // Test various dump options
    
    // Consume filter string (variable size)
    size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_size);
    
    // Create BPF program using pcap_compile_nopcap (no pcap handle needed)
    struct bpf_program bpf;
    
    // Try to compile the filter - pcap_compile_nopcap returns 0 on success
    int compile_result = pcap_compile_nopcap(snaplen, linktype, &bpf, 
                                             filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // BPF program compiled successfully
        
        // Test 1: bpf_dump with different format options
        // Option 0: Human-readable output (calls bpf_image)
        // Option 1: C-style array initializer format
        // Option 2: Raw instruction format
        // Option >2: Raw instruction format with count
        
        // Test bpf_dump with the chosen option
        // Note: bpf_dump prints to stdout, but we still want to exercise the code
        bpf_dump(&bpf, dump_option);
        
        // Test 2: bpf_image on individual instructions
        if (bpf.bf_len > 0 && bpf.bf_insns != NULL) {
            // Test bpf_image on a few instructions (up to 5 or available)
            int num_to_test = (bpf.bf_len < 5) ? bpf.bf_len : 5;
            for (int i = 0; i < num_to_test; i++) {
                const char* instruction_str = bpf_image(&bpf.bf_insns[i], i);
                // bpf_image returns a static/thread-local buffer, we just need to call it
                (void)instruction_str;  // Use result to avoid unused variable warning
            }
            
            // Also test with negative and out-of-bounds indices
            // Test with index -1 (should handle gracefully)
            const char* neg_str = bpf_image(&bpf.bf_insns[0], -1);
            (void)neg_str;
            
            // Test with index beyond array (should handle gracefully)
            if (bpf.bf_len > 0) {
                const char* beyond_str = bpf_image(&bpf.bf_insns[0], bpf.bf_len);
                (void)beyond_str;
            }
        }
        
        // Test 3: Test bpf_dump with different option values
        // Test with option = 0 (default human-readable)
        bpf_dump(&bpf, 0);
        
        // Test with option = 1 (C-style array)
        bpf_dump(&bpf, 1);
        
        // Test with option = 2 (raw instruction format)
        bpf_dump(&bpf, 2);
        
        // Test with option = 3 (raw with count)
        bpf_dump(&bpf, 3);
        
        // Test with option = 4 (also raw with count, same as >2)
        bpf_dump(&bpf, 4);
        
        // Clean up BPF program
        pcap_freecode(&bpf);
        
    } else {
        // Compilation failed - this is still valid testing of error paths
        // Ensure bpf is in a clean state
        bpf.bf_len = 0;
        bpf.bf_insns = NULL;
    }
    
    // Test 4: Try another compilation with remaining input
    if (fdp.remaining_bytes() > 16) {
        // Consume more parameters for second test
        int snaplen2 = fdp.ConsumeIntegralInRange<int>(68, 65535);
        int linktype2 = fdp.ConsumeIntegralInRange<int>(0, 255);
        int optimize2 = fdp.ConsumeBool() ? 1 : 0;
        bpf_u_int32 netmask2 = fdp.ConsumeIntegral<bpf_u_int32>();
        
        // Consume another filter string
        size_t filter_size2 = fdp.ConsumeIntegralInRange<size_t>(1, 
                                    (fdp.remaining_bytes() < 256) ? fdp.remaining_bytes() : 256);
        std::string filter_str2 = fdp.ConsumeBytesAsString(filter_size2);
        
        struct bpf_program bpf2;
        int compile_result2 = pcap_compile_nopcap(snaplen2, linktype2, &bpf2,
                                                 filter_str2.c_str(), optimize2, netmask2);
        
        if (compile_result2 == 0) {
            // Test different dump options
            int dump_option2 = fdp.ConsumeIntegralInRange<int>(0, 3);
            bpf_dump(&bpf2, dump_option2);
            
            // Test bpf_image on this program too
            if (bpf2.bf_len > 0 && bpf2.bf_insns != NULL) {
                // Test first and last instruction
                const char* first_str = bpf_image(&bpf2.bf_insns[0], 0);
                (void)first_str;
                
                if (bpf2.bf_len > 1) {
                    const char* last_str = bpf_image(&bpf2.bf_insns[bpf2.bf_len - 1], 
                                                     bpf2.bf_len - 1);
                    (void)last_str;
                }
            }
            
            pcap_freecode(&bpf2);
        }
    }
    
    // Test 5: Edge cases with empty/null filter string
    if (fdp.remaining_bytes() > 8) {
        struct bpf_program bpf3;
        // Try with empty filter string
        int result_empty = pcap_compile_nopcap(1500, 1, &bpf3, "", 1, 0xFFFFFF00);
        if (result_empty == 0) {
            bpf_dump(&bpf3, 0);
            pcap_freecode(&bpf3);
        }
        
        // Try with NULL filter string (if allowed by API)
        struct bpf_program bpf4;
        int result_null = pcap_compile_nopcap(1500, 1, &bpf4, NULL, 1, 0xFFFFFF00);
        if (result_null == 0) {
            bpf_dump(&bpf4, 1);
            pcap_freecode(&bpf4);
        }
    }
    
    return 0;
}

} // namespace harness_002
