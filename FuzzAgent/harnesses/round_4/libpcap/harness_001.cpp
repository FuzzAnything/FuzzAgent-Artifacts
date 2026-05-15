#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>
#include <pcap/bpf.h>

namespace harness_001 {


// Note: bpf_dump and bpf_image are not in the public pcap.h header
// They are defined in bpf_dump.c and bpf_image.c internally
// We need to declare them ourselves for the harness
extern "C" {
    void bpf_dump(const struct bpf_program *p, int option);
    char *bpf_image(const struct bpf_insn *p, int n);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input into multiple parts for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: link type for pcap_compile_nopcap
    int linktype = fdp.ConsumeIntegral<int8_t>();
    if (linktype < 0) linktype = -linktype;  // Make positive
    
    // Third byte: snapshot length
    int snaplen = fdp.ConsumeIntegral<uint8_t>();
    if (snaplen == 0) snaplen = 65535;  // Default to maximum
    
    // Fourth byte: optimize flag (0 or 1)
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Fifth byte: netmask (for pcap_compile_nopcap)
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Sixth byte: bpf_dump option (0, 1, or 2)
    int dump_option = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume remaining bytes for filter string
    std::string filter_str = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic pcap_compile_nopcap -> bpf_dump -> pcap_freecode
            int result = pcap_compile_nopcap(snaplen, linktype, &bpf, 
                                            filter_str.c_str(), optimize, netmask);
            if (result == 0) {
                // Successful compilation, dump the BPF program
                bpf_dump(&bpf, dump_option);
                
                // Clean up
                pcap_freecode(&bpf);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: pcap_compile_nopcap with bpf_image testing
            int result = pcap_compile_nopcap(snaplen, linktype, &bpf,
                                            filter_str.c_str(), optimize, netmask);
            if (result == 0 && bpf.bf_len > 0 && bpf.bf_insns != nullptr) {
                // Test bpf_image for each instruction
                for (int i = 0; i < bpf.bf_len && i < 10; i++) {  // Limit to 10 instructions
                    char *image = bpf_image(&bpf.bf_insns[i], i);
                    if (image != nullptr) {
                        // Image is a static thread-local buffer, we can just ignore it
                        // or print it for debugging (commented out in fuzzing)
                        // fprintf(stderr, "Instruction %d: %s\n", i, image);
                    }
                }
                
                // Also test bpf_dump
                bpf_dump(&bpf, dump_option);
                
                pcap_freecode(&bpf);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Multiple filter compilations with different options
            // Split filter string into multiple filter attempts
            size_t filter_len = filter_str.length();
            if (filter_len > 10) {
                // Try first part of string
                std::string filter1 = filter_str.substr(0, filter_len / 2);
                int result1 = pcap_compile_nopcap(snaplen, linktype, &bpf,
                                                 filter1.c_str(), optimize, netmask);
                if (result1 == 0) {
                    bpf_dump(&bpf, dump_option);
                    pcap_freecode(&bpf);
                }
                
                // Try second part of string with different optimization
                memset(&bpf, 0, sizeof(bpf));
                std::string filter2 = filter_str.substr(filter_len / 2);
                int result2 = pcap_compile_nopcap(snaplen, DLT_EN10MB, &bpf,
                                                 filter2.c_str(), 1 - optimize, netmask);
                if (result2 == 0) {
                    bpf_dump(&bpf, (dump_option + 1) % 3);
                    pcap_freecode(&bpf);
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Edge cases and error paths
            // Try empty filter string
            if (!filter_str.empty()) {
                // Try with empty string
                int result1 = pcap_compile_nopcap(snaplen, linktype, &bpf,
                                                 "", optimize, netmask);
                if (result1 == 0) {
                    bpf_dump(&bpf, dump_option);
                    pcap_freecode(&bpf);
                }
                
                memset(&bpf, 0, sizeof(bpf));
                
                // Try with very long filter string
                std::string long_filter = filter_str + filter_str + filter_str;
                int result2 = pcap_compile_nopcap(snaplen, linktype, &bpf,
                                                 long_filter.c_str(), optimize, netmask);
                if (result2 == 0) {
                    bpf_dump(&bpf, (dump_option + 1) % 3);
                    
                    // Test bpf_image on some instructions if available
                    if (bpf.bf_len > 0 && bpf.bf_insns != nullptr) {
                        for (int i = 0; i < bpf.bf_len && i < 5; i++) {
                            char *image = bpf_image(&bpf.bf_insns[i], i);
                            // Image buffer is thread-local static, just consume
                        }
                    }
                    
                    pcap_freecode(&bpf);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_001
