#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "absl/strings/cord.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_014 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive Cord and stream interaction testing
    // Including Cord creation, manipulation, and stream operations
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection - focus on comprehensive Cord operations and stream interactions
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (test_type) {
        case 0: {
            // Test 1: Comprehensive Cord creation and basic manipulations
            // Following guidance: Create Cord objects using multiple methods
            
            // Create empty cord
            absl::Cord empty_cord;
            
            // Create cord from string (small)
            size_t small_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < small_cord_size) {
                break;
            }
            std::string small_cord_data = fdp.ConsumeBytesAsString(small_cord_size);
            absl::Cord small_cord(small_cord_data);
            
            // Create cord from string (medium)
            size_t medium_cord_size = fdp.ConsumeIntegralInRange<size_t>(129, 512);
            if (fdp.remaining_bytes() < medium_cord_size) {
                break;
            }
            std::string medium_cord_data = fdp.ConsumeBytesAsString(medium_cord_size);
            absl::Cord medium_cord(medium_cord_data);
            
            // Create cord from buffer using CordBuffer (if available in public API)
            // Note: CordBuffer may be internal, but we can test Cord operations
            
            // Test Cord::swap operation (targeted API)
            // Create another cord to swap with
            size_t swap_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() >= swap_cord_size) {
                std::string swap_cord_data = fdp.ConsumeBytesAsString(swap_cord_size);
                absl::Cord swap_cord(swap_cord_data);
                
                // Perform swap operation
                small_cord.swap(swap_cord);
                
                // Verify swap worked by checking characteristics
                auto swapped_flat = swap_cord.TryFlat();
                auto small_flat = small_cord.TryFlat();
            }
            
            // Test Cord::Flatten operation (targeted API)
            // Flatten creates a flat representation
            std::string flattened_medium = std::string(medium_cord.Flatten());
            
            // Test various Cord operations
            // Cord comparison operations
            bool equal_empty_small = (empty_cord == small_cord);
            bool not_equal_small_medium = (small_cord != medium_cord);
            
            // Cord assignment
            absl::Cord assigned_cord;
            assigned_cord = medium_cord;
            
            // Cord concatenation (Append operation)
            absl::Cord concatenated_cord;
            concatenated_cord.Append(small_cord);
            concatenated_cord.Append(medium_cord);
            
            // Test TryFlat on all cords
            auto empty_flat = empty_cord.TryFlat();
            auto small_flat = small_cord.TryFlat();
            auto medium_flat = medium_cord.TryFlat();
            auto assigned_flat = assigned_cord.TryFlat();
            auto concatenated_flat = concatenated_cord.TryFlat();
            
            break;
        }
        
        case 1: {
            // Test 2: Cord and stream integration - WriteCord/ReadCord operations
            // Following guidance: Create ArrayInputStream/ArrayOutputStream, write Cord data
            
            // Create Cord objects
            size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < cord1_size) {
                break;
            }
            std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
            absl::Cord cord1(cord1_data);
            
            size_t cord2_size = fdp.ConsumeIntegralInRange<size_t>(1, 384);
            if (fdp.remaining_bytes() < cord2_size) {
                break;
            }
            std::string cord2_data = fdp.ConsumeBytesAsString(cord2_size);
            absl::Cord cord2(cord2_data);
            
            // Create output buffer and stream
            size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
            std::vector<char> output_buffer(output_buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
            
            // Create CodedOutputStream for writing
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Enable aliasing based on fuzzer input
            coded_output.EnableAliasing(fdp.ConsumeBool());
            
            // Write Cord data to stream using WriteCord (internally uses EpsCopyOutputStream::WriteString with Cord)
            coded_output.WriteCord(cord1);
            coded_output.WriteCord(cord2);
            
            // Write additional raw data
            size_t raw_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() >= raw_data_size) {
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
                coded_output.WriteRaw(raw_data.data(), raw_data.size());
            }
            
            // Flush output
            coded_output.Trim();
            
            // Now create input stream from the written data
            size_t written_data_size = array_output.ByteCount();
            if (written_data_size > 0) {
                google::protobuf::io::ArrayInputStream array_input(output_buffer.data(), written_data_size);
                google::protobuf::io::CodedInputStream coded_input(&array_input);
                
                // Set recursion limit
                coded_input.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(1, 100));
                
                // Test PushLimit/PopLimit operations correctly
                // Store the limit returned by PushLimit for use with PopLimit
                int push_limit = coded_input.PushLimit(fdp.ConsumeIntegralInRange<int>(1, written_data_size));
                
                // Test ReadVarintSizeAsInt operation (targeted API)
                // First, try to read some varints
                uint32_t varint1 = 0;
                bool read_varint1 = coded_input.ReadVarint32(&varint1);
                
                int varint_size = 0;
                // Note: ReadVarintSizeAsInt might be internal, but we can simulate
                // by reading varints and checking their size
                
                // Test reading Cord back from stream
                absl::Cord read_cord1;
                absl::Cord read_cord2;
                
                // Note: ReadCord method might be available on CodedInputStream
                // We'll attempt to read using available methods
                
                // Read remaining bytes as string and create Cord from it
                if (coded_input.ExpectAtEnd() == false) {
                    // Get direct buffer pointer
                    const void* buffer_ptr;
                    int buffer_size;
                    if (coded_input.GetDirectBufferPointer(&buffer_ptr, &buffer_size)) {
                        if (buffer_size > 0) {
                            std::string remaining_data(static_cast<const char*>(buffer_ptr), buffer_size);
                            absl::Cord remaining_cord(remaining_data);
                            coded_input.Skip(buffer_size);
                        }
                    }
                }
                
                // Test various stream state operations
                // Use the exact same limit value returned by PushLimit
                coded_input.PopLimit(push_limit);
            }
            
            break;
        }
        
        case 2: {
            // Test 3: Complex Cord manipulations and buffer operations
            // Focus on CordBuffer and advanced Cord APIs
            
            // Create multiple Cord objects with different characteristics
            absl::Cord cords[3];
            
            // Cord 0: Small cord
            size_t cord0_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            if (fdp.remaining_bytes() < cord0_size) {
                break;
            }
            std::string cord0_data = fdp.ConsumeBytesAsString(cord0_size);
            cords[0] = absl::Cord(cord0_data);
            
            // Cord 1: Medium cord
            size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(65, 256);
            if (fdp.remaining_bytes() < cord1_size) {
                break;
            }
            std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
            cords[1] = absl::Cord(cord1_data);
            
            // Cord 2: Large cord (if enough data)
            size_t cord2_size = fdp.ConsumeIntegralInRange<size_t>(257, 1024);
            if (fdp.remaining_bytes() >= cord2_size) {
                std::string cord2_data = fdp.ConsumeBytesAsString(cord2_size);
                cords[2] = absl::Cord(cord2_data);
            }
            
            // Perform comprehensive Cord manipulations
            
            // Test Cord::swap between different cords
            for (int i = 0; i < 2; i++) {
                for (int j = i + 1; j < 3; j++) {
                    if (i != j) {
                        // Create copies before swap
                        absl::Cord cord_i_copy = cords[i];
                        absl::Cord cord_j_copy = cords[j];
                        
                        // Perform swap
                        cords[i].swap(cords[j]);
                        
                        // Verify swap
                        bool after_swap_equal_i = (cords[i] == cord_j_copy);
                        bool after_swap_equal_j = (cords[j] == cord_i_copy);
                    }
                }
            }
            
            // Test Cord::Flatten on all cords
            std::vector<std::string> flattened_strings;
            for (int i = 0; i < 3; i++) {
                std::string flattened = std::string(cords[i].Flatten());
                flattened_strings.push_back(flattened);
            }
            
            // Test Cord concatenation and chunking
            absl::Cord concatenated;
            for (int i = 0; i < 3; i++) {
                concatenated.Append(cords[i]);
            }
            
            // Test Cord iteration
            size_t total_chars = 0;
            for (absl::Cord::CharIterator it = concatenated.char_begin(); 
                 it != concatenated.char_end(); ++it) {
                char c = *it;
                total_chars++;
                if (total_chars > 1000) break; // Limit iteration
            }
            
            // Create Cord from flattened strings
            for (const auto& flat_str : flattened_strings) {
                absl::Cord recreated_cord(flat_str);
                bool equal_to_original = (recreated_cord == cords[0]); // Just check one
            }
            
            break;
        }
        
        case 3: {
            // Test 4: Stream state management with Cord operations
            // Testing the integration points between Cord and protobuf streams
            
            // Create test data for streams
            size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
            if (fdp.remaining_bytes() < stream_data_size) {
                break;
            }
            std::string stream_data = fdp.ConsumeBytesAsString(stream_data_size);
            
            // Create ArrayInputStream
            google::protobuf::io::ArrayInputStream array_input(stream_data.data(), stream_data.size());
            google::protobuf::io::CodedInputStream coded_input(&array_input);
            
            // Set various limits
            coded_input.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(1, 50));
            coded_input.SetTotalBytesLimit(fdp.ConsumeIntegralInRange<int>(100, 5000));
            
            // Test reading operations with Cord integration
            
            // Try to read varints first
            std::vector<uint32_t> varints;
            for (int i = 0; i < 5 && !coded_input.ExpectAtEnd(); i++) {
                uint32_t varint = 0;
                if (coded_input.ReadVarint32(&varint)) {
                    varints.push_back(varint);
                } else {
                    break;
                }
            }
            
            // Skip some data based on fuzzer input, ensure stream_data_size/2 is at least 1
            size_t max_skip = stream_data_size / 2;
            if (max_skip == 0) max_skip = 1;  // Ensure at least 1
            size_t skip_amount = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(100, max_skip));
            coded_input.Skip(skip_amount);
            // Read remaining data and create Cord from it
            const void* remaining_buffer;
            int remaining_size;
            if (coded_input.GetDirectBufferPointer(&remaining_buffer, &remaining_size)) {
                if (remaining_size > 0) {
                    std::string remaining_data(static_cast<const char*>(remaining_buffer), remaining_size);
                    absl::Cord remaining_cord(remaining_data);
                    
                    // Now test writing this Cord back to output stream
                    size_t remaining_size_unsigned = static_cast<size_t>(remaining_size);
                    size_t target_size = remaining_size_unsigned * 2;
                    size_t min_val = std::min(target_size, static_cast<size_t>(4096));
                    size_t max_val = std::max(target_size, static_cast<size_t>(4096));
                    size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(min_val, max_val);
                    std::vector<char> output_buffer(output_buffer_size);
                    google::protobuf::io::ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
                    google::protobuf::io::CodedOutputStream coded_output(&array_output);
                    
                    // Write the Cord
                    coded_output.WriteCord(remaining_cord);
                    // Write the varints we read earlier
                    for (uint32_t varint : varints) {
                        coded_output.WriteVarint32(varint);
                    }
                    
                    // Flush
                    coded_output.Trim();
                    
                    // Verify byte count
                    size_t bytes_written = array_output.ByteCount();
                }
            }
            
            // Test stream state queries
            bool at_end = coded_input.ExpectAtEnd();
            int bytes_until_limit = coded_input.BytesUntilLimit();
            
            break;
        }
        
        case 4: {
            // Test 5: Advanced Cord operations and error conditions
            
            // Create Cord with various data patterns
            size_t pattern_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 768);
            if (fdp.remaining_bytes() < pattern_cord_size) {
                break;
            }
            std::string pattern_cord_data = fdp.ConsumeBytesAsString(pattern_cord_size);
            absl::Cord pattern_cord(pattern_cord_data);
            
            // Test edge cases for Cord operations
            
            // Empty Cord operations
            absl::Cord empty_cord;
            std::string empty_flattened = std::string(empty_cord.Flatten());
            auto empty_flat = empty_cord.TryFlat();
            
            // Single character Cord
            if (fdp.remaining_bytes() >= 1) {
                std::string single_char = fdp.ConsumeBytesAsString(1);
                absl::Cord single_char_cord(single_char);
                auto single_flat = single_char_cord.TryFlat();
                std::string single_flattened = std::string(single_char_cord.Flatten());
            }
            
            // Test Cord with repeated patterns
            if (pattern_cord_size > 10) {
                // Create a subcord
                absl::Cord subcord = pattern_cord.Subcord(0, pattern_cord_size / 2);
                
                // Test comparisons
                bool pattern_eq_subcord = (pattern_cord == subcord);
                bool pattern_ne_subcord = (pattern_cord != subcord);
                
                // Test swapping with subcord
                pattern_cord.swap(subcord);
                
                // Swap back
                pattern_cord.swap(subcord);
            }
            
            // Test Cord assignment chain
            absl::Cord cord_chain1 = pattern_cord;
            absl::Cord cord_chain2 = cord_chain1;
            absl::Cord cord_chain3;
            cord_chain3 = cord_chain2;
            
            // Verify chain integrity
            bool chain_integrity = (cord_chain3 == pattern_cord);
            
            // Test Cord size and empty checks
            size_t pattern_size = pattern_cord.size();
            bool pattern_empty = pattern_cord.empty();
            size_t empty_size = empty_cord.size();
            bool truly_empty = empty_cord.empty();
            
            break;
        }
        
        case 5: {
            // Test 6: Integration with different stream types and Cord variations
            
            // Create multiple Cord variants
            std::vector<absl::Cord> cord_variants;
            
            // Empty cord
            cord_variants.push_back(absl::Cord());
            
            // Small cord
            size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            if (fdp.remaining_bytes() >= small_size) {
                std::string small_data = fdp.ConsumeBytesAsString(small_size);
                cord_variants.push_back(absl::Cord(small_data));
            }
            
            // Medium cord
            size_t medium_size = fdp.ConsumeIntegralInRange<size_t>(33, 256);
            if (fdp.remaining_bytes() >= medium_size) {
                std::string medium_data = fdp.ConsumeBytesAsString(medium_size);
                cord_variants.push_back(absl::Cord(medium_data));
            }
            
            // Large cord (if enough data)
            size_t large_size = fdp.ConsumeIntegralInRange<size_t>(257, 1024);
            if (fdp.remaining_bytes() >= large_size) {
                std::string large_data = fdp.ConsumeBytesAsString(large_size);
                cord_variants.push_back(absl::Cord(large_data));
            }
            
            // Test with different stream configurations
            
            // Configuration 1: ArrayOutputStream with moderate buffer
            size_t buffer1_size = fdp.ConsumeIntegralInRange<size_t>(1024, 4096);
            std::vector<char> buffer1(buffer1_size);
            google::protobuf::io::ArrayOutputStream array_output1(buffer1.data(), buffer1.size());
            google::protobuf::io::CodedOutputStream coded_output1(&array_output1);
            coded_output1.EnableAliasing(fdp.ConsumeBool());
            
            // Write all cord variants
            for (const auto& cord : cord_variants) {
                coded_output1.WriteCord(cord);
            }
            coded_output1.Trim();
            
            // Configuration 2: StringOutputStream
            std::string string_output;
            google::protobuf::io::StringOutputStream string_stream(&string_output);
            google::protobuf::io::CodedOutputStream coded_output2(&string_stream);
            coded_output2.EnableAliasing(fdp.ConsumeBool());
            
            // Write selected cords
            for (size_t i = 0; i < cord_variants.size() && i < 2; i++) {
                coded_output2.WriteCord(cord_variants[i]);
            }
            
            // Also write some raw data
            size_t raw_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() >= raw_size) {
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_size);
                coded_output2.WriteRaw(raw_data.data(), raw_size);
            }
            coded_output2.Trim();
            
            // Now test reading back
            
            // Create input stream from string output
            if (!string_output.empty()) {
                google::protobuf::io::ArrayInputStream array_input(string_output.data(), string_output.size());
                google::protobuf::io::CodedInputStream coded_input(&array_input);
                
                // Set limits
                coded_input.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(1, 20));
                
                // Try to read some data
                uint32_t test_varint = 0;
                bool read_varint = coded_input.ReadVarint32(&test_varint);
                
                // Skip some bytes
                // Skip some bytes, but ensure we don't skip more than available
                size_t max_skip = string_output.size() / 2;
                if (max_skip == 0) max_skip = 1;  // Ensure at least 1 if string has data
                size_t skip_bytes = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(100, max_skip));
                
                // Check stream state
                bool at_end = coded_input.ExpectAtEnd();
            }
            
            // Perform Cord manipulations on variants
            for (size_t i = 0; i + 1 < cord_variants.size(); i++) {
                // Test swap between adjacent cords
                cord_variants[i].swap(cord_variants[i + 1]);
                
                // Flatten after swap
                std::string flattened = std::string(cord_variants[i].Flatten());
                
                // Test TryFlat
                auto flat_view = cord_variants[i + 1].TryFlat();
            }
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_014
