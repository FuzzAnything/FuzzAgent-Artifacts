#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least enough for basic parameters
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different BPF inspection workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume BPF compilation parameters
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume BPF filter string
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume bpf_dump option parameter
    int dump_option = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume remaining bytes for any additional data needs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    switch (operation) {
        case 0: {
            // Test pcap_compile_nopcap with bpf_dump and bpf_image
            struct bpf_program fp;
            
            // Use pcap_compile_nopcap to compile BPF filter without opening a pcap handle
            int compile_result = pcap_compile_nopcap(snaplen, linktype, &fp, 
                                                    filter_str.c_str(), optimize, netmask);
            
            if (compile_result == 0) {
                // Dump the compiled BPF program using bpf_dump
                bpf_dump(&fp, dump_option);
                
                // Test bpf_image for individual instructions
                if (fp.bf_len > 0) {
                    for (int i = 0; i < fp.bf_len; i++) {
                        const char* instruction_str = bpf_image(&fp.bf_insns[i], i);
                        if (instruction_str) {
                            // Just exercise the function - don't use output
                            (void)instruction_str;
                        }
                    }
                }
                
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 1: {
            // Test bpf_dump with different dump options on a compiled program
            struct bpf_program fp;
            
            if (pcap_compile_nopcap(snaplen, linktype, &fp, 
                                   filter_str.c_str(), optimize, netmask) == 0) {
                // Test all dump options
                for (int option = 0; option <= 3; option++) {
                    bpf_dump(&fp, option);
                }
                
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 2: {
            // Test bpf_image extensively on compiled BPF instructions
            struct bpf_program fp;
            
            if (pcap_compile_nopcap(snaplen, linktype, &fp, 
                                   filter_str.c_str(), optimize, netmask) == 0) {
                if (fp.bf_len > 0) {
                    // Test bpf_image on each instruction
                    for (int i = 0; i < fp.bf_len; i++) {
                        const char* img = bpf_image(&fp.bf_insns[i], i);
                        if (img) {
                            // Exercise the string length
                            size_t len = strlen(img);
                            (void)len;
                        }
                    }
                    
                    // Also test bpf_dump
                    bpf_dump(&fp, dump_option);
                }
                
                pcap_freecode(&fp);
            }
            break;
        }
        
        case 3: {
            // Test combination of pcap_compile, bpf_dump, and bpf_image with different filter inputs
            struct bpf_program fp;
            
            // Try with different optimization settings
            for (int opt = 0; opt <= 1; opt++) {
                if (pcap_compile_nopcap(snaplen, linktype, &fp, 
                                       filter_str.c_str(), opt, netmask) == 0) {
                    // Dump with current option
                    bpf_dump(&fp, dump_option);
                    
                    // Test bpf_image on first and last instructions
                    if (fp.bf_len > 0) {
                        const char* first = bpf_image(&fp.bf_insns[0], 0);
                        const char* last = bpf_image(&fp.bf_insns[fp.bf_len - 1], fp.bf_len - 1);
                        (void)first;
                        (void)last;
                    }
                    
                    pcap_freecode(&fp);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_001
