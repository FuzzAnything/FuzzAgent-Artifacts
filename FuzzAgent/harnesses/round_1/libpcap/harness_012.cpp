#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>

namespace harness_012 {
/*
 * libpcap fuzzing harness - harness_012.cpp
 * Targeted coverage for etherent.c with memory-based FILE* testing
 * 
 * Problem Analysis: Previous harnesses (008, 010) targeting etherent.c show 0% coverage.
 * Possible issues:
 * 1. Static inline functions (skip_space, skip_line) not instrumented in coverage builds
 * 2. Temporary file I/O may not exercise all code paths
 * 3. Coverage instrumentation differences between static inline and regular functions
 * 
 * Solution Strategy:
 * 1. Create FILE* from memory using fmemopen for direct, controlled testing
 * 2. Focus on byte-by-byte parsing to hit all branches in pcap_next_etherent
 * 3. Test edge cases: EOF handling, whitespace variations, hex digit validation
 * 4. Ensure static inline functions are actually called and potentially inlined
 * 
 * Key Improvement: Always ensure pcap_next_etherent is called with valid FILE*
 * by creating fallback content if fuzzer input is insufficient.
 */


#ifndef _GNU_SOURCE
#define _GNU_SOURCE  // For fmemopen
#endif
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful testing
    if (size < 10) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Strategy: Create various ethers file content patterns in memory
    // and parse them with pcap_next_etherent
    
    // Consume test pattern type
    int test_pattern = fdp.ConsumeIntegralInRange<int>(0, 7);
    
    std::string ethers_content;
    
    switch (test_pattern) {
        case 0: {
            // Pattern 1: Valid MAC address entries
            int num_entries = fdp.ConsumeIntegralInRange<int>(1, 10);
            for (int i = 0; i < num_entries && fdp.remaining_bytes() > 12; i++) {
                // Generate valid MAC address (6 bytes = 12 hex chars)
                if (fdp.remaining_bytes() >= 12) {
                    std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                    std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                     hex_bytes.substr(2, 2) + ":" + 
                                     hex_bytes.substr(4, 2) + ":" + 
                                     hex_bytes.substr(6, 2) + ":" + 
                                     hex_bytes.substr(8, 2) + ":" + 
                                     hex_bytes.substr(10, 2);
                    
                    // Add hostname
                    size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string hostname = fdp.ConsumeBytesAsString(
                        std::min(hostname_len, fdp.remaining_bytes()));
                    
                    ethers_content += mac + "\t" + hostname + "\n";
                }
            }
            break;
        }
        
        case 1: {
            // Pattern 2: Mixed content - valid entries, comments, whitespace
            int lines = fdp.ConsumeIntegralInRange<int>(3, 20);
            for (int i = 0; i < lines && fdp.remaining_bytes() > 5; i++) {
                int line_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                
                switch (line_type) {
                    case 0: { // Valid entry
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                            std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                             hex_bytes.substr(2, 2) + ":" + 
                                             hex_bytes.substr(4, 2) + ":" + 
                                             hex_bytes.substr(6, 2) + ":" + 
                                             hex_bytes.substr(8, 2) + ":" + 
                                             hex_bytes.substr(10, 2);
                            ethers_content += mac + "\t" + 
                                             fdp.ConsumeRandomLengthString(20) + "\n";
                        }
                        break;
                    }
                    case 1: { // Comment line
                        ethers_content += "# " + fdp.ConsumeRandomLengthString(30) + "\n";
                        break;
                    }
                    case 2: { // Whitespace line
                        int ws_count = fdp.ConsumeIntegralInRange<int>(1, 10);
                        ethers_content.append(ws_count, ' ');
                        ethers_content += "\n";
                        break;
                    }
                    case 3: { // Malformed entry
                        ethers_content += fdp.ConsumeRandomLengthString(30) + "\n";
                        break;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Pattern 3: Edge case testing - partial MAC addresses
            // Test parsing of incomplete MAC addresses
            if (fdp.remaining_bytes() > 0) {
                // Generate partial MAC (1-5 bytes instead of 6)
                int mac_bytes = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < mac_bytes && fdp.remaining_bytes() >= 2; i++) {
                    std::string hex_pair = fdp.ConsumeBytesAsString(2);
                    ethers_content += hex_pair;
                    if (i < mac_bytes - 1) {
                        ethers_content += ":";
                    }
                }
                ethers_content += "\tpartial_host\n";
            }
            break;
        }
        
        case 3: {
            // Pattern 4: Test whitespace handling variations
            // Leading/trailing whitespace, mixed separators
            if (fdp.remaining_bytes() >= 12) {
                std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                 hex_bytes.substr(2, 2) + ":" + 
                                 hex_bytes.substr(4, 2) + ":" + 
                                 hex_bytes.substr(6, 2) + ":" + 
                                 hex_bytes.substr(8, 2) + ":" + 
                                 hex_bytes.substr(10, 2);
                
                // Add varying whitespace
                int leading_ws = fdp.ConsumeIntegralInRange<int>(0, 5);
                int trailing_ws = fdp.ConsumeIntegralInRange<int>(0, 5);
                
                ethers_content.append(leading_ws, ' ');
                ethers_content += mac;
                
                // Use space or tab as separator
                char separator = fdp.ConsumeBool() ? '\t' : ' ';
                ethers_content += separator;
                
                ethers_content += fdp.ConsumeRandomLengthString(20);
                ethers_content.append(trailing_ws, ' ');
                ethers_content += "\n";
            }
            break;
        }
        
        case 4: {
            // Pattern 5: Test EOF handling - create content that will trigger EOF conditions
            // by having incomplete lines or sudden EOF
            if (fdp.remaining_bytes() >= 10) {
                // Write some content then truncate (simulate partial file)
                size_t content_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
                ethers_content = fdp.ConsumeBytesAsString(content_size);
                // Don't add newline - test EOF in middle of parsing
            }
            break;
        }
        
        case 5: {
            // Pattern 6: Test hex digit validation (PCAP_ISXDIGIT branches)
            // Create MAC addresses with non-hex characters
            std::string non_hex_mac;
            for (int i = 0; i < 6; i++) {
                if (i > 0) non_hex_mac += ":";
                // Mix hex and non-hex characters
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 2) {
                    non_hex_mac += fdp.ConsumeBytesAsString(2);
                } else {
                    // Add non-hex characters
                    non_hex_mac += "XY";
                }
            }
            ethers_content = non_hex_mac + "\t" + fdp.ConsumeRandomLengthString(20) + "\n";
            break;
        }
        
        case 6: {
            // Pattern 7: Test hostname buffer boundaries (e.name[122])
            // Create very long hostnames to test buffer limits
            if (fdp.remaining_bytes() > 0) {
                std::string hex_bytes = fdp.remaining_bytes() >= 12 ? 
                    fdp.ConsumeBytesAsString(12) : "001122334455";
                std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                 hex_bytes.substr(2, 2) + ":" + 
                                 hex_bytes.substr(4, 2) + ":" + 
                                 hex_bytes.substr(6, 2) + ":" + 
                                 hex_bytes.substr(8, 2) + ":" + 
                                 hex_bytes.substr(10, 2);
                
                // Create hostname that might exceed or fit within buffer
                size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(50, 200);
                std::string hostname = fdp.ConsumeBytesAsString(
                    std::min(hostname_len, fdp.remaining_bytes()));
                
                ethers_content = mac + "\t" + hostname + "\n";
            }
            break;
        }
        
        case 7: {
            // Pattern 8: Binary data mixed with text
            // Test handling of non-ASCII characters
            // Need at least 10 bytes for the binary_size parameter (min=10)
            if (fdp.remaining_bytes() >= 10) {
                size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
                std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
                
                // Convert to string (may contain null bytes)
                // Use binary_data.size() instead of binary_size in case ConsumeBytes consumed fewer bytes
                ethers_content.assign(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());
            }
            break;
        }
    }
    
    // Ensure we have some content to parse
    // If ethers_content is empty (e.g., insufficient fuzzer data), create a minimal valid entry
    if (ethers_content.empty()) {
        ethers_content = "00:11:22:33:44:55\ttest_host\n";
    }
    
    // Now parse the content using pcap_next_etherent
    // Use fmemopen to create FILE* from memory buffer
#ifdef __linux__
    FILE* mem_fp = fmemopen((void*)ethers_content.data(), ethers_content.size(), "r");
#else
    // Fallback: Use temporary file
    char tmp_filename[] = "/tmp/libpcap_fuzz_012_XXXXXX";
    int fd = mkstemp(tmp_filename);
    if (fd < 0) {
        return 0;
    }
    write(fd, ethers_content.data(), ethers_content.size());
    close(fd);
    FILE* mem_fp = fopen(tmp_filename, "r");
#endif
    
    if (mem_fp != nullptr) {
        // Parse entries until EOF
        struct pcap_etherent* entry;
        int entry_count = 0;
        while ((entry = pcap_next_etherent(mem_fp)) != nullptr) {
            entry_count++;
            // Access the parsed data to ensure it's used
            (void)entry->addr[0];
            (void)entry->name[0];
            
            // Limit parsing to prevent infinite loops on malformed data
            if (entry_count > 100) {
                break;
            }
        }
        
        fclose(mem_fp);
        
#ifndef __linux__
        // Clean up temporary file
        unlink(tmp_filename);
#endif
    }
    
    // Also test pcap_ether_aton with some MAC addresses
    // Always test at least one MAC address parsing
    std::vector<std::string> mac_formats;
    
    // Try to create a MAC from fuzzer data
    if (fdp.remaining_bytes() >= 12) {
        std::string hex_bytes = fdp.ConsumeBytesAsString(12);
        std::string std_mac = hex_bytes.substr(0, 2) + ":" + 
                             hex_bytes.substr(2, 2) + ":" + 
                             hex_bytes.substr(4, 2) + ":" + 
                             hex_bytes.substr(6, 2) + ":" + 
                             hex_bytes.substr(8, 2) + ":" + 
                             hex_bytes.substr(10, 2);
        mac_formats.push_back(std_mac);
    } else {
        // Fallback MAC if insufficient data
        mac_formats.push_back("00:11:22:33:44:55");
    }
    
    // Test each format
    for (const auto& mac_str : mac_formats) {
        u_char* mac_addr = pcap_ether_aton(mac_str.c_str());
        if (mac_addr != nullptr) {
            free(mac_addr);
        }
    }
    
    // Test with any remaining random string
    if (fdp.remaining_bytes() > 0) {
        std::string random_mac = fdp.ConsumeRemainingBytesAsString();
        u_char* random_result = pcap_ether_aton(random_mac.c_str());
        if (random_result != nullptr) {
            free(random_result);
        }
    }
    
    return 0;
}


} // namespace harness_012
