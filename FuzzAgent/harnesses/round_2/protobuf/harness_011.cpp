#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_011 {
// Harness for Google Protocol Buffers EpsCopyOutputStream::WriteString with Cord parameter
// Targets the most critical coverage gap identified: EpsCopyOutputStream::WriteString with Cord parameter
// Has 48 accumulated branches with 19 undiscovered branches and is completely uncovered
// Semantic differentiation from existing harnesses 000-010:
// - harness_000: binary parsing and serialization of message objects
// - harness_001: text format parsing and printing with Tokenizer
// - harness_002: Cord-based I/O streaming with compression (CordOutputStream/InputStream)
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream and stream backup functions
// - harness_006: File-based I/O operations with Gzip compression
// - harness_007: CodedStream abstraction layer for wire format handling with Cord read/write operations
// - harness_008: Modern absl::Cord integration focusing on zero-copy string handling with EpsCopyOutputStream (tests WriteCord and string_view WriteString)
// - harness_009: CodedOutputStream static ToArray methods with Cord support
// - harness_010: ZeroCopyInputStream composition patterns (ConcatenatingInputStream, LimitingInputStream)
// - harness_011: SPECIFICALLY targets EpsCopyOutputStream::WriteString with Cord parameter (uncovered API)


// Include protobuf headers for EpsCopyOutputStream and Cord operations

using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::EpsCopyOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;

// Helper to create different types of Cord objects for comprehensive testing
void CreateTestCords(FuzzedDataProvider& fdp, 
                     absl::Cord& empty_cord,
                     absl::Cord& small_cord,
                     absl::Cord& medium_cord,
                     absl::Cord& large_cord,
                     absl::Cord& fragmented_cord) {
    // Empty cord (base case)
    empty_cord = absl::Cord();
    
    // Small cord (fits in inline buffer)
    if (fdp.remaining_bytes() > 16) {
        std::string small_str = fdp.ConsumeRandomLengthString(16);
        small_cord = absl::Cord(small_str);
    }
    
    // Medium cord (requires allocation but single chunk)
    if (fdp.remaining_bytes() > 128) {
        std::string medium_str = fdp.ConsumeRandomLengthString(128);
        medium_cord = absl::Cord(medium_str);
    }
    
    // Large cord (multiple buffer allocations)
    if (fdp.remaining_bytes() > 1024) {
        std::string large_str = fdp.ConsumeRandomLengthString(1024);
        large_cord = absl::Cord(large_str);
    }
    
    // Fragmented cord (multiple chunks - tests Cord's chunk iteration)
    if (fdp.remaining_bytes() > 256) {
        fragmented_cord = absl::Cord();
        // Create 3-8 fragments
        int num_fragments = fdp.ConsumeIntegralInRange<int>(3, 8);
        for (int i = 0; i < num_fragments && fdp.remaining_bytes() > 32; i++) {
            std::string fragment = fdp.ConsumeRandomLengthString(32);
            fragmented_cord.Append(fragment);
        }
    }
}

// Test EpsCopyOutputStream::WriteString with Cord parameter directly
// This is the core uncovered API: WriteString(uint32_t num, const absl::Cord& s, uint8_t* ptr)
void TestEpsCopyWriteStringWithCord(ArrayOutputStream& array_output,
                                   const absl::Cord& cord,
                                   uint32_t field_num) {
    // Get buffer pointer for EpsCopyOutputStream operations
    uint8_t* buffer_ptr = nullptr;
    EpsCopyOutputStream eps_output(&array_output, false, &buffer_ptr);
    
    // Test the specific uncovered API: WriteString with Cord parameter
    // Note: buffer_ptr is updated by EpsCopyOutputStream constructor
    if (buffer_ptr != nullptr) {
        buffer_ptr = eps_output.WriteString(field_num, cord, buffer_ptr);
        
        // Trim to finalize
        buffer_ptr = eps_output.Trim(buffer_ptr);
    }
}

// Test WriteString with Cord through CodedOutputStream (public API path)
void TestCodedOutputStreamWriteCord(ArrayOutputStream& array_output,
                                   const absl::Cord& cord) {
    CodedOutputStream coded_output(&array_output);
    
    // Write Cord using CodedOutputStream::WriteCord
    // This internally calls EpsCopyOutputStream::WriteCord, not WriteString with Cord
    coded_output.WriteCord(cord);
    
    // Check for errors
    bool had_error = coded_output.HadError();
    (void)had_error; // Intentional unused
}

// Test mixed operations: WriteString with string_view vs WriteString with Cord
void TestMixedStringOperations(FuzzedDataProvider& fdp,
                              ArrayOutputStream& array_output,
                              const absl::Cord& cord,
                              const std::string& str_data) {
    uint8_t* buffer_ptr = nullptr;
    EpsCopyOutputStream eps_output(&array_output, false, &buffer_ptr);
    
    if (buffer_ptr != nullptr) {
        // Test WriteString with string_view (for comparison)
        absl::string_view str_view(str_data);
        uint32_t field_num1 = fdp.ConsumeIntegral<uint32_t>();
        buffer_ptr = eps_output.WriteString(field_num1, str_view, buffer_ptr);
        
        // Test WriteString with Cord (target API)
        uint32_t field_num2 = fdp.ConsumeIntegral<uint32_t>();
        buffer_ptr = eps_output.WriteString(field_num2, cord, buffer_ptr);
        
        // Trim to finalize
        buffer_ptr = eps_output.Trim(buffer_ptr);
    }
}

// Test error conditions and edge cases
void TestErrorConditions(FuzzedDataProvider& fdp,
                        const absl::Cord& cord) {
    // Test with very small buffer (might cause write failures)
    size_t small_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::vector<uint8_t> small_buffer(small_buffer_size);
    ArrayOutputStream small_output(small_buffer.data(), small_buffer.size());
    
    uint8_t* buffer_ptr = nullptr;
    EpsCopyOutputStream eps_output(&small_output, false, &buffer_ptr);
    
    if (buffer_ptr != nullptr) {
        // Try to write Cord (might fail if buffer too small)
        uint32_t field_num = fdp.ConsumeIntegral<uint32_t>();
        buffer_ptr = eps_output.WriteString(field_num, cord, buffer_ptr);
        
        // Check for errors
        bool had_error = eps_output.HadError();
        (void)had_error;
        
        // Trim (handles errors internally)
        buffer_ptr = eps_output.Trim(buffer_ptr);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful Cord testing
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Configuration options for different test scenarios
    bool test_direct_eps_write = fdp.ConsumeBool();
    bool test_coded_output_path = fdp.ConsumeBool();
    bool test_mixed_operations = fdp.ConsumeBool();
    bool test_error_conditions = fdp.ConsumeBool();
    bool test_multiple_fields = fdp.ConsumeBool();
    bool test_deterministic_mode = fdp.ConsumeBool();
    
    // Consume field numbers for testing
    uint32_t field_num1 = fdp.ConsumeIntegral<uint32_t>();
    uint32_t field_num2 = fdp.ConsumeIntegral<uint32_t>();
    uint32_t field_num3 = fdp.ConsumeIntegral<uint32_t>();
    
    // Create various Cord types for comprehensive testing
    absl::Cord empty_cord, small_cord, medium_cord, large_cord, fragmented_cord;
    CreateTestCords(fdp, empty_cord, small_cord, medium_cord, large_cord, fragmented_cord);
    
    // Consume string data for string_view comparison testing
    std::string str_data = fdp.ConsumeRandomLengthString(512);
    
    // Allocate output buffer - size based on remaining input
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 16384);
    std::vector<uint8_t> output_buffer(buffer_size);
    
    // Test 1: Direct EpsCopyOutputStream::WriteString with Cord parameter
    if (test_direct_eps_write) {
        ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
        
        // Test with different Cord types
        if (!empty_cord.empty()) {
            TestEpsCopyWriteStringWithCord(array_output, empty_cord, field_num1);
        }
        
        if (!small_cord.empty()) {
            TestEpsCopyWriteStringWithCord(array_output, small_cord, field_num2);
        }
        
        if (!medium_cord.empty()) {
            TestEpsCopyWriteStringWithCord(array_output, medium_cord, field_num3);
        }
        
        if (!large_cord.empty()) {
            TestEpsCopyWriteStringWithCord(array_output, large_cord, field_num1);
        }
        
        if (!fragmented_cord.empty()) {
            TestEpsCopyWriteStringWithCord(array_output, fragmented_cord, field_num2);
        }
    }
    
    // Test 2: CodedOutputStream path (public API that uses internal EpsCopyOutputStream)
    if (test_coded_output_path && !output_buffer.empty()) {
        ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
        
        // Test WriteCord through CodedOutputStream
        // Note: This tests EpsCopyOutputStream::WriteCord, not WriteString with Cord
        // but it's related and helps exercise the infrastructure
        if (!small_cord.empty()) {
            TestCodedOutputStreamWriteCord(array_output, small_cord);
        }
        
        if (!medium_cord.empty()) {
            TestCodedOutputStreamWriteCord(array_output, medium_cord);
        }
    }
    
    // Test 3: Mixed operations - comparing WriteString with string_view vs WriteString with Cord
    if (test_mixed_operations && !output_buffer.empty() && !str_data.empty()) {
        ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
        
        if (!small_cord.empty()) {
            TestMixedStringOperations(fdp, array_output, small_cord, str_data);
        }
    }
    
    // Test 4: Error conditions and edge cases
    if (test_error_conditions) {
        if (!large_cord.empty()) {
            TestErrorConditions(fdp, large_cord);
        }
    }
    
    // Test 5: Multiple field numbers and deterministic mode
    if (test_multiple_fields && !output_buffer.empty()) {
        ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
        
        // Test deterministic mode (affects serialization order)
        bool deterministic = test_deterministic_mode;
        uint8_t* buffer_ptr = nullptr;
        EpsCopyOutputStream eps_output(&array_output, deterministic, &buffer_ptr);
        
        if (buffer_ptr != nullptr) {
            // Write multiple Cords with different field numbers
            if (!small_cord.empty()) {
                buffer_ptr = eps_output.WriteString(1, small_cord, buffer_ptr);
            }
            
            if (!medium_cord.empty()) {
                buffer_ptr = eps_output.WriteString(2, medium_cord, buffer_ptr);
            }
            
            if (!fragmented_cord.empty()) {
                buffer_ptr = eps_output.WriteString(3, fragmented_cord, buffer_ptr);
            }
            
            // Trim to finalize
            buffer_ptr = eps_output.Trim(buffer_ptr);
            
            // Check error status
            bool had_error = eps_output.HadError();
            (void)had_error;
        }
    }
    
    return 0;
}

} // namespace harness_011
