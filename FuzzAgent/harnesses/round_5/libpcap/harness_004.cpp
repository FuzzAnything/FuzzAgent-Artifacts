#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>  // Includes bpf.h which defines bpf_u_int32
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_004 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful name resolution testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different name resolution workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume network name/string from fuzzer input
    std::string name = fdp.ConsumeRandomLengthString(256);
    
    // For port range testing, we need two port numbers
    int port1 = 0, port2 = 0, proto = 0;
    int port = 0;
    
    // For pcap_nametoaddr testing
    bpf_u_int32** addr_result = nullptr;
    
    // For pcap_nametonetaddr testing
    bpf_u_int32 net_addr = 0;
    
    // For pcap_ether_aton testing
    u_char* ether_addr = nullptr;
    
    // For pcap_nametoaddrinfo testing
    struct addrinfo* addrinfo_result = nullptr;
    
    // Consume remaining bytes for any additional data needs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    switch (operation) {
        case 0: {
            // Test pcap_nametoaddr - host name to internet address
            addr_result = pcap_nametoaddr(name.c_str());
            // Function returns pointer to array or 0 on failure
            // No cleanup needed as per documentation
            break;
        }
        
        case 1: {
            // Test pcap_nametoaddrinfo - more modern alternative to pcap_nametoaddr
            addrinfo_result = pcap_nametoaddrinfo(name.c_str());
            if (addrinfo_result != nullptr) {
                freeaddrinfo(addrinfo_result); // Must free the result
            }
            break;
        }
        
        case 2: {
            // Test pcap_nametonetaddr - network name to internet address
            net_addr = pcap_nametonetaddr(name.c_str());
            // Returns 0 on failure, network address on success
            break;
        }
        
        case 3: {
            // Test pcap_nametoport - port name to port number
            if (pcap_nametoport(name.c_str(), &port, &proto) != 0) {
                // Successfully resolved port name
                // port and proto are now populated
            }
            break;
        }
        
        case 4: {
            // Test pcap_nametoportrange - port range (e.g., "http-https")
            if (pcap_nametoportrange(name.c_str(), &port1, &port2, &proto) != 0) {
                // Successfully resolved port range
                // port1, port2, and proto are now populated
            }
            break;
        }
        
        case 5: {
            // Test pcap_nametoproto - protocol name to protocol number
            int proto_num = pcap_nametoproto(name.c_str());
            // Returns PROTO_UNDEF (0) on failure, protocol number on success
            break;
        }
        
        case 6: {
            // Test pcap_nametoeproto - Ethernet protocol name to protocol number
            int eproto_num = pcap_nametoeproto(name.c_str());
            // Returns PROTO_UNDEF (0) on failure, Ethernet protocol number on success
            break;
        }
        
        case 7: {
            // Test pcap_nametollc - LLC protocol name to protocol number
            int llc_proto = pcap_nametollc(name.c_str());
            // Returns PROTO_UNDEF (0) on failure, LLC protocol number on success
            break;
        }
        
        default: {
            // Test pcap_ether_aton - Ethernet address string to binary
            ether_addr = pcap_ether_aton(name.c_str());
            // Returns pointer to static buffer or NULL on failure
            // No cleanup needed as per documentation
            break;
        }
    }
    
    // Additional comprehensive testing combining multiple functions
    // Test pcap_nametoport if we have enough data
    if (remaining_data.size() > 10) {
        std::string port_name(reinterpret_cast<const char*>(remaining_data.data()), 
                              std::min(remaining_data.size(), (size_t)100));
        int test_port = 0, test_proto = 0;
        pcap_nametoport(port_name.c_str(), &test_port, &test_proto);
    }
    
    return 0;
}

} // namespace harness_004
