#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_018 {
/*
 * Specialized fuzzing harness for libmagic targeting apprentice_magic_strength_1 
 * with 97 blocked branches and compression functions with 0% coverage.
 * 
 * Primary internal targets (highest coverage gaps):
 * 1. apprentice_magic_strength_1 - 97 blocked branches, handles FILE_DEFAULT and weight calculation
 * 2. Compression functions - uncompressgzipped, uncompresszlib, uncompressbzlib, etc. (0% coverage)
 * 
 * APIs targeted:
 * - magic_open, magic_load, magic_check, magic_compile, magic_list, magic_buffer, magic_close
 * 
 * Strategy for semantic diversity from existing harnesses (000-017):
 * - harness_016: Deep magic matching pipeline targeting ALL FILE_* types
 * - harness_017: Real system database testing with corrupted files and multi-format testing
 * - harness_018: **Compression format triggering + FILE_DEFAULT database testing** (NEW)
 * 
 * NEW differentiation for harness_018:
 * 1. **Compression format testing**: Generate test data with valid compression headers (gzip, bzip2, xz, zstd)
 * 2. **FILE_DEFAULT database testing**: Create magic databases with FILE_DEFAULT entries to trigger apprentice_magic_strength_1
 * 3. **Combined compression + magic**: Test compression formats through magic_buffer API
 * 4. **Database compilation with compression**: Test magic_compile with compressed data references
 * 5. **Comprehensive compression coverage**: Test all compression methods supported by libmagic
 * 
 * Expected coverage gains:
 * - Unlock 97 blocked branches in apprentice_magic_strength_1
 * - Activate 0% coverage compression functions
 * - Improve internal function coverage from 46.13%
 * - Improve branch coverage from 19.62%
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for compression format testing + database testing
    // Need: flags + test strategy + compression data + magic database content
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int test_strategy = fdp.ConsumeIntegralInRange<int>(0, 5); // 0-5 different testing strategies
    bool test_compression_formats = fdp.ConsumeBool();
    bool test_file_default = fdp.ConsumeBool();
    bool test_database_compilation = fdp.ConsumeBool();
    bool test_combined_compression = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy-based testing
    switch (test_strategy) {
        case 0: {
            // Strategy 0: Compression format testing with magic_buffer
            // Generate test buffers with valid compression headers
            
            if (test_compression_formats && fdp.remaining_bytes() >= 100) {
                // Create test buffer with compression headers
                std::vector<uint8_t> test_buffer;
                
                // Consume compression type
                int comp_type = fdp.ConsumeIntegralInRange<int>(0, 6);
                
                // Add appropriate compression header based on type
                switch (comp_type) {
                    case 0: // gzip
                        if (fdp.remaining_bytes() >= 10) {
                            test_buffer.push_back(0x1F); // gzip magic
                            test_buffer.push_back(0x8B); // gzip magic
                            test_buffer.push_back(0x08); // DEFLATE compression
                            // Add some compressed data
                            size_t gzip_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> gzip_data = fdp.ConsumeBytes<uint8_t>(gzip_data_size);
                            test_buffer.insert(test_buffer.end(), gzip_data.begin(), gzip_data.end());
                        }
                        break;
                        
                    case 1: // bzip2
                        if (fdp.remaining_bytes() >= 10) {
                            test_buffer.push_back('B'); // bzip2 magic
                            test_buffer.push_back('Z'); // bzip2 magic
                            test_buffer.push_back('h'); // bzip2 magic
                            test_buffer.push_back('1'); // bzip2 version
                            // Add some compressed data
                            size_t bz2_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> bz2_data = fdp.ConsumeBytes<uint8_t>(bz2_data_size);
                            test_buffer.insert(test_buffer.end(), bz2_data.begin(), bz2_data.end());
                        }
                        break;
                        
                    case 2: // xz
                        if (fdp.remaining_bytes() >= 10) {
                            test_buffer.push_back(0xFD); // xz magic
                            test_buffer.push_back('7');  // xz magic
                            test_buffer.push_back('z');  // xz magic
                            test_buffer.push_back('X');  // xz magic
                            test_buffer.push_back('Z');  // xz magic
                            test_buffer.push_back(0x00); // xz magic
                            // Add some compressed data
                            size_t xz_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> xz_data = fdp.ConsumeBytes<uint8_t>(xz_data_size);
                            test_buffer.insert(test_buffer.end(), xz_data.begin(), xz_data.end());
                        }
                        break;
                        
                    case 3: // zstd
                        if (fdp.remaining_bytes() >= 10) {
                            test_buffer.push_back(0x28); // zstd magic
                            test_buffer.push_back(0xB5); // zstd magic
                            test_buffer.push_back(0x2F); // zstd magic
                            test_buffer.push_back(0xFD); // zstd magic
                            // Add some compressed data
                            size_t zstd_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> zstd_data = fdp.ConsumeBytes<uint8_t>(zstd_data_size);
                            test_buffer.insert(test_buffer.end(), zstd_data.begin(), zstd_data.end());
                        }
                        break;
                        
                    case 4: // compress (.Z)
                        if (fdp.remaining_bytes() >= 10) {
                            test_buffer.push_back(0x1F); // compress magic
                            test_buffer.push_back(0x9D); // compress magic
                            // Add some compressed data
                            size_t compress_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> compress_data = fdp.ConsumeBytes<uint8_t>(compress_data_size);
                            test_buffer.insert(test_buffer.end(), compress_data.begin(), compress_data.end());
                        }
                        break;
                        
                    case 5: // lzlib
                        if (fdp.remaining_bytes() >= 10) {
                            // LZIP header
                            test_buffer.push_back('L'); // LZIP magic
                            test_buffer.push_back('Z'); // LZIP magic
                            test_buffer.push_back('I'); // LZIP magic
                            test_buffer.push_back('P'); // LZIP magic
                            test_buffer.push_back(0x01); // version
                            // Add some compressed data
                            size_t lzip_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                            std::vector<uint8_t> lzip_data = fdp.ConsumeBytes<uint8_t>(lzip_data_size);
                            test_buffer.insert(test_buffer.end(), lzip_data.begin(), lzip_data.end());
                        }
                        break;
                        
                    default: // raw data
                        size_t raw_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 500);
                        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
                        test_buffer.insert(test_buffer.end(), raw_data.begin(), raw_data.end());
                        break;
                }
                
                // Try to load default magic database for testing
                const char* magic_path = "/usr/share/misc/magic";
                magic_load(magic, magic_path); // Ignore errors
                
                // Test magic_buffer with compression data
                if (!test_buffer.empty()) {
                    const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result;
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: FILE_DEFAULT database testing
            // Create magic database files with FILE_DEFAULT entries
            
            if (test_file_default && fdp.remaining_bytes() >= 100) {
                // Create a temporary magic file with FILE_DEFAULT entries
                std::string temp_magic_file = "/tmp/fuzz_magic_default_" + std::to_string(getpid()) + ".mgc";
                std::string temp_text_file = "/tmp/fuzz_magic_default_" + std::to_string(getpid()) + ".magic";
                
                // Generate magic file content with FILE_DEFAULT entries
                std::string magic_content;
                
                // Add some standard magic entries
                magic_content += "0 string  GIF8      GIF image\n";
                magic_content += "0 string  \\x89PNG   PNG image\n";
                magic_content += "0 string  \\xff\\xd8  JPEG image\n";
                
                // Add FILE_DEFAULT entries (these should sort last according to apprentice_magic_strength_1)
                // Based on apprentice.c line 948: case FILE_DEFAULT: /* make sure this sorts last */
                magic_content += "0 default           default entry 1\n";
                magic_content += "0 default x         default entry 2 with test\n";
                magic_content += "0 default           default entry 3\n";
                
                // Add more varied entries to trigger different code paths
                magic_content += "0 byte   0x42       Test byte\n";
                magic_content += "0 short  0x1234     Test short\n";
                magic_content += "0 long   0x12345678 Test long\n";
                magic_content += "0 string test       Test string\n";
                magic_content += "0 search test       Test search\n";
                
                // Write to text magic file
                FILE* fp = fopen(temp_text_file.c_str(), "w");
                if (fp) {
                    fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
                    fclose(fp);
                    
                    // Test magic_compile to create compiled database
                    int compile_result = magic_compile(magic, temp_text_file.c_str());
                    (void)compile_result;
                    
                    // Test magic_check on the compiled database
                    if (compile_result == 0) {
                        int check_result = magic_check(magic, temp_text_file.c_str());
                        (void)check_result;
                    }
                    
                    // Test magic_list to enumerate entries
                    magic_list(magic, temp_text_file.c_str());
                    
                    // Clean up temp files
                    unlink(temp_text_file.c_str());
                    unlink(temp_magic_file.c_str());
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Combined compression + magic database testing
            // Test magic_buffer with compression data after loading databases with FILE_DEFAULT
            
            if (test_combined_compression && fdp.remaining_bytes() >= 200) {
                // First create a simple magic file
                std::string temp_magic_file = "/tmp/fuzz_magic_comb_" + std::to_string(getpid()) + ".magic";
                std::string magic_content = "0 string  GIF8      GIF image\n";
                magic_content += "0 default           default entry\n";
                magic_content += "0 string  \\x89PNG   PNG image\n";
                
                FILE* fp = fopen(temp_magic_file.c_str(), "w");
                if (fp) {
                    fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
                    fclose(fp);
                    
                    // Compile the database
                    magic_compile(magic, temp_magic_file.c_str());
                    
                    // Load the database
                    magic_load(magic, temp_magic_file.c_str());
                    
                    // Generate compression test data
                    std::vector<uint8_t> comp_buffer;
                    
                    // Simple gzip header
                    comp_buffer.push_back(0x1F);
                    comp_buffer.push_back(0x8B);
                    comp_buffer.push_back(0x08);
                    
                    // Add some fuzzed data
                    size_t comp_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 500);
                    std::vector<uint8_t> comp_data = fdp.ConsumeBytes<uint8_t>(comp_data_size);
                    comp_buffer.insert(comp_buffer.end(), comp_data.begin(), comp_data.end());
                    
                    // Test magic_buffer with compression data
                    if (!comp_buffer.empty()) {
                        const char* result = magic_buffer(magic, comp_buffer.data(), comp_buffer.size());
                        (void)result;
                    }
                    
                    // Clean up
                    unlink(temp_magic_file.c_str());
                }
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Multiple compression formats in sequence
            // Test all compression formats that libmagic supports
            
            if (fdp.remaining_bytes() >= 300) {
                // Try to load default database
                const char* default_path = "/usr/share/misc/magic";
                magic_load(magic, default_path);
                
                // Test multiple compression formats
                const int num_formats = 7;
                for (int i = 0; i < num_formats && fdp.remaining_bytes() >= 20; i++) {
                    std::vector<uint8_t> format_buffer;
                    
                    // Create format-specific header
                    switch (i % num_formats) {
                        case 0: // gzip
                            format_buffer.push_back(0x1F);
                            format_buffer.push_back(0x8B);
                            format_buffer.push_back(0x08);
                            break;
                        case 1: // bzip2
                            format_buffer.push_back('B');
                            format_buffer.push_back('Z');
                            format_buffer.push_back('h');
                            format_buffer.push_back('1');
                            break;
                        case 2: // xz
                            format_buffer.push_back(0xFD);
                            format_buffer.push_back('7');
                            format_buffer.push_back('z');
                            format_buffer.push_back('X');
                            format_buffer.push_back('Z');
                            format_buffer.push_back(0x00);
                            break;
                        case 3: // zstd
                            format_buffer.push_back(0x28);
                            format_buffer.push_back(0xB5);
                            format_buffer.push_back(0x2F);
                            format_buffer.push_back(0xFD);
                            break;
                        case 4: // compress
                            format_buffer.push_back(0x1F);
                            format_buffer.push_back(0x9D);
                            break;
                        case 5: // lzip
                            format_buffer.push_back('L');
                            format_buffer.push_back('Z');
                            format_buffer.push_back('I');
                            format_buffer.push_back('P');
                            format_buffer.push_back(0x01);
                            break;
                        default: // raw
                            break;
                    }
                    
                    // Add some data
                    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(5, 50);
                    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
                    format_buffer.insert(format_buffer.end(), data.begin(), data.end());
                    
                    // Test with magic_buffer
                    if (!format_buffer.empty()) {
                        const char* result = magic_buffer(magic, format_buffer.data(), format_buffer.size());
                        (void)result;
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Strategy 4: Database compilation with compression references
            // Create magic files that reference compression patterns
            
            if (test_database_compilation && fdp.remaining_bytes() >= 150) {
                std::string temp_magic_file = "/tmp/fuzz_magic_comp_ref_" + std::to_string(getpid()) + ".magic";
                
                // Create magic file with compression pattern matching
                std::string magic_content;
                magic_content += "# Compression format tests\n";
                magic_content += "0string\\x1f\\x8b\\x08gzip compressed data\n";
                magic_content += "0stringBZhbzip2 compressed data\n";
                magic_content += "0string\\xfd7zXZ\\x00xz compressed data\n";
                magic_content += "0string\\x28\\xb5\\x2f\\xfdzstd compressed data\n";
                magic_content += "0string\\x1f\\x9dcompress (.Z) compressed data\n";
                magic_content += "0stringLZIPlzip compressed data\n";
                
                // Add FILE_DEFAULT entries
                magic_content += "0defaultdefault entry for compression testing\n";
                
                FILE* fp = fopen(temp_magic_file.c_str(), "w");
                if (fp) {
                    fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
                    fclose(fp);
                    
                    // Test compilation
                    int compile_result = magic_compile(magic, temp_magic_file.c_str());
                    (void)compile_result;
                    
                    // Test checking
                    int check_result = magic_check(magic, temp_magic_file.c_str());
                    (void)check_result;
                    
                    // Clean up
                    unlink(temp_magic_file.c_str());
                }
            }
            break;
        }
        
        default: {
            // Default strategy: Simple compression testing
            if (fdp.remaining_bytes() >= 50) {
                // Load default database
                const char* default_path = "/usr/share/misc/magic";
                magic_load(magic, default_path);
                
                // Create simple test buffer
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 500);
                std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Test magic_buffer
                if (!test_buffer.empty()) {
                    const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result;
                }
            }
            break;
        }
    }
    
    // Always test magic_list if we have a magic instance
    magic_list(magic, NULL);
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_018
