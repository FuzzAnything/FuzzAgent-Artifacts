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

namespace harness_011 {
/*
 * libmagic fuzzing harness #11 - DER (ASN.1) detection and uncovered modules
 * Target: der.c, softmagic.c, apprentice.c, file.c, buffer.c, seccomp.c, apptype.c
 * APIs: magic_file, magic_descriptor, magic_load_buffers, magic_setparam, 
 *       magic_getparam, magic_check, magic_version
 * 
 * This harness specifically targets DER (ASN.1) detection and other uncovered modules.
 * Focus areas:
 * 1. DER file detection - ASN.1 encoded certificates, PKCS#7, etc.
 * 2. Custom magic database validation with magic_check()
 * 3. Parameter configuration with magic_setparam/magic_getparam
 * 4. File and descriptor-based detection for DER content
 * 5. Edge cases: empty files, large DER structures, corrupted magic databases
 * 6. Version checking with magic_version()
 * 
 * Semantically distinct from previous harnesses:
 * - harness_001: General file-based detection
 * - harness_002: CDF-specific detection  
 * - harness_011: DER/ASN.1 specific detection, magic_check validation, version testing
 * 
 * Key focus areas:
 * - Exercise der.c with various ASN.1 encoded structures
 * - Trigger softmagic.c DER pattern matching
 * - Test apprentice.c with custom magic databases
 * - Validate magic databases with magic_check()
 * - Test file.c and buffer.c with DER content
 * - Include seccomp.c and apptype.c module paths
 */



// Common DER/ASN.1 tags and structures
#define DER_TAG_SEQUENCE       0x10
#define DER_TAG_SET            0x11
#define DER_TAG_INTEGER        0x02
#define DER_TAG_OCTET_STRING   0x04
#define DER_TAG_OBJECT_ID      0x06
#define DER_TAG_UTF8_STRING    0x0c
#define DER_TAG_PRINTABLE_STR  0x13
#define DER_TAG_IA5_STRING     0x16
#define DER_TAG_UTCTIME        0x17
#define DER_TAG_GENERALIZEDTIME 0x18
#define DER_TAG_BIT_STRING     0x03
#define DER_TAG_BOOLEAN        0x01
#define DER_TAG_NULL           0x05

// Helper to generate DER-encoded data
std::vector<uint8_t> generate_der_data(FuzzedDataProvider& fdp, bool generate_valid) {
    std::vector<uint8_t> der_data;
    
    if (generate_valid) {
        // Generate valid DER structure: SEQUENCE with random content
        size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::vector<uint8_t> content = fdp.ConsumeBytes<uint8_t>(content_size);
        
        // DER encoding: tag-length-value
        der_data.push_back(DER_TAG_SEQUENCE | 0x20); // Constructed sequence
        
        if (content_size < 128) {
            // Short form length
            der_data.push_back(static_cast<uint8_t>(content_size));
        } else {
            // Long form length
            size_t len_bytes = 1;
            if (content_size < 256) len_bytes = 1;
            else if (content_size < 65536) len_bytes = 2;
            else len_bytes = 3;
            
            der_data.push_back(static_cast<uint8_t>(0x80 | len_bytes));
            for (size_t i = 0; i < len_bytes; i++) {
                der_data.push_back(static_cast<uint8_t>((content_size >> (8 * (len_bytes - i - 1))) & 0xFF));
            }
        }
        
        der_data.insert(der_data.end(), content.begin(), content.end());
    } else {
        // Generate potentially malformed DER
        size_t der_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        der_data = fdp.ConsumeBytes<uint8_t>(der_size);
        
        // Optionally corrupt some DER patterns
        if (!der_data.empty() && fdp.ConsumeBool()) {
            size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(0, der_data.size() - 1);
            der_data[corrupt_pos] ^= 0xFF;
        }
    }
    
    return der_data;
}

// Create a magic database with DER patterns
std::string create_der_magic_database() {
    return std::string(
        "0 beshort 0x3080      DER Encoded Key\n"
        "0 string \\x30\\x82   DER SEQUENCE (long form)\n"
        "0 string \\x30\\x81   DER SEQUENCE (medium form)\n"
        "0 byte 0x30           DER SEQUENCE (short form)\n"
        "0 beshort 0x3082      PKCS#7 signedData\n"
        "0 beshort 0x3083      PKCS#7 envelopedData\n"
        "0 beshort 0x3084      PKCS#7 signedAndEnvelopedData\n"
        "0 beshort 0x3085      PKCS#7 digestedData\n"
        "0 beshort 0x3086      PKCS#7 encryptedData\n"
        "0 string \\x06        DER OBJECT IDENTIFIER\n"
        "0 string \\x02        DER INTEGER\n"
        "0 string \\x04        DER OCTET STRING\n"
        "0 string \\x13        DER PrintableString\n"
        "0 string \\x16        DER IA5String\n"
        "0 string \\x17        DER UTCTime\n"
        "0 string \\x18        DER GeneralizedTime\n"
        ">2 byte&0x1F x        \\b, tag %d\n"
        ">1 byte>127           \\b, long form length\n"
    );
}

// Temporary file utility
class TemporaryFile {
public:
    TemporaryFile(const uint8_t* data, size_t size) {
        char filename[] = "/tmp/libmagic_der_XXXXXX";
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 9);
    bool use_der_content = fdp.ConsumeBool();
    
    // Mask flags to valid MAGIC_* values
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION |
              MAGIC_NO_CHECK_COMPRESS | MAGIC_NO_CHECK_TAR | 
              MAGIC_NO_CHECK_SOFT | MAGIC_NO_CHECK_APPTYPE |
              MAGIC_NO_CHECK_ELF | MAGIC_NO_CHECK_TEXT |
              MAGIC_NO_CHECK_CDF | MAGIC_NO_CHECK_TOKENS);
    
    // Test magic_version() - should always work
    int version = magic_version();
    (void)version;
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Generate DER content or use raw fuzzer data
    std::vector<uint8_t> der_content;
    if (use_der_content) {
        bool generate_valid = fdp.ConsumeBool();
        der_content = generate_der_data(fdp, generate_valid);
    } else {
        der_content = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    // Create temporary file with the content
    TemporaryFile temp_file(der_content.data(), der_content.size());
    
    // Try to load standard magic database
    magic_load(magic, NULL);
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test magic_file() with DER content
            if (temp_file.getPath()[0] != '\0') {
                const char* result = magic_file(magic, temp_file.getPath());
                (void)result;
            }
            break;
            
        case 1:
            // Test magic_descriptor() with DER content
            if (temp_file.getPath()[0] != '\0') {
                int fd = open(temp_file.getPath(), O_RDONLY);
                if (fd >= 0) {
                    const char* result = magic_descriptor(magic, fd);
                    (void)result;
                    close(fd);
                }
            }
            break;
            
        case 2:
            // Test magic_load_buffers() with custom DER magic database
            {
                std::string magic_db = create_der_magic_database();
                std::vector<uint8_t> magic_buffer(magic_db.begin(), magic_db.end());
                
                if (!magic_buffer.empty()) {
                    void* buffers[1] = {magic_buffer.data()};
                    size_t sizes[1] = {magic_buffer.size()};
                    
                    magic_t custom_magic = magic_open(MAGIC_NONE);
                    if (custom_magic) {
                        magic_load_buffers(custom_magic, buffers, sizes, 1);
                        
                        // Test detection with custom database
                        if (temp_file.getPath()[0] != '\0') {
                            const char* result = magic_file(custom_magic, temp_file.getPath());
                            (void)result;
                        }
                        
                        magic_close(custom_magic);
                    }
                }
            }
            break;
            
        case 3:
            // Test magic_setparam() and magic_getparam() for DER-related parameters
            {
                // Test MAGIC_PARAM_INDIR_MAX (maximum indirection level)
                size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
                
                size_t retrieved_indir = 0;
                magic_getparam(magic, MAGIC_PARAM_INDIR_MAX, &retrieved_indir);
                (void)retrieved_indir;
                
                // Test MAGIC_PARAM_NAME_MAX (maximum name length)
                size_t name_max = fdp.ConsumeIntegralInRange<size_t>(1, 1000);
                magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
                
                size_t retrieved_name = 0;
                magic_getparam(magic, MAGIC_PARAM_NAME_MAX, &retrieved_name);
                (void)retrieved_name;
                
                // Test file detection with modified parameters
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
            }
            break;
            
        case 4:
            // Test magic_check() with custom magic database
            {
                std::string magic_db = create_der_magic_database();
                char tmpname[] = "/tmp/magic_der_check_XXXXXX";
                int fd = mkstemp(tmpname);
                if (fd >= 0) {
                    write(fd, magic_db.c_str(), magic_db.size());
                    close(fd);
                    
                    // Create a magic instance for checking
                    magic_t check_magic = magic_open(MAGIC_CHECK);
                    if (check_magic) {
                        // Try to check the magic database
                        int check_result = magic_check(check_magic, tmpname);
                        (void)check_result;
                        
                        magic_close(check_magic);
                    }
                    
                    unlink(tmpname);
                }
            }
            break;
            
        case 5:
            // Test DER-specific flag combinations
            {
                int current_flags = magic_getflags(magic);
                
                // Clear NO_CHECK flags to enable all checks
                int test_flags = current_flags & ~(MAGIC_NO_CHECK_SOFT | MAGIC_NO_CHECK_APPTYPE);
                magic_setflags(magic, test_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Test with RAW flag for binary detection
                test_flags = current_flags | MAGIC_RAW;
                magic_setflags(magic, test_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 6:
            // Test edge cases: empty files and very small content
            {
                // Test with empty file
                std::vector<uint8_t> empty_data;
                TemporaryFile empty_file(empty_data.data(), empty_data.size());
                if (empty_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, empty_file.getPath());
                    (void)result;
                }
                
                // Test with minimal DER content (just a tag)
                if (der_content.size() >= 1) {
                    const char* result = magic_buffer(magic, der_content.data(), 1);
                    (void)result;
                }
            }
            break;
            
        case 7:
            // Test buffer-based detection with various offsets and sizes
            {
                if (!der_content.empty()) {
                    // Test full buffer
                    const char* result1 = magic_buffer(magic, der_content.data(), der_content.size());
                    (void)result1;
                    
                    // Test partial buffer (first half)
                    size_t half_size = der_content.size() / 2;
                    if (half_size > 0) {
                        const char* result2 = magic_buffer(magic, der_content.data(), half_size);
                        (void)result2;
                    }
                    
                    // Test with offset if buffer is large enough
                    if (der_content.size() > 10) {
                        const char* result3 = magic_buffer(magic, der_content.data() + 5, der_content.size() - 5);
                        (void)result3;
                    }
                }
            }
            break;
            
        case 8:
            // Test with corrupted magic database
            {
                std::string magic_db = create_der_magic_database();
                // Corrupt the database
                if (!magic_db.empty() && fdp.ConsumeBool()) {
                    size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(0, magic_db.size() - 1);
                    const_cast<char*>(magic_db.c_str())[corrupt_pos] ^= 0xFF;
                }
                
                char tmpname[] = "/tmp/magic_corrupt_XXXXXX";
                int fd = mkstemp(tmpname);
                if (fd >= 0) {
                    write(fd, magic_db.c_str(), magic_db.size());
                    close(fd);
                    
                    magic_t corrupt_magic = magic_open(MAGIC_NONE);
                    if (corrupt_magic) {
                        // This may fail, which is expected
                        magic_load(corrupt_magic, tmpname);
                        magic_close(corrupt_magic);
                    }
                    
                    unlink(tmpname);
                }
            }
            break;
            
        case 9:
            // Comprehensive test: combine multiple operations
            {
                // Test magic_version() again
                int ver = magic_version();
                (void)ver;
                // Test file detection
                if (temp_file.getPath()[0] != '\0') {
                    const char* result1 = magic_file(magic, temp_file.getPath());
                    (void)result1;
                    
                    // Test with different flags
                    int current_flags = magic_getflags(magic);
                    int new_flags = current_flags ^ MAGIC_RAW; // Toggle RAW flag
                    magic_setflags(magic, new_flags);
                    
                    const char* result2 = magic_file(magic, temp_file.getPath());
                    (void)result2;
                    
                    // Restore flags
                    magic_setflags(magic, current_flags);
                }
                
                // Test error handling
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                int err = magic_errno(magic);
                (void)err;
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_011
