#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"
#include "tiny_gltf.h"

namespace harness_016 {
// Harness 016: stb_image zlib decompression fuzzer for tinygltf
// Targets: stbi_zlib_decode_malloc_guesssize, stbi_zlib_decode_malloc_guesssize_headerflag,
//          stbi_zlib_decode_malloc, stbi_zlib_decode_buffer,
//          stbi_zlib_decode_noheader_malloc, stbi_zlib_decode_noheader_buffer
// Focus: zlib decompression APIs in stb_image.h module (completely uncovered with 36+ undiscovered branches)
// Differentiates from existing harnesses by targeting zlib decompression specifically, not image loading or callbacks
// Exercises zlib decompression APIs with various compressed data formats, error conditions, and edge cases
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Include stb_image headers - DO NOT define STB_IMAGE_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Include tinygltf header - DO NOT define TINYGLTF_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Declare stb_image zlib functions we need (they're implemented in libtinygltf.a)
extern "C" {
    char *stbi_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen);
    char *stbi_zlib_decode_malloc_guesssize_headerflag(const char *buffer, int len, int initial_size, int *outlen, int parse_header);
    char *stbi_zlib_decode_malloc(const char *buffer, int len, int *outlen);
    int   stbi_zlib_decode_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
    char *stbi_zlib_decode_noheader_malloc(const char *buffer, int len, int *outlen);
    int   stbi_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
    void  stbi_image_free(void *retval_from_stbi_load);
}

// Helper to generate zlib test data with various compression levels and formats
static std::vector<uint8_t> GenerateZlibTestData(FuzzedDataProvider& fdp, size_t max_size = 4096) {
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, max_size);
    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // If we don't have enough input data, fill with pattern from remaining bytes
    if (data.size() < data_size) {
        size_t remaining = data_size - data.size();
        for (size_t i = 0; i < remaining; i++) {
            data.push_back(fdp.ConsumeIntegral<uint8_t>());
        }
    }
    
    return data;
}

// Helper to generate output buffer sizes
static int GenerateOutputSize(FuzzedDataProvider& fdp, int input_size) {
    // Vary output buffer sizes: too small, exact, oversized
    uint8_t size_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (size_type) {
        case 0: // Too small (should fail or truncate)
            return fdp.ConsumeIntegralInRange<int>(1, std::max(1, input_size / 4));
        case 1: // Exact or reasonable size
            return fdp.ConsumeIntegralInRange<int>(input_size, input_size * 2);
        case 2: // Oversized
            return fdp.ConsumeIntegralInRange<int>(input_size * 2, input_size * 4);
        default:
            return input_size;
    }
}

// Test stbi_zlib_decode_malloc_guesssize
static void TestZlibDecodeMallocGuesssize(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int initial_size = fdp.ConsumeIntegralInRange<int>(1, 65536);
    int outlen = 0;
    
    char* result = stbi_zlib_decode_malloc_guesssize(buffer, len, initial_size, &outlen);
    if (result) {
        // Verify we can read from the result (should be decompressed data)
        if (outlen > 0) {
            // Try to read first few bytes
            volatile char first_byte = result[0];
            (void)first_byte;
            
            // Try to read last byte
            if (outlen > 1) {
                volatile char last_byte = result[outlen - 1];
                (void)last_byte;
            }
        }
        stbi_image_free(result);
    }
    
    // Test with NULL outlen pointer
    char* result2 = stbi_zlib_decode_malloc_guesssize(buffer, len, initial_size, NULL);
    if (result2) {
        stbi_image_free(result2);
    }
}

// Test stbi_zlib_decode_malloc_guesssize_headerflag
static void TestZlibDecodeMallocGuesssizeHeaderflag(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int initial_size = fdp.ConsumeIntegralInRange<int>(1, 65536);
    int parse_header = fdp.ConsumeBool() ? 1 : 0;
    int outlen = 0;
    
    char* result = stbi_zlib_decode_malloc_guesssize_headerflag(buffer, len, initial_size, &outlen, parse_header);
    if (result) {
        if (outlen > 0) {
            volatile char first_byte = result[0];
            (void)first_byte;
            
            if (outlen > 1) {
                volatile char last_byte = result[outlen - 1];
                (void)last_byte;
            }
        }
        stbi_image_free(result);
    }
    
    // Test with NULL outlen pointer
    char* result2 = stbi_zlib_decode_malloc_guesssize_headerflag(buffer, len, initial_size, NULL, parse_header);
    if (result2) {
        stbi_image_free(result2);
    }
}

// Test stbi_zlib_decode_malloc
static void TestZlibDecodeMalloc(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int outlen = 0;
    
    char* result = stbi_zlib_decode_malloc(buffer, len, &outlen);
    if (result) {
        if (outlen > 0) {
            volatile char first_byte = result[0];
            (void)first_byte;
            
            if (outlen > 1) {
                volatile char last_byte = result[outlen - 1];
                (void)last_byte;
            }
        }
        stbi_image_free(result);
    }
    
    // Test with NULL outlen pointer
    char* result2 = stbi_zlib_decode_malloc(buffer, len, NULL);
    if (result2) {
        stbi_image_free(result2);
    }
}

// Test stbi_zlib_decode_buffer
static void TestZlibDecodeBuffer(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int olen = GenerateOutputSize(fdp, len);
    
    // Allocate output buffer
    std::vector<char> obuffer(olen);
    
    int result = stbi_zlib_decode_buffer(obuffer.data(), olen, buffer, len);
    // Result is -1 on error, or number of bytes written on success
    
    if (result > 0 && result <= olen) {
        // Verify we can read from the output buffer
        volatile char first_byte = obuffer[0];
        (void)first_byte;
        
        if (result > 1) {
            volatile char last_byte = obuffer[result - 1];
            (void)last_byte;
        }
    }
    
    // Test edge cases
    // 1. Zero-length output buffer
    if (olen > 0) {
        int result2 = stbi_zlib_decode_buffer(obuffer.data(), 0, buffer, len);
        (void)result2;
    }
    
    // 2. Negative length input (should be handled by API)
    if (len > 0) {
        int result3 = stbi_zlib_decode_buffer(obuffer.data(), olen, buffer, -1);
        (void)result3;
    }
}

// Test stbi_zlib_decode_noheader_malloc
static void TestZlibDecodeNoheaderMalloc(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int outlen = 0;
    
    char* result = stbi_zlib_decode_noheader_malloc(buffer, len, &outlen);
    if (result) {
        if (outlen > 0) {
            volatile char first_byte = result[0];
            (void)first_byte;
            
            if (outlen > 1) {
                volatile char last_byte = result[outlen - 1];
                (void)last_byte;
            }
        }
        stbi_image_free(result);
    }
    
    // Test with NULL outlen pointer
    char* result2 = stbi_zlib_decode_noheader_malloc(buffer, len, NULL);
    if (result2) {
        stbi_image_free(result2);
    }
}

// Test stbi_zlib_decode_noheader_buffer
static void TestZlibDecodeNoheaderBuffer(FuzzedDataProvider& fdp, const char* buffer, int len) {
    int olen = GenerateOutputSize(fdp, len);
    
    // Allocate output buffer
    std::vector<char> obuffer(olen);
    
    int result = stbi_zlib_decode_noheader_buffer(obuffer.data(), olen, buffer, len);
    // Result is -1 on error, or number of bytes written on success
    
    if (result > 0 && result <= olen) {
        // Verify we can read from the output buffer
        volatile char first_byte = obuffer[0];
        (void)first_byte;
        
        if (result > 1) {
            volatile char last_byte = obuffer[result - 1];
            (void)last_byte;
        }
    }
    
    // Test edge cases
    // 1. Zero-length output buffer
    if (olen > 0) {
        int result2 = stbi_zlib_decode_noheader_buffer(obuffer.data(), 0, buffer, len);
        (void)result2;
    }
    
    // 2. Negative length input
    if (len > 0) {
        int result3 = stbi_zlib_decode_noheader_buffer(obuffer.data(), olen, buffer, -1);
        (void)result3;
    }
}

// Test invalid input scenarios for malloc-based APIs only
// Buffer-based APIs (stbi_zlib_decode_buffer, stbi_zlib_decode_noheader_buffer)
// require valid output buffers, so we don't test NULL buffers with them
static void TestInvalidInputs(FuzzedDataProvider& fdp) {
    int outlen = 0;
    
    // Test malloc variants with NULL input buffer and zero length
    char* result = stbi_zlib_decode_malloc(NULL, 0, &outlen);
    if (result) {
        stbi_image_free(result);
    }
    
    // Test with actual data but zero length
    std::vector<uint8_t> empty_data(1);
    result = stbi_zlib_decode_malloc(reinterpret_cast<const char*>(empty_data.data()), 0, &outlen);
    if (result) {
        stbi_image_free(result);
    }
    
    // Test with negative length
    result = stbi_zlib_decode_malloc(reinterpret_cast<const char*>(empty_data.data()), -1, &outlen);
    if (result) {
        stbi_image_free(result);
    }
    
    // Test noheader variants with NULL input buffer
    result = stbi_zlib_decode_noheader_malloc(NULL, 0, &outlen);
    if (result) {
        stbi_image_free(result);
    }
    
    // Note: We don't test buffer-based APIs (stbi_zlib_decode_buffer, 
    // stbi_zlib_decode_noheader_buffer) with NULL output buffers because
    // they require valid output buffers according to the API contract.
    // Testing NULL output buffers would be harness misuse, not library testing.
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic zlib testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate test data for zlib decompression
    std::vector<uint8_t> test_data = GenerateZlibTestData(fdp, std::min<size_t>(size, 4096));
    const char* buffer = reinterpret_cast<const char*>(test_data.data());
    int len = static_cast<int>(test_data.size());
    
    // Consume operation selector to test different zlib APIs
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (operation) {
        case 0:
            TestZlibDecodeMallocGuesssize(fdp, buffer, len);
            break;
        case 1:
            TestZlibDecodeMallocGuesssizeHeaderflag(fdp, buffer, len);
            break;
        case 2:
            TestZlibDecodeMalloc(fdp, buffer, len);
            break;
        case 3:
            TestZlibDecodeBuffer(fdp, buffer, len);
            break;
        case 4:
            TestZlibDecodeNoheaderMalloc(fdp, buffer, len);
            break;
        case 5:
            TestZlibDecodeNoheaderBuffer(fdp, buffer, len);
            break;
        case 6:
            TestInvalidInputs(fdp);
            break;
        case 7:
            // Test all APIs in sequence
            TestZlibDecodeMallocGuesssize(fdp, buffer, len);
            TestZlibDecodeMallocGuesssizeHeaderflag(fdp, buffer, len);
            TestZlibDecodeMalloc(fdp, buffer, len);
            TestZlibDecodeBuffer(fdp, buffer, len);
            TestZlibDecodeNoheaderMalloc(fdp, buffer, len);
            TestZlibDecodeNoheaderBuffer(fdp, buffer, len);
            break;
    }
    
    return 0;
}

} // namespace harness_016
