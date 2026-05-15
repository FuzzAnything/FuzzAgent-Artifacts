#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "stb_image.h"

namespace harness_021 {
/*
 * Harness for testing stb_image.h zlib decompression functions
 * Target functions:
 *   - stbi_zlib_decode_malloc
 *   - stbi_zlib_decode_malloc_guesssize
 *   - stbi_zlib_decode_noheader_malloc
 *   - stbi_zlib_decode_buffer
 *   - stbi_zlib_decode_noheader_buffer
 * 
 * This harness tests zlib decompression with various compressed inputs
 * and buffer sizes, covering both malloc-based and buffer-based variants.
 */


// Include stb_image.h implementation

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        // Need minimum input for meaningful testing
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume parameters for testing
    int operation = fdp.ConsumeIntegralInRange<int>(0, 4);  // Which function to test
    int initial_size = fdp.ConsumeIntegralInRange<int>(1, 65536);  // Initial buffer size guess
    int output_buffer_size = fdp.ConsumeIntegralInRange<int>(1, 65536);  // Output buffer size
    int parse_header = fdp.ConsumeBool() ? 1 : 0;  // Whether to parse zlib header

    // Consume remaining bytes as compressed input data
    std::vector<uint8_t> compressed_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (compressed_data.empty()) {
        return 0;
    }

    int outlen = 0;  // Will store output length
    char* decompressed_ptr = nullptr;

    switch (operation) {
        case 0: {
            // Test stbi_zlib_decode_malloc
            decompressed_ptr = stbi_zlib_decode_malloc(
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size()),
                &outlen
            );
            if (decompressed_ptr) {
                free(decompressed_ptr);
            }
            break;
        }

        case 1: {
            // Test stbi_zlib_decode_malloc_guesssize
            decompressed_ptr = stbi_zlib_decode_malloc_guesssize(
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size()),
                initial_size,
                &outlen
            );
            if (decompressed_ptr) {
                free(decompressed_ptr);
            }
            break;
        }

        case 2: {
            // Test stbi_zlib_decode_malloc_guesssize_headerflag
            decompressed_ptr = stbi_zlib_decode_malloc_guesssize_headerflag(
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size()),
                initial_size,
                &outlen,
                parse_header
            );
            if (decompressed_ptr) {
                free(decompressed_ptr);
            }
            break;
        }

        case 3: {
            // Test stbi_zlib_decode_buffer (buffer-based variant with header)
            std::vector<char> output_buffer(output_buffer_size);
            int result = stbi_zlib_decode_buffer(
                output_buffer.data(),
                output_buffer_size,
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size())
            );
            // Result can be -1 (error) or number of bytes decompressed
            (void)result;  // Use result to avoid unused variable warning
            break;
        }

        case 4: {
            // Test stbi_zlib_decode_noheader_buffer (buffer-based variant without header)
            std::vector<char> output_buffer(output_buffer_size);
            int result = stbi_zlib_decode_noheader_buffer(
                output_buffer.data(),
                output_buffer_size,
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size())
            );
            // Result can be -1 (error) or number of bytes decompressed
            (void)result;  // Use result to avoid unused variable warning
            break;
        }

        default: {
            // Test stbi_zlib_decode_noheader_malloc
            decompressed_ptr = stbi_zlib_decode_noheader_malloc(
                reinterpret_cast<const char*>(compressed_data.data()),
                static_cast<int>(compressed_data.size()),
                &outlen
            );
            if (decompressed_ptr) {
                free(decompressed_ptr);
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_021
