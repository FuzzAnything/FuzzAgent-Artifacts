#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_018 {
// Protocol Buffers fuzzing harness for absl::Cord and protobuf IO stream subsystems
// This harness targets the critical coverage gap identified by CoverageAnalyzerAgent:
// 40+ undiscovered branches and current API coverage at 28.80%.
// Focuses on Cord operations (construction, appending, slicing, flattening) combined
// with protobuf stream operations (ZeroCopyStream, ArrayOutputStream, StringOutputStream).
// Explores edge cases: empty Cords, large Cords, Cord chunk boundaries, and integration
// with different stream types. Semantic differentiation from existing harnesses:
// - harness_016: ZeroCopyStreamByteSink wrapper
// - harness_015: Zero-copy stream operations (CordInputStream::BackUp, etc.)
// - This harness: Comprehensive Cord lifecycle and stream integration patterns



using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::StringOutputStream;
using google::protobuf::io::CordInputStream;
using google::protobuf::io::CordOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful Cord and stream operations
    // Required: enough for Cord data, operation selection, stream parameters
    if (size < 64) return 0;

    FuzzedDataProvider fdp(data, size);

    // Step 1: Create various Cord objects from fuzzer input with different patterns
    std::vector<absl::Cord> test_cords;
    
    // Cord 1: Direct construction from string
    size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
    if (!cord1_data.empty()) {
        test_cords.push_back(absl::Cord(cord1_data));
    }

    // Cord 2: Empty cord (edge case)
    test_cords.push_back(absl::Cord());

    // Cord 3: Cord built by appending multiple fragments
    if (fdp.remaining_bytes() > 32) {
        absl::Cord fragmented_cord;
        int fragment_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int i = 0; i < fragment_count && fdp.remaining_bytes() > 16; i++) {
            size_t frag_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            std::string fragment = fdp.ConsumeBytesAsString(frag_size);
            fragmented_cord.Append(fragment);
        }
        test_cords.push_back(std::move(fragmented_cord));
    }

    // Cord 4: Large cord (if enough data)
    if (fdp.remaining_bytes() > 1024) {
        size_t large_size = fdp.ConsumeIntegralInRange<size_t>(512, 2048);
        std::string large_data = fdp.ConsumeBytesAsString(large_size);
        test_cords.push_back(absl::Cord(large_data));
    }

    // Step 2: Test basic Cord operations on each cord
    for (const auto& cord : test_cords) {
        // Basic properties
        size_t cord_size = cord.size();
        bool is_empty = cord.empty();
        
        // Try to flatten the cord (if small enough)
        if (cord_size > 0 && cord_size < 4096) {
            std::string flattened;
            if (cord.TryFlat()) {
                // Copy to string for testing - need to make a non-const copy to call Flatten()
                absl::Cord cord_copy = cord;
                flattened = std::string(cord_copy.Flatten());
            }
        }

        // Test Subcord operation (if cord has content)
        if (cord_size > 2) {
            size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, cord_size / 2);
            size_t length = fdp.ConsumeIntegralInRange<size_t>(1, cord_size - offset);
            absl::Cord subcord = cord.Subcord(offset, length);
            size_t subcord_size = subcord.size();
        }

        // Test cord iteration
        for (absl::string_view chunk : cord.Chunks()) {
            size_t chunk_size = chunk.size();
            // Basic validation that chunk is not null
            if (!chunk.data()) {
                // This shouldn't happen with valid cords, but fuzzer might find edge cases
                break;
            }
        }
    }

    // Step 3: Cord and stream integration testing
    // Operation mode determines which stream integration pattern to test
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;

    switch (operation_mode) {
        case 0: {
            // Pattern 1: Cord -> CordInputStream -> ArrayOutputStream
            if (!test_cords.empty() && !test_cords[0].empty()) {
                const absl::Cord& source_cord = test_cords[0];
                
                // Create CordInputStream from source cord
                CordInputStream cord_input(&source_cord);
                
                // Create ArrayOutputStream buffer
                size_t buffer_size = source_cord.size() + 128; // Extra space
                std::vector<char> output_buffer(buffer_size, 0);
                ArrayOutputStream array_output(output_buffer.data(), buffer_size);
                
                // Read from CordInputStream and write to ArrayOutputStream
                const void* input_buffer;
                int input_size;
                void* output_buffer_ptr;
                int output_size;
                
                while (cord_input.Next(&input_buffer, &input_size) && input_size > 0) {
                    if (array_output.Next(&output_buffer_ptr, &output_size)) {
                        int bytes_to_copy = std::min(input_size, output_size);
                        memcpy(output_buffer_ptr, input_buffer, bytes_to_copy);
                        if (bytes_to_copy < output_size) {
                            array_output.BackUp(output_size - bytes_to_copy);
                        }
                    }
                }
                
                // Test CordInputStream::ByteCount
                int64_t bytes_read = cord_input.ByteCount();
            }
            break;
        }
        
        case 1: {
            // Pattern 2: ArrayInputStream -> CordOutputStream -> Cord operations
            if (fdp.remaining_bytes() > 128) {
                // Create test data for input stream
                size_t input_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
                std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
                
                if (!input_data.empty()) {
                    // Create ArrayInputStream
                    ArrayInputStream array_input(input_data.data(), input_data.size(), 64);
                    
                    // Create CordOutputStream
                    CordOutputStream cord_output;
                    
                    // Copy data from ArrayInputStream to CordOutputStream
                    const void* in_buffer;
                    int in_size;
                    void* out_buffer;
                    int out_size;
                    
                    while (array_input.Next(&in_buffer, &in_size) && in_size > 0) {
                        if (cord_output.Next(&out_buffer, &out_size)) {
                            int bytes_to_copy = std::min(in_size, out_size);
                            memcpy(out_buffer, in_buffer, bytes_to_copy);
                            if (bytes_to_copy < out_size) {
                                cord_output.BackUp(out_size - bytes_to_copy);
                            }
                        }
                    }
                    
                    // Consume the resulting cord
                    absl::Cord result_cord = cord_output.Consume();
                    size_t result_size = result_cord.size();
                    
                    // Test WriteCord from result cord to StringOutputStream
                    std::string string_output;
                    StringOutputStream string_stream(&string_output);
                    bool write_success = string_stream.WriteCord(result_cord);
                }
            }
            break;
        }
        
        case 2: {
            // Pattern 3: Cord -> CordOutputStream (with pre-existing cord)
            if (!test_cords.empty()) {
                // Take ownership of a cord for CordOutputStream constructor
                absl::Cord existing_cord = test_cords[0];
                
                // Create CordOutputStream with existing cord
                CordOutputStream cord_output(std::move(existing_cord));
                
                // Write additional data to the cord
                if (fdp.remaining_bytes() > 64) {
                    size_t additional_size = fdp.ConsumeIntegralInRange<size_t>(32, 256);
                    std::vector<uint8_t> additional_data = fdp.ConsumeBytes<uint8_t>(additional_size);
                    
                    void* out_buffer;
                    int out_size;
                    if (cord_output.Next(&out_buffer, &out_size) && out_size > 0) {
                        int bytes_to_write = std::min(static_cast<int>(additional_data.size()), out_size);
                        memcpy(out_buffer, additional_data.data(), bytes_to_write);
                        if (bytes_to_write < out_size) {
                            cord_output.BackUp(out_size - bytes_to_write);
                        }
                    }
                }
                
                // Consume the final cord
                absl::Cord final_cord = cord_output.Consume();
                size_t final_size = final_cord.size();
            }
            break;
        }
        
        case 3: {
            // Pattern 4: Complex cord-stream roundtrip with chunk boundaries
            if (fdp.remaining_bytes() > 256) {
                // Create a cord with explicit chunk boundaries
                absl::Cord chunked_cord;
                
                // Add multiple chunks of varying sizes
                int num_chunks = fdp.ConsumeIntegralInRange<int>(2, 6);
                for (int i = 0; i < num_chunks && fdp.remaining_bytes() > 32; i++) {
                    size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
                    std::string chunk_data = fdp.ConsumeBytesAsString(chunk_size);
                    chunked_cord.Append(chunk_data);
                }
                
                // Create CordInputStream from chunked cord
                CordInputStream chunked_input(&chunked_cord);
                
                // Create buffer to test reading across chunk boundaries
                size_t total_size = chunked_cord.size();
                if (total_size > 0) {
                    // Test reading in pieces that may cross chunk boundaries
                    const void* read_buffer;
                    int read_size;
                    
                    // First read
                    if (chunked_input.Next(&read_buffer, &read_size)) {
                        int consumed_bytes = read_size;  // Bytes available after Next()
                        
                        // Test Skip operation that may cross chunk boundary
                        if (read_size > 4) {
                            int skip_amount = fdp.ConsumeIntegralInRange<int>(1, read_size - 1);
                            bool skip_success = chunked_input.Skip(skip_amount);
                            if (skip_success) {
                                consumed_bytes -= skip_amount;  // Adjust for consumed bytes
                                // After Skip(), we cannot call BackUp() according to API contract
                                // Skip() consumes bytes but BackUp() requires the last call to be Next()
                                // So we should not attempt BackUp() after Skip()
                                // Instead, we'll skip the BackUp test in this case
                            } else {
                                // Skip failed - we also cannot call BackUp() because the last operation was Skip()
                                // According to API contract: BackUp() can only be called after Next()
                                // Once Skip() is called (whether it succeeds or fails), BackUp() is not allowed
                                // So we skip BackUp test in this case too
                            }
                        } else {
                            // No Skip attempted, can safely test BackUp
                            // Test BackUp operation - ensure we don't back up more than consumed
                            if (read_size > 2 && consumed_bytes > 0) {
                                int backup_amount = fdp.ConsumeIntegralInRange<int>(0, read_size / 2);
                                // Cap backup amount to consumed bytes
                                int max_backup = std::min(backup_amount, consumed_bytes);
                                if (max_backup > 0) {
                                    chunked_input.BackUp(max_backup);
                                }
                            }
                        }
                    }
                    
                    // Continue reading to test stream exhaustion
                    while (chunked_input.Next(&read_buffer, &read_size)) {
                        // Just consume the data
                    }
                    
                    // Test ByteCount after full read
                    int64_t total_bytes = chunked_input.ByteCount();
                }
                
                // Test WriteCord with the chunked cord
                std::string string_buffer;
                StringOutputStream string_stream(&string_buffer);
                bool write_success = string_stream.WriteCord(chunked_cord);
            }
            break;
        }
    }

    // Step 4: Additional edge case testing
    
    // Test with empty cords and streams
    {
        absl::Cord empty_cord;
        CordInputStream empty_input(&empty_cord);
        const void* buf;
        int buf_size;
        bool has_data = empty_input.Next(&buf, &buf_size);
        
        CordOutputStream empty_output;
        absl::Cord consumed_empty = empty_output.Consume();
        bool empty_is_empty = consumed_empty.empty();
    }
    
    // Test cord comparison operations (if we have multiple cords)
    if (test_cords.size() >= 2) {
        const absl::Cord& cord1 = test_cords[0];
        const absl::Cord& cord2 = test_cords[1];
        
        // Compare cords
        bool cords_equal = (cord1 == cord2);
        bool cords_not_equal = (cord1 != cord2);
        
        // Test cord ordering (lexicographical comparison)
        bool cord1_less = (cord1 < cord2);
        bool cord1_greater = (cord1 > cord2);
        bool cord1_less_equal = (cord1 <= cord2);
        bool cord1_greater_equal = (cord1 >= cord2);
    }
    return 0;
}

} // namespace harness_018
