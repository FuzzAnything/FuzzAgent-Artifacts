#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>
#include <pcap/namedb.h>

namespace harness_009 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum size needed for meaningful testing:
    // - At least 4 bytes for initialization options and scenario selection
    // - At least some bytes for string inputs
    if (Size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: pcap_init options
    uint8_t init_options_byte = fdp.ConsumeIntegral<uint8_t>();
    unsigned int init_options = 0;
    
    // Set appropriate pcap_init flags based on input
    if (init_options_byte & 0x01) {
        init_options |= PCAP_CHAR_ENC_LOCAL;
    }
    if (init_options_byte & 0x02) {
        init_options |= PCAP_CHAR_ENC_UTF_8;
    }
    if (init_options_byte & 0x04) {
        init_options |= PCAP_MMAP_32BIT;
    }
    
    // Third and fourth bytes: reserve for other parameters
    uint16_t param = fdp.ConsumeIntegral<uint16_t>();
    
    // Split remaining input into multiple test strings
    // We'll use the remaining bytes to generate test strings
    size_t remaining = fdp.remaining_bytes();
    
    // Create multiple test strings from the input
    std::vector<std::string> test_strings;
    
    // Generate up to 8 test strings from the input
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 1; i++) {
        // Vary string lengths to test different cases
        size_t max_len = (fdp.remaining_bytes() / (8 - i)) + 1;
        if (max_len > 256) max_len = 256;  // Cap at reasonable length
        
        std::string test_str = fdp.ConsumeRandomLengthString(max_len);
        if (!test_str.empty()) {
            test_strings.push_back(test_str);
        }
    }
    
    if (test_strings.empty()) {
        // If no strings could be generated, not enough input for testing
        return 0;
    }
    
    // Error buffer for pcap_init
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Step 1: Call pcap_init with various option flags
    // pcap_init returns 0 on success, PCAP_ERROR on failure
    int init_result = pcap_init(init_options, errbuf);
    
    // The result is not critical - we can continue testing other APIs
    // regardless of initialization success
    
    // Now test the name translation APIs
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_nametonetaddr with various network address strings
            for (size_t i = 0; i < test_strings.size() && i < 4; i++) {
                const std::string& name = test_strings[i];
                bpf_u_int32 netaddr = pcap_nametonetaddr(name.c_str());
                // Result can be 0 on failure or network address on success
                // Both cases are interesting for coverage
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_nametoeproto with protocol names
            for (size_t i = 0; i < test_strings.size() && i < 4; i++) {
                const std::string& name = test_strings[i];
                int eproto = pcap_nametoeproto(name.c_str());
                // Returns protocol number or -1 on failure
                // Test both success and failure paths
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test pcap_nametollc with LLC type names
            for (size_t i = 0; i < test_strings.size() && i < 4; i++) {
                const std::string& name = test_strings[i];
                int llc = pcap_nametollc(name.c_str());
                // Returns LLC type or -1 on failure
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_nametoportrange with port range strings
            // This is the primary target with 25 undiscovered branches
            for (size_t i = 0; i < test_strings.size() && i < 4; i++) {
                const std::string& name = test_strings[i];
                int port1, port2, proto;
                
                int result = pcap_nametoportrange(name.c_str(), &port1, &port2, &proto);
                
                if (result == 1) {
                    // Successfully parsed port range
                    // Validate returned values are reasonable
                    if (port1 >= 0 && port1 <= 65535 &&
                        port2 >= 0 && port2 <= 65535 &&
                        port1 <= port2) {
                        // Valid port range
                    }
                }
                // Failure cases are also important for coverage
            }
            
            // Create hyphenated strings for port range testing
            // pcap_nametoportrange expects strings like "http-https" or "20-21"
            // We'll create hyphenated combinations from test strings
            for (size_t i = 0; i < test_strings.size() && i < 3; i++) {
                for (size_t j = 0; j < test_strings.size() && j < 3; j++) {
                    if (i != j) {
                        std::string hyphenated = test_strings[i] + "-" + test_strings[j];
                        int port1, port2, proto;
                        int result = pcap_nametoportrange(hyphenated.c_str(), &port1, &port2, &proto);
                        // Test both success and failure cases
                    }
                }
            }
            
            // Also test numeric port ranges by generating numbers from fuzzer input
            if (fdp.remaining_bytes() >= 4) {
                // Generate numeric port ranges
                uint16_t num1 = fdp.ConsumeIntegral<uint16_t>();
                uint16_t num2 = fdp.ConsumeIntegral<uint16_t>();
                
                char num_range[32];
                snprintf(num_range, sizeof(num_range), "%u-%u", num1 % 65536, num2 % 65536);
                int port1, port2, proto;
                int result = pcap_nametoportrange(num_range, &port1, &port2, &proto);
            }
            break;
        }
    }
    
    // Additional test: pcap_nametoport (called internally by pcap_nametoportrange)
    // Test it directly as well
    if (scenario != 3 && !test_strings.empty()) {
        // Use first test string for pcap_nametoport
        int port, proto;
        int result = pcap_nametoport(test_strings[0].c_str(), &port, &proto);
        // Result 1 on success, 0 on failure
    }
    
    // Additional test: pcap_nametoproto (similar to pcap_nametoeproto)
    if (!test_strings.empty() && test_strings.size() > 1) {
        int proto = pcap_nametoproto(test_strings[1].c_str());
        // Returns protocol number or -1 on failure
    }
    
    return 0;
}

} // namespace harness_009
