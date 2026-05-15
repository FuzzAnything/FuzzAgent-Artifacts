#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful Ethernet address testing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different Ethernet address parsing workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // For pcap_ether_aton testing - test various MAC address formats
    std::string mac_string = fdp.ConsumeRandomLengthString(64);
    
    // For pcap_ether_hostton testing - test hostname lookup
    std::string hostname = fdp.ConsumeRandomLengthString(128);
    
    // For pcap_next_etherent testing - create temporary file with Ethernet database content
    std::string ether_db_content = fdp.ConsumeRandomLengthString(512);
    
    // Consume remaining bytes for any additional data needs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    switch (operation) {
        case 0: {
            // Test pcap_ether_aton - MAC address string to binary
            u_char* ether_addr = pcap_ether_aton(mac_string.c_str());
            if (ether_addr != nullptr) {
                // Successfully parsed MAC address
                // Verify it's 6 bytes (standard Ethernet address)
                // Clean up allocated memory
                free(ether_addr);
            }
            break;
        }
        
        case 1: {
            // Test pcap_ether_hostton - hostname to Ethernet address lookup
            u_char* ether_addr = pcap_ether_hostton(hostname.c_str());
            if (ether_addr != nullptr) {
                // Successfully looked up Ethernet address for hostname
                // Clean up allocated memory
                free(ether_addr);
            }
            break;
        }
        
        case 2: {
            // Test pcap_next_etherent - parse Ethernet database file
            // Create a temporary file with fuzzed Ethernet database content
            char tmp_filename[] = "/tmp/pcap_etherent_XXXXXX";
            int fd = mkstemp(tmp_filename);
            if (fd < 0) {
                // Failed to create temp file
                break;
            }
            
            FILE* tmp_file = fdopen(fd, "w+");
            if (tmp_file == nullptr) {
                close(fd);
                break;
            }
            
            // Write the fuzzed Ethernet database content to the temp file
            // Format should be like: "00:11:22:33:44:55 hostname"
            fwrite(ether_db_content.c_str(), 1, ether_db_content.size(), tmp_file);
            fflush(tmp_file);
            rewind(tmp_file);
            
            // Parse Ethernet entries from the file
            struct pcap_etherent* ether_entry;
            while ((ether_entry = pcap_next_etherent(tmp_file)) != nullptr) {
                // Process each Ethernet entry
                // Entry contains addr[6] and name[122]
                // No cleanup needed for pcap_next_etherent result - it uses thread-local storage
            }
            
            // Clean up
            fclose(tmp_file);
            remove(tmp_filename);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_005
