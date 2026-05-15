#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_009 {
// Harness for Google Protocol Buffers CodedOutputStream serialization with absl::Cord support
// Targets uncovered CodedOutputStream APIs with high complexity (48+ branches) as identified by coverage analysis
// Semantic differentiation from existing harnesses:
// - harness_000: binary parsing and serialization of message objects
// - harness_001: text format parsing and printing with Tokenizer
// - harness_002: Cord-based I/O streaming with compression (CordOutputStream/InputStream)
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream and stream backup functions
// - harness_006: File-based I/O operations with Gzip compression
// - harness_007: CodedStream abstraction layer for wire format handling with Cord read/write operations
// - harness_008: Modern absl::Cord integration focusing on zero-copy string handling with EpsCopyOutputStream
// - harness_009: CodedOutputStream static ToArray methods with Cord support, WriteStringWithSizeToArray, varint encoding with sign extension, and tag writing operations


// Include protobuf headers for CodedOutputStream and Cord operations

using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;

// Helper function to test static ToArray methods with various data types
template<typename T>
void TestWriteIntegralToArray(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    if (buffer.size() < 16) return;
    
    // Test different integral types
    uint8_t* ptr = buffer.data();
    
    // Write various integer types using ToArray methods
    uint32_t uint32_val = fdp.ConsumeIntegral<uint32_t>();
    uint64_t uint64_val = fdp.ConsumeIntegral<uint64_t>();
    int32_t int32_val = fdp.ConsumeIntegral<int32_t>();
    int64_t int64_val = fdp.ConsumeIntegral<int64_t>();
    
    // Test WriteLittleEndian32ToArray
    if (ptr + 4 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteLittleEndian32ToArray(uint32_val, ptr);
    }
    
    // Test WriteLittleEndian64ToArray
    if (ptr + 8 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteLittleEndian64ToArray(uint64_val, ptr);
    }
    
    // Test WriteVarint32ToArray
    if (ptr + 5 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteVarint32ToArray(uint32_val, ptr);
    }
    
    // Test WriteVarint64ToArray
    if (ptr + 10 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteVarint64ToArray(uint64_val, ptr);
    }
    
    // Test WriteVarint32SignExtendedToArray - CRITICAL UNCOVERED API (48+ branches)
    if (ptr + 10 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteVarint32SignExtendedToArray(int32_val, ptr);
    }
    
    // Test WriteTagToArray - optimized for writing tags
    if (ptr + 5 <= buffer.data() + buffer.size()) {
        uint32_t tag = fdp.ConsumeIntegral<uint32_t>();
        ptr = CodedOutputStream::WriteTagToArray(tag, ptr);
    }
}

// Test WriteStringWithSizeToArray - CRITICAL UNCOVERED API
void TestWriteStringWithSizeToArray(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    if (buffer.size() < 100) return;
    
    // Consume string data for testing
    std::string str_data = fdp.ConsumeRandomLengthString(256);
    absl::string_view str_view = str_data;
    
    uint8_t* ptr = buffer.data();
    
    // Test WriteStringToArray (without size prefix)
    if (ptr + str_data.size() <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteStringToArray(str_view, ptr);
    }
    
    // Test WriteStringWithSizeToArray - includes varint size prefix
    // This is the main uncovered API with high complexity
    if (ptr + str_data.size() + 5 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteStringWithSizeToArray(str_view, ptr);
    }
    
    // Test WriteRawToArray for comparison
    if (ptr + str_data.size() <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteRawToArray(str_data.data(), str_data.size(), ptr);
    }
}

// Test WriteCordToArray and related Cord operations - CRITICAL UNCOVERED APIs
void TestWriteCordToArray(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    if (buffer.size() < 200) return;
    
    // Create various Cord objects with different characteristics
    absl::Cord empty_cord;
    absl::Cord small_cord;
    absl::Cord fragmented_cord;
    absl::Cord large_cord;
    
    // Create small cord
    std::string small_str = fdp.ConsumeRandomLengthString(64);
    small_cord = absl::Cord(small_str);
    
    // Create larger cord
    std::string larger_str = fdp.ConsumeRandomLengthString(256);
    large_cord = absl::Cord(larger_str);
    
    // Create fragmented cord by appending multiple chunks
    if (fdp.remaining_bytes() > 128) {
        std::string frag1 = fdp.ConsumeRandomLengthString(32);
        std::string frag2 = fdp.ConsumeRandomLengthString(32);
        std::string frag3 = fdp.ConsumeRandomLengthString(32);
        fragmented_cord.Append(frag1);
        fragmented_cord.Append(frag2);
        fragmented_cord.Append(frag3);
    }
    
    uint8_t* ptr = buffer.data();
    
    // Test WriteCordToArray with empty cord
    if (ptr + 1 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteCordToArray(empty_cord, ptr);
    }
    
    // Test WriteCordToArray with small cord
    if (ptr + small_str.size() <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteCordToArray(small_cord, ptr);
    }
    
    // Test WriteCordToArray with large cord
    if (ptr + larger_str.size() <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteCordToArray(large_cord, ptr);
    }
    
    // Test WriteCordToArray with fragmented cord
    if (!fragmented_cord.empty() && ptr + 100 <= buffer.data() + buffer.size()) {
        ptr = CodedOutputStream::WriteCordToArray(fragmented_cord, ptr);
    }
}

// Test HadError() and error path scenarios
void TestErrorPaths(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    if (buffer.size() < 50) return;
    
    // Create ArrayOutputStream with buffer
    ArrayOutputStream array_output(buffer.data(), buffer.size());
    CodedOutputStream coded_output(&array_output);
    
    // Test initial state
    bool had_error_initial = coded_output.HadError();
    (void)had_error_initial;
    
    // Write some data
    uint32_t test_value = fdp.ConsumeIntegral<uint32_t>();
    coded_output.WriteVarint32(test_value);
    
    // Test HadError after successful write
    bool had_error_after_write = coded_output.HadError();
    (void)had_error_after_write;
    
    // Test ByteCount
    int byte_count = coded_output.ByteCount();
    (void)byte_count;
    
    // Test GetDirectBufferPointer - error paths when buffer is full
    void* direct_buffer;
    int direct_size;
    bool got_buffer = coded_output.GetDirectBufferPointer(&direct_buffer, &direct_size);
    (void)got_buffer;
    
    // Test Skip with various amounts
    int skip_amount = fdp.ConsumeIntegralInRange<int>(0, 100);
    if (skip_amount > 0) {
        bool skip_success = coded_output.Skip(skip_amount);
        (void)skip_success;
    }
    
    // Test GetDirectBufferForNBytesAndAdvance
    int advance_size = fdp.ConsumeIntegralInRange<int>(1, 100);
    uint8_t* direct_advance = coded_output.GetDirectBufferForNBytesAndAdvance(advance_size);
    (void)direct_advance;
}

// Test varint size calculation methods
void TestVarintSizeCalculations(FuzzedDataProvider& fdp) {
    // Test VarintSize32 with various values
    uint32_t val32_1 = fdp.ConsumeIntegral<uint32_t>();
    uint32_t val32_2 = fdp.ConsumeIntegral<uint32_t>();
    uint32_t val32_3 = fdp.ConsumeIntegral<uint32_t>();
    
    size_t size1 = CodedOutputStream::VarintSize32(val32_1);
    size_t size2 = CodedOutputStream::VarintSize32(val32_2);
    size_t size3 = CodedOutputStream::VarintSize32(val32_3);
    
    // Test VarintSize64
    uint64_t val64_1 = fdp.ConsumeIntegral<uint64_t>();
    uint64_t val64_2 = fdp.ConsumeIntegral<uint64_t>();
    
    size_t size64_1 = CodedOutputStream::VarintSize64(val64_1);
    size_t size64_2 = CodedOutputStream::VarintSize64(val64_2);
    
    // Test VarintSize32SignExtended - CRITICAL for negative values
    int32_t signed_val1 = fdp.ConsumeIntegral<int32_t>();
    int32_t signed_val2 = fdp.ConsumeIntegral<int32_t>();
    
    size_t signed_size1 = CodedOutputStream::VarintSize32SignExtended(signed_val1);
    size_t signed_size2 = CodedOutputStream::VarintSize32SignExtended(signed_val2);
    
    // Test VarintSize32PlusOne variants
    size_t plus_one1 = CodedOutputStream::VarintSize32PlusOne(val32_1);
    size_t plus_one64 = CodedOutputStream::VarintSize64PlusOne(val64_1);
    size_t plus_one_signed = CodedOutputStream::VarintSize32SignExtendedPlusOne(signed_val1);
    
    // All results are intentionally unused to avoid compilation warnings
    (void)size1; (void)size2; (void)size3;
    (void)size64_1; (void)size64_2;
    (void)signed_size1; (void)signed_size2;
    (void)plus_one1; (void)plus_one64; (void)plus_one_signed;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful CodedOutputStream testing
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which CodedOutputStream pattern to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume configuration options for different test scenarios
    bool test_static_toarray = fdp.ConsumeBool();
    bool test_string_with_size = fdp.ConsumeBool();
    bool test_cord_toarray = fdp.ConsumeBool();
    bool test_error_paths = fdp.ConsumeBool();
    bool test_varint_calc = fdp.ConsumeBool();
    bool test_little_endian = fdp.ConsumeBool();
    
    // Consume buffer size for output testing
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 8192);
    std::vector<uint8_t> output_buffer(buffer_size);
    
    // Consume remaining data for various test inputs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Execute tests based on operation type
    switch (operation) {
        case 0:
            // Test static ToArray methods with integral types
            if (test_static_toarray) {
                TestWriteIntegralToArray<uint32_t>(fdp, output_buffer);
            }
            break;
            
        case 1:
            // Test WriteStringWithSizeToArray - CRITICAL UNCOVERED API
            if (test_string_with_size) {
                TestWriteStringWithSizeToArray(fdp, output_buffer);
            }
            break;
            
        case 2:
            // Test WriteCordToArray - CRITICAL UNCOVERED API
            if (test_cord_toarray) {
                TestWriteCordToArray(fdp, output_buffer);
            }
            break;
            
        case 3:
            // Test error paths and HadError() functionality
            if (test_error_paths) {
                TestErrorPaths(fdp, output_buffer);
            }
            break;
            
        case 4:
            // Test varint size calculation methods
            if (test_varint_calc) {
                TestVarintSizeCalculations(fdp);
            }
            break;
            
        case 5:
            // Test little-endian operations
            if (test_little_endian && output_buffer.size() >= 20) {
                uint8_t* ptr = output_buffer.data();
                
                // Test WriteLittleEndian16ToArray
                uint16_t val16 = fdp.ConsumeIntegral<uint16_t>();
                if (ptr + 2 <= output_buffer.data() + output_buffer.size()) {
                    ptr = CodedOutputStream::WriteLittleEndian16ToArray(val16, ptr);
                }
                
                // Test WriteLittleEndian32ToArray
                uint32_t val32 = fdp.ConsumeIntegral<uint32_t>();
                if (ptr + 4 <= output_buffer.data() + output_buffer.size()) {
                    ptr = CodedOutputStream::WriteLittleEndian32ToArray(val32, ptr);
                }
                
                // Test WriteLittleEndian64ToArray
                uint64_t val64 = fdp.ConsumeIntegral<uint64_t>();
                if (ptr + 8 <= output_buffer.data() + output_buffer.size()) {
                    ptr = CodedOutputStream::WriteLittleEndian64ToArray(val64, ptr);
                }
            }
            break;
    }
    
    return 0;
}

} // namespace harness_009
