#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <climits>

namespace harness_006 {
/*
 * libpcap fuzzing harness - harness_006.cpp
 * Targets network address translation subsystem APIs with complete invocation sequence
 * 
 * Primary APIs targeted (network address translation functions):
 * - pcap_nametoportrange (convert port range string to port numbers and protocol)
 * - pcap_nametoaddr (deprecated hostname to address translation)
 * - pcap_nametoaddrinfo (modern hostname to address info translation)
 * - pcap_nametoport (convert port name/number to port and protocol)
 * - pcap_nametoproto (convert protocol name to protocol number)
 * - pcap_nametoeproto (convert Ethernet protocol name to protocol number)
 * - pcap_nametollc (convert LLC protocol name to protocol number)
 * - pcap_nametonetaddr (convert network name to network address)
 * - pcap_ether_aton (convert MAC address string to binary)
 * - pcap_ether_hostton (convert hostname to MAC address via /etc/ethers)
 * - pcap_next_etherent (read next entry from /etc/ethers)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses pcap_open_dead for filtering)
 * - harness_001: BPF filter handling functions  
 * - harness_002: File I/O subsystem operations
 * - harness_003: Live device management lifecycle
 * - harness_004: Savefile-specific APIs with timestamp precision
 * - harness_005: Timestamp handling subsystem APIs
 * - harness_006: Network address translation subsystem (THIS HARNESS)
 * 
 * Specifically targets the 33+ undiscovered branches in nametoaddr.c functions
 * identified by CoverageAnalyzerAgent. Focuses on network address parsing and
 * conversion functions that were not covered by previous harnesses.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic string operations
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 10);  // Which test to run
    
    // Consume strings for name/address testing (variable size)
    std::string name_str = fdp.ConsumeRandomLengthString(256);
    std::string port_str = fdp.ConsumeRandomLengthString(128);
    std::string port_range_str = fdp.ConsumeRandomLengthString(128);
    std::string proto_str = fdp.ConsumeRandomLengthString(128);
    std::string mac_str = fdp.ConsumeRandomLengthString(64);
    std::string eproto_str = fdp.ConsumeRandomLengthString(128);
    std::string llc_str = fdp.ConsumeRandomLengthString(128);
    std::string net_str = fdp.ConsumeRandomLengthString(128);
    
    // Based on test_type, execute different API sequences
    switch (test_type % 11) {
        case 0: {
            // Test pcap_nametoport - convert port name/number to port and protocol
            int port = 0;
            int proto = 0;
            int result = pcap_nametoport(port_str.c_str(), &port, &proto);
            (void)result;  // Use result to avoid unused variable warning
            break;
        }
        
        case 1: {
            // Test pcap_nametoportrange - convert port range string
            int port1 = 0, port2 = 0, proto = 0;
            
            // Create a port range string by combining two port strings with dash
            // Use fuzzer data to determine if we create a valid range
            bool create_range = fdp.ConsumeBool();
            std::string range_str;
            
            if (create_range && fdp.remaining_bytes() > 10) {
                // Create a proper port range format "port1-port2"
                std::string part1 = fdp.ConsumeRandomLengthString(16);
                std::string part2 = fdp.ConsumeRandomLengthString(16);
                range_str = part1 + "-" + part2;
            } else {
                range_str = port_range_str;
            }
            
            int result = pcap_nametoportrange(range_str.c_str(), &port1, &port2, &proto);
            (void)result;
            break;
        }
        
        case 2: {
            // Test pcap_nametoaddr (deprecated) and pcap_nametoaddrinfo (modern)
            // First test the deprecated API
            bpf_u_int32 **addr_list = pcap_nametoaddr(name_str.c_str());
            (void)addr_list;
            
            // Then test the modern replacement
            struct addrinfo *addr_info = pcap_nametoaddrinfo(name_str.c_str());
            if (addr_info != nullptr) {
                // Free the allocated addrinfo structure
                freeaddrinfo(addr_info);
            }
            break;
        }
        
        case 3: {
            // Test pcap_nametoproto - convert protocol name to protocol number
            int proto_result = pcap_nametoproto(proto_str.c_str());
            (void)proto_result;
            break;
        }
        
        case 4: {
            // Test pcap_nametoeproto - convert Ethernet protocol name to protocol number
            int eproto_result = pcap_nametoeproto(eproto_str.c_str());
            (void)eproto_result;
            break;
        }
        
        case 5: {
            // Test pcap_nametollc - convert LLC protocol name to protocol number
            int llc_result = pcap_nametollc(llc_str.c_str());
            (void)llc_result;
            break;
        }
        
        case 6: {
            // Test pcap_nametonetaddr - convert network name to network address
            bpf_u_int32 net_addr = pcap_nametonetaddr(net_str.c_str());
            (void)net_addr;
            break;
        }
        
        case 7: {
            // Test pcap_ether_aton - convert MAC address string to binary
            u_char *mac_addr = pcap_ether_aton(mac_str.c_str());
            if (mac_addr != nullptr) {
                free(mac_addr);  // Must free allocated memory
            }
            break;
        }
        
        case 8: {
            // Test pcap_ether_hostton - convert hostname to MAC address
            // Note: This requires /etc/ethers to exist, but API should handle missing file
            u_char *mac_from_host = pcap_ether_hostton(name_str.c_str());
            if (mac_from_host != nullptr) {
                free(mac_from_host);  // Must free allocated memory
            }
            break;
        }
        
        case 9: {
            // Test pcap_next_etherent - read from /etc/ethers
            // Try to open /etc/ethers if it exists
            FILE *ethers_fp = fopen(PCAP_ETHERS_FILE, "r");
            if (ethers_fp != nullptr) {
                // Read a few entries to test the API
                for (int i = 0; i < 3; i++) {
                    struct pcap_etherent *etherent = pcap_next_etherent(ethers_fp);
                    if (etherent == nullptr) {
                        break;  // End of file
                    }
                    // etherent contains the MAC address and hostname
                    (void)etherent;
                }
                fclose(ethers_fp);
            }
            break;
        }
        
        case 10: {
            // Comprehensive test: try multiple APIs in sequence
            // Test port conversion
            int port = 0, proto = 0;
            (void)pcap_nametoport(port_str.c_str(), &port, &proto);
            
            // Test protocol conversion
            (void)pcap_nametoproto(proto_str.c_str());
            
            // Test MAC address conversion if we have enough data
            if (fdp.remaining_bytes() > 10) {
                std::string test_mac = fdp.ConsumeRandomLengthString(32);
                u_char *mac = pcap_ether_aton(test_mac.c_str());
                if (mac != nullptr) {
                    free(mac);
                }
            }
            
            // Test hostname resolution
            struct addrinfo *info = pcap_nametoaddrinfo(name_str.c_str());
            if (info != nullptr) {
                freeaddrinfo(info);
            }
            break;
        }
    }
    
    // Additional comprehensive testing with remaining data
    if (fdp.remaining_bytes() > 20) {
        // Test port range with remaining data
        std::string remaining_str = fdp.ConsumeRemainingBytesAsString();
        
        // Try to extract potential port range patterns
        size_t dash_pos = remaining_str.find('-');
        if (dash_pos != std::string::npos && dash_pos > 0 && dash_pos < remaining_str.length() - 1) {
            int port1 = 0, port2 = 0, proto = 0;
            (void)pcap_nametoportrange(remaining_str.c_str(), &port1, &port2, &proto);
        }
        
        // Try as a protocol name
        (void)pcap_nametoproto(remaining_str.c_str());
        
        // Try as an Ethernet protocol name
        (void)pcap_nametoeproto(remaining_str.c_str());
        
        // Try as an LLC protocol name
        (void)pcap_nametollc(remaining_str.c_str());
    }
    
    return 0;
}

} // namespace harness_006
