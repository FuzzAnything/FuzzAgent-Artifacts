#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include "magic.h"

namespace harness_021 {
/*
 * Fuzzing harness for libmagic library specifically targeting the magiccheck function's 
 * 206 blocked branches in softmagic.c.
 * 
 * BACKGROUND: Despite 21 previous harnesses, magiccheck has 206/240 branches blocked.
 * This harness focuses on penetrating these blocked branches with targeted magic patterns.
 * 
 * TARGETED UNCOVERED CONDITIONS:
 * 1. Default cases in FILE_FLOAT and FILE_DOUBLE handling (completely uncovered - 0 hits)
 * 2. All relation types ('x', '!', '=', '>', '<') not sufficiently tested across data types
 * 3. FILE_SEARCH logic with ms->search.s == NULL condition (0 hits)
 * 4. FILE_STRING, FILE_PSTRING, FILE_OCTAL comparisons with various string flags
 * 5. FILE_BESTRING16 and FILE_LESTRING16 Unicode string comparisons
 * 6. Error condition triggers for default switch cases
 * 
 * APPROACH: Create magic file patterns that exercise ALL file type codes in magiccheck switch,
 * specifically targeting the uncovered conditions. Use FuzzedDataProvider to generate diverse
 * test cases from fuzzer input.
 * 
 * APIs to target:
 * - magic_open (Initialization with MAGIC_DEBUG flag to see search operations)
 * - magic_load (Load magic database with comprehensive type coverage)
 * - magic_buffer (Test with crafted binary data targeting specific magic patterns)
 * - magic_close (Cleanup)
 * 
 * KEY MAGIC PATTERNS TO INCLUDE:
 * - Float/double comparisons with all relations (x, !, =, >, <)
 * - String searches with NULL search buffers
 * - Unicode string comparisons (BESTRING16, LESTRING16)
 * - Default case triggers for error conditions
 * - Comprehensive coverage of all FILE_TYPE switch cases
 * 
 * INVOCATION SEQUENCE:
 * 1. magic_open with MAGIC_DEBUG flag
 * 2. magic_load to load magic database
 * 3. magic_buffer with crafted binary data targeting specific magic patterns
 * 4. magic_close
 * 
 * Project: $SRC (/root/src/libmagic)
 * Output directory: $OUTPUT/harnesses
 * Existing harnesses: 000-020 (analyze for semantic diversity)
 * 
 * Generate harness_021.cpp with a highly targeted approach to penetrate the magiccheck 
 * function's 206 blocked branches, focusing on the specific uncovered conditions 
 * identified in coverage analysis.
 */


// Include system headers for POSIX functions

// Include libmagic headers



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize magic library with DEBUG flag to see operations
    magic_t magic_cookie = magic_open(MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_CONTINUE);
    if (magic_cookie == nullptr) {
        return 0;
    }
    
    // Create a custom magic file with patterns targeting uncovered magiccheck conditions
    std::string magic_file_content;
    
    // Add patterns for all the uncovered conditions in magiccheck
    
    // 1. FILE_FLOAT patterns with different relations
    magic_file_content += "# FILE_FLOAT patterns with different relations\n";
    magic_file_content += "0           float       =            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<float>()) + 
                         "    Float equal test\n";
    magic_file_content += "0           float       !            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<float>()) + 
                         "    Float not equal test\n";
    magic_file_content += "0           float       >            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<float>()) + 
                         "    Float greater test\n";
    magic_file_content += "0           float       <            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<float>()) + 
                         "    Float less test\n";
    magic_file_content += "0           float       x            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<float>()) + 
                         "    Float any test (relation x)\n";
    
    // 2. FILE_DOUBLE patterns with different relations
    magic_file_content += "\n# FILE_DOUBLE patterns with different relations\n";
    magic_file_content += "0           double      =            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                         "    Double equal test\n";
    magic_file_content += "0           double      !            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                         "    Double not equal test\n";
    magic_file_content += "0           double      >            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                         "    Double greater test\n";
    magic_file_content += "0           double      <            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                         "    Double less test\n";
    magic_file_content += "0           double      x            " + 
                         std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                         "    Double any test (relation x)\n";
    
    // 3. FILE_STRING patterns
    magic_file_content += "\n# FILE_STRING patterns\n";
    magic_file_content += "0           string      =            " + 
                         fdp.ConsumeRandomLengthString(20) + 
                         "    String equal test\n";
    magic_file_content += "0           string      !            " + 
                         fdp.ConsumeRandomLengthString(20) + 
                         "    String not equal test\n";
    
    // 4. FILE_PSTRING patterns
    magic_file_content += "\n# FILE_PSTRING patterns\n";
    std::string pstring_val = fdp.ConsumeRandomLengthString(10);
    magic_file_content += "0           pstring     =            " + 
                         pstring_val + "\\000" + 
                         "    Pascal string test\n";
    
    // 5. FILE_OCTAL patterns
    magic_file_content += "\n# FILE_OCTAL patterns\n";
    magic_file_content += "0           octal       =            " + 
                         std::to_string(fdp.ConsumeIntegral<uint32_t>()) + 
                         "    Octal test\n";
    
    // 6. FILE_BESTRING16 patterns
    magic_file_content += "\n# FILE_BESTRING16 patterns\n";
    std::string bestring16_val;
    for (int i = 0; i < 4; i++) {
        uint16_t val = fdp.ConsumeIntegral<uint16_t>();
        bestring16_val += static_cast<char>((val >> 8) & 0xFF);
        bestring16_val += static_cast<char>(val & 0xFF);
    }
    magic_file_content += "0           bestring16  =            " + 
                         bestring16_val + 
                         "    Big-endian Unicode string test\n";
    
    // 7. FILE_LESTRING16 patterns
    magic_file_content += "\n# FILE_LESTRING16 patterns\n";
    std::string lestring16_val;
    for (int i = 0; i < 4; i++) {
        uint16_t val = fdp.ConsumeIntegral<uint16_t>();
        lestring16_val += static_cast<char>(val & 0xFF);
        lestring16_val += static_cast<char>((val >> 8) & 0xFF);
    }
    magic_file_content += "0           lestring16  =            " + 
                         lestring16_val + 
                         "    Little-endian Unicode string test\n";
    
    // 8. FILE_SEARCH patterns (target ms->search.s == NULL condition)
    magic_file_content += "\n# FILE_SEARCH patterns\n";
    magic_file_content += "0           search      /" + 
                         fdp.ConsumeRandomLengthString(10) + 
                         "/    Search pattern test\n";
    
    // 9. FILE_REGEX patterns
    magic_file_content += "\n# FILE_REGEX patterns\n";
    magic_file_content += "0           regex       /[a-zA-Z0-9]+/    Regex pattern test\n";
    
    // Write the custom magic file to a temporary file
    char magic_template[] = "/tmp/magic_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        magic_close(magic_cookie);
        return 0;
    }
    
    write(magic_fd, magic_file_content.c_str(), magic_file_content.length());
    fsync(magic_fd);
    close(magic_fd);
    
    // Load our custom magic database
    if (magic_load(magic_cookie, magic_template) != 0) {
        // If custom magic load fails, try loading default database
        if (magic_load(magic_cookie, nullptr) != 0) {
            unlink(magic_template);
            magic_close(magic_cookie);
            return 0;
        }
    }
    
    // Clean up temporary file
    unlink(magic_template);
    
    // Generate test data based on fuzzer input to trigger magic patterns
    int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests && fdp.remaining_bytes() > 16; i++) {
        // Consume data type and generate appropriate test data
        int data_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        std::vector<uint8_t> test_data;
        
        switch (data_type % 6) {
            case 0: // Float data
                {
                    float f = fdp.ConsumeFloatingPoint<float>();
                    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&f);
                    test_data.insert(test_data.end(), bytes, bytes + sizeof(float));
                }
                break;
                
            case 1: // Double data
                {
                    double d = fdp.ConsumeFloatingPoint<double>();
                    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&d);
                    test_data.insert(test_data.end(), bytes, bytes + sizeof(double));
                }
                break;
                
            case 2: // String data
                {
                    std::string str = fdp.ConsumeRandomLengthString(50);
                    test_data.insert(test_data.end(), str.begin(), str.end());
                    test_data.push_back(0); // Null terminator
                }
                break;
                
            case 3: // Unicode string data (UTF-16 BE)
                {
                    std::string str = fdp.ConsumeRandomLengthString(25);
                    for (char c : str) {
                        test_data.push_back(0); // High byte
                        test_data.push_back(static_cast<uint8_t>(c)); // Low byte
                    }
                    test_data.push_back(0); // Null high byte
                    test_data.push_back(0); // Null low byte
                }
                break;
                
            case 4: // Unicode string data (UTF-16 LE)
                {
                    std::string str = fdp.ConsumeRandomLengthString(25);
                    for (char c : str) {
                        test_data.push_back(static_cast<uint8_t>(c)); // Low byte
                        test_data.push_back(0); // High byte
                    }
                    test_data.push_back(0); // Null low byte
                    test_data.push_back(0); // Null high byte
                }
                break;
                
            case 5: // Mixed binary data
                {
                    size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                        std::min<size_t>(1024, fdp.remaining_bytes()));
                    std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(test_size);
                    test_data.insert(test_data.end(), bytes.begin(), bytes.end());
                }
                break;
        }
        
        if (test_data.empty()) {
            continue;
        }
        
        // Call magic_buffer with the generated test data
        // This will trigger magiccheck function with our custom patterns
        const char* result = magic_buffer(magic_cookie, test_data.data(), test_data.size());
        (void)result;
        
        // Also test with different flags to exercise different paths
        int flags = fdp.ConsumeIntegral<int>();
        magic_setflags(magic_cookie, flags & 0xFFFF);
    }
    
    // Test FILE_SEARCH with potentially NULL search buffer
    // Create minimal data
    if (fdp.remaining_bytes() > 8) {
        size_t search_size = fdp.ConsumeIntegralInRange<size_t>(1, 
            std::min<size_t>(100, fdp.remaining_bytes()));
        std::vector<uint8_t> search_test = fdp.ConsumeBytes<uint8_t>(search_size);
        
        if (!search_test.empty()) {
            const char* search_result = magic_buffer(magic_cookie, 
                                                    search_test.data(), 
                                                    search_test.size());
            (void)search_result;
        }
    }
    
    // Clean up
    magic_close(magic_cookie);
    
    return 0;
}

} // namespace harness_021
