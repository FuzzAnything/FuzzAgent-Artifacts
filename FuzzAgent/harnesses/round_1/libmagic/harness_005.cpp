#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cfloat>
#include <cmath>
#include <array>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

namespace harness_005 {
/*
 * Specialized fuzzing harness for libmagic targeting deep internal coverage gaps:
 * - magiccheck() float/double comparison branches (152 blocked branches, 0% coverage)
 * - apprentice.c parsing functions (parse_mime, parse_extra, parse_apple, parse_ext)
 * - compress.c decompression functions (uncompressgzipped, uncompresszlib, etc.)
 * 
 * Primary targets:
 * - magiccheck() FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT, FILE_DOUBLE, FILE_BEDOUBLE, FILE_LEDOUBLE
 * - apprentice.c parsing pipeline via magic_compile and magic_load_buffers
 * - compress.c decompression via MAGIC_COMPRESS flag and compressed test data
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_load_buffers, magic_buffer, magic_close
 * 
 * Strategy:
 * 1. Generate custom magic database buffers with various entry types using fuzzed values
 * 2. Include MIME, APPLE, EXTENSION, and special float/double entries
 * 3. Test compressed file formats (gzip, zlib, bzip2, zstd, xz) via MAGIC_COMPRESS flag
 * 4. Validate and compile databases using magic_check/magic_compile
 * 5. Load with magic_load_buffers and test with magic_buffer
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_001: Parameter configuration functions  
 * - harness_002: File/descriptor operations
 * - harness_003: Custom magic database compilation + float/double (has hardcoded values)
 * - harness_004: Focused float comparison branch testing
 * - harness_005: Apprentice parsing + compress decompression + float/double (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + strategy + database content + test buffer data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 different testing strategies
    bool enable_compress = fdp.ConsumeBool();
    bool test_apprentice_parsing = fdp.ConsumeBool();
    bool use_magic_load_buffers = fdp.ConsumeBool();
    
    // Build flags with emphasis on compression and checking
    int actual_flags = flags & 0xFFFFF;
    if (enable_compress) {
        actual_flags |= MAGIC_COMPRESS;
    }
    if (test_apprentice_parsing) {
        actual_flags |= MAGIC_CHECK;  // Enable syntax checking for apprentice functions
    }
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy selection
    switch (strategy) {
        case 0: {
            // Strategy 0: Comprehensive apprentice.c parsing with all entry types
            std::vector<std::string> magic_buffers;
            std::vector<size_t> buffer_sizes;
            
            // Generate magic database with various entry types using fuzzed values
            std::string magic_db;
            
            // Add standard magic entries first
            magic_db += "0  string  MZ          MS-DOS executable\n";
            magic_db += "0  string  #!          script text\n";
            magic_db += "0  string  %PDF        PDF document\n";
            
            // Generate random float/double values for magic patterns
            std::vector<float> float_values;
            std::vector<double> double_values;
            
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                float_values.push_back(val);
            }
            
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(double); i++) {
                double val = fdp.ConsumeFloatingPoint<double>();
                double_values.push_back(val);
            }
            
            // Add float entries with various relations and types
            for (size_t i = 0; i < float_values.size(); i++) {
                int offset = (i * 4) % 32;
                float val = float_values[i];
                
                // FILE_FLOAT patterns
                magic_db += std::to_string(offset) + " float " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " float != " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " float > " + std::to_string(val/2.0f) + "\n";
                magic_db += std::to_string(offset) + " float < " + std::to_string(val*2.0f) + "\n";
                
                // Endian-specific float patterns
                magic_db += std::to_string(offset) + " befloat " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " lefloat " + std::to_string(val) + "\n";
                
                // Special IEEE 754 values
                if (fdp.ConsumeBool()) {
                    magic_db += std::to_string(offset) + " float +NaN\n";
                    magic_db += std::to_string(offset) + " float -NaN\n";
                    magic_db += std::to_string(offset) + " float +Inf\n";
                    magic_db += std::to_string(offset) + " float -Inf\n";
                }
            }
            
            // Add double entries
            for (size_t i = 0; i < double_values.size(); i++) {
                int offset = (i * 8) % 64;
                double val = double_values[i];
                
                // FILE_DOUBLE patterns
                magic_db += std::to_string(offset) + " double " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " double != " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " double > " + std::to_string(val/2.0) + "\n";
                magic_db += std::to_string(offset) + " double < " + std::to_string(val*2.0) + "\n";
                
                // Endian-specific double patterns
                magic_db += std::to_string(offset) + " bedouble " + std::to_string(val) + "\n";
                magic_db += std::to_string(offset) + " ledouble " + std::to_string(val) + "\n";
            }
            
            // Add apprentice.c parsing-specific entries (MIME, APPLE, EXTENSION)
            // These will exercise parse_mime, parse_apple, parse_ext functions
            
            // MIME type entries (exercise parse_mime)
            magic_db += "!:mime application/pdf\n";
            magic_db += "!:mime text/plain\n";
            magic_db += "!:mime image/jpeg\n";
            
            // Generate random MIME types from fuzzer input
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string mime_type = fdp.ConsumeRandomLengthString(50);
                if (!mime_type.empty()) {
                    // Simple sanitization for MIME type format
                    std::replace(mime_type.begin(), mime_type.end(), ' ', '-');
                    std::replace(mime_type.begin(), mime_type.end(), '\n', '-');
                    magic_db += "!:mime " + mime_type + "\n";
                }
            }
            
            // APPLE creator/type entries (exercise parse_apple)
            magic_db += "!:apple CREA/TYPE\n";
            magic_db += "!:apple TEXT/TEXT\n";
            
            // EXTENSION entries (exercise parse_ext)
            magic_db += "!:ext pdf,txt,jpg\n";
            magic_db += "!:ext exe,bin,dll\n";
            
            // Generate random extension lists
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 5; i++) {
                std::string ext_list = fdp.ConsumeRandomLengthString(30);
                if (!ext_list.empty()) {
                    // Simple sanitization for extension format
                    std::replace(ext_list.begin(), ext_list.end(), ' ', ',');
                    std::replace(ext_list.begin(), ext_list.end(), '\n', ',');
                    magic_db += "!:ext " + ext_list + "\n";
                }
            }
            
            // Prepare buffers for magic_load_buffers
            magic_buffers.push_back(magic_db);
            buffer_sizes.push_back(magic_db.size());
            
            // Create a second buffer with additional magic entries
            if (fdp.remaining_bytes() > 50) {
                std::string magic_db2;
                magic_db2 += "0  string  \\x89PNG     PNG image\n";
                magic_db2 += "0  string  GIF       GIF image\n";
                
                // Add more float patterns with different offsets
                if (!float_values.empty()) {
                    magic_db2 += "16 float " + std::to_string(float_values[0]) + "\n";
                    magic_db2 += "32 befloat " + std::to_string(float_values[0]) + "\n";
                }
                
                magic_buffers.push_back(magic_db2);
                buffer_sizes.push_back(magic_db2.size());
            }
            
            // Convert to arrays for magic_load_buffers
            const size_t num_buffers = magic_buffers.size();
            void** buffers = new void*[num_buffers];
            size_t* sizes = new size_t[num_buffers];
            
            for (size_t i = 0; i < num_buffers; i++) {
                buffers[i] = malloc(buffer_sizes[i]);
                if (buffers[i]) {
                    memcpy(buffers[i], magic_buffers[i].c_str(), buffer_sizes[i]);
                    sizes[i] = buffer_sizes[i];
                }
            }
            
            // Create temporary file for magic_compile if needed
            std::string temp_filename = "/tmp/magic_fuzz_005_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp && !magic_db.empty()) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                // Validate database syntax (exercises apprentice.c parsing)
                if (test_apprentice_parsing) {
                    magic_check(magic, temp_filename.c_str());
                }
                
                // Compile database (exercises full apprentice.c pipeline)
                magic_compile(magic, temp_filename.c_str());
                
                // Clean up
                unlink(temp_filename.c_str());
            }
            
            // Load magic database using magic_load_buffers (preferred) or magic_load
            if (use_magic_load_buffers && num_buffers > 0) {
                magic_load_buffers(magic, buffers, sizes, num_buffers);
            }
            
            // Generate test buffers that may trigger compression detection
            // Create data with compressed file signatures to exercise compress.c
            std::vector<uint8_t> test_buffer;
            
            // Always include some regular data
            size_t regular_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            if (fdp.remaining_bytes() >= regular_size) {
                std::vector<uint8_t> regular_data = fdp.ConsumeBytes<uint8_t>(regular_size);
                test_buffer.insert(test_buffer.end(), regular_data.begin(), regular_data.end());
            }
            
            // Add compressed file signatures to trigger compress.c functions
            if (enable_compress && fdp.ConsumeBool()) {
                // GZIP signature (\037\213)
                test_buffer.push_back(0x1F);
                test_buffer.push_back(0x8B);
                
                // Add some compressed-like data
                for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                    test_buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
                
                // Try other compression signatures
                if (fdp.ConsumeBool()) {
                    // BZIP2 signature ("BZh")
                    test_buffer.push_back('B');
                    test_buffer.push_back('Z');
                    test_buffer.push_back('h');
                    
                    // Add more data
                    for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                        test_buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                    }
                }
                
                if (fdp.ConsumeBool()) {
                    // ZLIB header (deflate)
                    test_buffer.push_back(0x78);
                    test_buffer.push_back(0x9C);
                    
                    // Add more data
                    for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                        test_buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                    }
                }
            }
            
            // Test with magic_buffer
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result; // Result can be null, that's OK
            }
            
            // Clean up buffers
            for (size_t i = 0; i < num_buffers; i++) {
                if (buffers[i]) free(buffers[i]);
            }
            delete[] buffers;
            delete[] sizes;
            
            break;
        }
        
        case 1: {
            // Strategy 1: Focus on compression testing with various formats
            // Set compression flag explicitly
            magic_setflags(magic, actual_flags | MAGIC_COMPRESS);
            
            // Create test data with various compression signatures
            std::vector<uint8_t> compressed_test_data;
            
            // Start with GZIP signature (most common)
            compressed_test_data.push_back(0x1F);  // \037
            compressed_test_data.push_back(0x8B);  // \213
            
            // Add DEFLATE compression method
            compressed_test_data.push_back(0x08);  // DEFLATE
            
            // Add flags (example: FNAME)
            compressed_test_data.push_back(0x08);  // FNAME flag
            
            // Add MTIME (4 bytes)
            for (int i = 0; i < 4 && fdp.remaining_bytes() > 0; i++) {
                compressed_test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Add XFL and OS
            compressed_test_data.push_back(0x00);  // XFL
            compressed_test_data.push_back(0x03);  // OS (Unix)
            
            // Add optional filename if FNAME is set
            compressed_test_data.push_back('t');
            compressed_test_data.push_back('e');
            compressed_test_data.push_back('s');
            compressed_test_data.push_back('t');
            compressed_test_data.push_back('.');
            compressed_test_data.push_back('t');
            compressed_test_data.push_back('x');
            compressed_test_data.push_back('t');
            compressed_test_data.push_back(0x00);  // Null terminator
            
            // Add some compressed data (simplified)
            for (int i = 0; i < 50 && fdp.remaining_bytes() > 0; i++) {
                compressed_test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Add CRC32 and ISIZE (simplified)
            for (int i = 0; i < 8; i++) {
                compressed_test_data.push_back(0x00);
            }
            
            // Test with magic_buffer
            if (!compressed_test_data.empty()) {
                const char* result = magic_buffer(magic, compressed_test_data.data(), compressed_test_data.size());
                (void)result;
            }
            
            // Try with BZIP2 signature
            std::vector<uint8_t> bzip2_data;
            bzip2_data.push_back('B');
            bzip2_data.push_back('Z');
            bzip2_data.push_back('h');
            
            // Add version
            bzip2_data.push_back('1');
            
            // Add blocksize
            bzip2_data.push_back('1');
            
            // Add some data
            for (int i = 0; i < 50 && fdp.remaining_bytes() > 0; i++) {
                bzip2_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            if (!bzip2_data.empty()) {
                const char* result2 = magic_buffer(magic, bzip2_data.data(), bzip2_data.size());
                (void)result2;
            }
            
            break;
        }
        
        case 2: {
            // Strategy 2: Test with empty/null buffers (edge cases)
            // Test magic_load_buffers with empty buffers
            void* empty_buffers[1] = {nullptr};
            size_t empty_sizes[1] = {0};
            magic_load_buffers(magic, empty_buffers, empty_sizes, 1);
            
            // Test magic_buffer with empty data
            const char* result = magic_buffer(magic, nullptr, 0);
            (void)result;
            
            // Test with minimal data
            if (fdp.remaining_bytes() > 0) {
                uint8_t single_byte = fdp.ConsumeIntegral<uint8_t>();
                const char* result2 = magic_buffer(magic, &single_byte, 1);
                (void)result2;
            }
            
            break;
        }
        
        case 3: {
            // Strategy 3: Mixed approach with file operations
            // Create a temporary file with magic database content
            std::string temp_db_file = "/tmp/magic_db_005_" + std::to_string(getpid()) + ".mgc";
            
            // Generate simple magic database with float entries
            std::string simple_db;
            
            // Add some float patterns with random values
            for (int i = 0; i < 3 && fdp.remaining_bytes() >= sizeof(float); i++) {
                float val = fdp.ConsumeFloatingPoint<float>();
                simple_db += std::to_string(i * 4) + " float " + std::to_string(val) + "\n";
            }
            
            // Add MIME type
            simple_db += "!:mime application/octet-stream\n";
            
            // Write to file
            FILE* db_fp = fopen(temp_db_file.c_str(), "w");
            if (db_fp) {
                fwrite(simple_db.c_str(), 1, simple_db.size(), db_fp);
                fclose(db_fp);
                
                // Compile and check
                if (test_apprentice_parsing) {
                    magic_check(magic, temp_db_file.c_str());
                }
                magic_compile(magic, temp_db_file.c_str());
                
                // Clean up
                unlink(temp_db_file.c_str());
            }
            
            // Generate test data with mixed content
            std::vector<uint8_t> mixed_data;
            
            // Add some text
            std::string text_part = "Test data for libmagic fuzzing";
            mixed_data.insert(mixed_data.end(), text_part.begin(), text_part.end());
            
            // Add some binary data
            for (int i = 0; i < 20 && fdp.remaining_bytes() > 0; i++) {
                mixed_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Add float values at specific offsets
            if (fdp.remaining_bytes() >= sizeof(float) * 2) {
                float f1 = fdp.ConsumeFloatingPoint<float>();
                float f2 = fdp.ConsumeFloatingPoint<float>();
                
                // Convert floats to bytes and add to buffer
                uint8_t* f1_bytes = reinterpret_cast<uint8_t*>(&f1);
                uint8_t* f2_bytes = reinterpret_cast<uint8_t*>(&f2);
                
                // Add at offset 0 and 4 to match our magic patterns
                if (mixed_data.size() < 8) {
                    mixed_data.resize(8, 0);
                }
                
                for (int i = 0; i < 4; i++) {
                    if (0 + i < mixed_data.size()) mixed_data[0 + i] = f1_bytes[i];
                    if (4 + i < mixed_data.size()) mixed_data[4 + i] = f2_bytes[i];
                }
            }
            
            // Test with magic_buffer
            if (!mixed_data.empty()) {
                const char* result = magic_buffer(magic, mixed_data.data(), mixed_data.size());
                (void)result;
            }
            
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_005
