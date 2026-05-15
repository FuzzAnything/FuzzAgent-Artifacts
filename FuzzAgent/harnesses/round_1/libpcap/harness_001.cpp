#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/bpf.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

namespace harness_001 {
/*
 * libpcap fuzzing harness - harness_001.cpp
 * Targets BPF filter handling functions with complete lifecycle sequence
 * 
 * Primary APIs targeted:
 * - pcap_compile_nopcap (deprecated but still present, tests BPF compilation without pcap handle)
 * - bpf_dump (BPF filter program dump/display)
 * - bpf_image (BPF instruction to string conversion)
 * - pcap_compile (standard BPF compilation)
 * - pcap_setfilter (apply compiled filter)
 * - pcap_freecode (clean up BPF program)
 * 
 * Differentiates from harness_000 by focusing exclusively on BPF filter
 * manipulation APIs rather than packet capture operations
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume filter string (variable size, limited to reasonable length)
    std::string filter_str = fdp.ConsumeRandomLengthString(512);
    
    // Test pcap_compile_nopcap (deprecated API but still present)
    struct bpf_program bpf_prog_nopcap;
    int compile_nopcap_result = pcap_compile_nopcap(
        linktype, snaplen, &bpf_prog_nopcap, 
        filter_str.c_str(), optimize, netmask
    );
    
    if (compile_nopcap_result == 0) {
        // Test bpf_dump with different options
        int dump_option = fdp.ConsumeIntegralInRange<int>(0, 3);
        bpf_dump(&bpf_prog_nopcap, dump_option);
        
        // Test bpf_image for individual instructions
        if (bpf_prog_nopcap.bf_len > 0 && bpf_prog_nopcap.bf_insns != nullptr) {
            // Test a few instructions
            int num_insns_to_test = (bpf_prog_nopcap.bf_len < 5) ? bpf_prog_nopcap.bf_len : 5;
            for (int i = 0; i < num_insns_to_test; i++) {
                const char* insn_str = bpf_image(&bpf_prog_nopcap.bf_insns[i], i);
                (void)insn_str;  // Use result to avoid unused variable warning
            }
        }
        
        // Clean up
        pcap_freecode(&bpf_prog_nopcap);
    }
    
    // Test standard pcap_compile with pcap_open_dead
    pcap_t *pcap_handle = pcap_open_dead(linktype, snaplen);
    if (pcap_handle == nullptr) {
        return 0;
    }
    
    struct bpf_program bpf_prog;
    int compile_result = pcap_compile(pcap_handle, &bpf_prog, 
                                      filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Test bpf_dump again with the compiled program
        int dump_option2 = fdp.ConsumeIntegralInRange<int>(0, 3);
        bpf_dump(&bpf_prog, dump_option2);
        
        // Test pcap_setfilter
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_result;
        
        // Test bpf_image for all instructions
        if (bpf_prog.bf_len > 0 && bpf_prog.bf_insns != nullptr) {
            for (int i = 0; i < bpf_prog.bf_len; i++) {
                const char* insn_str = bpf_image(&bpf_prog.bf_insns[i], i);
                (void)insn_str;
            }
        }
        
        // Test pcap_offline_filter if we have packet data
        if (fdp.remaining_bytes() > 0) {
            size_t max_packet_size = fdp.remaining_bytes() < 4096 ? fdp.remaining_bytes() : 4096;
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, max_packet_size);
            std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
            
            if (packet_data.size() > 0) {
                struct pcap_pkthdr header;
                header.caplen = packet_data.size();
                header.len = packet_data.size();
                header.ts.tv_sec = 0;
                header.ts.tv_usec = 0;
                
                int filter_match = pcap_offline_filter(&bpf_prog, &header, packet_data.data());
                (void)filter_match;
            }
        }
        
        // Clean up
        pcap_freecode(&bpf_prog);
    }
    
    // Clean up pcap handle
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_001
