#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test to run (0-7 for individual functions, 8 for combined test)
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 8);
    
    // Consume test string(s) - use multiple strings for different functions
    size_t str1_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string test_str1 = fdp.ConsumeBytesAsString(str1_size);
    
    size_t str2_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string test_str2 = fdp.ConsumeBytesAsString(str2_size);
    
    size_t str3_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string test_str3 = fdp.ConsumeBytesAsString(str3_size);
    
    // Variables for function results
    int port1 = 0, port2 = 0, proto = 0;
    int port = 0;
    u_char* ether_result = nullptr;
    bpf_u_int32** addr_result = nullptr;
    struct addrinfo* addrinfo_result = nullptr;
    bpf_u_int32 netaddr_result = 0;
    int proto_result = 0;
    int eproto_result = 0;
    int llc_result = 0;
    
    switch (test_type) {
        case 0:
            // Test 1: pcap_nametoportrange - expects format like "http-https" or "20-80"
            if (fdp.remaining_bytes() > 10) {
                size_t range_str_size = fdp.ConsumeIntegralInRange<size_t>(3, 128);
                std::string range_str = fdp.ConsumeBytesAsString(range_str_size);
                pcap_nametoportrange(range_str.c_str(), &port1, &port2, &proto);
                
                // Try pcap_strerror for error handling demonstration
                const char* err_str = pcap_strerror(-1);  // Invalid error code
                (void)err_str;
            }
            break;
            
        case 1:
            // Test 2: pcap_nametoport
            pcap_nametoport(test_str1.c_str(), &port, &proto);
            break;
            
        case 2:
            // Test 3: pcap_ether_aton - expects MAC address like "00:11:22:33:44:55"
            if (fdp.remaining_bytes() > 10) {
                size_t mac_str_size = fdp.ConsumeIntegralInRange<size_t>(12, 128);
                std::string mac_str = fdp.ConsumeBytesAsString(mac_str_size);
                ether_result = pcap_ether_aton(mac_str.c_str());
                if (ether_result != nullptr) {
                    free(ether_result);
                    ether_result = nullptr;
                }
            }
            break;
            
        case 3:
            // Test 4: pcap_nametoaddr (deprecated but still testable)
            // NOTE: pcap_nametoaddr returns system-managed memory (from gethostbyname)
            // that should NOT be freed by the caller. Consider using pcap_nametoaddrinfo()
            // as a modern replacement with proper memory management.
            addr_result = pcap_nametoaddr(test_str1.c_str());
            // Do NOT free the result - it's managed by the system's name resolution functions
            break;
            
        case 4:
            // Test 5: pcap_nametoaddrinfo (modern replacement)
            addrinfo_result = pcap_nametoaddrinfo(test_str1.c_str());
            if (addrinfo_result != nullptr) {
                freeaddrinfo(addrinfo_result);
            }
            break;
            
        case 5:
            // Test 6: pcap_ether_hostton - expects hostname from /etc/ethers
            ether_result = pcap_ether_hostton(test_str1.c_str());
            if (ether_result != nullptr) {
                free(ether_result);
                ether_result = nullptr;
            }
            break;
            
        case 6:
            // Test 7: pcap_nametollc
            llc_result = pcap_nametollc(test_str1.c_str());
            break;
            
        case 7:
            // Test 8: pcap_nametoeproto
            eproto_result = pcap_nametoeproto(test_str1.c_str());
            break;
            
        case 8:
            // Test 9: pcap_nametoproto
            proto_result = pcap_nametoproto(test_str1.c_str());
            break;
            
        default:
            // Combined test: call multiple functions
            // Test port-related functions
            pcap_nametoport(test_str1.c_str(), &port, &proto);
            
            // Test port range
            if (fdp.remaining_bytes() > 20) {
                size_t range_str_size = fdp.ConsumeIntegralInRange<size_t>(3, 64);
                std::string range_str = fdp.ConsumeBytesAsString(range_str_size);
                pcap_nametoportrange(range_str.c_str(), &port1, &port2, &proto);
            }
            
            // Test protocol functions
            proto_result = pcap_nametoproto(test_str2.c_str());
            eproto_result = pcap_nametoeproto(test_str3.c_str());
            llc_result = pcap_nametollc(test_str2.c_str());
            
            // Test address functions
            netaddr_result = pcap_nametonetaddr(test_str1.c_str());
            
            // Test ethernet functions
            if (fdp.remaining_bytes() > 15) {
                size_t mac_str_size = fdp.ConsumeIntegralInRange<size_t>(12, 64);
                std::string mac_str = fdp.ConsumeBytesAsString(mac_str_size);
                ether_result = pcap_ether_aton(mac_str.c_str());
                if (ether_result != nullptr) {
                    free(ether_result);
                    ether_result = nullptr;
                }
            }
            
            // Test error handling with pcap_strerror
            for (int err_code = -5; err_code <= 5; err_code++) {
                const char* err_msg = pcap_strerror(err_code);
                (void)err_msg;
            }
            break;
    }
    
    // Additional error handling tests
    if (fdp.remaining_bytes() > 0) {
        // Test pcap_strerror with various error codes from fuzzer input
        int err_test = fdp.ConsumeIntegralInRange<int>(-100, 100);
        const char* err_str = pcap_strerror(err_test);
        (void)err_str;
    }
    
    // Clean up any remaining allocations
    if (ether_result != nullptr) {
        free(ether_result);
    }
    
    return 0;
}

} // namespace harness_003
