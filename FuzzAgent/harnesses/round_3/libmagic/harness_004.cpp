#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <algorithm>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_004 {
/*
 * Fuzzing harness for libmagic stress testing and edge cases
 * This harness focuses on robustness testing rather than API coverage:
 * 1) Multiple concurrent magic contexts
 * 2) Testing with corrupted/invalid magic database files
 * 3) Large file/buffer handling (size limits)
 * 4) Specific problematic file formats
 * 5) Testing all MAGIC flag combinations systematically
 * 6) Resource exhaustion scenarios (memory, file descriptors)
 * 
 * Based on analysis of existing harnesses:
 * - harness_000: Basic file detection
 * - harness_001: Parameter configuration
 * - harness_002: Magic file compilation lifecycle
 * - harness_003: Error handling and advanced flags
 * 
 * This harness complements by focusing on stress testing and systematic flag validation
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Maximum number of concurrent magic contexts to test
constexpr size_t MAX_CONCURRENT_CONTEXTS = 5;

// Flag combinations to test systematically
constexpr int FLAG_COMBINATIONS[][2] = {
    {MAGIC_NONE, 0},
    {MAGIC_SYMLINK, 0},
    {MAGIC_COMPRESS, 0},
    {MAGIC_DEVICES, 0},
    {MAGIC_MIME_TYPE, 0},
    {MAGIC_CONTINUE, 0},
    {MAGIC_CHECK, 0},
    {MAGIC_PRESERVE_ATIME, 0},
    {MAGIC_RAW, 0},
    {MAGIC_ERROR, 0},
    {MAGIC_MIME_ENCODING, 0},
    {MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING, 0}, // MAGIC_MIME
    {MAGIC_APPLE, 0},
    {MAGIC_EXTENSION, 0},
    {MAGIC_COMPRESS_TRANSP, 0},
    {MAGIC_NO_COMPRESS_FORK, 0},
    // NO_CHECK flags combinations
    {MAGIC_NO_CHECK_COMPRESS, 0},
    {MAGIC_NO_CHECK_TAR, 0},
    {MAGIC_NO_CHECK_SOFT, 0},
    {MAGIC_NO_CHECK_APPTYPE, 0},
    {MAGIC_NO_CHECK_ELF, 0},
    {MAGIC_NO_CHECK_TEXT, 0},
    {MAGIC_NO_CHECK_CDF, 0},
    {MAGIC_NO_CHECK_CSV, 0},
    {MAGIC_NO_CHECK_TOKENS, 0},
    {MAGIC_NO_CHECK_ENCODING, 0},
    {MAGIC_NO_CHECK_JSON, 0},
    {MAGIC_NO_CHECK_SIMH, 0},
    // Combined NO_CHECK flags
    {MAGIC_NO_CHECK_BUILTIN, 0},
    // Complex combinations
    {MAGIC_MIME | MAGIC_CONTINUE | MAGIC_CHECK, 0},
    {MAGIC_RAW | MAGIC_ERROR | MAGIC_SYMLINK, 0},
    {MAGIC_COMPRESS | MAGIC_DEVICES | MAGIC_PRESERVE_ATIME, 0}
};

constexpr size_t FLAG_COMBINATIONS_COUNT = sizeof(FLAG_COMBINATIONS) / sizeof(FLAG_COMBINATIONS[0]);
// Problematic file format signatures for edge case testing
constexpr const char* PROBLEMATIC_FORMATS[] = {
    "\x00\x00\x00\x00",  // All zeros
    "\xFF\xFF\xFF\xFF",  // All ones
    "\x89PNG\r\n\x1a\n", // PNG but corrupted
    "GIF87a",            // GIF but potentially malformed
    "GIF89a",
    "\xFF\xD8\xFF",      // JPEG start
    "\x7F" "ELF",        // ELF start (split to avoid hex escape issue)
    "BM",                // BMP start
    "PK\x03\x04",        // ZIP start
    "PK\x05\x06",        // ZIP end
    "PK\x07\x08",        // ZIP encrypted
    "%PDF-",             // PDF start
    "<!DOCTYPE",         // HTML
    "<?xml",             // XML
    "#!",                // Script
    "\xCA\xFE\xBA\xBE",  // Java class
    "\xFE\xED\xFA\xCE",  // Mach-O
    "\xCE\xFA\xED\xFE",  // Mach-O little endian
    "\xCF\xFA\xED\xFE",  // Mach-O 64-bit
    "\xFE\xED\xFA\xCF"   // Mach-O 64-bit little endian
};

constexpr size_t PROBLEMATIC_FORMATS_COUNT = sizeof(PROBLEMATIC_FORMATS) / sizeof(PROBLEMATIC_FORMATS[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for stress testing
    // Minimum: operation selection (1 byte) + some test data
    if (size < 2) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario from fuzzed input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (scenario) {
        case 0: {
            // ============================================================
            // SCENARIO 1: Multiple concurrent magic contexts
            // ============================================================
            
            size_t num_contexts = fdp.ConsumeIntegralInRange<size_t>(1, MAX_CONCURRENT_CONTEXTS);
            std::vector<magic_t> contexts;
            contexts.reserve(num_contexts);
            
            // Create multiple contexts with different flags
            for (size_t i = 0; i < num_contexts && fdp.remaining_bytes() > 0; ++i) {
                int flags = 0;
                
                // Consume flags for this context
                if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
                if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
                if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
                if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
                if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
                if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
                if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
                if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
                
                magic_t magic = magic_open(flags);
                if (magic != nullptr) {
                    contexts.push_back(magic);
                    
                    // Load default magic database if possible
                    magic_load(magic, nullptr);
                    
                    // Test with some data if available
                    if (fdp.remaining_bytes() > 10) {
                        size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                            std::min<size_t>(100, fdp.remaining_bytes()));
                        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                        magic_buffer(magic, test_data.data(), test_size);
                    }
                }
            }
            
            // Clean up all contexts
            for (magic_t magic : contexts) {
                magic_close(magic);
            }
            break;
        }
        
        case 1: {
            // ============================================================
            // SCENARIO 2: Corrupted/invalid magic database files
            // ============================================================
            
            // Create magic handle
            magic_t magic = magic_open(MAGIC_NONE);
            if (magic == nullptr) {
                return 0;
            }
            
            // Generate corrupted magic database from fuzzed input
            size_t magic_size = fdp.ConsumeIntegralInRange<size_t>(10, 1024);
            if (fdp.remaining_bytes() < magic_size) {
                magic_size = fdp.remaining_bytes();
            }
            
            std::string corrupted_magic = fdp.ConsumeBytesAsString(magic_size);
            
            // Write corrupted magic to temporary file
            char magic_template[] = "/tmp/corrupted_magic_XXXXXX";
            int magic_fd = mkstemp(magic_template);
            if (magic_fd != -1) {
                write(magic_fd, corrupted_magic.c_str(), corrupted_magic.size());
                fsync(magic_fd);
                close(magic_fd);
                
                // Try to load corrupted magic database
                // This should test error handling paths
                magic_load(magic, magic_template);
                
                // Even if load fails, try to use the handle
                if (fdp.remaining_bytes() > 0) {
                    size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                        std::min<size_t>(100, fdp.remaining_bytes()));
                    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                    magic_buffer(magic, test_data.data(), test_size);
                }
                
                // Clean up
                unlink(magic_template);
            }
            
            magic_close(magic);
            break;
        }
        
        case 2: {
            // ============================================================
            // SCENARIO 3: Large file/buffer handling
            // ============================================================
            
            magic_t magic = magic_open(MAGIC_NONE);
            if (magic == nullptr) {
                return 0;
            }
            
            // Load default database
            magic_load(magic, nullptr);
            
            // Test with various buffer sizes, including very large ones
            // Use remaining bytes as test data
            size_t remaining = fdp.remaining_bytes();
            if (remaining > 0) {
                // Test with different chunk sizes
                size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                    std::min<size_t>(65536, remaining));
                
                if (chunk_size > 0) {
                    std::vector<uint8_t> large_buffer = fdp.ConsumeBytes<uint8_t>(chunk_size);
                    
                    // Test magic_buffer with large data
                    const char* result = magic_buffer(magic, large_buffer.data(), large_buffer.size());
                    (void)result;
                    
                    // Test magic_file with large data if we have enough left
                    if (fdp.remaining_bytes() > 1000) {
                        size_t file_size = fdp.ConsumeIntegralInRange<size_t>(100, 5000);
                        if (fdp.remaining_bytes() >= file_size) {
                            std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
                            
                            char file_template[] = "/tmp/large_file_XXXXXX";
                            int file_fd = mkstemp(file_template);
                            if (file_fd != -1) {
                                write(file_fd, file_data.data(), file_data.size());
                                fsync(file_fd);
                                close(file_fd);
                                
                                magic_file(magic, file_template);
                                unlink(file_template);
                            }
                        }
                    }
                }
            }
            
            magic_close(magic);
            break;
        }
        
        case 3: {
            // ============================================================
            // SCENARIO 4: Specific problematic file formats
            // ============================================================
            
            magic_t magic = magic_open(MAGIC_NONE);
            if (magic == nullptr) {
                magic = magic_open(MAGIC_NONE);
                if (magic == nullptr) {
                    return 0;
                }
            }
            
            magic_load(magic, nullptr);
            
            // Select problematic format based on fuzzed input
            size_t format_idx = fdp.ConsumeIntegralInRange<size_t>(0, 
                PROBLEMATIC_FORMATS_COUNT - 1);
            
            const char* format_sig = PROBLEMATIC_FORMATS[format_idx];
            size_t sig_len = strlen(format_sig);
            
            // Create test buffer with problematic format signature
            std::vector<uint8_t> test_buffer;
            if (fdp.remaining_bytes() > 0) {
                // Start with the problematic signature
                test_buffer.insert(test_buffer.end(), 
                    reinterpret_cast<const uint8_t*>(format_sig),
                    reinterpret_cast<const uint8_t*>(format_sig) + sig_len);
                
                // Append fuzzed data
                size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 
                    std::min<size_t>(1024, fdp.remaining_bytes()));
                if (extra_size > 0) {
                    std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_size);
                    test_buffer.insert(test_buffer.end(), extra_data.begin(), extra_data.end());
                }
                
                // Test the buffer
                magic_buffer(magic, test_buffer.data(), test_buffer.size());
            }
            
            magic_close(magic);
            break;
        }
        
        case 4: {
            // ============================================================
            // SCENARIO 5: Systematic flag combination testing
            // ============================================================
            
            // Select flag combination to test
            size_t flag_idx = fdp.ConsumeIntegralInRange<size_t>(0, 
                FLAG_COMBINATIONS_COUNT - 1);
            
            int flags = FLAG_COMBINATIONS[flag_idx][0];
            
            // Also add some random flags from remaining input
            if (fdp.ConsumeBool()) flags |= MAGIC_DEBUG;
            if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
            if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
            if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
            if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
            if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
            if (fdp.ConsumeBool()) flags |= MAGIC_CHECK;
            if (fdp.ConsumeBool()) flags |= MAGIC_PRESERVE_ATIME;
            if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
            if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
            if (fdp.ConsumeBool()) flags |= MAGIC_MIME_ENCODING;
            if (fdp.ConsumeBool()) flags |= MAGIC_APPLE;
            if (fdp.ConsumeBool()) flags |= MAGIC_EXTENSION;
            if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS_TRANSP;
            if (fdp.ConsumeBool()) flags |= MAGIC_NO_COMPRESS_FORK;
            
            magic_t magic = magic_open(flags);
            if (magic == nullptr) {
                // Try with simpler flags if complex combination fails
                magic = magic_open(MAGIC_NONE);
                if (magic == nullptr) {
                    return 0;
                }
            }
            
            // Load database
            magic_load(magic, nullptr);
            
            // Test with fuzzed data
            if (fdp.remaining_bytes() > 0) {
                size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                    std::min<size_t>(4096, fdp.remaining_bytes()));
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Test multiple API calls with the same handle
                magic_buffer(magic, test_data.data(), test_data.size());
                
                // Check error state
                const char* error = magic_error(magic);
                (void)error;
                
                int err = magic_errno(magic);
                (void)err;
                
                // Test file descriptor API if we have a file descriptor
                if (test_size > 10) {
                    char file_template[] = "/tmp/flag_test_XXXXXX";
                    int file_fd = mkstemp(file_template);
                    if (file_fd != -1) {
                        write(file_fd, test_data.data(), std::min<size_t>(test_size, 100));
                        fsync(file_fd);
                        
                        // Test magic_descriptor
                        magic_descriptor(magic, file_fd);
                        
                        close(file_fd);
                        unlink(file_template);
                    }
                }
            }
            
            magic_close(magic);
            break;
        }
        
        case 5: {
            // ============================================================
            // SCENARIO 6: Resource exhaustion testing
            // ============================================================
            
            // Test creating many magic handles to exhaust resources
            size_t max_handles = fdp.ConsumeIntegralInRange<size_t>(1, 20);
            std::vector<magic_t> handles;
            
            for (size_t i = 0; i < max_handles; ++i) {
                magic_t magic = magic_open(MAGIC_NONE);
                if (magic != nullptr) {
                    handles.push_back(magic);
                    
                    // Load database on some handles
                    if (i % 3 == 0) {
                        magic_load(magic, nullptr);
                    }
                    
                    // Use some handles
                    if (i % 2 == 0 && fdp.remaining_bytes() > 10) {
                        size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                            std::min<size_t>(100, fdp.remaining_bytes()));
                        if (test_size > 0) {
                            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                            magic_buffer(magic, test_data.data(), test_data.size());
                        }
                    }
                } else {
                    // Handle creation failed - this tests resource exhaustion
                    break;
                }
            }
            
            // Clean up all handles
            for (magic_t magic : handles) {
                magic_close(magic);
            }
            
            // Test file descriptor exhaustion by creating many temp files
            size_t num_temp_files = fdp.ConsumeIntegralInRange<size_t>(0, 10);
            std::vector<std::string> temp_files;
            
            for (size_t i = 0; i < num_temp_files && fdp.remaining_bytes() > 10; ++i) {
                char file_template[] = "/tmp/exhaust_XXXXXX";
                int fd = mkstemp(file_template);
                if (fd != -1) {
                    temp_files.push_back(file_template);
                    
                    // Write some data
                    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                    if (fdp.remaining_bytes() >= write_size) {
                        std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                        write(fd, write_data.data(), write_data.size());
                    }
                    
                    close(fd);
                }
            }
            
            // Create one magic handle and test with all temp files
            magic_t magic = magic_open(MAGIC_NONE);
            if (magic != nullptr) {
                magic_load(magic, nullptr);
                
                for (const std::string& filename : temp_files) {
                    magic_file(magic, filename.c_str());
                }
                
                magic_close(magic);
            }
            
            // Clean up temp files
            for (const std::string& filename : temp_files) {
                unlink(filename.c_str());
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_004
