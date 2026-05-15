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
#include <unistd.h>
#include <fcntl.h>
#include <climits>
#include <algorithm>

namespace harness_010 {
/*
 * libpcap fuzzing harness - harness_010.cpp
 * Deep coverage targeting for Ethernet address database subsystem (etherent.c)
 * 
 * Primary APIs targeted:
 * - pcap_next_etherent (deep parsing coverage with precise input control)
 * - Internal helper functions: skip_space, skip_line, pcapint_xdtoi
 * - MAC address parsing functions: pcapint_atomac48_xxxxxxxxxxxx, pcapint_atomac48_xxxx_3_times, pcapint_atomac48_x_xx_6_times
 * - pcap_ether_aton (through the internal parsing path)
 * 
 * Differentiates from harness_008:
 * - harness_008: Broad synthetic ethers file generation with various formats
 * - harness_010: Deep, targeted parsing of specific byte sequences to hit remaining 72+ branches
 * 
 * Focus areas for uncovered branches:
 * 1. Precise getc()/EOF handling at every parsing step
 * 2. Hex digit validation (PCAP_ISXDIGIT) edge cases
 * 3. Whitespace and comment character combinations
 * 4. Buffer boundaries in hostname field (sizeof(e.name) - 1)
 * 5. Specific parsing failure modes and continuation logic
 * 6. Thread-local static variable edge cases
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test mode
    int test_mode = fdp.ConsumeIntegralInRange<int>(0, 6);
    
    switch (test_mode) {
        case 0: {
            // Test 1: Direct pcap_next_etherent with precise byte-by-byte control
            
            // First, consume parameters
            int num_entries = fdp.ConsumeIntegralInRange<int>(1, 5);
            int max_line_length = fdp.ConsumeIntegralInRange<int>(10, 200);
            
            // Build synthetic ethers content with precise control
            std::string ethers_content;
            
            for (int entry = 0; entry < num_entries && fdp.remaining_bytes() > 20; entry++) {
                // Decide entry type
                int entry_type = fdp.ConsumeIntegralInRange<int>(0, 5);
                
                switch (entry_type) {
                    case 0: {
                        // Valid MAC + hostname entry
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                            std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                            hex_bytes.substr(2, 2) + ":" + 
                                            hex_bytes.substr(4, 2) + ":" + 
                                            hex_bytes.substr(6, 2) + ":" + 
                                            hex_bytes.substr(8, 2) + ":" + 
                                            hex_bytes.substr(10, 2);
                            
                            // Add optional leading whitespace
                            int leading_ws = fdp.ConsumeIntegralInRange<int>(0, 3);
                            ethers_content.append(leading_ws, ' ');
                            
                            ethers_content += mac;
                            
                            // Add separator (space or tab)
                            char separator = fdp.ConsumeBool() ? '\t' : ' ';
                            ethers_content += separator;
                            
                            // Hostname
                            size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                            std::string hostname = fdp.ConsumeBytesAsString(
                                std::min(hostname_len, fdp.remaining_bytes()));
                            ethers_content += hostname;
                            
                            // Optional trailing whitespace/comment
                            if (fdp.ConsumeBool()) {
                                ethers_content += "  # comment";
                            }
                            ethers_content += "\n";
                        }
                        break;
                    }
                    
                    case 1: {
                        // Invalid MAC (non-hex characters)
                        std::string invalid_mac = fdp.ConsumeRandomLengthString(17);
                        ethers_content += invalid_mac + "\thostname\n";
                        break;
                    }
                    
                    case 2: {
                        // Comment line
                        ethers_content += "# " + fdp.ConsumeRandomLengthString(30) + "\n";
                        break;
                    }
                    
                    case 3: {
                        // Empty line or whitespace only
                        int ws_count = fdp.ConsumeIntegralInRange<int>(1, 10);
                        ethers_content.append(ws_count, ' ');
                        if (fdp.ConsumeBool()) {
                            ethers_content.append(ws_count, '\t');
                        }
                        ethers_content += "\n";
                        break;
                    }
                    
                    case 4: {
                        // MAC with wrong separator count
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                            // Build with wrong number of separators
                            int num_parts = fdp.ConsumeIntegralInRange<int>(2, 8);
                            for (int i = 0; i < num_parts; i++) {
                                if (i > 0) ethers_content += ":";
                                if (i*2 < 12) {
                                    ethers_content += hex_bytes.substr(i*2, 2);
                                } else {
                                    ethers_content += "XX";
                                }
                            }
                            ethers_content += "\thostname\n";
                        }
                        break;
                    }
                    
                    case 5: {
                        // Very long hostname to test buffer boundaries
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_bytes = fdp.ConsumeBytesAsString(12);
                            std::string mac = hex_bytes.substr(0, 2) + ":" + 
                                            hex_bytes.substr(2, 2) + ":" + 
                                            hex_bytes.substr(4, 2) + ":" + 
                                            hex_bytes.substr(6, 2) + ":" + 
                                            hex_bytes.substr(8, 2) + ":" + 
                                            hex_bytes.substr(10, 2);
                            ethers_content += mac + "\t";
                            
                            // Generate hostname near buffer boundary (PCAP_ETHERTYPE_LEN_NAME - 1 = 256)
                            size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(240, 300);
                            std::string long_hostname = fdp.ConsumeBytesAsString(
                                std::min(hostname_len, fdp.remaining_bytes()));
                            ethers_content += long_hostname + "\n";
                        }
                        break;
                    }
                }
                
                // Limit total size
                if (ethers_content.size() > 1000) break;
            }
            
            // Write to temp file and test
            char tmp_filename[] = "/tmp/libpcap_fuzz_deep_XXXXXX";
            int tmp_fd = mkstemp(tmp_filename);
            if (tmp_fd < 0) {
                return 0;
            }
            
            write(tmp_fd, ethers_content.c_str(), ethers_content.size());
            close(tmp_fd);
            
            FILE* fp = fopen(tmp_filename, "r");
            if (fp != nullptr) {
                // Read entries
                int entries_read = 0;
                while (entries_read < 100) {  // Safety limit
                    struct pcap_etherent* etherent = pcap_next_etherent(fp);
                    if (etherent == nullptr) {
                        break;
                    }
                    entries_read++;
                    // Access fields to ensure they're used
                    (void)etherent->addr[0];
                    (void)etherent->name[0];
                }
                fclose(fp);
            }
            
            unlink(tmp_filename);
            break;
        }
        
        case 1: {
            // Test 2: Direct pcap_ether_aton with various MAC address formats
            
            std::vector<std::string> test_strings;
            
            // Generate various test cases
            for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                int string_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                
                switch (string_type) {
                    case 0: {
                        // Valid 12-hex string
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            // Format as standard MAC
                            std::string mac = hex_chars.substr(0, 2) + ":" + 
                                            hex_chars.substr(2, 2) + ":" + 
                                            hex_chars.substr(4, 2) + ":" + 
                                            hex_chars.substr(6, 2) + ":" + 
                                            hex_chars.substr(8, 2) + ":" + 
                                            hex_chars.substr(10, 2);
                            test_strings.push_back(mac);
                        }
                        break;
                    }
                    
                    case 1: {
                        // Invalid length
                        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                        test_strings.push_back(fdp.ConsumeBytesAsString(
                            std::min(len, fdp.remaining_bytes())));
                        break;
                    }
                    
                    case 2: {
                        // Contains non-hex characters
                        std::string mixed;
                        size_t len = fdp.ConsumeIntegralInRange<size_t>(10, 15);
                        for (size_t j = 0; j < len && fdp.remaining_bytes() > 0; j++) {
                            if (fdp.ConsumeBool()) {
                                // Hex digit
                                uint8_t hex_val = fdp.ConsumeIntegral<uint8_t>() % 16;
                                mixed += (hex_val < 10) ? ('0' + hex_val) : ('a' + hex_val - 10);
                            } else {
                                // Non-hex
                                mixed += fdp.ConsumeIntegralInRange<char>('g', 'z');
                            }
                        }
                        test_strings.push_back(mixed);
                        break;
                    }
                    
                    case 3: {
                        // Mixed case
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            std::string mac;
                            for (size_t j = 0; j < 6; j++) {
                                if (j > 0) mac += ":";
                                std::string byte = hex_chars.substr(j*2, 2);
                                // Randomly change case
                                for (char& c : byte) {
                                    if (fdp.ConsumeBool()) {
                                        c = toupper(c);
                                    } else {
                                        c = tolower(c);
                                    }
                                }
                                mac += byte;
                            }
                            test_strings.push_back(mac);
                        }
                        break;
                    }
                    
                    case 4: {
                        // Different separator formats
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            int sep_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                            std::string mac;
                            for (int j = 0; j < 6; j++) {
                                if (j > 0) {
                                    switch (sep_type) {
                                        case 0: mac += ":"; break;
                                        case 1: mac += "-"; break;
                                        case 2: mac += "."; break;
                                        case 3: mac += " "; break;
                                    }
                                }
                                mac += hex_chars.substr(j*2, 2);
                            }
                            test_strings.push_back(mac);
                        }
                        break;
                    }
                }
            }
            
            // Test each string with pcap_ether_aton
            for (const auto& test_str : test_strings) {
                u_char* result = pcap_ether_aton(test_str.c_str());
                if (result != nullptr) {
                    free(result);
                }
            }
            
            break;
        }
        
        case 2: {
            // Test 3: pcap_ether_aton with xxxx-xxxx-xxxx format
            
            std::vector<std::string> test_strings;
            
            for (int i = 0; i < 8 && fdp.remaining_bytes() > 0; i++) {
                int format_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                
                switch (format_type) {
                    case 0: {
                        // Valid xxxx-xxxx-xxxx format
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            test_strings.push_back(
                                hex_chars.substr(0, 4) + "-" +
                                hex_chars.substr(4, 4) + "-" +
                                hex_chars.substr(8, 4));
                        }
                        break;
                    }
                    
                    case 1: {
                        // Wrong separator
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            char sep = fdp.ConsumeIntegralInRange<char>('!', '/');
                            test_strings.push_back(
                                hex_chars.substr(0, 4) + sep +
                                hex_chars.substr(4, 4) + sep +
                                hex_chars.substr(8, 4));
                        }
                        break;
                    }
                    
                    case 2: {
                        // Wrong group size
                        if (fdp.remaining_bytes() >= 12) {
                            std::string hex_chars = fdp.ConsumeBytesAsString(12);
                            int group1 = fdp.ConsumeIntegralInRange<int>(3, 5);
                            int group2 = fdp.ConsumeIntegralInRange<int>(3, 5);
                            int group3 = fdp.ConsumeIntegralInRange<int>(3, 5);
                            test_strings.push_back(
                                hex_chars.substr(0, group1) + "-" +
                                hex_chars.substr(4, group2) + "-" +
                                hex_chars.substr(8, group3));
                        }
                        break;
                    }
                    
                    case 3: {
                        // Mixed valid/invalid hex
                        std::string part1, part2, part3;
                        for (int j = 0; j < 4; j++) {
                            part1 += fdp.ConsumeBool() ? 
                                (char)('0' + fdp.ConsumeIntegralInRange<int>(0, 9)) :
                                (char)('a' + fdp.ConsumeIntegralInRange<int>(0, 5));
                        }
                        for (int j = 0; j < 4; j++) {
                            part2 += fdp.ConsumeBool() ? 
                                (char)('0' + fdp.ConsumeIntegralInRange<int>(0, 9)) :
                                fdp.ConsumeIntegralInRange<char>('g', 'z');
                        }
                        for (int j = 0; j < 4; j++) {
                            part3 += fdp.ConsumeBool() ? 
                                (char)('A' + fdp.ConsumeIntegralInRange<int>(0, 5)) :
                                (char)('G' + fdp.ConsumeIntegralInRange<int>(0, 10));
                        }
                        test_strings.push_back(part1 + "-" + part2 + "-" + part3);
                        break;
                    }
                }
            }
            
            // Test each string
            for (const auto& test_str : test_strings) {
                u_char* result = pcap_ether_aton(test_str.c_str());
                if (result != nullptr) {
                    free(result);
                }
            }
            
            break;
        }
        
        case 3: {
            // Test 4: pcap_ether_aton with x:xx:xx:xx:xx:xx or similar patterns
            
            std::vector<std::string> test_strings;
            
            for (int i = 0; i < 8 && fdp.remaining_bytes() > 0; i++) {
                int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                
                switch (pattern_type) {
                    case 0: {
                        // Valid x:xx:xx:xx:xx:xx
                        std::string mac;
                        for (int j = 0; j < 6; j++) {
                            if (j > 0) mac += ":";
                            if (j == 0) {
                                // Single hex for first byte
                                uint8_t hex = fdp.ConsumeIntegral<uint8_t>() % 16;
                                mac += (hex < 10) ? ('0' + hex) : ('a' + hex - 10);
                            } else {
                                // Two hex for other bytes
                                for (int k = 0; k < 2; k++) {
                                    uint8_t hex = fdp.ConsumeIntegral<uint8_t>() % 16;
                                    mac += (hex < 10) ? ('0' + hex) : ('a' + hex - 10);
                                }
                            }
                        }
                        test_strings.push_back(mac);
                        break;
                    }
                    
                    case 1: {
                        // Wrong separator positions
                        std::string mac;
                        int sep_pos = fdp.ConsumeIntegralInRange<int>(1, 10);
                        for (int j = 0; j < 12; j++) {  // 12 hex chars total
                            if (j > 0 && j % sep_pos == 0) {
                                mac += ":";
                            }
                            uint8_t hex = fdp.ConsumeIntegral<uint8_t>() % 16;
                            mac += (hex < 10) ? ('0' + hex) : ('a' + hex - 10);
                        }
                        test_strings.push_back(mac);
                        break;
                    }
                    
                    case 2: {
                        // Mixed separators
                        std::string mac;
                        const char* separators[] = {":", "-", ".", " "};
                        for (int j = 0; j < 6; j++) {
                            if (j > 0) {
                                mac += separators[fdp.ConsumeIntegralInRange<int>(0, 3)];
                            }
                            for (int k = 0; k < (j == 0 ? 1 : 2); k++) {
                                uint8_t hex = fdp.ConsumeIntegral<uint8_t>() % 16;
                                mac += (hex < 10) ? ('0' + hex) : ('a' + hex - 10);
                            }
                        }
                        test_strings.push_back(mac);
                        break;
                    }
                    
                    case 3: {
                        // Invalid hex in specific positions
                        std::string mac;
                        for (int j = 0; j < 6; j++) {
                            if (j > 0) mac += ":";
                            for (int k = 0; k < (j == 0 ? 1 : 2); k++) {
                                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                                    // Valid hex
                                    uint8_t hex = fdp.ConsumeIntegral<uint8_t>() % 16;
                                    mac += (hex < 10) ? ('0' + hex) : ('a' + hex - 10);
                                } else {
                                    // Invalid char
                                    mac += fdp.ConsumeIntegralInRange<char>('g', 'z');
                                }
                            }
                        }
                        test_strings.push_back(mac);
                        break;
                    }
                }
            }
            
            // Test each string
            for (const auto& test_str : test_strings) {
                u_char* result = pcap_ether_aton(test_str.c_str());
                if (result != nullptr) {
                    free(result);
                }
            }
            
            break;
        }
        
        case 4: {
            // Test 5: pcap_ether_aton with edge cases
            
            // Test pcap_ether_aton with various random strings
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 12; i++) {
                std::string test_mac = fdp.ConsumeRandomLengthString(20);
                u_char* result = pcap_ether_aton(test_mac.c_str());
                if (result != nullptr) {
                    free(result);
                }
            }
            
            // Also test empty/null strings
            const char* empty_str = "";
            u_char* empty_result = pcap_ether_aton(empty_str);
            if (empty_result != nullptr) {
                free(empty_result);
            }
            
            break;
        }
        
        case 5: {
            // Test 6: Thread-local and stateful behavior
            // Create multiple files and test across them
            
            std::vector<std::string> temp_files;
            
            // Create 2-3 test files
            int num_files = fdp.ConsumeIntegralInRange<int>(2, 3);
            for (int file_idx = 0; file_idx < num_files && fdp.remaining_bytes() > 20; file_idx++) {
                char filename[] = "/tmp/libpcap_fuzz_state_XXXXXX";
                int fd = mkstemp(filename);
                if (fd < 0) continue;
                
                // Write some entries
                int num_entries = fdp.ConsumeIntegralInRange<int>(1, 3);
                for (int e = 0; e < num_entries && fdp.remaining_bytes() >= 12; e++) {
                    std::string hex_chars = fdp.ConsumeBytesAsString(12);
                    std::string entry = 
                        hex_chars.substr(0, 2) + ":" + hex_chars.substr(2, 2) + ":" +
                        hex_chars.substr(4, 2) + ":" + hex_chars.substr(6, 2) + ":" +
                        hex_chars.substr(8, 2) + ":" + hex_chars.substr(10, 2) + "\t" +
                        "file" + std::to_string(file_idx) + "_host" + std::to_string(e) + "\n";
                    write(fd, entry.c_str(), entry.size());
                }
                close(fd);
                temp_files.push_back(std::string(filename));
            }
            
            // Test alternating between files
            for (int iteration = 0; iteration < 5 && !temp_files.empty(); iteration++) {
                for (const auto& filename : temp_files) {
                    FILE* fp = fopen(filename.c_str(), "r");
                    if (fp != nullptr) {
                        // Read one entry
                        struct pcap_etherent* etherent = pcap_next_etherent(fp);
                        if (etherent != nullptr) {
                            (void)etherent;
                        }
                        fclose(fp);
                    }
                }
            }
            
            // Clean up
            for (const auto& filename : temp_files) {
                unlink(filename.c_str());
            }
            
            break;
        }
        
        case 6: {
            // Test 7: Extreme edge cases and error injection
            
            // Test with very small inputs
            if (fdp.remaining_bytes() > 0) {
                size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                std::vector<uint8_t> small_data = fdp.ConsumeBytes<uint8_t>(small_size);
                
                // Try to create a temp file with this minimal data
                char tiny_filename[] = "/tmp/libpcap_fuzz_tiny_XXXXXX";
                int tiny_fd = mkstemp(tiny_filename);
                if (tiny_fd >= 0) {
                    write(tiny_fd, small_data.data(), small_data.size());
                    close(tiny_fd);
                    
                    FILE* fp = fopen(tiny_filename, "r");
                    if (fp != nullptr) {
                        struct pcap_etherent* etherent = pcap_next_etherent(fp);
                        (void)etherent;  // Might be null, that's OK
                        fclose(fp);
                    }
                    unlink(tiny_filename);
                }
            }
            
            // Test with binary/non-text data
            if (fdp.remaining_bytes() > 0) {
                size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(20, 100);
                std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
                
                char binary_filename[] = "/tmp/libpcap_fuzz_binary_XXXXXX";
                int binary_fd = mkstemp(binary_filename);
                if (binary_fd >= 0) {
                    write(binary_fd, binary_data.data(), binary_data.size());
                    close(binary_fd);
                    
                    FILE* fp = fopen(binary_filename, "r");
                    if (fp != nullptr) {
                        // Try to read - might get garbage, that's the point
                        for (int i = 0; i < 10; i++) {
                            struct pcap_etherent* etherent = pcap_next_etherent(fp);
                            if (etherent == nullptr) break;
                            (void)etherent;
                        }
                        fclose(fp);
                    }
                    unlink(binary_filename);
                }
            }
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_010
