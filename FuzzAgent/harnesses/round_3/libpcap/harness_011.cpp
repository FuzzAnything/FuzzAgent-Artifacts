#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>

namespace harness_011 {
// Forward declarations for helper functions
static void test_basic_filters(FuzzedDataProvider& fdp, pcap_t* pcap, 
                               int linktype, int snaplen, int optimize,
                               bpf_u_int32 netmask, time_t timestamp_sec,
                               suseconds_t timestamp_usec);
static void test_protocol_filters(FuzzedDataProvider& fdp, pcap_t* pcap,
                                  int linktype, int snaplen, int optimize,
                                  bpf_u_int32 netmask, time_t timestamp_sec,
                                  suseconds_t timestamp_usec);
static std::vector<uint8_t> generate_protocol_packet(FuzzedDataProvider& fdp, int protocol_type);
static void test_complex_filters(FuzzedDataProvider& fdp, pcap_t* pcap,
                                 int linktype, int snaplen, int optimize,
                                 bpf_u_int32 netmask, time_t timestamp_sec,
                                 suseconds_t timestamp_usec);
static void test_edge_cases(FuzzedDataProvider& fdp, pcap_t* pcap,
                            int linktype, int snaplen, int optimize,
                            bpf_u_int32 netmask, time_t timestamp_sec,
                            suseconds_t timestamp_usec);
static void test_mixed_packets(FuzzedDataProvider& fdp, pcap_t* pcap,
                               int linktype, int snaplen, int optimize,
                               bpf_u_int32 netmask, time_t timestamp_sec,
                               suseconds_t timestamp_usec);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful BPF filter testing
    // We need: linktype (int), snaplen (int), optimize (bool), netmask (uint32_t), 
    // filter string, packet data for testing, and test scenario selection
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT value
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);  // Snapshot length
    int optimize = fdp.ConsumeBool() ? 1 : 0;  // Optimization flag
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();  // Network mask
    
    // Consume timestamp values before packet data
    time_t timestamp_sec = 0;
    suseconds_t timestamp_usec = 0;
    
    // Try to consume timestamp values if enough data remains
    if (fdp.remaining_bytes() >= sizeof(time_t) + sizeof(suseconds_t)) {
        timestamp_sec = fdp.ConsumeIntegral<time_t>();
        timestamp_usec = fdp.ConsumeIntegral<suseconds_t>();
    }
    
    // Step 1: Create pcap handle using pcap_open_dead
    pcap_t* pcap = pcap_open_dead(linktype, snaplen);
    if (pcap == nullptr) {
        // Try alternative: pcap_open_dead_with_tstamp_precision
        pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, PCAP_TSTAMP_PRECISION_MICRO);
        if (pcap == nullptr) {
            return 0;  // Can't proceed without pcap handle
        }
    }
    
    // Different test scenarios based on consumed value
    if (test_scenario == 0) {
        // Scenario 0: Basic filter testing with various filter strings
        test_basic_filters(fdp, pcap, linktype, snaplen, optimize, netmask, 
                           timestamp_sec, timestamp_usec);
    } else if (test_scenario == 1) {
        // Scenario 1: Protocol-specific filters (TCP, UDP, ICMP)
        test_protocol_filters(fdp, pcap, linktype, snaplen, optimize, netmask,
                              timestamp_sec, timestamp_usec);
    } else if (test_scenario == 2) {
        // Scenario 2: Complex filter expressions with logical operators
        test_complex_filters(fdp, pcap, linktype, snaplen, optimize, netmask,
                             timestamp_sec, timestamp_usec);
    } else if (test_scenario == 3) {
        // Scenario 3: Edge cases - empty filters, malformed filters, etc.
        test_edge_cases(fdp, pcap, linktype, snaplen, optimize, netmask,
                        timestamp_sec, timestamp_usec);
    } else {
        // Scenario 4: Mixed packet types with various filters
        test_mixed_packets(fdp, pcap, linktype, snaplen, optimize, netmask,
                           timestamp_sec, timestamp_usec);
    }
    
    // Always clean up pcap handle
    pcap_close(pcap);
    
    return 0;
}

// Helper function for basic filter testing
static void test_basic_filters(FuzzedDataProvider& fdp, pcap_t* pcap, 
                               int linktype, int snaplen, int optimize,
                               bpf_u_int32 netmask, time_t timestamp_sec,
                               suseconds_t timestamp_usec) {
    // Consume filter string (variable size)
    size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_size);
    
    struct bpf_program bpf;
    int compile_result = pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Validate compiled filter
        int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
        (void)validation_result;
        
        // Prepare packet data
        std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (packet_data.size() > 0) {
            // Execute both bpf_filter and pcap_offline_filter
            u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(),
                                             packet_data.size(), packet_data.size());
            (void)filter_result;
            
            struct pcap_pkthdr pkthdr;
            pkthdr.ts.tv_sec = timestamp_sec;
            pkthdr.ts.tv_usec = timestamp_usec;
            pkthdr.caplen = packet_data.size();
            pkthdr.len = packet_data.size();
            
            int offline_result = pcap_offline_filter(&bpf, &pkthdr, packet_data.data());
            (void)offline_result;
        }
        
        pcap_freecode(&bpf);
    } else {
        bpf.bf_len = 0;
        bpf.bf_insns = nullptr;
    }
}

// Helper function for protocol-specific filters
static void test_protocol_filters(FuzzedDataProvider& fdp, pcap_t* pcap,
                                  int linktype, int snaplen, int optimize,
                                  bpf_u_int32 netmask, time_t timestamp_sec,
                                  suseconds_t timestamp_usec) {
    // Select protocol type
    int protocol_type = fdp.ConsumeIntegralInRange<int>(0, 2);
    std::string filter_str;
    
    switch (protocol_type) {
        case 0:
            filter_str = "tcp";  // TCP filter
            break;
        case 1:
            filter_str = "udp";  // UDP filter
            break;
        case 2:
            filter_str = "icmp"; // ICMP filter
            break;
        default:
            filter_str = "ip";   // Default IP filter
    }
    
    // Add port constraints for TCP/UDP
    if (protocol_type == 0 || protocol_type == 1) {
        if (fdp.remaining_bytes() >= 2) {
            uint16_t port = fdp.ConsumeIntegral<uint16_t>();
            filter_str += " port " + std::to_string(port);
        }
    }
    
    struct bpf_program bpf;
    int compile_result = pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Validate compiled filter
        int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
        (void)validation_result;
        
        // Generate appropriate packet data for the protocol
        std::vector<uint8_t> packet_data = generate_protocol_packet(fdp, protocol_type);
        
        if (packet_data.size() > 0) {
            // Execute both filter functions
            u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(),
                                             packet_data.size(), packet_data.size());
            (void)filter_result;
            
            struct pcap_pkthdr pkthdr;
            pkthdr.ts.tv_sec = timestamp_sec;
            pkthdr.ts.tv_usec = timestamp_usec;
            pkthdr.caplen = packet_data.size();
            pkthdr.len = packet_data.size();
            
            int offline_result = pcap_offline_filter(&bpf, &pkthdr, packet_data.data());
            (void)offline_result;
        }
        
        pcap_freecode(&bpf);
    } else {
        bpf.bf_len = 0;
        bpf.bf_insns = nullptr;
    }
}

// Helper function to generate protocol-specific packets
static std::vector<uint8_t> generate_protocol_packet(FuzzedDataProvider& fdp, int protocol_type) {
    std::vector<uint8_t> packet;
    
    // Basic Ethernet header (14 bytes)
    for (int i = 0; i < 14; i++) {
        if (fdp.remaining_bytes() > 0) {
            packet.push_back(fdp.ConsumeIntegral<uint8_t>());
        } else {
            packet.push_back(0);
        }
    }
    
    // Set Ethernet type to IPv4 (0x0800) at bytes 12-13
    if (packet.size() >= 14) {
        packet[12] = 0x08;
        packet[13] = 0x00;
    }
    
    // Basic IP header (20 bytes minimum)
    for (int i = 0; i < 20; i++) {
        if (fdp.remaining_bytes() > 0) {
            packet.push_back(fdp.ConsumeIntegral<uint8_t>());
        } else {
            packet.push_back(0);
        }
    }
    
    // Set IP protocol field (byte 9 of IP header, which is byte 23 overall)
    if (packet.size() >= 24) {
        switch (protocol_type) {
            case 0:  // TCP
                packet[23] = 6;  // TCP protocol number
                break;
            case 1:  // UDP
                packet[23] = 17; // UDP protocol number
                break;
            case 2:  // ICMP
                packet[23] = 1;  // ICMP protocol number
                break;
            default:
                packet[23] = 6;  // Default to TCP
        }
    }
    
    // Add protocol-specific data
    size_t extra_data = fdp.ConsumeIntegralInRange<size_t>(0, 100);
    for (size_t i = 0; i < extra_data && fdp.remaining_bytes() > 0; i++) {
        packet.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return packet;
}

// Helper function for complex filter expressions
static void test_complex_filters(FuzzedDataProvider& fdp, pcap_t* pcap,
                                 int linktype, int snaplen, int optimize,
                                 bpf_u_int32 netmask, time_t timestamp_sec,
                                 suseconds_t timestamp_usec) {
    // Generate complex filter with logical operators
    std::string filter_str;
    
    // Start with basic expression
    filter_str = "ip";
    
    // Add logical operators based on fuzzed data
    int num_conditions = fdp.ConsumeIntegralInRange<int>(0, 3);
    for (int i = 0; i < num_conditions; i++) {
        if (fdp.remaining_bytes() < 2) break;
        
        int condition_type = fdp.ConsumeIntegralInRange<int>(0, 2);
        switch (condition_type) {
            case 0:
                filter_str += " and tcp";
                break;
            case 1:
                filter_str += " or udp";
                break;
            case 2:
                if (fdp.remaining_bytes() >= 2) {
                    uint16_t port = fdp.ConsumeIntegral<uint16_t>();
                    filter_str += " and port " + std::to_string(port);
                }
                break;
        }
    }
    
    struct bpf_program bpf;
    int compile_result = pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Validate compiled filter
        int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
        (void)validation_result;
        
        // Prepare packet data
        std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (packet_data.size() > 0) {
            // Execute both filter functions
            u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(),
                                             packet_data.size(), packet_data.size());
            (void)filter_result;
            
            struct pcap_pkthdr pkthdr;
            pkthdr.ts.tv_sec = timestamp_sec;
            pkthdr.ts.tv_usec = timestamp_usec;
            pkthdr.caplen = packet_data.size();
            pkthdr.len = packet_data.size();
            
            int offline_result = pcap_offline_filter(&bpf, &pkthdr, packet_data.data());
            (void)offline_result;
        }
        
        pcap_freecode(&bpf);
    } else {
        bpf.bf_len = 0;
        bpf.bf_insns = nullptr;
    }
}

// Helper function for edge cases
static void test_edge_cases(FuzzedDataProvider& fdp, pcap_t* pcap,
                            int linktype, int snaplen, int optimize,
                            bpf_u_int32 netmask, time_t timestamp_sec,
                            suseconds_t timestamp_usec) {
    // Test various edge cases
    int edge_case = fdp.ConsumeIntegralInRange<int>(0, 3);
    std::string filter_str;
    
    switch (edge_case) {
        case 0:
            filter_str = "";  // Empty filter (accept all)
            break;
        case 1:
            filter_str = "host 255.255.255.255";  // Broadcast address
            break;
        case 2:
            filter_str = "port 0";  // Port 0 (reserved)
            break;
        case 3:
            // Malformed filter string from fuzzer input
            size_t malformed_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            filter_str = fdp.ConsumeBytesAsString(malformed_size);
            break;
    }
    
    struct bpf_program bpf;
    int compile_result = pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Validate compiled filter
        int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
        (void)validation_result;
        
        // Prepare packet data
        std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (packet_data.size() > 0) {
            // Execute both filter functions
            u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(),
                                             packet_data.size(), packet_data.size());
            (void)filter_result;
            
            struct pcap_pkthdr pkthdr;
            pkthdr.ts.tv_sec = timestamp_sec;
            pkthdr.ts.tv_usec = timestamp_usec;
            pkthdr.caplen = packet_data.size();
            pkthdr.len = packet_data.size();
            
            int offline_result = pcap_offline_filter(&bpf, &pkthdr, packet_data.data());
            (void)offline_result;
        }
        
        pcap_freecode(&bpf);
    } else {
        bpf.bf_len = 0;
        bpf.bf_insns = nullptr;
    }
}

// Helper function for mixed packet types
static void test_mixed_packets(FuzzedDataProvider& fdp, pcap_t* pcap,
                               int linktype, int snaplen, int optimize,
                               bpf_u_int32 netmask, time_t timestamp_sec,
                               suseconds_t timestamp_usec) {
    // Test multiple filters on same pcap handle
    int num_filters = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_filters && fdp.remaining_bytes() > 16; i++) {
        // Consume a filter string
        size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string filter_str = fdp.ConsumeBytesAsString(filter_size);
        
        struct bpf_program bpf;
        int compile_result = pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask);
        
        if (compile_result == 0) {
            // Validate compiled filter
            int validation_result = bpf_validate(bpf.bf_insns, bpf.bf_len);
            (void)validation_result;
            
            // Generate some packet data for this filter
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(14, 200);
            std::vector<uint8_t> packet_data;
            for (size_t j = 0; j < packet_size && fdp.remaining_bytes() > 0; j++) {
                packet_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            if (packet_data.size() > 0) {
                // Execute both filter functions
                u_int filter_result = bpf_filter(bpf.bf_insns, packet_data.data(),
                                                 packet_data.size(), packet_data.size());
                (void)filter_result;
                
                struct pcap_pkthdr pkthdr;
                pkthdr.ts.tv_sec = timestamp_sec;
                pkthdr.ts.tv_usec = timestamp_usec;
                pkthdr.caplen = packet_data.size();
                pkthdr.len = packet_data.size();
                
                int offline_result = pcap_offline_filter(&bpf, &pkthdr, packet_data.data());
                (void)offline_result;
            }
            
            pcap_freecode(&bpf);
        } else {
            bpf.bf_len = 0;
            bpf.bf_insns = nullptr;
        }
    }
}

} // namespace harness_011
