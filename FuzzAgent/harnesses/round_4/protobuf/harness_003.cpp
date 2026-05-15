#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "absl/strings/cord.h"

namespace harness_003 {
// Fuzzing harness for Google Protocol Buffers Cord-based Coded Stream Operations
// Targets uncovered APIs: EpsCopyOutputStream::WriteString (30 branches), 
// EpsCopyOutputStream::WriteCord (24 branches), CodedInputStream::ReadCord (22 branches),
// ZeroCopyOutputStream::WriteCord (22 branches), LimitingInputStream::ReadCord (6 branches)
// 
// Differentiates from existing harnesses:
// - harness_000: message operations
// - harness_001: Tokenizer parsing  
// - harness_002: CordInputStream/CordOutputStream lite wrappers
// This harness focuses on CodedInputStream/CodedOutputStream with Cord integration as a different I/O stack layer.



// Protocol Buffers headers

using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::LimitingInputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful Cord operations
    if (size < 32) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_string_write = fdp.ConsumeBool();
    bool use_limiting_stream = fdp.ConsumeBool();
    size_t limit_amount = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Create Cord from fuzzer input (Step 1)
    size_t cord_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes() / 2);
    std::string cord_data = fdp.ConsumeBytesAsString(cord_data_size);
    absl::Cord source_cord(cord_data);
    
    // Prepare buffer for ArrayInputStream (Step 2)
    std::vector<uint8_t> input_buffer(cord_data_size);
    std::copy(cord_data.begin(), cord_data.end(), input_buffer.begin());
    
    // Create ArrayInputStream and CodedInputStream (Steps 2-3)
    ArrayInputStream array_input(input_buffer.data(), input_buffer.size());
    CodedInputStream coded_input(&array_input);
    
    // Step 4: Call ReadCord
    absl::Cord read_cord;
    bool read_success = coded_input.ReadCord(&read_cord, cord_data_size);
    // Note: read_success may be false if there's not enough data, but we continue anyway
    
    // Prepare output buffer for ArrayOutputStream (Step 5)
    std::vector<uint8_t> output_buffer(cord_data_size * 2);  // Extra space for possible writes
    ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
    
    // Create CodedOutputStream (Step 6)
    CodedOutputStream coded_output(&array_output);
    
    // Step 7: Call WriteCord (write the cord we read, or original if read failed)
    if (read_success && read_cord.size() > 0) {
        coded_output.WriteCord(read_cord);
    } else {
        coded_output.WriteCord(source_cord);
    }
    
    // Step 8: Test EpsCopyOutputStream::WriteString
    // EpsCopyOutputStream is an internal implementation detail of CodedOutputStream
    // We can test WriteString through CodedOutputStream's public API
    if (use_string_write && fdp.remaining_bytes() > 0) {
        size_t string_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
        std::string test_string = fdp.ConsumeBytesAsString(string_size);
        
        // Write string using CodedOutputStream (which uses EpsCopyOutputStream internally)
        coded_output.WriteString(test_string);
    }
    
    // Step 9: Test LimitingInputStream::ReadCord with size limits
    if (use_limiting_stream && fdp.remaining_bytes() > 0) {
        // Create a new input stream with remaining fuzzer data
        size_t remaining_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> limit_buffer = fdp.ConsumeBytes<uint8_t>(remaining_data_size);
        
        ArrayInputStream limit_array_input(limit_buffer.data(), limit_buffer.size());
        
        // Create LimitingInputStream with a limit
        LimitingInputStream limiting_input(&limit_array_input, limit_amount);
        
        // Create CodedInputStream wrapping the LimitingInputStream
        CodedInputStream limit_coded_input(&limiting_input);
        
        // Try to read a Cord with the limiting stream
        absl::Cord limit_cord;
        size_t read_amount = std::min(limit_amount, remaining_data_size);
        if (read_amount > 0) {
            limit_coded_input.ReadCord(&limit_cord, read_amount);
        }
        
        // Test reading beyond limit (should fail)
        if (remaining_data_size > limit_amount) {
            absl::Cord beyond_limit_cord;
            limit_coded_input.ReadCord(&beyond_limit_cord, remaining_data_size - limit_amount + 1);
        }
    }
    
    // Step 10: Clean up (RAII handles cleanup automatically)
    // Flush any remaining data in CodedOutputStream
    coded_output.Trim();
    
    // Test different modes based on test_mode
    switch (test_mode) {
        case 0: {
            // Test ZeroCopyOutputStream::WriteCord directly
            if (fdp.remaining_bytes() > 0) {
                size_t direct_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(1024, fdp.remaining_bytes()));
                std::string direct_cord_data = fdp.ConsumeBytesAsString(direct_cord_size);
                absl::Cord direct_cord(direct_cord_data);
                
                // Create a new output stream
                std::vector<uint8_t> direct_buffer(direct_cord_size);
                ArrayOutputStream direct_output(direct_buffer.data(), direct_buffer.size());
                
                // Call WriteCord directly on ZeroCopyOutputStream
                direct_output.WriteCord(direct_cord);
            }
            break;
        }
        
        case 1: {
            // Test CodedInputStream::ReadCord with various sizes
            if (fdp.remaining_bytes() > 0) {
                size_t var_read_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(512, fdp.remaining_bytes()));
                std::vector<uint8_t> var_buffer = fdp.ConsumeBytes<uint8_t>(var_read_size);
                
                ArrayInputStream var_input(var_buffer.data(), var_buffer.size());
                CodedInputStream var_coded_input(&var_input);
                
                // Try reading different amounts
                absl::Cord var_cord;
                for (size_t i = 0; i < 3 && var_read_size > 0; i++) {
                    size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, var_read_size);
                    var_coded_input.ReadCord(&var_cord, chunk_size);
                    var_read_size -= chunk_size;
                }
            }
            break;
        }
        
        case 2: {
            // Test mixed operations with fragmented Cord
            if (fdp.remaining_bytes() > 0) {
                size_t frag_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
                std::string frag_data = fdp.ConsumeBytesAsString(frag_size);
                
                // Create fragmented Cord
                absl::Cord fragmented_cord;
                size_t pos = 0;
                while (pos < frag_data.size()) {
                    size_t chunk = std::min<size_t>(fdp.ConsumeIntegralInRange<size_t>(1, 64), frag_data.size() - pos);
                    fragmented_cord.Append(absl::string_view(frag_data.data() + pos, chunk));
                    pos += chunk;
                }
                
                // Write fragmented cord
                std::vector<uint8_t> frag_buffer(frag_data.size());
                ArrayOutputStream frag_output(frag_buffer.data(), frag_buffer.size());
                CodedOutputStream frag_coded_output(&frag_output);
                frag_coded_output.WriteCord(fragmented_cord);
            }
            break;
        }
        
        case 3: {
            // Test error conditions and edge cases
            // Try reading/writing with zero size
            absl::Cord empty_cord;
            std::vector<uint8_t> empty_buffer(1);
            ArrayOutputStream empty_output(empty_buffer.data(), empty_buffer.size());
            CodedOutputStream empty_coded_output(&empty_output);
            
            // These should handle zero-sized cords gracefully
            empty_coded_output.WriteCord(empty_cord);
            
            // Test with very small buffer
            if (fdp.remaining_bytes() > 0) {
                size_t tiny_size = 1;
                std::vector<uint8_t> tiny_buffer = fdp.ConsumeBytes<uint8_t>(tiny_size);
                ArrayInputStream tiny_input(tiny_buffer.data(), tiny_buffer.size());
                CodedInputStream tiny_coded_input(&tiny_input);
                
                absl::Cord tiny_cord;
                tiny_coded_input.ReadCord(&tiny_cord, tiny_size);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
