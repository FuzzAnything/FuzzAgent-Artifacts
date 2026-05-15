#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_014 {
/*
 * Fuzzing harness for libmagic softmagic.c targeting uncovered magiccheck() branches
 * 
 * Critical coverage gaps identified:
 * 1. Float/Double 'x' relations (don't care) - lines 2199-2201, 2232-2234 (0 coverage)
 * 2. FILE_BESTRING16 comparisons - line 2273 (0 coverage) 
 * 3. Various indirect/offset operations - many 0 coverage branches
 * 4. FILE_OCTAL, FILE_MELONG, FILE_BEID3, FILE_LEID3 types - 0 coverage
 * 5. Search functionality with MAGIC_DEBUG flag paths
 * 
 * Strategy: Generate specialized magic patterns to exercise uncovered relations
 * and data types, focusing on the specific branches that show 0 coverage.
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Magic pattern type definitions from libmagic internal headers
#define FILE_BYTE         1
#define FILE_SHORT        2
#define FILE_LONG         3
#define FILE_QUAD         4
#define FILE_FLOAT        5
#define FILE_BEFLOAT      6
#define FILE_LEFLOAT      7
#define FILE_STRING       8
#define FILE_REGEX        9
#define FILE_INDIRECT     10
#define FILE_NAME         11
#define FILE_USE          12
#define FILE_DER          13
#define FILE_GUID         14
#define FILE_BESHORT      15
#define FILE_LESHORT      16
#define FILE_BELONG       17
#define FILE_LELONG       18
#define FILE_BEQUAD       19
#define FILE_LEQUAD       20
#define FILE_DOUBLE       21
#define FILE_BEDOUBLE     22
#define FILE_LEDOUBLE     23
#define FILE_BESTRING16   24
#define FILE_LESTRING16   25
#define FILE_OCTAL        26
#define FILE_MELONG       27
#define FILE_BEID3        28
#define FILE_LEID3        29

// Helper to generate magic pattern targeting uncovered branches
std::string generate_targeted_pattern(FuzzedDataProvider& fdp, int pattern_type) {
    std::string pattern;
    
    // Generate offset (0-1024)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    pattern += std::to_string(offset);
    
    switch (pattern_type) {
        case 0: // Float/Double with 'x' relation (don't care) - UNCOVERED
            if (fdp.ConsumeBool()) {
                pattern += " float";
                pattern += 'x';  // 'x' relation for don't care - UNCOVERED
                pattern += ' ';
                pattern += std::to_string(fdp.ConsumeFloatingPoint<float>());
            } else {
                pattern += " double";
                pattern += 'x';  // 'x' relation for don't care - UNCOVERED
                pattern += ' ';
                pattern += std::to_string(fdp.ConsumeFloatingPoint<double>());
            }
            break;
            
        case 1: // FILE_BESTRING16 - UNCOVERED
            pattern += " bestring16";
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            // Generate UTF-16 BE string
            for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 16); i++) {
                uint16_t ch = fdp.ConsumeIntegral<uint16_t>();
                pattern += "\\x";
                pattern += "0123456789abcdef"[(ch >> 12) & 0xF];
                pattern += "0123456789abcdef"[(ch >> 8) & 0xF];
                pattern += "\\x";
                pattern += "0123456789abcdef"[(ch >> 4) & 0xF];
                pattern += "0123456789abcdef"[ch & 0xF];
            }
            break;
            
        case 2: // FILE_LESTRING16 (has some coverage but include for completeness)
            pattern += " lestring16";
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            // Generate UTF-16 LE string
            for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 16); i++) {
                uint16_t ch = fdp.ConsumeIntegral<uint16_t>();
                pattern += "\\x";
                pattern += "0123456789abcdef"[ch & 0xF];
                pattern += "0123456789abcdef"[(ch >> 4) & 0xF];
                pattern += "\\x";
                pattern += "0123456789abcdef"[(ch >> 8) & 0xF];
                pattern += "0123456789abcdef"[(ch >> 12) & 0xF];
            }
            break;
            
        case 3: // FILE_OCTAL - UNCOVERED
            pattern += " octal";
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            // Generate octal value
            pattern += "0";
            pattern += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 7));
            pattern += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 7));
            pattern += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 7));
            break;
            
        case 4: // FILE_MELONG - UNCOVERED (middle-endian)
            pattern += " melong";
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
            break;
            
        case 5: // FILE_BEID3 or FILE_LEID3 - UNCOVERED
            if (fdp.ConsumeBool()) {
                pattern += " beid3";
            } else {
                pattern += " leid3";
            }
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
            break;
            
        case 6: // Indirect with offset operations - target uncovered indirect paths
            pattern += " indirect";
            pattern += fdp.ConsumeBool() ? '=' : '!';
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegralInRange<uint32_t>(0, 1000));
            break;
            
        case 7: // Search pattern with potential debug output
            pattern += " search";
            pattern += '=';
            pattern += ' ';
            pattern += fdp.ConsumeRandomLengthString(32);
            break;
            
        default: // Mix of other types for general coverage
            {
                uint8_t type = fdp.ConsumeIntegralInRange<uint8_t>(1, 20);
                char reln_chars[] = {'=', '!', '>', '<', 'x'};
                char reln = reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 4)];
                
                switch (type) {
                    case FILE_FLOAT:
                    case FILE_BEFLOAT:
                    case FILE_LEFLOAT:
                        pattern += " float";
                        pattern += reln;
                        pattern += ' ';
                        pattern += std::to_string(fdp.ConsumeFloatingPoint<float>());
                        break;
                    case FILE_DOUBLE:
                    case FILE_BEDOUBLE:
                    case FILE_LEDOUBLE:
                        pattern += " double";
                        pattern += reln;
                        pattern += ' ';
                        pattern += std::to_string(fdp.ConsumeFloatingPoint<double>());
                        break;
                    default:
                        pattern += " byte";
                        pattern += reln;
                        pattern += ' ';
                        pattern += std::to_string(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            break;
    }
    
    // Add description
    pattern += " ";
    pattern += fdp.ConsumeRandomLengthString(32);
    
    return pattern;
}

// Helper to create specialized magic file targeting uncovered branches
std::string create_targeted_magic_file(FuzzedDataProvider& fdp) {
    char magic_template[] = "/tmp/magic_fuzz_targeted_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        return "";
    }
    
    std::string magic_content;
    
    // Always include patterns for uncovered branches
    // We need to consume data from the provider, not copy it
    // 1. Float/Double with 'x' relation
    if (fdp.remaining_bytes() > 10) {
        magic_content += generate_targeted_pattern(fdp, 0) + "\n";
    }
    
    // 2. FILE_BESTRING16
    if (fdp.remaining_bytes() > 10) {
        magic_content += generate_targeted_pattern(fdp, 1) + "\n";
    }
    
    // 3. FILE_OCTAL
    if (fdp.remaining_bytes() > 10) {
        magic_content += generate_targeted_pattern(fdp, 3) + "\n";
    }
    
    // 4. FILE_MELONG or FILE_BEID3/LEID3
    if (fdp.remaining_bytes() > 10) {
        int pattern_type = fdp.ConsumeBool() ? 4 : 5;
        magic_content += generate_targeted_pattern(fdp, pattern_type) + "\n";
    }
    
    // 5. Add random patterns for general coverage
    int num_additional = fdp.ConsumeIntegralInRange<int>(1, 20);
    for (int i = 0; i < num_additional && fdp.remaining_bytes() > 10; i++) {
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 8);
        magic_content += generate_targeted_pattern(fdp, pattern_type) + "\n";
    }
    
    // Write to temporary file
    write(magic_fd, magic_content.c_str(), magic_content.length());
    fsync(magic_fd);
    close(magic_fd);
    
    return std::string(magic_template);
}

// Helper to generate test data that matches magic patterns
std::vector<uint8_t> generate_matching_test_data(FuzzedDataProvider& fdp, const std::string& pattern_type) {
    std::vector<uint8_t> data;
    
    if (pattern_type.find("float") != std::string::npos || 
        pattern_type.find("double") != std::string::npos) {
        // Generate floating point data
        if (pattern_type.find("float") != std::string::npos) {
            float f = fdp.ConsumeFloatingPoint<float>();
            uint8_t* fbytes = reinterpret_cast<uint8_t*>(&f);
            for (size_t i = 0; i < sizeof(float); i++) {
                data.push_back(fbytes[i]);
            }
        } else {
            double d = fdp.ConsumeFloatingPoint<double>();
            uint8_t* dbytes = reinterpret_cast<uint8_t*>(&d);
            for (size_t i = 0; i < sizeof(double); i++) {
                data.push_back(dbytes[i]);
            }
        }
    } else if (pattern_type.find("bestring16") != std::string::npos ||
               pattern_type.find("lestring16") != std::string::npos) {
        // Generate UTF-16 data
        int len = fdp.ConsumeIntegralInRange<int>(1, 32);
        for (int i = 0; i < len; i++) {
            uint16_t ch = fdp.ConsumeIntegral<uint16_t>();
            if (pattern_type.find("bestring16") != std::string::npos) {
                // Big-endian
                data.push_back((ch >> 8) & 0xFF);
                data.push_back(ch & 0xFF);
            } else {
                // Little-endian
                data.push_back(ch & 0xFF);
                data.push_back((ch >> 8) & 0xFF);
            }
        }
    } else if (pattern_type.find("octal") != std::string::npos) {
        // Generate octal string
        std::string octal_str = "0";
        for (int i = 0; i < 3; i++) {
            octal_str += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 7));
        }
        for (char c : octal_str) {
            data.push_back(static_cast<uint8_t>(c));
        }
    } else {
        // Generic binary data
        size_t size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(size);
        data.insert(data.end(), bytes.begin(), bytes.end());
    }
    
    return data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for targeted testing
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Use MAGIC_DEBUG flag to potentially trigger debug output paths
    int flags = MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEBUG;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    
    // Flags to disable other checks and focus on softmagic
    flags |= MAGIC_NO_CHECK_COMPRESS;
    flags |= MAGIC_NO_CHECK_TAR;
    flags |= MAGIC_NO_CHECK_ELF;
    flags |= MAGIC_NO_CHECK_TEXT;
    flags |= MAGIC_NO_CHECK_CDF;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Fallback to basic flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Create targeted magic database
    std::string magic_file_path = create_targeted_magic_file(fdp);
    if (!magic_file_path.empty()) {
        // Load targeted magic database
        if (magic_load(magic, magic_file_path.c_str()) == 0) {
            // Generate test data that should exercise uncovered branches
            int test_iterations = fdp.ConsumeIntegralInRange<int>(1, 20);
            
            for (int i = 0; i < test_iterations && fdp.remaining_bytes() > 50; i++) {
                // Generate test data based on pattern types
                std::vector<uint8_t> test_data;
                
                // Always include some data at offset 0 for basic patterns
                size_t base_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<uint8_t> base_data = fdp.ConsumeBytes<uint8_t>(base_size);
                test_data.insert(test_data.end(), base_data.begin(), base_data.end());
                
                // Add floating point data at specific offsets for float/double patterns
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(float)) {
                    // Place float at offset 100
                    if (test_data.size() < 100 + sizeof(float)) {
                        test_data.resize(100 + sizeof(float), 0);
                    }
                    float f = fdp.ConsumeFloatingPoint<float>();
                    uint8_t* fbytes = reinterpret_cast<uint8_t*>(&f);
                    for (size_t j = 0; j < sizeof(float); j++) {
                        test_data[100 + j] = fbytes[j];
                    }
                }
                
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double)) {
                    // Place double at offset 200
                    if (test_data.size() < 200 + sizeof(double)) {
                        test_data.resize(200 + sizeof(double), 0);
                    }
                    double d = fdp.ConsumeFloatingPoint<double>();
                    uint8_t* dbytes = reinterpret_cast<uint8_t*>(&d);
                    for (size_t j = 0; j < sizeof(double); j++) {
                        test_data[200 + j] = dbytes[j];
                    }
                }
                
                // Add UTF-16 data for BESTRING16/LESTRING16 patterns
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 4) {
                    // Place UTF-16 data at offset 300
                    size_t utf16_len = fdp.ConsumeIntegralInRange<size_t>(2, 32);
                    if (test_data.size() < 300 + utf16_len * 2) {
                        test_data.resize(300 + utf16_len * 2, 0);
                    }
                    
                    for (size_t j = 0; j < utf16_len && fdp.remaining_bytes() >= 2; j++) {
                        uint16_t ch = fdp.ConsumeIntegral<uint16_t>();
                        // Use big-endian for BESTRING16 patterns
                        test_data[300 + j * 2] = (ch >> 8) & 0xFF;
                        test_data[300 + j * 2 + 1] = ch & 0xFF;
                    }
                }
                
                // Test magic_buffer with generated data
                if (!test_data.empty()) {
                    const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                    (void)result; // Result may be NULL
                }
                
                // Also test with magic_file for file-based paths
                if (fdp.ConsumeBool() && !test_data.empty()) {
                    char data_template[] = "/tmp/targeted_test_data_XXXXXX";
                    int data_fd = mkstemp(data_template);
                    if (data_fd != -1) {
                        write(data_fd, test_data.data(), test_data.size());
                        close(data_fd);
                        
                        const char* file_result = magic_file(magic, data_template);
                        (void)file_result;
                        
                        unlink(data_template);
                    }
                }
            }
        }
        
        // Clean up magic file
        unlink(magic_file_path.c_str());
    }
    
    // Also test with default database for broader coverage
    if (magic_load(magic, NULL) == 0) {
        // Test with remaining fuzzed data
        while (fdp.remaining_bytes() > 0) {
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                std::min<size_t>(fdp.remaining_bytes(), 1024));
            
            if (test_size == 0) break;
            
            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
            
            // Test magic_buffer
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result;
        }
    }
    
    // Test parameter setting that might affect magiccheck behavior
    for (int param_id = 0; param_id < 5 && fdp.remaining_bytes() >= sizeof(size_t); param_id++) {
        size_t param_value = fdp.ConsumeIntegral<size_t>();
        magic_setparam(magic, param_id, &param_value);
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_014
