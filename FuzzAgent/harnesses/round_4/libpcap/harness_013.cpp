#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>
#include <pcap/bpf.h>

namespace harness_013 {


// Helper function to create a minimal pcap packet header
static void create_packet_header(struct pcap_pkthdr *hdr, size_t len, size_t caplen) {
    hdr->ts.tv_sec = 0;
    hdr->ts.tv_usec = 0;
    hdr->len = len;
    hdr->caplen = caplen;
}

// Helper to create simple packet data
static std::vector<uint8_t> create_packet_data(size_t len) {
    std::vector<uint8_t> data(len);
    for (size_t i = 0; i < len; i++) {
        data[i] = i % 256;
    }
    return data;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum input size needed:
    // - Scenario selection (1 byte)
    // - Link type (1 byte)  
    // - Snaplen (2 bytes)
    // - Optimize flag (1 byte)
    // - Netmask (4 bytes)
    // - Filter string (at least 1 byte)
    // - Packet data (at least 1 byte)
    if (Size < 32) {
        return 0;  // Not enough input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: link type for pcap_open_dead
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third and fourth bytes: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Fifth byte: optimize flag
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Sixth to ninth bytes: netmask for filter compilation
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Tenth byte: filter string length selector
    uint8_t filter_len_sel = fdp.ConsumeIntegral<uint8_t>();
    size_t max_filter_len = (filter_len_sel % 10) + 1;  // 1-10 bytes
    
    // Consume filter string
    std::string filter_str = fdp.ConsumeRandomLengthString(max_filter_len);
    
    // Consume remaining bytes for packet data (will be used for packet testing)
    std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    // Create a pcap handle for filter compilation
    // Using pcap_open_dead as it doesn't require actual devices
    pcap_handle = pcap_open_dead(linktype, snaplen);
    if (pcap_handle == nullptr) {
        // If we can't create a pcap handle, we can't proceed with filter compilation
        return 0;
    }
    
    // Try to compile the filter
    int compile_result = pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask);
    
    if (compile_result != 0) {
        // Filter compilation failed, clean up and exit
        pcap_close(pcap_handle);
        return 0;
    }
    
    // Now we have a compiled BPF program, test the target functions
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_offline_filter with various packet data
            // Create packet headers with different lengths
            struct pcap_pkthdr hdr;
            
            // Test with packet data of various sizes
            size_t packet_size = packet_data.size();
            if (packet_size == 0) {
                packet_size = 64;  // Default size if no packet data
            }
            
            // Limit packet size to prevent excessive memory usage
            if (packet_size > 4096) {
                packet_size = 4096;
            }
            
            // Create packet data if we don't have enough
            std::vector<uint8_t> actual_packet_data;
            if (packet_data.size() >= packet_size) {
                actual_packet_data.assign(packet_data.begin(), packet_data.begin() + packet_size);
            } else {
                actual_packet_data = create_packet_data(packet_size);
            }
            
            // Create headers with different capture lengths
            // Test 1: Full packet capture
            create_packet_header(&hdr, packet_size, packet_size);
            int result1 = pcap_offline_filter(&bpf, &hdr, actual_packet_data.data());
            
            // Test 2: Partial packet capture (common in real pcap files)
            if (packet_size > 1) {
                size_t caplen = packet_size / 2;
                create_packet_header(&hdr, packet_size, caplen);
                int result2 = pcap_offline_filter(&bpf, &hdr, actual_packet_data.data());
            }
            
            // Test 3: Zero-length packet (edge case)
            create_packet_header(&hdr, 0, 0);
            int result3 = pcap_offline_filter(&bpf, &hdr, nullptr);
            break;
        }
        
        case 1: {
            // Scenario 1: Test bpf_filter directly (lower-level API)
            // This is the function that pcap_offline_filter calls internally
            
            size_t packet_size = packet_data.size();
            if (packet_size == 0) {
                packet_size = 64;
            }
            
            if (packet_size > 4096) {
                packet_size = 4096;
            }
            
            std::vector<uint8_t> actual_packet_data;
            if (packet_data.size() >= packet_size) {
                actual_packet_data.assign(packet_data.begin(), packet_data.begin() + packet_size);
            } else {
                actual_packet_data = create_packet_data(packet_size);
            }
            
            // Test bpf_filter with various wirelen/buflen combinations
            if (bpf.bf_insns != nullptr) {
                // Test 1: Wire length equals buffer length (full packet)
                u_int result1 = bpf_filter(bpf.bf_insns, actual_packet_data.data(), 
                                         packet_size, packet_size);
                
                // Test 2: Buffer length smaller than wire length (truncated packet)
                if (packet_size > 1) {
                    u_int buflen = packet_size / 2;
                    u_int result2 = bpf_filter(bpf.bf_insns, actual_packet_data.data(),
                                             packet_size, buflen);
                }
                
                // Test 3: Zero-length packet
                u_int result3 = bpf_filter(bpf.bf_insns, nullptr, 0, 0);
                
                // Test 4: NULL BPF instructions (should accept all packets)
                u_int result4 = bpf_filter(nullptr, actual_packet_data.data(),
                                         packet_size, packet_size);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test both functions with multiple packet variations
            
            // Create multiple packet sizes from the input data
            size_t remaining_data = packet_data.size();
            size_t offset = 0;
            size_t test_count = 0;
            
            while (remaining_data > 0 && test_count < 5) {
                size_t packet_size = (remaining_data > 256) ? 256 : remaining_data;
                
                struct pcap_pkthdr hdr;
                create_packet_header(&hdr, packet_size, packet_size);
                
                // Test pcap_offline_filter
                int offline_result = pcap_offline_filter(&bpf, &hdr, 
                                                       packet_data.data() + offset);
                
                // Test bpf_filter if we have instructions
                if (bpf.bf_insns != nullptr) {
                    u_int bpf_result = bpf_filter(bpf.bf_insns, 
                                                packet_data.data() + offset,
                                                packet_size, packet_size);
                }
                
                offset += packet_size;
                remaining_data -= packet_size;
                test_count++;
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Edge cases and error conditions
            
            // Test with empty packet but valid header
            struct pcap_pkthdr hdr;
            create_packet_header(&hdr, 0, 0);
            
            // Test 1: NULL packet pointer (should be handled gracefully)
            int result1 = pcap_offline_filter(&bpf, &hdr, nullptr);
            
            // Test 2: Valid packet pointer but zero length
            uint8_t zero_packet[1] = {0};
            int result2 = pcap_offline_filter(&bpf, &hdr, zero_packet);
            
            // Test 3: Very small packet (1 byte)
            if (packet_data.size() >= 1) {
                create_packet_header(&hdr, 1, 1);
                int result3 = pcap_offline_filter(&bpf, &hdr, packet_data.data());
                
                // Also test bpf_filter
                if (bpf.bf_insns != nullptr) {
                    u_int bpf_result = bpf_filter(bpf.bf_insns, packet_data.data(), 1, 1);
                }
            }
            
            // Test 4: Mismatched header lengths (caplen > len should be impossible but test anyway)
            if (packet_data.size() >= 10) {
                create_packet_header(&hdr, 5, 10);  // caplen > len (invalid but test edge case)
                int result4 = pcap_offline_filter(&bpf, &hdr, packet_data.data());
            }
            
            break;
        }
    }
    
    // Clean up
    pcap_freecode(&bpf);
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_013
