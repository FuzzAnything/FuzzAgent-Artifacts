#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
#include <memory>

namespace harness_012 {
/*
 * libmagic fuzzing harness #12 - Magic database compilation and soft magic matching
 * Target: apprentice.c (magic database compiler) and softmagic.c (soft magic matching)
 * Additional modules: funcs.c, print.c, is_simh.c, time modules (asctime_r.c, ctime_r.c, gmtime_r.c, localtime_r.c)
 * 
 * This harness specifically targets magic database compilation and soft magic matching:
 * 1. Test magic database compilation from fuzzer-provided magic rules (FILE_COMPILE action)
 * 2. Test soft magic matching with various patterns and edge cases  
 * 3. Exercise magic_list() for database inspection
 * 4. Test magic_check() for database validation with MAGIC_CHECK flag
 * 5. Test magic_setparam() for database limits configuration
 * 6. Include edge cases: invalid magic rules, large databases, corrupted patterns
 * 
 * Semantically distinct from previous harnesses:
 * - harness_000: General buffer detection and flag manipulation
 * - harness_001: File-based detection and parameter configuration  
 * - harness_002: CDF-specific detection
 * - harness_003: Compression module testing
 * - harness_004: MIME and encoding detection
 * - harness_005: Apple and extension detection
 * - harness_006: Text and CSV detection
 * - harness_007: JSON and token detection
 * - harness_008: Soft magic patterns (basic)
 * - harness_009: Apprentice loading (basic)
 * - harness_010: Time module integration
 * - harness_011: DER/ASN.1 detection
 * - harness_012: Magic database compilation, soft magic matching, time modules integration
 * 
 * Key focus areas:
 * - Exercise apprentice.c with FILE_COMPILE, FILE_CHECK, FILE_LIST actions
 * - Trigger softmagic.c pattern matching with complex magic rules
 * - Test funcs.c utility functions through magic rule parsing
 * - Exercise print.c output formatting
 * - Test is_simh.c SIMH tape file detection
 * - Integrate time modules for timestamp parsing in magic rules
 */



// Temporary file utility for magic database files
class TemporaryFile {
public:
    TemporaryFile(const uint8_t* data, size_t size) {
        char filename[] = "/tmp/magic_db_XXXXXX";
        fd = mkstemp(filename);
        if (fd < 0) {
            return;
        }
        
        strncpy(path, filename, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
        
        ssize_t written = write(fd, data, size);
        if (written < 0) {
            close(fd);
            fd = -1;
            return;
        }
        
        close(fd);
        fd = -1;
    }
    
    ~TemporaryFile() {
        if (path[0] != '\0') {
            unlink(path);
        }
    }
    
    const char* getPath() const { return path; }
    
private:
    char path[256] = {0};
    int fd = -1;
};

// Generate various types of magic rules for testing
std::string generate_magic_rules(FuzzedDataProvider& fdp, int rule_type) {
    std::string rules;
    
    switch (rule_type % 6) {
        case 0:
            // Simple file type detection rules
            rules = "# Simple magic rules\n";
            rules += "0 string \\x89PNG\\r\\n\\x1a\\n PNG image data\n";
            rules += "0 string GIF87a GIF image data, version 87a\n";
            rules += "0 string GIF89a GIF image data, version 89a\n";
            rules += "0 string BM      BMP image data\n";
            rules += "0 string \\xff\\xd8\\xff JPEG image data\n";
            rules += "0 string \\x25\\x50\\x44\\x46 PDF document\n";
            rules += "0 string \\x7fELF ELF executable\n";
            rules += "0 belong 0xd0cf11e0 Composite Document File V2 Document\n";
            rules += "0 string MZ      DOS executable (EXE)\n";
            break;
            
        case 1:
            // Complex soft magic rules with offsets and conditions
            rules = "# Complex soft magic rules\n";
            rules += "0 beshort 0x4d5a          MZ executable (DOS)\n";
            rules += ">0x18 leshort <0x40       \\b, not a COM file\n";
            rules += ">0x18 leshort >0x40       \\b, real mode\n";
            rules += ">0x3c lelong  >0x40       \\b, PE executable\n";
            rules += ">0x3c lelong  <0x40       \\b, invalid\n";
            rules += "\n";
            rules += "0 string PK\\x03\\x04      Zip archive data\n";
            rules += ">0x06 beshort             \\b, v%d.%d\n";
            rules += ">0x04 leshort             \\b, need v%d.%d to extract\n";
            rules += "\n";
            rules += "0 string Rar!             RAR archive\n";
            rules += ">0x07 byte                \\b, v%d\n";
            rules += ">0x08 byte                \\b, revision %d\n";
            break;
            
        case 2:
            // Numeric and date-based rules (targets time modules)
            rules = "# Numeric and date-based rules\n";
            rules += "0 belong 0x04034b50       Zip archive (endian independent)\n";
            rules += "0 lelong 0x04034b50       Zip archive (little-endian)\n";
            rules += "0 belong 0x504b0304       Zip archive (big-endian)\n";
            rules += "\n";
            rules += "# Unix timestamp rules\n";
            rules += "0 belong x                \\b, timestamp %lu\n";
            rules += ">0 belong x               \\b, value %lu\n";
            rules += "\n";
            rules += "# Floating point rules\n";
            rules += "0 lefloat x               \\b, float %g\n";
            rules += "0 bedouble x              \\b, double %g\n";
            rules += "\n";
            rules += "# Date and time rules (targets time modules)\n";
            rules += "0 belong x                \\b, date: ";
            rules += ">0 string/c               \\b%s\n";
            break;
            
        case 3:
            // SIMH tape format rules (targets is_simh.c)
            rules = "# SIMH tape format rules\n";
            rules += "0 string simh             SIMH tape image\n";
            rules += ">4 belong                 \\b, block size %lu\n";
            rules += ">8 belong                 \\b, record count %lu\n";
            rules += ">12 string/c              \\b, label: %s\n";
            rules += "\n";
            rules += "# More tape formats\n";
            rules += "0 string TAPE             Generic tape image\n";
            rules += "0 string MTF              Microsoft Tape Format\n";
            break;
            
        case 4:
            // Invalid/corrupted magic rules for edge case testing
            rules = "# Invalid/corrupted magic rules\n";
            rules += "0 invalid_token           Invalid token at start\n";
            rules += ">1000 string test         Very large offset\n";
            rules += "0 string                  Missing pattern\n";
            rules += "0                         Missing type and pattern\n";
            rules += "0 string test\\x00         Null byte in pattern\n";
            rules += "0 string test\n";
            rules += "                          Missing description\n";
            rules += "0 belong 0x12345678       \\b, %s format string without args\n";
            rules += "0 byte 0xff               \\b, continuation line without parent\n";
            break;
            
        case 5:
            // Large magic database for stress testing
            rules = "# Large magic database (stress test)\n";
            for (int i = 0; i < 100; i++) {
                char rule[256];
                snprintf(rule, sizeof(rule), 
                    "0 byte 0x%02x               Test rule %d: byte 0x%02x\n",
                    i % 256, i, i % 256);
                rules += rule;
                
                if (i % 10 == 0) {
                    snprintf(rule, sizeof(rule),
                        ">0x%02x lelong x            \\b, offset value %%lu\n", i);
                    rules += rule;
                }
            }
            break;
    }
    
    // Add some fuzzer-generated content to the rules
    std::string fuzzer_content = fdp.ConsumeRandomLengthString(512);
    if (!fuzzer_content.empty()) {
        rules += "\n# Fuzzer-generated content\n";
        rules += fuzzer_content;
    }
    
    return rules;
}

// Generate test content for soft magic matching
std::vector<uint8_t> generate_test_content(FuzzedDataProvider& fdp, int content_type) {
    std::vector<uint8_t> content;
    
    switch (content_type % 8) {
        case 0:
            // PNG-like content
            content = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
            break;
            
        case 1:
            // GIF-like content  
            content = {'G', 'I', 'F', '8', '9', 'a'};
            break;
            
        case 2:
            // PDF-like content
            content = {0x25, 0x50, 0x44, 0x46};
            break;
            
        case 3:
            // ELF-like content
            content = {0x7f, 'E', 'L', 'F'};
            break;
            
        case 4:
            // ZIP-like content
            content = {'P', 'K', 0x03, 0x04};
            break;
            
        case 5:
            // BMP-like content
            content = {'B', 'M'};
            break;
            
        case 6:
            // SIMH-like content
            content = {'s', 'i', 'm', 'h'};
            break;
            
        case 7:
            // Random fuzzer-generated content
            size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            content = fdp.ConsumeBytes<uint8_t>(content_size);
            break;
    }
    
    // Add more fuzzer data to the content
    size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    if (extra_size > 0) {
        std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_size);
        content.insert(content.end(), extra.begin(), extra.end());
    }
    
    return content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 11);
    int rule_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    int content_type = fdp.ConsumeIntegralInRange<int>(0, 7);
    
    // Mask flags to valid MAGIC_* values, always include MAGIC_CHECK for validation
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION |
              MAGIC_NO_CHECK_COMPRESS | MAGIC_NO_CHECK_TAR | 
              MAGIC_NO_CHECK_SOFT | MAGIC_NO_CHECK_APPTYPE |
              MAGIC_NO_CHECK_ELF | MAGIC_NO_CHECK_TEXT |
              MAGIC_NO_CHECK_CDF | MAGIC_NO_CHECK_TOKENS |
              MAGIC_NO_CHECK_SIMH);
    
    // Ensure MAGIC_CHECK is set for validation testing
    if (operation % 3 == 0) {
        flags |= MAGIC_CHECK;
    }
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Generate magic rules and test content
    std::string magic_rules = generate_magic_rules(fdp, rule_type);
    std::vector<uint8_t> test_content = generate_test_content(fdp, content_type);
    
    // Create temporary files
    TemporaryFile magic_db_file(reinterpret_cast<const uint8_t*>(magic_rules.c_str()), magic_rules.size());
    TemporaryFile content_test_file(test_content.data(), test_content.size());
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test magic_compile() - compile magic database from rules
            if (magic_db_file.getPath()[0] != '\0') {
                int compile_result = magic_compile(magic, magic_db_file.getPath());
                (void)compile_result;
                
                // Test detection with compiled database
                if (content_test_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, content_test_file.getPath());
                    (void)result;
                }
            }
            break;
            
        case 1:
            // Test magic_check() - validate magic database
            if (magic_db_file.getPath()[0] != '\0') {
                int check_result = magic_check(magic, magic_db_file.getPath());
                (void)check_result;
            }
            break;
            
        case 2:
            // Test magic_list() - list magic database contents
            if (magic_db_file.getPath()[0] != '\0') {
                int list_result = magic_list(magic, magic_db_file.getPath());
                (void)list_result;
            }
            break;
            
        case 3:
            // Test magic_load() with custom magic database
            if (magic_db_file.getPath()[0] != '\0') {
                magic_load(magic, magic_db_file.getPath());
                
                // Test detection with custom database
                if (content_test_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, content_test_file.getPath());
                    (void)result;
                }
            }
            break;
            
        case 4:
            // Test magic_load_buffers() with custom magic rules
            {
                std::vector<uint8_t> magic_buffer(magic_rules.begin(), magic_rules.end());
                void* buffers[1] = {magic_buffer.data()};
                size_t sizes[1] = {magic_buffer.size()};
                
                magic_t custom_magic = magic_open(MAGIC_NONE);
                if (custom_magic) {
                    magic_load_buffers(custom_magic, buffers, sizes, 1);
                    
                    // Test detection with buffer-loaded database
                    if (content_test_file.getPath()[0] != '\0') {
                        const char* result = magic_file(custom_magic, content_test_file.getPath());
                        (void)result;
                    }
                    
                    magic_close(custom_magic);
                }
            }
            break;
            
        case 5:
            // Test magic_setparam() for database limits configuration
            {
                // Set various parameter limits
                size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
                
                size_t name_max = fdp.ConsumeIntegralInRange<size_t>(1, 1000);
                magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
                
                size_t bytes_max = fdp.ConsumeIntegralInRange<size_t>(1, 1048576);
                magic_setparam(magic, MAGIC_PARAM_BYTES_MAX, &bytes_max);
                
                // Load and test with configured limits
                if (magic_db_file.getPath()[0] != '\0') {
                    magic_load(magic, magic_db_file.getPath());
                    
                    if (content_test_file.getPath()[0] != '\0') {
                        const char* result = magic_file(magic, content_test_file.getPath());
                        (void)result;
                    }
                }
            }
            break;
            
        case 6:
            // Test combination of compile, check, and list
            if (magic_db_file.getPath()[0] != '\0') {
                magic_compile(magic, magic_db_file.getPath());
                magic_check(magic, magic_db_file.getPath());
                magic_list(magic, magic_db_file.getPath());
                
                if (content_test_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, content_test_file.getPath());
                    (void)result;
                }
            }
            break;
            
        case 7:
            // Test with MAGIC_NO_CHECK_SOFT flag (bypass soft magic checking)
            {
                int no_soft_flags = flags | MAGIC_NO_CHECK_SOFT;
                magic_t no_soft_magic = magic_open(no_soft_flags);
                if (no_soft_magic) {
                    if (magic_db_file.getPath()[0] != '\0') {
                        magic_load(no_soft_magic, magic_db_file.getPath());
                        
                        if (content_test_file.getPath()[0] != '\0') {
                            const char* result = magic_file(no_soft_magic, content_test_file.getPath());
                            (void)result;
                        }
                    }
                    magic_close(no_soft_magic);
                }
            }
            break;
            
        case 8:
            // Test with MAGIC_NO_CHECK_SIMH flag (bypass SIMH checking)
            {
                int no_simh_flags = flags | MAGIC_NO_CHECK_SIMH;
                magic_t no_simh_magic = magic_open(no_simh_flags);
                if (no_simh_magic) {
                    if (magic_db_file.getPath()[0] != '\0') {
                        magic_load(no_simh_magic, magic_db_file.getPath());
                        
                        if (content_test_file.getPath()[0] != '\0') {
                            const char* result = magic_file(no_simh_magic, content_test_file.getPath());
                            (void)result;
                        }
                    }
                    magic_close(no_simh_magic);
                }
            }
            break;
            
        case 9:
            // Test error handling with invalid magic database paths
            {
                const char* invalid_path = "/nonexistent/path/to/magic.db";
                magic_compile(magic, invalid_path);
                magic_check(magic, invalid_path);
                magic_list(magic, invalid_path);
                magic_load(magic, invalid_path);
                
                // Check for errors
                const char* error = magic_error(magic);
                (void)error;
            }
            break;
            
        case 10:
            // Test buffer-based detection with compiled database
            if (magic_db_file.getPath()[0] != '\0') {
                magic_load(magic, magic_db_file.getPath());
                
                if (!test_content.empty()) {
                    const char* result = magic_buffer(magic, test_content.data(), test_content.size());
                    (void)result;
                }
            }
            break;
            
        case 11:
            // Test descriptor-based detection with compiled database
            if (magic_db_file.getPath()[0] != '\0' && content_test_file.getPath()[0] != '\0') {
                magic_load(magic, magic_db_file.getPath());
                
                int fd = open(content_test_file.getPath(), O_RDONLY);
                if (fd >= 0) {
                    const char* result = magic_descriptor(magic, fd);
                    (void)result;
                    close(fd);
                }
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_012
