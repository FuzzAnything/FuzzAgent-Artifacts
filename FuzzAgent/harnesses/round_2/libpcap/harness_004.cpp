#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_004 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least some content for file
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = basic parsing, 1 = with pcap_ether_aton validation, 
    // 2 = with pcap_ether_hostton lookup, 3 = all tests
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume number of test files to create (1-5)
    size_t num_files = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    // Consume max file size (1KB - 64KB)
    size_t max_file_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    
    // Remaining bytes are for file content
    std::vector<uint8_t> file_content = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (file_content.empty()) {
        return 0;
    }
    
    // Limit file content to max_file_size
    if (file_content.size() > max_file_size) {
        file_content.resize(max_file_size);
    }
    
    std::vector<std::string> temp_filenames;
    
    // Create temporary test files with /etc/ethers format content
    for (size_t i = 0; i < num_files; i++) {
        char filename[] = "/tmp/libpcap_ethers_fuzz_XXXXXX";
        int fd = mkstemp(filename);
        if (fd < 0) {
            // Clean up any already created files
            for (const auto& f : temp_filenames) {
                unlink(f.c_str());
            }
            return 0;
        }
        
        // Write file content to temp file
        // For multiple files, we could vary the content, but for simplicity
        // we'll write the same content to all files
        if (write(fd, file_content.data(), file_content.size()) != (ssize_t)file_content.size()) {
            close(fd);
            unlink(filename);
            // Clean up any already created files
            for (const auto& f : temp_filenames) {
                unlink(f.c_str());
            }
            return 0;
        }
        close(fd);
        
        temp_filenames.push_back(filename);
    }
    
    // Process each temporary file
    for (const auto& filename : temp_filenames) {
        FILE* fp = fopen(filename.c_str(), "r");
        if (fp == NULL) {
            continue;  // Skip if can't open
        }
        
        // Test pcap_next_etherent in a loop
        struct pcap_etherent* ep;
        int entry_count = 0;
        const int MAX_ENTRIES = 100;  // Limit to prevent infinite loops
        
        while ((ep = pcap_next_etherent(fp)) != NULL && entry_count < MAX_ENTRIES) {
            entry_count++;
            
            // Test 1: Validate the parsed address
            if (mode == 1 || mode == 3) {
                // Convert address bytes back to string for pcap_ether_aton test
                char addr_str[18];  // xx:xx:xx:xx:xx:xx + null terminator
                snprintf(addr_str, sizeof(addr_str), 
                        "%02x:%02x:%02x:%02x:%02x:%02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5]);
                
                // Test pcap_ether_aton with the parsed address
                u_char* aton_result = pcap_ether_aton(addr_str);
                if (aton_result != NULL) {
                    // Verify the conversion matches original address
                    bool matches = memcmp(aton_result, ep->addr, 6) == 0;
                    free(aton_result);
                }
                
                // Test pcap_ether_aton with variations of the address string
                // Test with uppercase
                char upper_addr_str[18];
                snprintf(upper_addr_str, sizeof(upper_addr_str), 
                        "%02X:%02X:%02X:%02X:%02X:%02X",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5]);
                
                u_char* upper_aton_result = pcap_ether_aton(upper_addr_str);
                if (upper_aton_result != NULL) {
                    free(upper_aton_result);
                }
                
                // Test with single digit hex (when possible)
                // Find bytes that could be represented as single digit
                for (int j = 0; j < 6; j++) {
                    if (ep->addr[j] < 0x10) {
                        char single_digit_str[18];
                        snprintf(single_digit_str, sizeof(single_digit_str), 
                                "%x:%x:%x:%x:%x:%x",
                                ep->addr[0] & 0x0F, ep->addr[1] & 0x0F,
                                ep->addr[2] & 0x0F, ep->addr[3] & 0x0F,
                                ep->addr[4] & 0x0F, ep->addr[5] & 0x0F);
                        
                        u_char* single_digit_result = pcap_ether_aton(single_digit_str);
                        if (single_digit_result != NULL) {
                            free(single_digit_result);
                        }
                        break;
                    }
                }
            }
            
            // Test 2: Lookup hostname using pcap_ether_hostton
            if ((mode == 2 || mode == 3) && ep->name[0] != '\0') {
                u_char* hostton_result = pcap_ether_hostton(ep->name);
                if (hostton_result != NULL) {
                    // Compare with parsed address
                    bool matches = memcmp(hostton_result, ep->addr, 6) == 0;
                    free(hostton_result);
                }
            }
            
            // Test 3: Also test pcap_ether_aton with malformed variations
            // Generate malformed variations from the parsed address bytes
            if (mode == 1 || mode == 3) {
                // Create malformed variations using the parsed address bytes
                // Test with too few bytes (5 bytes instead of 6)
                char short_addr[15];
                snprintf(short_addr, sizeof(short_addr),
                        "%02x:%02x:%02x:%02x:%02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4]);
                
                u_char* short_result = pcap_ether_aton(short_addr);
                if (short_result != NULL) {
                    free(short_result);
                }
                
                // Test with too many bytes (7 bytes instead of 6)
                char long_addr[21];
                snprintf(long_addr, sizeof(long_addr),
                        "%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5],
                        (ep->addr[0] + ep->addr[1]) & 0xFF); // Extra byte
                
                u_char* long_result = pcap_ether_aton(long_addr);
                if (long_result != NULL) {
                    free(long_result);
                }
                
                // Test with invalid separators (dashes instead of colons)
                char dash_addr[18];
                snprintf(dash_addr, sizeof(dash_addr),
                        "%02x-%02x-%02x-%02x-%02x-%02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5]);
                
                u_char* dash_result = pcap_ether_aton(dash_addr);
                if (dash_result != NULL) {
                    free(dash_result);
                }
                
                // Test with spaces
                char space_addr[18];
                snprintf(space_addr, sizeof(space_addr),
                        "%02x %02x %02x %02x %02x %02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5]);
                
                u_char* space_result = pcap_ether_aton(space_addr);
                if (space_result != NULL) {
                    free(space_result);
                }
                
                // Test with mixed separators
                char mixed_addr[18];
                snprintf(mixed_addr, sizeof(mixed_addr),
                        "%02x:%02x-%02x:%02x %02x:%02x",
                        ep->addr[0], ep->addr[1], ep->addr[2],
                        ep->addr[3], ep->addr[4], ep->addr[5]);
                
                u_char* mixed_result = pcap_ether_aton(mixed_addr);
                if (mixed_result != NULL) {
                    free(mixed_result);
                }
            }
        }
        
        fclose(fp);
    }
    
    // Clean up temporary files
    for (const auto& filename : temp_filenames) {
        unlink(filename.c_str());
    }
    
    // Generate additional test addresses from fuzzer input to cover various formats
    // Use some of the file content to create test address strings
    if (mode == 1 || mode == 3 && file_content.size() >= 20) {
        // Use portions of the file content to create test address strings
        size_t offset = 0;
        
        // Test different address formats using the file content
        while (offset + 17 <= file_content.size()) {
            // Create colon-separated format (xx:xx:xx:xx:xx:xx)
            char colon_addr[18];
            snprintf(colon_addr, sizeof(colon_addr),
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    file_content[offset] & 0xFF, file_content[offset+1] & 0xFF,
                    file_content[offset+2] & 0xFF, file_content[offset+3] & 0xFF,
                    file_content[offset+4] & 0xFF, file_content[offset+5] & 0xFF);
            
            u_char* colon_result = pcap_ether_aton(colon_addr);
            if (colon_result != NULL) {
                free(colon_result);
            }
            
            // Create dash-separated format (xx-xx-xx-xx-xx-xx)
            char dash_addr[18];
            snprintf(dash_addr, sizeof(dash_addr),
                    "%02x-%02x-%02x-%02x-%02x-%02x",
                    file_content[offset] & 0xFF, file_content[offset+1] & 0xFF,
                    file_content[offset+2] & 0xFF, file_content[offset+3] & 0xFF,
                    file_content[offset+4] & 0xFF, file_content[offset+5] & 0xFF);
            
            u_char* dash_result = pcap_ether_aton(dash_addr);
            if (dash_result != NULL) {
                free(dash_result);
            }
            
            // Create space-separated format (xx xx xx xx xx xx)
            char space_addr[18];
            snprintf(space_addr, sizeof(space_addr),
                    "%02x %02x %02x %02x %02x %02x",
                    file_content[offset] & 0xFF, file_content[offset+1] & 0xFF,
                    file_content[offset+2] & 0xFF, file_content[offset+3] & 0xFF,
                    file_content[offset+4] & 0xFF, file_content[offset+5] & 0xFF);
            
            u_char* space_result = pcap_ether_aton(space_addr);
            if (space_result != NULL) {
                free(space_result);
            }
            
            // Create Cisco dot format (xxxx.xxxx.xxxx)
            char dot_addr[15];
            uint16_t first = ((file_content[offset] & 0xFF) << 8) | (file_content[offset+1] & 0xFF);
            uint16_t second = ((file_content[offset+2] & 0xFF) << 8) | (file_content[offset+3] & 0xFF);
            uint16_t third = ((file_content[offset+4] & 0xFF) << 8) | (file_content[offset+5] & 0xFF);
            snprintf(dot_addr, sizeof(dot_addr), "%04x.%04x.%04x", first, second, third);
            
            u_char* dot_result = pcap_ether_aton(dot_addr);
            if (dot_result != NULL) {
                free(dot_result);
            }
            
            offset += 6;
            if (offset + 6 > file_content.size()) {
                break;
            }
        }
    }
    
    return 0;
}

} // namespace harness_004
