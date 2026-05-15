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

namespace harness_007 {
/*
 * libmagic fuzzing harness - MIME detection and Apple format modules
 * Target: MIME detection (MAGIC_MIME_TYPE, MAGIC_MIME_ENCODING) and Apple format (MAGIC_APPLE)
 * 
 * This harness specifically targets MIME and Apple format detection modules:
 * 1) MAGIC_MIME_TYPE flag - Returns MIME type (e.g., text/plain, application/xml)
 * 2) MAGIC_MIME_ENCODING flag - Returns MIME encoding (e.g., charset=utf-8, binary)
 * 3) MAGIC_APPLE flag - Returns Apple creator/type for Apple-specific formats
 * 4) APIs triggering MIME/Apple detection paths in fsmagic.c, ascmagic.c, encoding.c
 * 5) Testing various content types: text, HTML, XML, JSON, CSV, binary, Apple formats
 * 
 * Focus areas:
 * - MIME type detection for different content types (text, markup, data, binary)
 * - MIME encoding detection (UTF-8, ASCII, binary, etc.)
 * - Apple format detection (AppleDouble, AppleSingle, resource forks)
 * - Testing MAGIC_MIME (combined MIME_TYPE | MIME_ENCODING)
 * - Testing with various flags combinations and edge cases
 * - Semantic differentiation from previous harnesses:
 *   - harness_000: General buffer detection and basic flag manipulation
 *   - harness_001: File-based detection and parameter configuration  
 *   - harness_002: CDF-specific detection
 *   - harness_003-005: ELF parsing modules
 *   - harness_006: Compression and encoding modules
 *   - harness_007: MIME detection and Apple format modules (this harness)
 * 
 * Compilation requirements: -lz -lbz2 -llzma -lzstd -llz4
 */



// Common MIME type signatures for testing
static const struct {
    const char* content;
    size_t len;
    const char* expected_mime;
    const char* expected_encoding;
} mime_test_cases[] = {
    {"<!DOCTYPE html>", 15, "text/html", "us-ascii"},
    {"<?xml version=\"1.0\"?>", 21, "application/xml", "us-ascii"},
    {"{", 1, "application/json", "us-ascii"},
    {"name,age", 8, "text/csv", "us-ascii"},
    {"#!/bin/bash", 11, "text/x-shellscript", "us-ascii"},
    {"%PDF", 4, "application/pdf", "binary"},
    {"GIF87a", 6, "image/gif", "binary"},
    {"\xFF\xD8\xFF", 3, "image/jpeg", "binary"},
    {"\x89PNG", 4, "image/png", "binary"},
    {"BM", 2, "image/bmp", "binary"},
    {"RIFF", 4, "audio/x-wav", "binary"},
    {"ID3", 3, "audio/mpeg", "binary"},
    {"\x00\x00\x01\x00", 4, "image/x-icon", "binary"},
    {"\x1F\x8B", 2, "application/x-gzip", "binary"},
    {"BZh", 3, "application/x-bzip2", "binary"},
    {"\xFD\x37\x7A\x58\x5A\x00", 6, "application/x-xz", "binary"},
};

// Apple format signatures
static const struct {
    const char* magic;
    size_t len;
    const char* description;
} apple_formats[] = {
    {".SIGN", 5, "AppleSingle resource fork"},
    {"\x00\x05\x16\x07", 4, "AppleDouble resource fork"},
    {"\x00\x05\x16\x00", 4, "AppleDouble data fork"},
    {"Bud1", 4, "Apple MPEG-4 video"},
    {"moov", 4, "Apple QuickTime movie"},
    {"ftypqt", 6, "Apple QuickTime movie"},
    {"ftypM4V", 7, "Apple MPEG-4 video"},
    {"ftypM4A", 7, "Apple MPEG-4 audio"},
    {"ftypM4P", 7, "Apple MPEG-4 protected"},
};

// Helper to create test data with MIME signature
static std::vector<uint8_t> create_mime_test_data(FuzzedDataProvider& fdp, int mime_type_idx) {
    if (mime_type_idx < 0 || mime_type_idx >= static_cast<int>(sizeof(mime_test_cases)/sizeof(mime_test_cases[0]))) {
        // Random data if index is out of bounds
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        return fdp.ConsumeBytes<uint8_t>(data_size);
    }
    
    const auto& test_case = mime_test_cases[mime_type_idx];
    std::vector<uint8_t> data;
    
    // Add the MIME signature
    data.insert(data.end(), test_case.content, test_case.content + test_case.len);
    
    // Add additional fuzzed data
    size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_size);
    data.insert(data.end(), extra.begin(), extra.end());
    
    return data;
}

// Helper to create Apple format test data
static std::vector<uint8_t> create_apple_test_data(FuzzedDataProvider& fdp, int apple_format_idx) {
    if (apple_format_idx < 0 || apple_format_idx >= static_cast<int>(sizeof(apple_formats)/sizeof(apple_formats[0]))) {
        // Random data if index is out of bounds
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        return fdp.ConsumeBytes<uint8_t>(data_size);
    }
    
    const auto& format = apple_formats[apple_format_idx];
    std::vector<uint8_t> data;
    
    // Add the Apple format signature
    data.insert(data.end(), format.magic, format.magic + format.len);
    
    // Add additional fuzzed data
    size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_size);
    data.insert(data.end(), extra.begin(), extra.end());
    
    return data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 7);
    int mime_type_idx = fdp.ConsumeIntegralInRange<int>(0, sizeof(mime_test_cases)/sizeof(mime_test_cases[0]) - 1);
    int apple_format_idx = fdp.ConsumeIntegralInRange<int>(0, sizeof(apple_formats)/sizeof(apple_formats[0]) - 1);
    bool test_mime_type = fdp.ConsumeBool();
    bool test_mime_encoding = fdp.ConsumeBool();
    bool test_apple = fdp.ConsumeBool();
    
    // Configure MIME flags based on fuzzer input
    int mime_flags = 0;
    if (test_mime_type) mime_flags |= MAGIC_MIME_TYPE;
    if (test_mime_encoding) mime_flags |= MAGIC_MIME_ENCODING;
    
    // Configure Apple flag
    int apple_flag = test_apple ? MAGIC_APPLE : 0;
    
    // Mask base flags to valid MAGIC_* values, excluding MIME and APPLE flags we'll handle separately
    int other_flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
                                   MAGIC_CONTINUE | MAGIC_CHECK | 
                                   MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
                                   MAGIC_EXTENSION);
    
    // Combine all flags
    int flags = other_flags | mime_flags | apple_flag;
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load standard magic database (ignore failure)
    magic_load(magic, NULL);
    
    // Get remaining data for testing
    std::vector<uint8_t> test_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (test_data.empty()) {
        test_data.push_back(0); // Ensure at least 1 byte
    }
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test with MIME_TYPE flag only
            if (test_mime_type) {
                int mime_only_flags = other_flags | MAGIC_MIME_TYPE;
                magic_setflags(magic, mime_only_flags);
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result; // Use result to avoid unused variable warning
            }
            break;
            
        case 1:
            // Test with MIME_ENCODING flag only
            if (test_mime_encoding) {
                int encoding_only_flags = other_flags | MAGIC_MIME_ENCODING;
                magic_setflags(magic, encoding_only_flags);
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            break;
            
        case 2:
            // Test with both MIME_TYPE and MIME_ENCODING (MAGIC_MIME)
            if (test_mime_type && test_mime_encoding) {
                int mime_both_flags = other_flags | MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING;
                magic_setflags(magic, mime_both_flags);
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            break;
            
        case 3:
            // Test with APPLE flag only
            if (test_apple) {
                int apple_only_flags = other_flags | MAGIC_APPLE;
                magic_setflags(magic, apple_only_flags);
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            break;
            
        case 4:
            // Test with MIME and APPLE flags combined
            {
                int combined_flags = other_flags | mime_flags | apple_flag;
                magic_setflags(magic, combined_flags);
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result;
            }
            break;
            
        case 5:
            // Test specific MIME type detection with known signatures
            {
                std::vector<uint8_t> mime_data = create_mime_test_data(fdp, mime_type_idx);
                if (!mime_data.empty()) {
                    const char* result = magic_buffer(magic, mime_data.data(), mime_data.size());
                    (void)result;
                }
            }
            break;
            
        case 6:
            // Test Apple format detection with known signatures
            {
                std::vector<uint8_t> apple_data = create_apple_test_data(fdp, apple_format_idx);
                if (!apple_data.empty()) {
                    // Set APPLE flag for Apple format detection
                    int apple_detection_flags = other_flags | MAGIC_APPLE;
                    magic_setflags(magic, apple_detection_flags);
                    const char* result = magic_buffer(magic, apple_data.data(), apple_data.size());
                    (void)result;
                }
            }
            break;
            
        case 7:
            // Test flag toggling between different configurations
            {
                // Start with MIME_TYPE
                magic_setflags(magic, other_flags | MAGIC_MIME_TYPE);
                const char* result1 = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result1;
                
                // Switch to MIME_ENCODING
                magic_setflags(magic, other_flags | MAGIC_MIME_ENCODING);
                const char* result2 = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result2;
                
                // Switch to APPLE
                if (test_apple) {
                    magic_setflags(magic, other_flags | MAGIC_APPLE);
                    const char* result3 = magic_buffer(magic, test_data.data(), test_data.size());
                    (void)result3;
                }
                
                // Switch to all combined
                magic_setflags(magic, other_flags | mime_flags | apple_flag);
                const char* result4 = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result4;
            }
            break;
    }
    
    // Test error handling APIs with current configuration
    const char* error_msg = magic_error(magic);
    (void)error_msg;
    
    int err = magic_errno(magic);
    (void)err;
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_007
