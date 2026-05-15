#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>

namespace harness_004 {


// Helper function to create a temporary ethers database file from fuzzer input
static FILE* create_temp_ethers_file(const uint8_t* data, size_t size, char* temp_filename) {
    // Create a temporary filename
    static int counter = 0;
    snprintf(temp_filename, 256, "/tmp/libpcap_ethers_fuzz_%d_%d", getpid(), counter++);
    
    FILE* fp = fopen(temp_filename, "wb");
    if (fp == NULL) {
        return NULL;
    }
    
    // Write the fuzzer data as the ethers file content
    // Ethers file format: MAC address followed by hostname, one per line
    // Example: "00:11:22:33:44:55 hostname"
    if (fwrite(data, 1, size, fp) != size) {
        fclose(fp);
        remove(temp_filename);
        return NULL;
    }
    
    fclose(fp);
    
    // Reopen for reading
    fp = fopen(temp_filename, "r");
    return fp;
}

// Helper function to generate MAC address strings from fuzzer data
static void generate_mac_strings(const std::string& input, std::vector<std::string>& mac_strings) {
    // Generate various MAC address string formats from input bytes
    if (input.size() >= 6) {
        // Format 1: XX:XX:XX:XX:XX:XX (standard colon-separated)
        char mac1[18];
        snprintf(mac1, sizeof(mac1), "%02x:%02x:%02x:%02x:%02x:%02x",
                (uint8_t)input[0], (uint8_t)input[1], (uint8_t)input[2],
                (uint8_t)input[3], (uint8_t)input[4], (uint8_t)input[5]);
        mac_strings.push_back(mac1);
        
        // Format 2: XXXXXXXXXXXX (no separators)
        char mac2[13];
        snprintf(mac2, sizeof(mac2), "%02x%02x%02x%02x%02x%02x",
                (uint8_t)input[0], (uint8_t)input[1], (uint8_t)input[2],
                (uint8_t)input[3], (uint8_t)input[4], (uint8_t)input[5]);
        mac_strings.push_back(mac2);
        
        // Format 3: XX-XX-XX-XX-XX-XX (hyphen-separated)
        char mac3[18];
        snprintf(mac3, sizeof(mac3), "%02x-%02x-%02x-%02x-%02x-%02x",
                (uint8_t)input[0], (uint8_t)input[1], (uint8_t)input[2],
                (uint8_t)input[3], (uint8_t)input[4], (uint8_t)input[5]);
        mac_strings.push_back(mac3);
        
        // Format 4: X.X.X.X.X.X (dot-separated)
        char mac4[18];
        snprintf(mac4, sizeof(mac4), "%02x.%02x.%02x.%02x.%02x.%02x",
                (uint8_t)input[0], (uint8_t)input[1], (uint8_t)input[2],
                (uint8_t)input[3], (uint8_t)input[4], (uint8_t)input[5]);
        mac_strings.push_back(mac4);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need enough input for meaningful testing
    // Minimum: 1 byte scenario + some data for ethers file + some for address strings
    if (Size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: number of iterations for pcap_next_etherent
    uint8_t max_iterations = fdp.ConsumeIntegral<uint8_t>();
    if (max_iterations == 0) max_iterations = 10; // Default limit
    
    // Consume some bytes for temporary ethers file data
    size_t ethers_file_size = fdp.ConsumeIntegralInRange<size_t>(16, Size / 2);
    std::vector<uint8_t> ethers_file_data = fdp.ConsumeBytes<uint8_t>(ethers_file_size);
    
    // Consume remaining bytes for address strings and hostnames
    std::string remaining_input = fdp.ConsumeRemainingBytesAsString();
    
    char temp_ethers_filename[256];
    memset(temp_ethers_filename, 0, sizeof(temp_ethers_filename));
    
    // Create a temporary ethers file for testing pcap_next_etherent
    FILE* ethers_fp = create_temp_ethers_file(ethers_file_data.data(), ethers_file_data.size(), 
                                             temp_ethers_filename);
    if (ethers_fp == NULL) {
        // Couldn't create file, but we can still test some functions
        // that don't require file access
    }
    
    // Generate MAC address strings from remaining input for testing pcap_ether_aton
    std::vector<std::string> mac_strings;
    generate_mac_strings(remaining_input, mac_strings);
    
    // Test different scenarios targeting different API combinations
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_ether_aton with various MAC address string formats
            // Test generated MAC strings
            for (const auto& mac_str : mac_strings) {
                u_char* ether_addr = pcap_ether_aton(mac_str.c_str());
                if (ether_addr != NULL) {
                    free(ether_addr);
                }
            }
            
            // Also test random substrings from remaining input
            size_t pos = 0;
            int tests_done = 0;
            while (pos < remaining_input.size() && pos < 100 && tests_done < 10) {
                size_t end_pos = pos;
                while (end_pos < remaining_input.size() && 
                       end_pos - pos < 30 && // Limit string length
                       remaining_input[end_pos] != ' ' && 
                       remaining_input[end_pos] != '\n' && 
                       remaining_input[end_pos] != '\t') {
                    end_pos++;
                }
                
                if (end_pos > pos) {
                    std::string test_str = remaining_input.substr(pos, end_pos - pos);
                    u_char* ether_addr = pcap_ether_aton(test_str.c_str());
                    if (ether_addr != NULL) {
                        free(ether_addr);
                    }
                    tests_done++;
                }
                
                // Skip whitespace
                pos = end_pos;
                while (pos < remaining_input.size() && 
                       (remaining_input[pos] == ' ' || 
                        remaining_input[pos] == '\n' || 
                        remaining_input[pos] == '\t')) {
                    pos++;
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_next_etherent with temporary ethers file
            // This exercises the etherent.c module which has 0% coverage
            if (ethers_fp != NULL) {
                struct pcap_etherent* ether_entry;
                int iteration_count = 0;
                
                // Read entries from the ethers file using pcap_next_etherent
                while ((ether_entry = pcap_next_etherent(ethers_fp)) != NULL && 
                       iteration_count < max_iterations) {
                    // For each valid entry, test pcap_ether_aton on the parsed address
                    char addr_str[18]; // XX:XX:XX:XX:XX:XX format
                    snprintf(addr_str, sizeof(addr_str), 
                            "%02x:%02x:%02x:%02x:%02x:%02x",
                            ether_entry->addr[0], ether_entry->addr[1],
                            ether_entry->addr[2], ether_entry->addr[3],
                            ether_entry->addr[4], ether_entry->addr[5]);
                    
                    u_char* parsed_addr = pcap_ether_aton(addr_str);
                    if (parsed_addr != NULL) {
                        free(parsed_addr);
                    }
                    
                    iteration_count++;
                }
                
                // Test rewinding and reading again (different code path)
                rewind(ethers_fp);
                iteration_count = 0;
                while ((ether_entry = pcap_next_etherent(ethers_fp)) != NULL && 
                       iteration_count < 3) {
                    iteration_count++;
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Mixed testing of pcap_ether_hostton and pcap_nametoaddr
            // These functions may fail without proper system configuration,
            // but we can still exercise their error handling paths
            
            // Test pcap_ether_hostton with various strings
            // Note: pcap_ether_hostton reads from /etc/ethers or uses system call
            // This will likely fail, testing error paths
            size_t pos = 0;
            int hostname_tests = 0;
            while (pos < remaining_input.size() && pos < 80 && hostname_tests < 8) {
                size_t end_pos = pos;
                while (end_pos < remaining_input.size() && 
                       end_pos - pos < 30 &&
                       remaining_input[end_pos] != ' ' && 
                       remaining_input[end_pos] != '\n' && 
                       remaining_input[end_pos] != '\t') {
                    end_pos++;
                }
                
                if (end_pos > pos) {
                    std::string hostname = remaining_input.substr(pos, end_pos - pos);
                    u_char* host_addr = pcap_ether_hostton(hostname.c_str());
                    if (host_addr != NULL) {
                        free(host_addr);
                    }
                    hostname_tests++;
                }
                
                pos = end_pos;
                while (pos < remaining_input.size() && 
                       (remaining_input[pos] == ' ' || 
                        remaining_input[pos] == '\n' || 
                        remaining_input[pos] == '\t')) {
                    pos++;
                }
            }
            
            // Test pcap_nametoaddr with various names
            // This function is deprecated but still testable
            pos = 0;
            int nametoaddr_tests = 0;
            while (pos < remaining_input.size() && pos < 60 && nametoaddr_tests < 5) {
                size_t end_pos = pos;
                while (end_pos < remaining_input.size() && 
                       end_pos - pos < 40 &&
                       remaining_input[end_pos] != ' ' && 
                       remaining_input[end_pos] != '\n' && 
                       remaining_input[end_pos] != '\t') {
                    end_pos++;
                }
                
                if (end_pos > pos) {
                    std::string name = remaining_input.substr(pos, end_pos - pos);
                    bpf_u_int32** addr_list = pcap_nametoaddr(name.c_str());
                    // pcap_nametoaddr returns pointer to internal data or NULL
                    // We don't need to free it
                    
                    nametoaddr_tests++;
                }
                
                pos = end_pos;
                while (pos < remaining_input.size() && 
                       (remaining_input[pos] == ' ' || 
                        remaining_input[pos] == '\n' || 
                        remaining_input[pos] == '\t')) {
                    pos++;
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Comprehensive testing combining all target APIs
            // Test pcap_ether_aton with generated MAC strings
            for (const auto& mac_str : mac_strings) {
                u_char* ether_addr = pcap_ether_aton(mac_str.c_str());
                if (ether_addr != NULL) {
                    free(ether_addr);
                }
            }
            
            // Test pcap_next_etherent if we have a file
            if (ethers_fp != NULL) {
                struct pcap_etherent* ether_entry;
                int entries_read = 0;
                
                rewind(ethers_fp);
                while ((ether_entry = pcap_next_etherent(ethers_fp)) != NULL && 
                       entries_read < 5) {
                    entries_read++;
                }
            }
            
            // Test pcap_ether_hostton and pcap_nametoaddr with selective inputs
            // Pick a few substrings from remaining input for testing
            if (remaining_input.size() > 10) {
                // Test first 10 chars as hostname
                std::string test_hostname = remaining_input.substr(0, 10);
                u_char* host_addr = pcap_ether_hostton(test_hostname.c_str());
                if (host_addr != NULL) {
                    free(host_addr);
                }
                
                // Test middle 10 chars as network name
                if (remaining_input.size() > 20) {
                    std::string test_netname = remaining_input.substr(10, 10);
                    bpf_u_int32** addr_list = pcap_nametoaddr(test_netname.c_str());
                }
            }
            break;
        }
    }
    
    // Cleanup
    if (ethers_fp != NULL) {
        fclose(ethers_fp);
        remove(temp_ethers_filename);
    }
    
    return 0;
}

} // namespace harness_004
