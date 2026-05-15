#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_007 {
// Protocol Buffers fuzzing harness - Comprehensive Cord-based I/O operations
// Targets APIs not covered by harness_002.cpp:
// - google::protobuf::io::ZeroCopyOutputStream::WriteCord
// - google::protobuf::io::EpsCopyOutputStream::WriteString (with Cord parameter)
// - google::protobuf::io::CopyingOutputStreamAdaptor::WriteCord
// - google::protobuf::io::CodedOutputStream::WriteCordToArray
// - google::protobuf::io::ZeroCopyInputStream::ReadCord
// - absl::Cord::RemoveSuffix
//
// Uses FuzzedDataProvider for structured input splitting
// Differentiates from harness_002.cpp by focusing on different stream adaptors
// and comprehensive Cord manipulation operations.



// Simple CopyingOutputStream implementation for testing CopyingOutputStreamAdaptor
class TestCopyingOutputStream : public google::protobuf::io::CopyingOutputStream {
public:
    explicit TestCopyingOutputStream(std::string* output) : output_(output) {}
    
    bool Write(const void* buffer, int size) override {
        output_->append(static_cast<const char*>(buffer), size);
        return true;
    }
    
private:
    std::string* output_;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - we need enough data for basic operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // ===================================================================
    // Step 1: Create test data using absl::Cord constructors
    // ===================================================================
    
    // Create Cords from strings of varying lengths
    // First check if we have enough bytes for the size parameters
    if (fdp.remaining_bytes() < 2 * sizeof(size_t)) return 0;
    
    size_t cord1_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    size_t cord2_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    
    // Check if we have enough bytes for the strings themselves
    if (fdp.remaining_bytes() < cord1_len + cord2_len) return 0;
    
    std::string cord_data1 = fdp.ConsumeBytesAsString(cord1_len);
    std::string cord_data2 = fdp.ConsumeBytesAsString(cord2_len);
    
    absl::Cord source_cord1(cord_data1);
    absl::Cord source_cord2(cord_data2);
    
    // Create a Cord from some remaining bytes (not all)
    // Leave at least 256 bytes for other operations
    size_t binary_cord_size = std::min<size_t>(fdp.remaining_bytes(), 1024);
    if (binary_cord_size == 0) return 0;
    
    std::vector<uint8_t> binary_cord_bytes = fdp.ConsumeBytes<uint8_t>(binary_cord_size);
    absl::Cord binary_cord(absl::string_view(
        reinterpret_cast<const char*>(binary_cord_bytes.data()), 
        binary_cord_bytes.size()));
    // Step 2: Initialize different stream objects
    // ===================================================================
    
    // Create ArrayOutputStream with buffer
    const size_t array_buffer_size = 4096;
    std::unique_ptr<uint8_t[]> array_buffer(new uint8_t[array_buffer_size]);
    google::protobuf::io::ArrayOutputStream array_output(array_buffer.get(), 
                                                        array_buffer_size);
    
    // Create StringOutputStream
    std::string string_output_buffer;
    google::protobuf::io::StringOutputStream string_output(&string_output_buffer);
    
    // Create CopyingOutputStreamAdaptor
    std::string copying_output_buffer;
    TestCopyingOutputStream test_copying_stream(&copying_output_buffer);
    google::protobuf::io::CopyingOutputStreamAdaptor copying_adaptor(&test_copying_stream);
    
    // Create ArrayInputStream for reading
    google::protobuf::io::ArrayInputStream array_input(
        binary_cord_bytes.data(), binary_cord_bytes.size());
    // ===================================================================
    // Step 3: Call WriteCord on different stream types
    // ===================================================================
    
    // Test ZeroCopyOutputStream::WriteCord on ArrayOutputStream
    bool write_success1 = array_output.WriteCord(source_cord1);
    (void)write_success1; // Result doesn't matter for fuzzing
    
    // Test ZeroCopyOutputStream::WriteCord on StringOutputStream
    bool write_success2 = string_output.WriteCord(source_cord2);
    (void)write_success2;
    
    // Test CopyingOutputStreamAdaptor::WriteCord
    bool write_success3 = copying_adaptor.WriteCord(binary_cord);
    (void)write_success3;
    
    // ===================================================================
    // Step 4: For CodedOutputStream, call WriteCordToArray
    // ===================================================================
    
    // Create a buffer for WriteCordToArray
    const size_t coded_buffer_size = 8192;
    std::unique_ptr<uint8_t[]> coded_buffer(new uint8_t[coded_buffer_size]);
    
    // Test CodedOutputStream::WriteCordToArray with different Cords
    uint8_t* cursor1 = google::protobuf::io::CodedOutputStream::WriteCordToArray(
        source_cord1, coded_buffer.get());
    (void)cursor1;
    
    uint8_t* cursor2 = google::protobuf::io::CodedOutputStream::WriteCordToArray(
        source_cord2, coded_buffer.get() + 1024);
    (void)cursor2;
    
    // ===================================================================
    // Step 5: For EpsCopyOutputStream, call WriteString with Cord parameter
    // ===================================================================
    
    // EpsCopyOutputStream is internal to CodedOutputStream, so we need to
    // trigger it through CodedOutputStream operations
    {
        // Create CodedOutputStream wrapping ArrayOutputStream
        google::protobuf::io::CodedOutputStream coded_output(&array_output);
        
        // Create test data from fuzzer input for EpsCopyOutputStream testing
        // Check if we have enough bytes for the string length parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t test_string_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        
        // Check if we have enough bytes for the string itself
        if (fdp.remaining_bytes() < test_string_len) return 0;
        std::string test_string = fdp.ConsumeBytesAsString(test_string_len);
        absl::Cord test_cord(test_string);
        
        // Write Cord using CodedOutputStream::WriteCord which internally
        // uses EpsCopyOutputStream::WriteString
        coded_output.WriteCord(test_cord);
        
        // Also test WriteString with string_view (Cord can be converted)
        coded_output.WriteString(test_string);
        
        // Ensure the stream is properly finalized
        coded_output.Trim();
    }
    
    // ===================================================================
    // Step 6: Test ReadCord on input streams
    // ===================================================================
    
    // Test ZeroCopyInputStream::ReadCord on ArrayInputStream
    absl::Cord read_cord;
    
    // Ensure we have enough input for reading size
    if (fdp.remaining_bytes() < sizeof(int)) return 0;
    
    int max_read_size = static_cast<int>(binary_cord_bytes.size() / 2);
    if (max_read_size < 1) return 0;  // Ensure range is valid (min=1, max>=1)
    int read_size = fdp.ConsumeIntegralInRange<int>(1, max_read_size);
    bool read_success = array_input.ReadCord(&read_cord, read_size);
    (void)read_success;

    // Test ReadCord with different sizes
    if (!array_input.ReadCord(&read_cord, read_size)) {
        // Try with smaller size - check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(int)) return 0;
        read_size = fdp.ConsumeIntegralInRange<int>(1, 100);
        array_input.ReadCord(&read_cord, read_size);
    }
    
    // ===================================================================
    // Step 7: Manipulate Cord objects using RemoveSuffix
    // ===================================================================
    
    // Test absl::Cord::RemoveSuffix on various Cords
    if (source_cord1.size() > 0) {
        // Check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t remove_amount = fdp.ConsumeIntegralInRange<size_t>(
            1, std::min<size_t>(source_cord1.size(), 100));
        source_cord1.RemoveSuffix(remove_amount);
    }
    
    if (source_cord2.size() > 0) {
        // Check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t remove_amount = fdp.ConsumeIntegralInRange<size_t>(
            1, std::min<size_t>(source_cord2.size(), 50));
        source_cord2.RemoveSuffix(remove_amount);
    }
    
    if (binary_cord.size() > 0) {
        // Check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t remove_amount = fdp.ConsumeIntegralInRange<size_t>(
            1, std::min<size_t>(binary_cord.size(), 200));
        binary_cord.RemoveSuffix(remove_amount);
    }
    
    // Test RemoveSuffix on the Cord we read from stream
    if (read_cord.size() > 0) {
        // Check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t remove_amount = fdp.ConsumeIntegralInRange<size_t>(
            1, std::min<size_t>(read_cord.size(), 150));
        read_cord.RemoveSuffix(remove_amount);
    }
    
    // ===================================================================
    // Additional tests: Combine operations for more coverage
    // ===================================================================
    
    // Create a new Cord by concatenating modified Cords
    absl::Cord combined_cord;
    combined_cord.Append(source_cord1);
    combined_cord.Append(source_cord2);
    
    // Write combined Cord to another output stream
    std::string combined_buffer;
    google::protobuf::io::StringOutputStream combined_output(&combined_buffer);
    combined_output.WriteCord(combined_cord);
    
    // Test RemoveSuffix on combined Cord
    if (combined_cord.size() > 0) {
        // Check if we have enough bytes for the size parameter
        if (fdp.remaining_bytes() < sizeof(size_t)) return 0;
        size_t remove_amount = fdp.ConsumeIntegralInRange<size_t>(
            1, std::min<size_t>(combined_cord.size(), 300));
        combined_cord.RemoveSuffix(remove_amount);
    }
    
    // Test with empty or very small Cords
    absl::Cord empty_cord;
    empty_cord.RemoveSuffix(0);  // Should be safe
    
    // Create small cord from fuzzer input
    if (fdp.remaining_bytes() >= 4) {
        std::string small_cord_data = fdp.ConsumeBytesAsString(4);
        absl::Cord small_cord(small_cord_data);
        if (small_cord.size() > 2) {
            small_cord.RemoveSuffix(2);
        }
    }
    
    // ===================================================================
    // Test error handling and edge cases
    // ===================================================================
    
    // Test WriteCord with empty Cord
    absl::Cord empty_cord2;
    array_output.WriteCord(empty_cord2);
    
    // Test RemoveSuffix with valid bounds (respecting API contract: n <= size())
    if (fdp.remaining_bytes() >= 4) {
        std::string test_cord_data = fdp.ConsumeBytesAsString(4);
        absl::Cord small_cord2(test_cord_data);
        if (small_cord2.size() > 0) {
            // Only remove up to the cord size - respect API contract
            size_t max_remove = std::min<size_t>(small_cord2.size(), 10);
            if (max_remove > 0) {
                small_cord2.RemoveSuffix(max_remove);
            }
        }
    }
    
    // ===================================================================
    // Cleanup and finalization
    // ===================================================================
    
    // Flush CopyingOutputStreamAdaptor
    copying_adaptor.Flush();
    
    // All objects will be automatically destroyed when they go out of scope
    // The destructors will handle proper cleanup
    
    return 0;
}

} // namespace harness_007
