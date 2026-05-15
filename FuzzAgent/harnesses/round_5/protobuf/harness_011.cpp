#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_011 {
// Protocol Buffers fuzzing harness for CodedInputStream and CodedOutputStream operations
// This harness targets uncovered APIs with significant undiscovered branch complexity:
// - CodedInputStream::ReadLittleEndian64 (11 undiscovered branches)
// - CodedInputStream::ReadVarint64 (8 branches)
// - CodedInputStream::ReadVarintSizeAsInt (8 branches)
// - CodedOutputStream::WriteVarint64 (3 branches)
// - CodedOutputStream::WriteLittleEndian64 (3 branches)
// - CodedInputStream::DecrementRecursionDepthAndPopLimit (8 branches)
// - CodedInputStream::IncrementRecursionDepthAndPushLimit (5 branches)
// Focus on complete CodedInputStream/CodedOutputStream lifecycle with recursion depth management



using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful binary I/O operations
    if (size < 32) return 0;  // Need enough for multiple parameters
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into multiple parts:
    // 1. For creating input buffer data
    // 2. For operation parameters (recursion limits, loop counts, etc.)
    // 3. For generating additional test values
    
    // Step 1: Create input data for ArrayInputStream
    size_t input_buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 4096);
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
    if (input_buffer.empty()) return 0;
    
    // Step 2: Create output buffer for ArrayOutputStream
    size_t output_buffer_size = input_buffer_size * 2 + 256;  // Ensure enough space
    std::vector<uint8_t> output_buffer(output_buffer_size, 0);
    
    // Step 3: Create ZeroCopy streams
    ArrayInputStream array_input(input_buffer.data(), input_buffer.size());
    ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
    
    // Step 4: Create CodedInputStream and CodedOutputStream wrappers
    CodedInputStream coded_input(&array_input);
    CodedOutputStream coded_output(&array_output);
    
    // Step 5: Configure recursion depth and push limits
    int recursion_depth = 0;
    const int max_recursion_depth = fdp.ConsumeIntegralInRange<int>(0, 100);
    
    // Test recursion depth management APIs
    while (recursion_depth < max_recursion_depth && fdp.remaining_bytes() > 0) {
        // Determine byte limit for this recursion level
        int byte_limit = fdp.ConsumeIntegralInRange<int>(0, 1024);
        
        // Target API: CodedInputStream::IncrementRecursionDepthAndPushLimit (5 branches)
        auto limit_pair = coded_input.IncrementRecursionDepthAndPushLimit(byte_limit);
        int old_limit = limit_pair.first;
        // Read operations within recursion level
        while (!coded_input.ExpectAtEnd() && fdp.remaining_bytes() > 0) {
            // Consume operation type from fuzzer input
            uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
            
            switch (operation) {
                case 0: {
                    // Target API: CodedInputStream::ReadLittleEndian64 (11 branches)
                    uint64_t le64_value = 0;
                    if (coded_input.ReadLittleEndian64(&le64_value)) {
                        // Target API: CodedOutputStream::WriteLittleEndian64 (3 branches)
                        coded_output.WriteLittleEndian64(le64_value);
                    }
                    break;
                }
                    
                case 1: {
                    // Target API: CodedInputStream::ReadVarint64 (8 branches)
                    uint64_t varint64_value = 0;
                    if (coded_input.ReadVarint64(&varint64_value)) {
                        // Target API: CodedOutputStream::WriteVarint64 (3 branches)
                        coded_output.WriteVarint64(varint64_value);
                    }
                    break;
                }
                    
                case 2: {
                    // Target API: CodedInputStream::ReadVarintSizeAsInt (8 branches)
                    int varint_size_value = 0;
                    if (coded_input.ReadVarintSizeAsInt(&varint_size_value)) {
                        // Write the size as varint
                        coded_output.WriteVarint64(static_cast<uint64_t>(varint_size_value));
                    }
                    break;
                }
                    
                case 3: {
                    // Read LittleEndian32 and write it
                    uint32_t le32_value = 0;
                    if (coded_input.ReadLittleEndian32(&le32_value)) {
                        coded_output.WriteLittleEndian32(le32_value);
                    }
                    break;
                }
                    
                case 4: {
                    // Read Varint32 and write it
                    uint32_t varint32_value = 0;
                    if (coded_input.ReadVarint32(&varint32_value)) {
                        coded_output.WriteVarint32(varint32_value);
                    }
                    break;
                }
                    
                case 5: {
                    // Read raw bytes
                    uint32_t length = fdp.ConsumeIntegralInRange<uint32_t>(0, 256);
                    if (length > 0) {
                        std::vector<uint8_t> buffer(length);
                        if (coded_input.ReadRaw(buffer.data(), length)) {
                            coded_output.WriteRaw(buffer.data(), length);
                        }
                    }
                    break;
                }
                    
                case 6: {
                    // Read string with length from fuzzer
                    uint32_t str_length = fdp.ConsumeIntegralInRange<uint32_t>(0, 256);
                    if (str_length > 0) {
                        std::string str;
                        coded_input.ReadString(&str, str_length);
                        if (!str.empty()) {
                            coded_output.WriteString(str);
                        }
                    }
                    break;
                }
                    
                case 7: {
                    // Skip bytes
                    int skip_amount = fdp.ConsumeIntegralInRange<int>(0, 256);
                    if (skip_amount > 0) {
                        coded_input.Skip(skip_amount);
                    }
                    break;
                }
            }
            
            // Check if we should break early
            if (fdp.remaining_bytes() < 8) break;
        }
        
        recursion_depth++;
        
        // Target API: CodedInputStream::DecrementRecursionDepthAndPopLimit (8 branches)
        // Only pop limit if we actually pushed one
        if (byte_limit > 0) {
            coded_input.DecrementRecursionDepthAndPopLimit(old_limit);
        }
    }
    
    // Step 6: Test additional operations outside recursion loops
    if (fdp.remaining_bytes() > 16) {
        // Test boundary cases for ReadLittleEndian64
        uint64_t test_le64_1 = 0, test_le64_2 = 0;
        bool read_success_1 = coded_input.ReadLittleEndian64(&test_le64_1);
        bool read_success_2 = coded_input.ReadLittleEndian64(&test_le64_2);
        
        if (read_success_1) {
            coded_output.WriteLittleEndian64(test_le64_1);
        }
        if (read_success_2) {
            coded_output.WriteLittleEndian64(test_le64_2);
        }
        
        // Test boundary cases for ReadVarint64
        uint64_t test_varint64_1 = 0, test_varint64_2 = 0;
        bool varint_success_1 = coded_input.ReadVarint64(&test_varint64_1);
        bool varint_success_2 = coded_input.ReadVarint64(&test_varint64_2);
        
        if (varint_success_1) {
            coded_output.WriteVarint64(test_varint64_1);
        }
        if (varint_success_2) {
            coded_output.WriteVarint64(test_varint64_2);
        }
        
        // Test boundary cases for ReadVarintSizeAsInt
        int test_size_1 = 0, test_size_2 = 0;
        bool size_success_1 = coded_input.ReadVarintSizeAsInt(&test_size_1);
        bool size_success_2 = coded_input.ReadVarintSizeAsInt(&test_size_2);
        
        if (size_success_1) {
            coded_output.WriteVarint64(static_cast<uint64_t>(test_size_1));
        }
        if (size_success_2) {
            coded_output.WriteVarint64(static_cast<uint64_t>(test_size_2));
        }
    }
    
    // Step 7: Test error handling and edge cases
    // Create a new CodedInputStream with exhausted buffer to test error paths
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> small_buffer = fdp.ConsumeBytes<uint8_t>(4);
        ArrayInputStream small_input(small_buffer.data(), small_buffer.size());
        CodedInputStream small_coded(&small_input);
        
        uint64_t le64_dummy = 0;
        uint64_t varint64_dummy = 0;
        int size_dummy = 0;
        
        // These should fail due to insufficient data
        (void)small_coded.ReadLittleEndian64(&le64_dummy);
        (void)small_coded.ReadVarint64(&varint64_dummy);
        (void)small_coded.ReadVarintSizeAsInt(&size_dummy);
        
        // Test recursion APIs with small buffer
        auto dummy_pair = small_coded.IncrementRecursionDepthAndPushLimit(10);
        small_coded.DecrementRecursionDepthAndPopLimit(dummy_pair.first);
    }
    
    // Step 8: Test BytesUntilLimit and ExpectAtEnd APIs
    if (fdp.remaining_bytes() > 0) {
        int bytes_until_limit = coded_input.BytesUntilLimit();
        bool expect_at_end = coded_input.ExpectAtEnd();
        (void)bytes_until_limit;
        (void)expect_at_end;
        
        // Test with different recursion configurations
        int test_limit = fdp.ConsumeIntegralInRange<int>(0, 1000);
        auto pushed_pair = coded_input.IncrementRecursionDepthAndPushLimit(test_limit);
        int pushed_limit = pushed_pair.first;
        
        int new_bytes_until_limit = coded_input.BytesUntilLimit();
        (void)new_bytes_until_limit;
        
        // Pop the limit
        coded_input.DecrementRecursionDepthAndPopLimit(pushed_limit);
    }
    
    // Step 9: Test writing operations with various values
    if (fdp.remaining_bytes() > 0) {
        // Consume test values from fuzzer input
        uint64_t le64_to_write = fdp.ConsumeIntegral<uint64_t>();
        uint64_t varint64_to_write = fdp.ConsumeIntegral<uint64_t>();
        int size_to_write = fdp.ConsumeIntegral<int>();
        
        // Write values to output stream
        coded_output.WriteLittleEndian64(le64_to_write);
        coded_output.WriteVarint64(varint64_to_write);
        // Write size as varint (safe absolute value to avoid INT_MIN overflow)
        // Use safe absolute calculation that handles INT_MIN by converting to unsigned first
        if (size_to_write == std::numeric_limits<int>::min()) {
            // INT_MIN special case: convert directly to unsigned 64-bit
            size_to_write = std::numeric_limits<int>::max();
        } else {
            size_to_write = std::abs(size_to_write);
        }
        coded_output.WriteVarint64(static_cast<uint64_t>(size_to_write));
    }

    // Step 10: Test serialization of complex patterns
    if (fdp.remaining_bytes() > 64) {
        // Create a pattern: write alternating LittleEndian64 and Varint64 values
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 8; i++) {
            if (i % 2 == 0) {
                uint64_t pattern_le64 = fdp.ConsumeIntegral<uint64_t>();
                coded_output.WriteLittleEndian64(pattern_le64);
            } else {
                uint64_t pattern_varint64 = fdp.ConsumeIntegral<uint64_t>();
                coded_output.WriteVarint64(pattern_varint64);
            }
        }
        
        // Test nested recursion
        int outer_limit = fdp.ConsumeIntegralInRange<int>(0, 100);
        auto outer_pair = coded_input.IncrementRecursionDepthAndPushLimit(outer_limit);
        int outer_pushed = outer_pair.first;
        
        int inner_limit = fdp.ConsumeIntegralInRange<int>(0, 50);
        auto inner_pair = coded_input.IncrementRecursionDepthAndPushLimit(inner_limit);
        int inner_pushed = inner_pair.first;
        
        // Perform some operations at inner recursion level
        uint64_t nested_le64 = 0;
        if (coded_input.ReadLittleEndian64(&nested_le64)) {
            coded_output.WriteLittleEndian64(nested_le64);
        }
        
        // Pop inner limit
        coded_input.DecrementRecursionDepthAndPopLimit(inner_pushed);
        
        // Perform some operations at outer recursion level
        uint64_t outer_varint64 = 0;
        if (coded_input.ReadVarint64(&outer_varint64)) {
            coded_output.WriteVarint64(outer_varint64);
        }
        
        // Pop outer limit
        coded_input.DecrementRecursionDepthAndPopLimit(outer_pushed);
    }
    
    // Step 11: Test cleanup and final operations
    // Check if we reached end of input
    bool final_expect_at_end = coded_input.ExpectAtEnd();
    (void)final_expect_at_end;
    
    // Get remaining buffer positions if any
    int final_bytes_until_limit = coded_input.BytesUntilLimit();
    (void)final_bytes_until_limit;
    
    // The streams will be automatically cleaned up when they go out of scope
    
    return 0;
}

} // namespace harness_011
