#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <algorithm>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    // We need at least some bytes for file content and control parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control parameters first
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    int max_entries = fdp.ConsumeIntegralInRange<int>(1, 100);
    int include_comments = fdp.ConsumeBool();
    int include_blank_lines = fdp.ConsumeBool();
    int include_invalid = fdp.ConsumeBool();
    
    // Determine file content strategy based on test scenario
    std::string file_content;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Generate structured ethernet entries with various formats
            for (int i = 0; i < max_entries && fdp.remaining_bytes() > 0; i++) {
                // Decide entry type
                int entry_type = fdp.ConsumeIntegralInRange<int>(0, 5);
                
                switch (entry_type) {
                    case 0: {
                        // Standard format: "00:11:22:33:44:55 hostname"
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " ";
                        
                        // Add hostname
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                        std::string hostname = fdp.ConsumeBytesAsString(name_len);
                        // Sanitize hostname - remove spaces and newlines
                        for (char c : hostname) {
                            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                                file_content += c;
                            }
                        }
                        file_content += "\n";
                        break;
                    }
                    
                    case 1: {
                        // Short hex digits: "0:1:2:3:4:5 hostname"
                        for (int j = 0; j < 6; j++) {
                            uint8_t digit = fdp.ConsumeIntegralInRange<uint8_t>(0, 15);
                            char hex[2];
                            snprintf(hex, sizeof(hex), "%x", digit);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " ";
                        
                        // Add hostname
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                        std::string hostname = fdp.ConsumeBytesAsString(name_len);
                        for (char c : hostname) {
                            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                                file_content += c;
                            }
                        }
                        file_content += "\n";
                        break;
                    }
                    
                    case 2: {
                        // Mixed case hex: "00:11:22:AA:BB:CC hostname"
                        const char* hex_chars = "0123456789abcdefABCDEF";
                        for (int j = 0; j < 6; j++) {
                            for (int k = 0; k < 2; k++) {
                                int idx = fdp.ConsumeIntegralInRange<int>(0, 21);
                                file_content += hex_chars[idx % 22];
                            }
                            if (j < 5) file_content += ":";
                        }
                        file_content += " ";
                        
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 40);
                        std::string hostname = fdp.ConsumeBytesAsString(name_len);
                        for (char c : hostname) {
                            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                                file_content += c;
                            }
                        }
                        file_content += "\n";
                        break;
                    }
                    
                    case 3: {
                        // Entry with trailing comment: "00:11:22:33:44:55 hostname # comment"
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " ";
                        
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                        std::string hostname = fdp.ConsumeBytesAsString(name_len);
                        for (char c : hostname) {
                            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                                file_content += c;
                            }
                        }
                        file_content += " # ";
                        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                        std::string comment = fdp.ConsumeBytesAsString(comment_len);
                        file_content += comment;
                        file_content += "\n";
                        break;
                    }
                    
                    case 4: {
                        // Entry with extra whitespace variations
                        // Add leading whitespace
                        int leading_ws = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int w = 0; w < leading_ws; w++) {
                            file_content += (fdp.ConsumeBool() ? " " : "\t");
                        }
                        
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        
                        // Variable whitespace between address and hostname
                        int mid_ws = fdp.ConsumeIntegralInRange<int>(1, 4);
                        for (int w = 0; w < mid_ws; w++) {
                            file_content += (fdp.ConsumeBool() ? " " : "\t");
                        }
                        
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                        std::string hostname = fdp.ConsumeBytesAsString(name_len);
                        for (char c : hostname) {
                            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                                file_content += c;
                            }
                        }
                        file_content += "\n";
                        break;
                    }
                    
                    case 5: {
                        // Long hostname testing buffer boundaries (name field is 122 chars)
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " ";
                        
                        // Generate hostname near the 122 character limit
                        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(100, 130);
                        if (name_len > fdp.remaining_bytes()) {
                            name_len = fdp.remaining_bytes();
                        }
                        if (name_len > 0) {
                            std::string hostname = fdp.ConsumeBytesAsString(name_len);
                            // Truncate to fit in output
                            size_t output_len = std::min(hostname.size(), size_t(130));
                            for (size_t c = 0; c < output_len; c++) {
                                char ch = hostname[c];
                                if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') {
                                    file_content += ch;
                                }
                            }
                        }
                        file_content += "\n";
                        break;
                    }
                }
                
                // Optionally add comments
                if (include_comments && fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                    file_content += "# ";
                    size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
                    if (comment_len > fdp.remaining_bytes()) {
                        comment_len = fdp.remaining_bytes();
                    }
                    if (comment_len > 0) {
                        std::string comment = fdp.ConsumeBytesAsString(comment_len);
                        file_content += comment;
                    }
                    file_content += "\n";
                }
                
                // Optionally add blank lines
                if (include_blank_lines && fdp.ConsumeBool()) {
                    file_content += "\n";
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Raw bytes as file content - let parser handle arbitrary data
            size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
            if (content_size > fdp.remaining_bytes()) {
                content_size = fdp.remaining_bytes();
            }
            if (content_size > 0) {
                std::vector<uint8_t> raw_content = fdp.ConsumeBytes<uint8_t>(content_size);
                file_content.assign(reinterpret_cast<const char*>(raw_content.data()), raw_content.size());
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Mixed valid and invalid entries
            int num_lines = fdp.ConsumeIntegralInRange<int>(1, 50);
            for (int i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                int line_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                
                switch (line_type) {
                    case 0: {
                        // Valid entry
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " host";
                        file_content += std::to_string(i);
                        file_content += "\n";
                        break;
                    }
                    
                    case 1: {
                        // Invalid: missing colon
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 3) file_content += " ";  // Space instead of colon
                        }
                        file_content += " invalid\n";
                        break;
                    }
                    
                    case 2: {
                        // Invalid: non-hex characters
                        const char* chars = "ghijklmnopqrstuvwxyz!@#$%^&*()";
                        for (int j = 0; j < 6; j++) {
                            for (int k = 0; k < 2; k++) {
                                int idx = fdp.ConsumeIntegralInRange<int>(0, 30);
                                file_content += chars[idx % 31];
                            }
                            if (j < 5) file_content += ":";
                        }
                        file_content += " badhex\n";
                        break;
                    }
                    
                    case 3: {
                        // Comment line
                        file_content += "# Comment line ";
                        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                        std::string comment = fdp.ConsumeBytesAsString(comment_len);
                        file_content += comment;
                        file_content += "\n";
                        break;
                    }
                    
                    case 4: {
                        // Blank line or whitespace only
                        int ws_count = fdp.ConsumeIntegralInRange<int>(0, 5);
                        for (int w = 0; w < ws_count; w++) {
                            file_content += (fdp.ConsumeBool() ? " " : "\t");
                        }
                        file_content += "\n";
                        break;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Boundary cases and edge conditions
            // Test various separators and formats
            int num_tests = fdp.ConsumeIntegralInRange<int>(1, 20);
            
            for (int i = 0; i < num_tests && fdp.remaining_bytes() > 0; i++) {
                int test_type = fdp.ConsumeIntegralInRange<int>(0, 6);
                
                switch (test_type) {
                    case 0: {
                        // Incomplete address (fewer than 6 bytes)
                        int num_bytes = fdp.ConsumeIntegralInRange<int>(1, 5);
                        for (int j = 0; j < num_bytes; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < num_bytes - 1) file_content += ":";
                        }
                        file_content += " partial\n";
                        break;
                    }
                    
                    case 1: {
                        // Too many colons
                        for (int j = 0; j < 8; j++) {  // 8 bytes instead of 6
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 7) file_content += ":";
                        }
                        file_content += " toomany\n";
                        break;
                    }
                    
                    case 2: {
                        // No hostname (just address)
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += "\n";
                        break;
                    }
                    
                    case 3: {
                        // Hostname with spaces (should be truncated at first space)
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " host name with spaces\n";
                        break;
                    }
                    
                    case 4: {
                        // CRLF line endings (Windows style)
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += " host\r\n";
                        break;
                    }
                    
                    case 5: {
                        // Tab as separator instead of space
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += "\thostname\n";
                        break;
                    }
                    
                    case 6: {
                        // Multiple spaces/tabs as separator
                        for (int j = 0; j < 6; j++) {
                            uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                            char hex[3];
                            snprintf(hex, sizeof(hex), "%02x", byte_val);
                            file_content += hex;
                            if (j < 5) file_content += ":";
                        }
                        file_content += "   \t  \t hostname\n";
                        break;
                    }
                }
            }
            break;
        }
    }
    
    // If we have no content, use remaining bytes as raw content
    if (file_content.empty() && fdp.remaining_bytes() > 0) {
        size_t remaining = fdp.remaining_bytes();
        std::vector<uint8_t> remaining_data = fdp.ConsumeBytes<uint8_t>(remaining);
        file_content.assign(reinterpret_cast<const char*>(remaining_data.data()), remaining_data.size());
    }
    
    // Create temporary file
    char filename[] = "/tmp/pcap_etherent_fuzz_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write content to file
    if (write(fd, file_content.c_str(), file_content.size()) != static_cast<ssize_t>(file_content.size())) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    // Seek to beginning for reading
    if (lseek(fd, 0, SEEK_SET) != 0) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    // Convert file descriptor to FILE* for pcap_next_etherent
    FILE* fp = fdopen(fd, "r");
    if (fp == nullptr) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    // Parse ethernet entries using pcap_next_etherent
    struct pcap_etherent* entry;
    int entry_count = 0;
    
    while ((entry = pcap_next_etherent(fp)) != nullptr) {
        entry_count++;
        
        // Validate parsed entry if we want to
        // The entry contains addr[6] and name[122] fields
        
        // We could add validation logic here, but for fuzzing
        // we mainly want to exercise the parsing code paths
    }
    
    // Clean up
    fclose(fp);  // This also closes the fd
    unlink(filename);
    
    return 0;
}

} // namespace harness_006
